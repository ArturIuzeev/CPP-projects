#pragma once
#include <atomic>
#include <functional>
#include <type_traits>
#include <utility>

template <typename T>
struct intrusive_ptr {

  intrusive_ptr() noexcept = default;

  intrusive_ptr(T* p, bool add_ref = true) : ptr(p) {
    if (ptr != nullptr && add_ref) {
      intrusive_ptr_add_ref(ptr);
    }
  }

  intrusive_ptr(const intrusive_ptr& r) : ptr(r.get()) {
    if (ptr != nullptr) {
      intrusive_ptr_add_ref(ptr);
    }
  }

  template <class Y>
    requires(std::is_convertible_v<Y*, T*>)
  intrusive_ptr(const intrusive_ptr<Y>& r) : ptr(r.get()) {
    if (ptr != nullptr) {
      intrusive_ptr_add_ref(ptr);
    }
  }

  intrusive_ptr(intrusive_ptr&& r) noexcept : ptr(r.get()) {
    r.ptr = nullptr;
  }

  template <class Y>
    requires(std::is_convertible_v<Y*, T*>)
  intrusive_ptr(intrusive_ptr<Y>&& r) : ptr(r.get()) {
    r.ptr = nullptr;
  }

  ~intrusive_ptr() {
    if (ptr) {
      intrusive_ptr_release(ptr);
    }
  }

  template <class Y>
  requires(std::is_convertible_v<Y*, T*>)
  intrusive_ptr& operator=(const intrusive_ptr<Y>& r) {
    intrusive_ptr(r).swap(*this);
    return *this;
  }

  intrusive_ptr& operator=(const intrusive_ptr& r) {
    intrusive_ptr(r).swap(*this);
    return *this;
  }

  intrusive_ptr& operator=(T* r) {
    intrusive_ptr(r).swap(*this);
    return *this;
  }

  intrusive_ptr& operator=(intrusive_ptr&& r) {
    intrusive_ptr(std::move(r)).swap(*this);
    return *this;
  }

  template <class Y>
    requires(std::is_convertible_v<Y*, T*>)
  intrusive_ptr& operator=(intrusive_ptr<Y>&& r) {
    intrusive_ptr(std::move(r)).swap(*this);
    return *this;
  }

  void reset() {
    intrusive_ptr().swap(*this);
  }

  void reset(T* r) {
    intrusive_ptr(r).swap(*this);
  }

  void reset(T* r, bool add_ref) {
    intrusive_ptr(r, add_ref).swap(*this);
  }

  T& operator*() const noexcept {
    return *ptr;
  }

  T* operator->() const noexcept {
    return ptr;
  }

  T* get() const noexcept {
    return ptr;
  }

  T* detach() noexcept {
    auto* p = ptr;
    ptr = nullptr;
    return p;
  }

  explicit operator bool() const noexcept {
    return get() != nullptr;
  }

  void swap(intrusive_ptr& b) noexcept {
    std::swap(b.ptr, ptr);
  }

private:
  T* ptr = nullptr;

  template <typename Y>
  friend struct intrusive_ptr;
};

template <class T, class U>
bool operator==(const intrusive_ptr<T>& a, const intrusive_ptr<U>& b) noexcept {
  return a.get() == b.get();
}

template <class T, class U>
bool operator!=(const intrusive_ptr<T>& a, const intrusive_ptr<U>& b) noexcept {
  return a.get() != b.get();
}

template <class T, class U>
bool operator==(const intrusive_ptr<T>& a, U* b) noexcept {
  return a.get() == b;
}

template <class T, class U>
bool operator!=(const intrusive_ptr<T>& a, U* b) noexcept {
  return a.get() != b;
}

template <class T, class U>
bool operator==(T* a, const intrusive_ptr<U>& b) noexcept {
  return a == b.get();
}

template <class T, class U>
bool operator!=(T* a, const intrusive_ptr<U>& b) noexcept {
  return a != b.get();
}

template <class T>
bool operator<(const intrusive_ptr<T>& a, const intrusive_ptr<T>& b) noexcept {
  return std::less<T*>()(a.get(), b.get());
}

template <class T>
void swap(intrusive_ptr<T>& a, intrusive_ptr<T>& b) noexcept {
  a.swap(b);
}

template <typename T>
struct intrusive_ref_counter {
  intrusive_ref_counter() noexcept = default;

  intrusive_ref_counter(const intrusive_ref_counter&) noexcept {}

  intrusive_ref_counter& operator=(const intrusive_ref_counter&) noexcept {
    return *this;
  }

  unsigned int use_count() const noexcept {
    return m_size.load(std::memory_order_relaxed);
  }

  template <class Derived>
  friend void intrusive_ptr_add_ref(const intrusive_ref_counter<Derived>* p) noexcept;

  template <class Derived>
  friend void intrusive_ptr_release(const intrusive_ref_counter<Derived>* p) noexcept;

protected:
  virtual ~intrusive_ref_counter() = default;

private:
  mutable std::atomic<std::size_t> m_size = 0;
};

template <class Derived>
void intrusive_ptr_add_ref(const intrusive_ref_counter<Derived>* p) noexcept {
  p->m_size.fetch_add(1, std::memory_order_relaxed);
}

template <class Derived>
void intrusive_ptr_release(const intrusive_ref_counter<Derived>* p) noexcept {
  if (p->m_size.fetch_sub(1, std::memory_order_acq_rel) == 1) {
    delete static_cast<const Derived*>(p);
  }
}

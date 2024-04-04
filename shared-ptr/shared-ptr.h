#pragma once

#include "obj_block.h"
#include "ptr_block.h"
#include "weak_ptr.h"

template <typename T>
class shared_ptr {
public:
  shared_ptr() noexcept = default;

  shared_ptr(std::nullptr_t) noexcept {}

  template <typename Y, typename Deleter = std::default_delete<T>>
    requires(std::is_convertible_v<Y*, T*>)
  shared_ptr(Y* p, Deleter&& deleter = Deleter()) : ptr(p) {
    try {
      _block = new ptr_block<T, Deleter>(ptr, std::forward<Deleter>(deleter));
    } catch (...) {
      deleter(ptr);
      throw;
    }
    update_cb_counters();
  }

  template <typename Y>
  shared_ptr(const shared_ptr<Y>& other, T* p) noexcept : ptr(p),
                                                          _block(other._block) {
    update_cb_counters();
  }

  template <typename Y>
  shared_ptr(shared_ptr<Y>&& other, T* p) noexcept : ptr(p),
                                                     _block(other._block) {
    other.ptr = nullptr;
    other._block = nullptr;
  }

  shared_ptr(const shared_ptr& other) noexcept {
    if (other.ptr == nullptr) {
      return;
    }
    ptr = other.ptr;
    _block = other._block;
    update_cb_counters();
  }

  template <typename Y>
  shared_ptr(const shared_ptr<Y>& other) noexcept {
    if (other.ptr == nullptr) {
      return;
    }
    ptr = other.ptr;
    _block = other._block;
    update_cb_counters();
  }

  shared_ptr(shared_ptr&& other) noexcept {
    *this = std::move(other);
  }

  template <typename Y>
  shared_ptr(shared_ptr<Y>&& other) noexcept {
    *this = std::move(other);
  }

  shared_ptr& operator=(const shared_ptr& other) noexcept {
    if (this == &other) {
      return *this;
    }
    dec_strong_count();

    ptr = other.ptr;
    _block = other._block;

    update_cb_counters();
    return *this;
  }

  template <typename Y>
  shared_ptr& operator=(const shared_ptr<Y>& other) noexcept {
    if (*this == other) {
      return *this;
    }

    dec_strong_count();

    ptr = other.ptr;
    _block = other._block;

    update_cb_counters();
    return *this;
  }

  shared_ptr& operator=(shared_ptr&& other) noexcept {
    if (this == &other) {
      return *this;
    }

    dec_strong_count();
    ptr = other.ptr;
    _block = other._block;
    other.ptr = nullptr;
    other._block = nullptr;
    return *this;
  }

  template <typename Y>
  shared_ptr& operator=(shared_ptr<Y>&& other) noexcept {
    if (*this == other) {
      return *this;
    }

    dec_strong_count();
    ptr = other.ptr;
    _block = other._block;
    other.ptr = nullptr;
    other._block = nullptr;
    return *this;
  }

  T* get() const noexcept {
    return ptr;
  }

  operator bool() const noexcept {
    return ptr != nullptr;
  }

  T& operator*() const noexcept {
    return *ptr;
  }

  T* operator->() const noexcept {
    return ptr;
  }

  std::size_t use_count() const noexcept {
    return !_block ? 0 : _block->strong_ref;
  }

  void reset() noexcept {
    *this = shared_ptr();
  }

  template <typename Y, typename Deleter = std::default_delete<T>>
  void reset(Y* p, Deleter&& deleter = Deleter()) {
    *this = shared_ptr<Y>(p, std::forward<Deleter>(deleter));
  }

  template <typename Y>
  friend bool operator==(const shared_ptr<T>& lhs, const shared_ptr<Y>& rhs) noexcept {
    return lhs.ptr == rhs.ptr;
  }

  template <typename Y>
  friend bool operator!=(const shared_ptr<T>& lhs, const shared_ptr<Y>& rhs) noexcept {
    return lhs.ptr != rhs.ptr;
  }

  ~shared_ptr() {
    dec_strong_count();
    ptr = nullptr;
    _block = nullptr;
  }

private:
  T* ptr = nullptr;
  control_block* _block = nullptr;

  shared_ptr(control_block* block, T* p) : ptr(p), _block(block) {
    _block->inc_strong();
  }

  void update_cb_counters() {
    if (_block != nullptr) {
      _block->inc_strong();
    }
  }

  void dec_strong_count() {
    if (_block != nullptr) {
      _block->dec_strong();
    }
  }

  template <typename X>
  friend class weak_ptr;

  template <typename X>
  friend class shared_ptr;

  template <typename X, typename... Args>
  friend shared_ptr<X> make_shared(Args&&... args);
};

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
  auto* block = new obj_block<T>(std::forward<Args>(args)...);
  return shared_ptr<T>(block, block->get_ptr());
}

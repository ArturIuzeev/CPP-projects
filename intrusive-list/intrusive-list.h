#pragma once

#include <iostream>
#include <ostream>
#include <type_traits>

namespace intrusive {

class default_tag;

struct list_element_base {
  list_element_base* next = nullptr;
  list_element_base* prev = nullptr;

  list_element_base() = default;

  list_element_base(const list_element_base&);
  list_element_base(list_element_base&& r);

  list_element_base& operator=(const list_element_base& r);
  list_element_base& operator=(list_element_base&& r) noexcept;
};

template <typename Tag = default_tag>
struct list_element : list_element_base {};

template <typename T, typename Tag = default_tag>
class list {
  static_assert(std::is_base_of_v<list_element<Tag>, T>, "T must derive from list_element");

  using element = list_element<Tag>;

  template <typename U>
  class list_iterator {

    element* ptr = nullptr;

    list_iterator(list_element_base* p) : ptr(static_cast<element*>(p)) {}
    friend class list;

  public:
    using value_type = U;
    using reference = U&;
    using pointer = U*;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = ptrdiff_t;

    list_iterator() {}

    list_iterator(const list_iterator<T>& iter) : ptr(iter.ptr) {}

    reference operator*() const {
      return static_cast<reference>(*ptr);
    }

    pointer operator->() const {
      return static_cast<pointer>(ptr);
    }

    list_iterator& operator--() {
      ptr = static_cast<element*>(ptr->prev);
      return *this;
    }

    list_iterator operator--(int) {
      list_iterator res(*this);
      --(*this);
      return res;
    }

    list_iterator& operator++() {
      ptr = static_cast<element*>(ptr->next);
      return *this;
    }

    list_iterator operator++(int) {
      list_iterator res(*this);
      ++(*this);
      return res;
    }

    template <typename Y>
    bool operator==(const list_iterator<Y>& right) const {
      return this->ptr == right.ptr;
    }

    template <typename Y>
    bool operator!=(const list_iterator<Y>& right) const {
      return !(*this == right);
    }

    list_iterator<value_type>& operator=(const list_iterator<T>& right) {
      this->ptr = right.ptr;
      return *this;
    }

    explicit operator list_iterator<const T>() const {
      return {ptr};
    }

    ~list_iterator() noexcept = default;
  };

public:
  using iterator = list_iterator<T>;
  using const_iterator = list_iterator<const T>;

private:
  element* head = nullptr;
  element* tail = nullptr;

  element& get_ref_from_base(list_element_base* p) const {
    return static_cast<element&>(*p);
  }

public:
  // O(1)
  list() : head(new element), tail(new element) {
    head->next = tail;
    tail->prev = head;
  }

  list(const list&) = delete;
  list& operator=(const list&) = delete;

  // O(1)
  list(list&& other) noexcept {
    if (head == nullptr && tail == nullptr) {
      head = new element;
      tail = new element;
      head->next = tail;
    }
    auto* save = other.head;
    auto* save2 = other.tail;
    other.head = head;
    other.tail = tail;
    head = save;
    tail = save2;
  }

  // O(1)
  list& operator=(list&& other) noexcept {
    if (this == &other) {
      return *this;
    }

    if (!empty()) {
      clear();
    }
    auto* save = other.head;
    auto* save2 = other.tail;
    other.head = head;
    other.tail = tail;
    head = save;
    tail = save2;
    return *this;
  }

  // O(n)  ok
  size_t size() const noexcept {
    if (empty()) {
      return 0;
    }
    size_t count = 1;
    iterator iter(head->next);
    while (iter.ptr != tail->prev) {
      ++count;
      ++iter;
    }
    return count;
  }

  // O(1) ok
  T& front() noexcept {
    return static_cast<T&>(get_ref_from_base(head->next));
  }

  // O(1) ok
  const T& front() const noexcept {
    return static_cast<const T&>(get_ref_from_base(head->next));
  }

  // O(1) ok
  T& back() noexcept {
    return static_cast<T&>(get_ref_from_base(tail->prev));
  }

  // O(1) ok
  const T& back() const noexcept {
    return static_cast<const T&>(get_ref_from_base(tail->prev));
  }

  // O(1) ok
  void push_front(T& value) noexcept {
    insert(const_iterator(head->next), value);
  }

  // O(1) ok
  void push_back(T& value) noexcept {
    if (&value == tail) {
      return;
    }

    element* new_obj = &value;

    new_obj->prev = tail->prev;
    tail->prev->next = new_obj;
    tail->prev = new_obj;
    new_obj->next = tail;
  }

  // ok
  void clear() noexcept {
    while (!empty()) {
      pop_back();
    }
  }

  // O(1) ok
  iterator begin() noexcept {
    return iterator(head->next);
  }

  // O(1) ok
  const_iterator begin() const noexcept {
    return const_iterator(head->next);
  }

  // O(1) ok
  iterator end() noexcept {
    return iterator(tail);
  }

  // O(1) ok
  const_iterator end() const noexcept {
    return const_iterator(tail);
  }

  // O(1) ok
  bool empty() const noexcept {
    return head->next == tail;
  }

  // O(1) ok
  void pop_front() noexcept {
    erase(const_iterator(head->next));
  }

  // O(1) ok
  void pop_back() noexcept {
    erase(const_iterator(tail->prev));
  }

  // O(1) ok
  iterator insert(const_iterator pos, T& value) noexcept {
    if (&value == pos.ptr) {
      return iterator(pos.ptr);
    }

    element* new_obj = &value;
    element* old_obj = pos.ptr;

    if (new_obj->next != nullptr && new_obj->prev != nullptr) {
      new_obj->next->prev = new_obj->prev;
      new_obj->prev->next = new_obj->next;
    }

    new_obj->prev = old_obj->prev;
    old_obj->prev->next = new_obj;
    old_obj->prev = new_obj;
    new_obj->next = old_obj;

    return iterator(new_obj);
  }

  // O(1) ok
  iterator erase(const_iterator pos) noexcept {
    auto* ptr = pos.ptr;

    ptr->prev->next = ptr->next;
    ptr->next->prev = ptr->prev;
    return iterator(ptr->next);
  }

  void splice(const_iterator pos, list&, const_iterator first, const_iterator last) noexcept {
    if (first == last) {
      return;
    }

    auto* f = first.ptr;
    auto* l = static_cast<element*>(last.ptr->prev);

    last.ptr->prev = f->prev;
    f->prev->next = last.ptr;

    auto* addr = pos.ptr;

    l->next = addr;
    f->prev = addr->prev;

    addr->prev->next = f;
    addr->prev = l;
  }

  // O(1)
  ~list() {
    delete head;
    delete tail;
  }
};
} // namespace intrusive

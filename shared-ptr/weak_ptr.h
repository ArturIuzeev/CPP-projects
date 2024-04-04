#pragma once

#include "control_block.h"

template <typename T>
class shared_ptr;

template <typename T>
class weak_ptr {
public:
  weak_ptr() noexcept : ptr(nullptr), _block(nullptr) {}

  template <typename Y>
  weak_ptr(const shared_ptr<Y>& other) noexcept : ptr(other.ptr),
                                                  _block(other._block) {
    if (ptr != nullptr && _block != nullptr) {
      _block->inc_weak();
    }
  }

  weak_ptr(const weak_ptr& other) noexcept : ptr(other.ptr), _block(other._block) {
    if (ptr != nullptr && _block != nullptr) {
      _block->inc_weak();
    }
  }

  template <typename Y>
  weak_ptr(const weak_ptr<Y>& other) noexcept : ptr(other.ptr),
                                                _block(other._block) {
    if (ptr != nullptr && _block != nullptr) {
      _block->inc_weak();
    }
  }

  weak_ptr(weak_ptr&& other) noexcept {
    *this = std::move(other);
  }

  template <typename Y>
  weak_ptr(weak_ptr<Y>&& other) noexcept {
    *this = std::move(other);
  }

  template <typename Y>
  weak_ptr& operator=(const shared_ptr<Y>& other) noexcept {
    ptr = other.ptr;
    _block = other._block;
    inc_weak_count(other);
    return *this;
  }

  weak_ptr& operator=(const weak_ptr& other) noexcept {
    if (*this == other) {
      return *this;
    }
    ptr = other.ptr;
    _block = other._block;
    inc_weak_count(other);
    return *this;
  }

  template <typename Y>
  weak_ptr& operator=(const weak_ptr<Y>& other) noexcept {
    if (*this == other) {
      return *this;
    }
    ptr = other.ptr;
    _block = other._block;
    inc_weak_count(other);
    return *this;
  }

  weak_ptr& operator=(weak_ptr&& other) noexcept {
    if (other == *this) {
      return *this;
    }
    ptr = other.ptr;
    _block = other._block;
    other._block = nullptr;
    other.ptr = nullptr;
    return *this;
  }

  template <typename Y>
  weak_ptr& operator=(weak_ptr<Y>&& other) noexcept {
    if (other == *this) {
      return *this;
    }
    ptr = other.ptr;
    _block = other._block;
    other._block = nullptr;
    other.ptr = nullptr;
    return *this;
  }

  shared_ptr<T> lock() const noexcept {
    if (_block != nullptr && _block->strong_ref != 0) {
      return shared_ptr<T>(_block, ptr);
    }
    return shared_ptr<T>();
  }

  void reset() noexcept {
    *this = weak_ptr();
  }

  ~weak_ptr() {
    if (_block != nullptr) {
      _block->dec_weak();
    }
    ptr = nullptr;
    _block = nullptr;
  }

private:
  T* ptr;
  control_block* _block;

  void inc_weak_count(const weak_ptr& other) {
    if (other._block != nullptr) {
      _block->inc_weak();
    }
  }

  template <typename Y>
  bool operator==(const weak_ptr<Y>& other) const noexcept {
    return ptr == other.ptr && other._block == _block;
  }

  template <typename X>
  friend class weak_ptr;

  template <typename X>
  friend class shared_ptr;
};

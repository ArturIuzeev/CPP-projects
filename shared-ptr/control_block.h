#pragma once

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

struct control_block {
  control_block() = default;

  void dec_strong() {
    --strong_ref;
    if (strong_ref == 0) {
      unlink();
    }
    dec_weak();
  }

  void dec_weak() {
    --weak_ref;
    if (weak_ref == 0) {
      delete this;
    }
  }

  void inc_strong() {
    ++strong_ref;
    inc_weak();
  }

  void inc_weak() {
    ++weak_ref;
  }

  virtual ~control_block() = default;

private:
  size_t strong_ref = 0;
  size_t weak_ref = 0;

  virtual void unlink() = 0;

  template <typename X>
  friend class shared_ptr;

  template <typename X>
  friend class weak_ptr;
};

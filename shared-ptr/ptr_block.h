#include "control_block.h"

template <typename T, typename Deleter>
struct ptr_block : control_block {

  ptr_block(T* ptr, Deleter&& deleter) : ptr(ptr), deleter(std::forward<Deleter>(deleter)) {}

  void unlink() override {
    deleter(ptr);
    ptr = nullptr;
  }

  ~ptr_block() override = default;

private:
  T* ptr = nullptr;
  Deleter deleter;
};

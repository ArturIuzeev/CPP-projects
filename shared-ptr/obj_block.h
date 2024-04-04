#include "control_block.h"

template <typename T>
struct obj_block : control_block {

  template <typename... Args>
  explicit obj_block(Args&&... args) {
    new (&obj) T(std::forward<Args>(args)...);
  }

  T* get_ptr() {
    return reinterpret_cast<T*>(&obj);
  }

  void unlink() override {
    get_ptr()->~T();
  }

  ~obj_block() override = default;

private:
  std::aligned_storage_t<sizeof(T), alignof(T)> obj;
};

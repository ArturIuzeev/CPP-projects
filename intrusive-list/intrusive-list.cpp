#include "intrusive-list.h"

intrusive::list_element_base::list_element_base(const intrusive::list_element_base&) {}

intrusive::list_element_base::list_element_base(intrusive::list_element_base&& r) : next(r.next), prev(r.prev) {
  if (prev != nullptr && prev->next != nullptr) {
    prev->next = this;
  }
  if (next != nullptr && next->prev != nullptr) {
    next->prev = this;
  }

  r.next = nullptr;
  r.prev = nullptr;
}

intrusive::list_element_base& intrusive::list_element_base::operator=(const intrusive::list_element_base& r) {
  if (this == &r) {
    return *this;
  }

  if (next != nullptr && prev != nullptr) {
    prev->next = next;
    next->prev = prev;
  }

  next = r.next;
  prev = r.prev;
  return *this;
}

intrusive::list_element_base& intrusive::list_element_base::operator=(intrusive::list_element_base&& r) noexcept {
  *this = r;

  if (prev != nullptr && prev->next != nullptr) {
    prev->next = this;
  }
  if (next != nullptr && next->prev != nullptr) {
    next->prev = this;
  }
  r.next = nullptr;
  r.prev = nullptr;
  return *this;
}

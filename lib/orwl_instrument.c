#include "orwl_instrument.h"


void print_statistics_instr(size_t id) {
  printf("Nothing is done\n");
}

orwl_state orwl_write_request2_instr(orwl_mirror* location, orwl_handle2* rh2, size_t flag) {
  return orwl_write_request2(location, rh2);
}

orwl_state orwl_read_request2_instr(orwl_mirror* location, orwl_handle2* rh2, size_t flag) {
  return orwl_read_request2(location, rh2);
}

orwl_state orwl_acquire2_instr(orwl_handle2* rh2, size_t flag) {
  return orwl_acquire2(rh2);
}

orwl_state orwl_release2_instr(orwl_handle2* rh2, size_t flag) {
  return orwl_release2(rh2);
}

orwl_state orwl_cancel2_instr(orwl_handle2* rh2, size_t flag) {
  return orwl_cancel2(rh2);
}

orwl_state orwl_test2_instr(orwl_handle2* rh2, size_t flag) {
  return orwl_test2(rh2);
}

void* orwl_write_map2_instr(orwl_handle2* rh2, size_t data_len, size_t flag) {
  return orwl_write_map2(rh2);
}

void const* orwl_read_map2_instr(orwl_handle2* rh2, size_t data_len, size_t flag) {
  return orwl_read_map2(rh2);
}

void orwl_truncate2_instr(orwl_handle2* rh2, size_t data_len, size_t flag) {
  return orwl_truncate2(rh2, data_len);
}

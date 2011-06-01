#include "orwl_instrument.h"


void instr_print_statistics(size_t id) {
  printf("Nothing is done\n");
}

orwl_state instr_orwl_write_request2(orwl_mirror* location, orwl_handle2* rh2, size_t flag) {
  return orwl_write_request2(location, rh2);
}

orwl_state instr_orwl_read_request2(orwl_mirror* location, orwl_handle2* rh2, size_t flag) {
  return orwl_read_request2(location, rh2);
}

orwl_state instr_orwl_acquire2(orwl_handle2* rh2, size_t flag) {
  return orwl_acquire2(rh2);
}

orwl_state instr_orwl_release2(orwl_handle2* rh2, size_t flag) {
  return orwl_release2(rh2);
}

orwl_state instr_orwl_cancel2(orwl_handle2* rh2, size_t flag) {
  return orwl_cancel2(rh2);
}

orwl_state instr_orwl_test2(orwl_handle2* rh2, size_t flag) {
  return orwl_test2(rh2);
}

void* instr_orwl_write_map2(orwl_handle2* rh2, size_t data_len, size_t flag) {
  return orwl_write_map2(rh2);
}

void const* instr_orwl_read_map2(orwl_handle2* rh2, size_t data_len, size_t flag) {
  return orwl_read_map2(rh2);
}

void instr_orwl_truncate2(orwl_handle2* rh2, size_t data_len, size_t flag) {
  return orwl_truncate2(rh2, data_len);
}

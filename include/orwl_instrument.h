#ifndef   	ORWL_INSTRUMENT_H_
# define   	ORWL_INSTRUMENT_H_

#include "orwl.h"

void print_statistics_instr(size_t id);
orwl_state orwl_write_request2_instr(orwl_mirror* location, orwl_handle2* rh2, size_t flag);
orwl_state orwl_read_request2_instr(orwl_mirror* location, orwl_handle2* rh2, size_t flag);
orwl_state orwl_acquire2_instr(orwl_handle2* rh2, size_t flag);
orwl_state orwl_release2_instr(orwl_handle2* rh2, size_t flag);
orwl_state orwl_cancel2_instr(orwl_handle2* rh2, size_t flag);
orwl_state orwl_test2_instr(orwl_handle2* rh2, size_t flag);
void* orwl_write_map2_instr(orwl_handle2* rh2, size_t data_len, size_t flag);
void const* orwl_read_map2_instr(orwl_handle2* rh2, size_t data_len, size_t flag);
void orwl_truncate2_instr(orwl_handle2* rh2, size_t data_len, size_t flag);

#endif

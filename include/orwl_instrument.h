#ifndef   	ORWL_INSTRUMENT_H_
# define   	ORWL_INSTRUMENT_H_

#include "orwl.h"

void instr_print_statistics(size_t id);
orwl_state instr_orwl_write_request2(orwl_mirror* location, orwl_handle2* rh2, size_t flag);
orwl_state instr_orwl_read_request2(orwl_mirror* location, orwl_handle2* rh2, size_t flag);
orwl_state instr_orwl_acquire2(orwl_handle2* rh2, size_t flag);
orwl_state instr_orwl_release2(orwl_handle2* rh2, size_t flag);
orwl_state instr_orwl_cancel2(orwl_handle2* rh2, size_t flag);
orwl_state instr_orwl_test2(orwl_handle2* rh2, size_t flag);
void* instr_orwl_write_map2(orwl_handle2* rh2, size_t data_len, size_t flag);
void const* instr_orwl_read_map2(orwl_handle2* rh2, size_t data_len, size_t flag);
void instr_orwl_truncate2(orwl_handle2* rh2, size_t data_len, size_t flag);

#endif

#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdarg.h>
#include "orwl.h"
#include "orwl_instrument.h"

P99_DECLARE_ENUM(timing_types,
                 write_request,
                 read_request,
                 acquire,
                 release,
                 cancel,
                 map,
                 mapro,
                 resize,
                 test,
                 write_request2,
                 read_request2,
                 acquire2,
                 release2,
                 disconnect2,
                 next2,
                 cancel2,
                 forced_cancel2,
                 write_map2,
                 read_map2,
                 truncate2,
                 test2
                 );

P99_DEFINE_ENUM(timing_types);

static atomic_float total_time_mt[timing_types_amount] = {0};
static atomic_size_t total_access_mt[timing_types_amount] = {0};
static atomic_float total_time_st[timing_types_amount] = {0};
static atomic_size_t total_access_st[timing_types_amount] = {0};

void print_statistics_instr(size_t id) {
  dlsym(RTLD_NEXT, "print_statistics");
  for (unsigned t = 0 ; t < timing_types_amount ; t++) {
    char const *str = timing_types_getname(t);
    printf("__%zu__timing:1:%s:%f:%zu\n", id, str, total_time_mt[t], total_access_mt[t]);
    printf("__%zu__timing:0:%s:%f:%zu\n", id, str, total_time_st[t], total_access_st[t]);
  }
}

#define O_RWL_INSTRUMENT(RET, NAME, TYPES, ARGS)                        \
RET P99_PASTE3(orwl_, NAME, _instr)TYPES {                              \
  RET(*P99_PASTE3(orwl_, NAME, _fp))TYPES = (RET(*)TYPES)dlsym(RTLD_NEXT, P99_STRINGIFY(P99_PASTE3(orwl_, NAME, _instr))); \
  struct timespec start = orwl_gettime();                               \
  P99_IF_VOID(RET)()(RET result = )P99_PASTE3(orwl_, NAME, _fp)ARGS;    \
  struct timespec end = orwl_gettime();                                 \
  timespec_minus(&end, &start);                                         \
  if (flag == 1) {                                                      \
    atomic_fetch_atomic_float_add(&total_time_mt[NAME], timespec2seconds(end)); \
    atomic_fetch_add(&total_access_mt[NAME], 1);                        \
  } else {                                                              \
    atomic_fetch_atomic_float_add(&total_time_st[NAME], timespec2seconds(end)); \
    atomic_fetch_add(&total_access_st[NAME], 1);                        \
  }                                                                     \
  P99_IF_VOID(RET)()(return result;)                                    \
}                                                                       \
P99_MACRO_END(o_rwl_instrument, NAME)

O_RWL_INSTRUMENT(orwl_state,
                 write_request2,
                 (orwl_mirror* location, orwl_handle2* rh2, size_t flag),
                 (location, rh2, flag));
O_RWL_INSTRUMENT(orwl_state,
                 read_request2,
                 (orwl_mirror* location, orwl_handle2* rh2, size_t flag),
                 (location, rh2, flag));
O_RWL_INSTRUMENT(orwl_state,
                 acquire2,
                 (orwl_handle2* rh2, size_t flag),
                 (rh2, flag));
O_RWL_INSTRUMENT(orwl_state,
                 release2,
                 (orwl_handle2* rh2, size_t flag),
                 (rh2, flag));
O_RWL_INSTRUMENT(orwl_state,
                 cancel2,
                 (orwl_handle2* rh2, size_t flag),
                 (rh2, flag));
O_RWL_INSTRUMENT(orwl_state,
                 disconnect2,
                 (orwl_handle2* rh2, size_t flag),
                 (rh2, flag));
O_RWL_INSTRUMENT(orwl_state,
                 forced_cancel2,
                 (orwl_handle2* rh2, size_t flag),
                 (rh2, flag));
O_RWL_INSTRUMENT(orwl_state,
                 test2,
                 (orwl_handle2* rh2, size_t flag),
                 (rh2, flag));
O_RWL_INSTRUMENT(void*,
                 write_map2,
                 (orwl_handle2* rh2, size_t data_len, size_t flag),
                 (rh2, data_len, flag));
O_RWL_INSTRUMENT(void const*,
                 read_map2,
                 (orwl_handle2* rh2, size_t data_len, size_t flag),
                 (rh2, data_len, flag));
O_RWL_INSTRUMENT(void,
                 truncate2,
                 (orwl_handle2* rh2, size_t data_len, size_t flag),
                 (rh2, data_len, flag));

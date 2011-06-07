#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdarg.h>
#include "orwl.h"
#include "orwl_instrument.h"

enum timing_types {
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
  cancel2,
  map2,
  mapro2,
  resize2,
  test2,
  timing_types_nb
};

void print_type_from_value(enum timing_types t, char str[32]) {
  switch (t) {
  case write_request:
    strncpy(str, "write_request", 32);
    break;
  case read_request:
    strncpy(str, "read_request", 32);
    break;
  case acquire:
    strncpy(str, "acquire", 32);
    break;
  case release:
    strncpy(str, "release", 32);
    break;
  case cancel:
    strncpy(str, "cancel", 32);
    break;
  case map:
    strncpy(str, "map", 32);
    break;
  case mapro:
    strncpy(str, "mapro", 32);
    break;
  case resize:
    strncpy(str, "resize", 32);
    break;
  case test:
    strncpy(str, "test", 32);
   break;
  case write_request2:
    strncpy(str, "write_request2", 32);
    break;
  case read_request2:
    strncpy(str, "read_request2", 32);
    break;
  case acquire2:
    strncpy(str, "acquire2", 32);
    break;
  case release2:
    strncpy(str, "release2", 32);
    break;
  case cancel2:
    strncpy(str, "cancel2", 32);
    break;
  case map2:
    strncpy(str, "map2", 32);
    break;
  case mapro2:
    strncpy(str, "mapro2", 32);
    break;
  case resize2:
    strncpy(str, "resize2", 32);
    break;
  case test2:
    strncpy(str, "test2", 32);
   break;
  default:
    raise(0);
  }
}

static struct timespec total_time_mt[timing_types_nb] = {0};
static uint64_t total_access_mt[timing_types_nb] = {0};
static struct timespec total_time_st[timing_types_nb] = {0};
static uint64_t total_access_st[timing_types_nb] = {0};



inline struct timespec add_tvspec(struct timespec *a, struct timespec *b) {
  struct timespec result;
  result.tv_sec = a->tv_sec + b->tv_sec;
  result.tv_nsec = a->tv_nsec + b->tv_nsec;
  if (result.tv_nsec >= 1000000000L) {
    result.tv_sec++;
    result.tv_nsec = result.tv_nsec - 1000000000L;
  }
  return result;
}

inline struct timespec diff_tvspec(struct timespec *a, struct timespec *b) {
  struct timespec result;
  result.tv_sec = a->tv_sec - b->tv_sec;
  if (a->tv_nsec < b->tv_nsec) {
    result.tv_nsec = a->tv_nsec + 1000000000L - b->tv_nsec;
    result.tv_sec--;
  } else {
    result.tv_nsec = a->tv_nsec - b->tv_nsec;
  }
  return result;
}

void print_statistics_instr(size_t id) {
  dlsym(RTLD_NEXT, "print_statistics");
  for (unsigned t = 0 ; t < timing_types_nb ; t++) {
    char str[32] = {0};
    print_type_from_value(t, str);
    printf("__%zu__timing:1:%s:%f:%"PRIu64"\n", id, str, timespec2seconds(total_time_mt[t]), total_access_mt[t]);
    printf("__%zu__timing:0:%s:%f:%"PRIu64"\n", id, str, timespec2seconds(total_time_st[t]), total_access_st[t]);
  }
}

orwl_state orwl_write_request2_instr(orwl_mirror* location, orwl_handle2* rh2, size_t flag) {
  orwl_state result;
  orwl_state(*_orwl_write_request2)(orwl_mirror*, orwl_handle2*, size_t) = (orwl_state(*)(orwl_mirror*, orwl_handle2*, size_t))dlsym(RTLD_NEXT, "orwl_write_request2_instr");
  struct timespec start = orwl_gettime();
  result = _orwl_write_request2(location, rh2, flag);
  struct timespec end = orwl_gettime();
  ORWL_CRITICAL {
    struct timespec diff = diff_tvspec(&end, &start);
    if (flag == 1) {
      struct timespec old = total_time_mt[write_request2];
      total_time_mt[write_request2] = add_tvspec(&old, &diff);
      total_access_mt[write_request2] += 1;
    } else {
      struct timespec old = total_time_st[write_request2];
      total_time_st[write_request2] = add_tvspec(&old, &diff);
      total_access_st[write_request2] += 1;
    }
  }
  return result;
}

orwl_state orwl_read_request2_instr(orwl_mirror* location, orwl_handle2* rh2, size_t flag) {
  orwl_state result;
  orwl_state(*_orwl_read_request2)(orwl_mirror*, orwl_handle2*, size_t) = (orwl_state(*)(orwl_mirror*, orwl_handle2*, size_t))dlsym(RTLD_NEXT, "orwl_read_request2_instr");
  struct timespec start = orwl_gettime();
  result = _orwl_read_request2(location, rh2, flag);
  struct timespec end = orwl_gettime();
  ORWL_CRITICAL {
    struct timespec diff = diff_tvspec(&end, &start);
    if (flag == 1) {
      struct timespec old = total_time_mt[read_request2];
      total_time_mt[read_request2] = add_tvspec(&old, &diff);
      total_access_mt[read_request2] += 1;
    } else {
      struct timespec old = total_time_st[read_request2];
      total_time_st[read_request2] = add_tvspec(&old, &diff);
      total_access_st[read_request2] += 1;
    }
  }
  return result;

}

orwl_state orwl_acquire2_instr(orwl_handle2* rh2, size_t flag) {
  orwl_state result;
  orwl_state(*_orwl_acquire2)(orwl_handle2*, size_t) = (orwl_state(*)(orwl_handle2*, size_t))dlsym(RTLD_NEXT, "orwl_acquire2_instr");
  struct timespec start = orwl_gettime();
  result = _orwl_acquire2(rh2, flag);
  struct timespec end = orwl_gettime();
  ORWL_CRITICAL {
    struct timespec diff = diff_tvspec(&end, &start);
    if (flag == 1) {
      struct timespec old = total_time_mt[acquire2];
      total_time_mt[acquire2] = add_tvspec(&old, &diff);
      total_access_mt[acquire2] += 1;
    } else {
      struct timespec old = total_time_st[acquire2];
      total_time_st[acquire2] = add_tvspec(&old, &diff);
      total_access_st[acquire2] += 1;
    }
  }
  return result;
}

orwl_state orwl_release2_instr(orwl_handle2* rh2, size_t flag)  {
  orwl_state result;
  orwl_state(*_orwl_release2)(orwl_handle2*, size_t) = (orwl_state(*)(orwl_handle2*, size_t))dlsym(RTLD_NEXT, "orwl_release2_instr");
  struct timespec start = orwl_gettime();
  result = _orwl_release2(rh2, flag);
  struct timespec end = orwl_gettime();
  ORWL_CRITICAL {
    struct timespec diff = diff_tvspec(&end, &start);
    if (flag == 1) {
      struct timespec old = total_time_mt[release2];
      total_time_mt[release2] = add_tvspec(&old, &diff);
      total_access_mt[release2] += 1;
    } else {
      struct timespec old = total_time_st[release2];
      total_time_st[release2] = add_tvspec(&old, &diff);
      total_access_st[release2] += 1;
    }
  }
  return result;
}

orwl_state orwl_cancel2_instr(orwl_handle2* rh2, size_t flag) {
  orwl_state result;
  orwl_state(*_orwl_cancel2)(orwl_handle2*, size_t) = (orwl_state(*)(orwl_handle2*, size_t))dlsym(RTLD_NEXT, "orwl_cancel2_instr");
  struct timespec start = orwl_gettime();
  result = _orwl_cancel2(rh2, flag);
  struct timespec end = orwl_gettime();
  ORWL_CRITICAL {
    struct timespec diff = diff_tvspec(&end, &start);
    if (flag == 1) {
      struct timespec old = total_time_mt[cancel2];
      total_time_mt[cancel2] = add_tvspec(&old, &diff);
      total_access_mt[cancel2] += 1;
    } else {
      struct timespec old = total_time_st[cancel2];
      total_time_st[cancel2] = add_tvspec(&old, &diff);
      total_access_st[cancel2] += 1;
    }
  }
  return result;
}

orwl_state orwl_test2_instr(orwl_handle2* rh2, size_t flag) {
  orwl_state result;
  orwl_state(*_orwl_test2)(orwl_handle2*, size_t) = (orwl_state(*)(orwl_handle2*, size_t))dlsym(RTLD_NEXT, "orwl_test2_instr");
  struct timespec start = orwl_gettime();
  result = _orwl_test2(rh2, flag);
  struct timespec end = orwl_gettime();
  ORWL_CRITICAL {
    struct timespec diff = diff_tvspec(&end, &start);
    if (flag == 1) {
      struct timespec old = total_time_mt[test2];
      total_time_mt[test2] = add_tvspec(&old, &diff);
      total_access_mt[test2] += 1;
    } else {
      struct timespec old = total_time_st[test2];
      total_time_st[test2] = add_tvspec(&old, &diff);
      total_access_st[test2] += 1;
    }
  }
  return result;
}

void* orwl_write_map2_instr(orwl_handle2* rh2, size_t data_len, size_t flag) {
  void * result = 0;
  void*(*_orwl_write_map2)(orwl_handle2*, size_t, size_t) = (void*(*)(orwl_handle2*, size_t, size_t))dlsym(RTLD_NEXT, "orwl_write_map2_instr");
  struct timespec start = orwl_gettime();
  result = _orwl_write_map2(rh2, data_len, flag);
  struct timespec end = orwl_gettime();
  ORWL_CRITICAL {
    struct timespec diff = diff_tvspec(&end, &start);
    if (flag == 1) {
      struct timespec old = total_time_mt[map2];
      total_time_mt[map2] = add_tvspec(&old, &diff);
      total_access_mt[map2] += 1;
    } else {
      struct timespec old = total_time_st[map2];
      total_time_st[map2] = add_tvspec(&old, &diff);
      total_access_st[map2] += 1;
    }
  }
  return result;
}

void const* orwl_read_map2_instr(orwl_handle2* rh2, size_t data_len, size_t flag) {
  void const * result;
  void const*(*_orwl_mapro2)(orwl_handle2*, size_t, size_t) = (void const*(*)(orwl_handle2*, size_t, size_t))dlsym(RTLD_NEXT, "orwl_read_map2_instr");
  struct timespec start = orwl_gettime();
  result = _orwl_mapro2(rh2, data_len, flag);
  struct timespec end = orwl_gettime();
  ORWL_CRITICAL {
    struct timespec diff = diff_tvspec(&end, &start);
    if (flag == 1) {
      struct timespec old = total_time_mt[mapro2];
      total_time_mt[mapro2] = add_tvspec(&old, &diff);
      total_access_mt[mapro2] += 1;
    } else {
      struct timespec old = total_time_st[mapro2];
      total_time_st[mapro2] = add_tvspec(&old, &diff);
      total_access_st[mapro2] += 1;
    }
  }
  return result;
}

void orwl_truncate2_instr(orwl_handle2* rh2, size_t data_len, size_t flag) {
  void(*_orwl_truncate2)(orwl_handle2*, size_t, size_t) = (void(*)(orwl_handle2*, size_t, size_t))dlsym(RTLD_NEXT, "orwl_truncate2_instr");
  struct timespec start = orwl_gettime();
  _orwl_truncate2(rh2, data_len, flag);
  struct timespec end = orwl_gettime();
  ORWL_CRITICAL {
    struct timespec diff = diff_tvspec(&end, &start);
    if (flag == 1) {
      struct timespec old = total_time_mt[resize2];
      total_time_mt[resize2] = add_tvspec(&old, &diff);
      total_access_mt[resize2] += 1;
    } else {
      struct timespec old = total_time_st[resize2];
      total_time_st[resize2] = add_tvspec(&old, &diff);
      total_access_st[resize2] += 1;
    }
  }
}




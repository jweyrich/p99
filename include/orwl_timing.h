#ifndef   	ORWL_TIMING_H_
#define   	ORWL_TIMING_H_

#include "orwl_time.h"
#include "orwl_document.h"
#include "orwl_new.h"

P99_DECLARE_STRUCT(orwl_timing);

struct orwl_timing {
  uint64_t time_total_acquire;
  uint64_t nb_total_acquire;
  uint64_t time_wait_on_cond_acquire;
  uint64_t nb_wait_on_cond_acquire;

  uint64_t time_total_release;
  uint64_t nb_total_release;
  uint64_t time_copy_data_release;
  uint64_t nb_copy_data_release;
  uint64_t time_send_data_release;
  uint64_t nb_send_data_release;

  uint64_t time_total_read_request;
  uint64_t nb_total_read_request;
  uint64_t time_rpc_read_request;
  uint64_t nb_rpc_read_request;

  uint64_t time_total_write_request;
  uint64_t nb_total_write_request;
  uint64_t time_rpc_write_request;
  uint64_t nb_rpc_write_request;
};


orwl_timing * orwl_timing_info(void);

void orwl_timing_print_stats(void);

enum { orwl_timing_var = 0, orwl_timing_fetched = 0 };

/**
 ** @def ORWL_TIMING(NAME)
 ** @brief Timing of a statement or block
 **
 ** This macro is used as a prefix of a particular statement or block
 ** of code. @a NAME must correspond to one of the resources that are
 ** listed in the ::orwl_timing (without the "nb_" or "time_" prefix).
 **
 ** Care is taken that the function calls are optimized
 ** - ::orwl_timing_info is only called once per block. When timing is
 **   used again inside a block that is itself timed no new call to
 **   that function is issued
 ** - the two calls to ::orwl_gettime are as close as possible to the
 **   application code. I.e we have something like a sequence
 **   (0) ::orwl_timing_info()
 **   (1) ::orwl_gettime()
 **   (2) application code
 **   (3) ::orwl_gettime()
 **   (4) update of statistics
 **
 ** The update of the statistics are done with atomic operations such
 ** that all of this can be done without danger in a threaded
 ** environment.
 **/



#ifdef GETTIMING
# define ORWL_TIMING(NAME)                                              \
P00_BLK_START                                                           \
P00_BLK_DECL(register orwl_timing*const, _timing,                       \
             (orwl_timing_fetched ? orwl_timing_var : orwl_timing_info())) \
  P00_BLK_DECL(register orwl_timing*const, orwl_timing_var, _timing)    \
  P00_BLK_DECL(register bool const, orwl_timing_fetched, true)          \
  P00_BLK_DECL(struct timespec, p00_end)                                \
  P00_BLK_DECL(struct timespec, p00_start, orwl_gettime())              \
  P00_BLK_AFTER(atomic_fetch_add(&(orwl_timing_var->P99_PASTE2(nb_, NAME)), 1)) \
  P00_BLK_AFTER(atomic_fetch_add(&(orwl_timing_var->P99_PASTE2(time_, NAME)), \
                                 timespec2useconds(timespec_diff(p00_start, p00_end)))) \
  P00_BLK_AFTER(p00_end = orwl_gettime())                               \
  P00_BLK_END
#else
# define ORWL_TIMING(NAME)
#endif

#endif 	    /* !ORWL_TIMING_H_ */

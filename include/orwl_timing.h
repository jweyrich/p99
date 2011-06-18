#ifdef GETTIMING
#ifndef   	ORWL_TIMING_H_
#define   	ORWL_TIMING_H_

#include "orwl_time.h"
#include "orwl_document.h"
#include "orwl_new.h"

/**
 ** @brief Accumulate the time difference between @a start and @a end
 ** in @result.
 **
 ** @todo Make ::diff_and_add_tvspec thread safe by using
 ** ::atomic_fetch_add & Co. This would avoid the necessity to lock a
 ** mutex around calls to this function. This may be critical, since
 ** taking a mutex here could be of the same order of magnitude of the
 ** times that we measure.
 **/
inline void diff_and_add_tvspec(struct timespec const *start,
				struct timespec const *end,
				struct timespec *result) {
  struct timespec tmp = timespec_diff(start, end);
  timespec_add(result, &tmp);
}

P99_DECLARE_STRUCT(orwl_timing);

struct orwl_timing {
  struct timespec time_total_acquire;
  uint64_t nb_total_acquire;
  pthread_mutex_t mutex_total_acquire;
  struct timespec time_wait_on_cond_acquire;
  uint64_t nb_wait_on_cond_acquire;
  pthread_mutex_t mutex_wait_on_cond_acquire;

  struct timespec time_total_release;
  uint64_t nb_total_release;
  pthread_mutex_t mutex_total_release;
  struct timespec time_copy_data_release;
  uint64_t nb_copy_data_release;
  pthread_mutex_t mutex_copy_data_release;
  struct timespec time_send_data_release;
  uint64_t nb_send_data_release;
  pthread_mutex_t mutex_send_data_release;

  struct timespec time_total_read_request;
  uint64_t nb_total_read_request;
  pthread_mutex_t mutex_total_read_request;
  struct timespec time_rpc_read_request;
  uint64_t nb_rpc_read_request;
  pthread_mutex_t mutex_rpc_read_request;

  struct timespec time_total_write_request;
  uint64_t nb_total_write_request;
  pthread_mutex_t mutex_total_write_request;
  struct timespec time_rpc_write_request;
  uint64_t nb_rpc_write_request;
  pthread_mutex_t mutex_rpc_write_request;
};


orwl_timing * orwl_timing_info(void);

void orwl_timing_print_stats(void);

#endif 	    /* !ORWL_TIMING_H_ */

#endif /* !GETTIMING */

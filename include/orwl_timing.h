#ifdef GETTIMING
#ifndef   	ORWL_TIMING_H_
#define   	ORWL_TIMING_H_

#include "orwl_time.h"
#include "orwl_document.h"
#include "orwl_new.h"

inline void diff_and_add_tvspec(struct timespec *start, 
				struct timespec *end,
				struct timespec *result) {
  struct timespec tmp;
  tmp.tv_sec = end->tv_sec - start->tv_sec;
  if (end->tv_nsec < start->tv_nsec) {
    tmp.tv_nsec = end->tv_nsec + 1000000000L - start->tv_nsec;
    tmp.tv_sec--;
  } else {
    tmp.tv_nsec = end->tv_nsec - start->tv_nsec;
  }

  result->tv_sec = result->tv_sec + tmp.tv_sec;
  result->tv_nsec = result->tv_nsec + tmp.tv_nsec;
  if (result->tv_nsec >= 1000000000L) {
    result->tv_sec++;
    result->tv_nsec = result->tv_nsec - 1000000000L;
  }
}

inline struct timespec diff_tvspec(struct timespec *start, struct timespec *end) {
  struct timespec result;
  result.tv_sec = end->tv_sec - start->tv_sec;
  if (end->tv_nsec < start->tv_nsec) {
    result.tv_nsec = end->tv_nsec + 1000000000L - start->tv_nsec;
    result.tv_sec--;
  } else {
    result.tv_nsec = end->tv_nsec - start->tv_nsec;
  }
  return result;
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


static orwl_timing timing_info = {0};

orwl_timing * orwl_timing_info(void);

void orwl_timing_print_stats(void);

#endif 	    /* !ORWL_TIMING_H_ */

#endif /* !GETTIMING */

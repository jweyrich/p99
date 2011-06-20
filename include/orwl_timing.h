#ifdef GETTIMING
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

#endif 	    /* !ORWL_TIMING_H_ */

#endif /* !GETTIMING */

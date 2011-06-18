#ifdef GETTIMING

#include "orwl_timing.h"
#include "p99_defarg.h"
#include "p99_new.h"

static orwl_timing timing_info = P99_INIT;

P99_INSTANTIATE(void, diff_and_add_tvspec, struct timespec const*, struct timespec const*, struct timespec*);

orwl_timing * orwl_timing_info(void) {
  return &timing_info;
}

void orwl_timing_print_stats(void) {
  printf("__total_acquire: %"PRIu64",%"PRIu64"\n",
	 timing_info.nb_total_acquire, 
	 timespec2useconds(timing_info.time_total_acquire));
  printf("__wait_on_cond_acquire: %"PRIu64",%"PRIu64"\n",
	 timing_info.nb_wait_on_cond_acquire, 
	 timespec2useconds(timing_info.time_wait_on_cond_acquire));
  printf("__total_release: %"PRIu64",%"PRIu64"\n",
	 timing_info.nb_total_release, 
	 timespec2useconds(timing_info.time_total_release));
  printf("__copy_data_release: %"PRIu64",%"PRIu64"\n",
	 timing_info.nb_copy_data_release, 
	 timespec2useconds(timing_info.time_copy_data_release));
  printf("__send_data_release: %"PRIu64",%"PRIu64"\n",
	 timing_info.nb_send_data_release, 
	 timespec2useconds(timing_info.time_send_data_release));
  printf("__total_read_request: %"PRIu64",%"PRIu64"\n",
	 timing_info.nb_total_read_request, 
	 timespec2useconds(timing_info.time_total_read_request));
  printf("__rpc_read_request: %"PRIu64",%"PRIu64"\n",
	 timing_info.nb_rpc_read_request, 
	 timespec2useconds(timing_info.time_rpc_read_request));
  printf("__total_write_request: %"PRIu64",%"PRIu64"\n",
	 timing_info.nb_total_write_request, 
	 timespec2useconds(timing_info.time_total_write_request));
  printf("__rpc_write_request: %"PRIu64",%"PRIu64"\n",
	 timing_info.nb_rpc_write_request, 
	 timespec2useconds(timing_info.time_rpc_write_request));
}

#endif /* !GETTIMING */

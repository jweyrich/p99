#include "orwl_timing.h"
#include "p99_defarg.h"
#include "p99_new.h"

static orwl_timing_element* head = 0;
static orwl_timing_element* end = 0;

void orwl_timing_element_insert(orwl_timing_element* el) {
  ORWL_CRITICAL {
    if (end) {
      end->next = el;
      end = el;
    } else {
      head = el;
      end = el;
    }
  }
}


static orwl_timing timing_info = P99_INIT;

orwl_timing * orwl_timing_info(void) {
  return &timing_info;
}

void orwl_timing_print_stats(void) {
  printf("__total_acquire: %"PRIu64",%"PRIu64"\n",
	 timing_info.total_acquire.nb,
	 timing_info.total_acquire.time);
  printf("__wait_on_cond_acquire: %"PRIu64",%"PRIu64"\n",
	 timing_info.wait_on_cond_acquire.nb,
	 timing_info.wait_on_cond_acquire.time);
  printf("__total_release: %"PRIu64",%"PRIu64"\n",
	 timing_info.total_release.nb,
	 timing_info.total_release.time);
  printf("__copy_data_release: %"PRIu64",%"PRIu64"\n",
	 timing_info.copy_data_release.nb,
	 timing_info.copy_data_release.time);
  printf("__send_data_release: %"PRIu64",%"PRIu64"\n",
	 timing_info.send_data_release.nb,
	 timing_info.send_data_release.time);
  printf("__total_read_request: %"PRIu64",%"PRIu64"\n",
	 timing_info.total_read_request.nb,
	 timing_info.total_read_request.time);
  printf("__rpc_read_request: %"PRIu64",%"PRIu64"\n",
	 timing_info.rpc_read_request.nb,
	 timing_info.rpc_read_request.time);
  printf("__total_write_request: %"PRIu64",%"PRIu64"\n",
	 timing_info.total_write_request.nb,
	 timing_info.total_write_request.time);
  printf("__rpc_write_request: %"PRIu64",%"PRIu64"\n",
	 timing_info.rpc_write_request.nb,
	 timing_info.rpc_write_request.time);
  for (orwl_timing_element* el = head; el; el = el->next)
    printf("__%s: %"PRIu64",%"PRIu64"\n",
           el->name ? el->name : "<unamed>",
           el->nb,
           el->time);
}

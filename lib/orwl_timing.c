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

static
void orwl_timing_element_print(FILE *out, orwl_timing_element* el) {
  fprintf(out,
          "__%s: %"PRIu64",%"PRIu64"\n",
          el->name ? el->name : "<unamed>",
          el->nb,
          el->time);
}


#define ORWL_TIMING_ELEMENT(NAME) .NAME = { .nb = 0, .name = #NAME }

static orwl_timing timing_info = {
  ORWL_TIMING_ELEMENT(total_acquire),
};

orwl_timing * orwl_timing_info(void) {
  return &timing_info;
}

void orwl_timing_print_stats(void) {
  orwl_timing_element_print(stderr, &timing_info.total_acquire);
  for (orwl_timing_element* el = head; el; el = el->next)
    orwl_timing_element_print(stderr, el);
}

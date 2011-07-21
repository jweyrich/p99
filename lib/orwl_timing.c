/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the authors and copyright holders for this work are as follows:           */
/* all rights reserved,  2011 Emmanuel Jeanvoine, INRIA, France              */
/* all rights reserved,  2011 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
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
  uint64_t nb = el->nb;
  if (nb) {
    atomic_float time = el->time;
    atomic_float mu = time/nb;
    atomic_float time2 = el->time2;
    atomic_float var = (time2 - (time * mu)) / nb;
    if (var < 0.0) var = 0.0;
    char const* name = el->name ? el->name : "<unamed>";
    fprintf(out,
            "TIMING: %-24s%10"PRIu64"\t%-8ss\t%-8ss\t%-8ss\n",
            name,
            nb,
            orwl_seconds2str(time),
            orwl_seconds2str(mu),
            orwl_seconds2str(sqrt(var))
           );
  }
}


#define ORWL_TIMING_ELEMENT(NAME) .NAME = { .nb = 0, .name = #NAME }

static orwl_timing timing_info = {
  ORWL_TIMING_ELEMENT(total_acquire),
  ORWL_TIMING_ELEMENT(total_write_map),
  ORWL_TIMING_ELEMENT(total_read_map),
  ORWL_TIMING_ELEMENT(total_truncate),
};

orwl_timing * orwl_timing_info(void) {
  return &timing_info;
}

void orwl_timing_print_stats(void) {
  fprintf(stderr,
          "TIMING: %-24s%10s\t%-8ss\t%-8ss\t%-8ss\n",
          "pt of measure",
          "n",
          "time",
          "time/n",
          "dev"
         );
  orwl_timing_element_print(stderr, &timing_info.total_acquire);
  orwl_timing_element_print(stderr, &timing_info.total_write_map);
  orwl_timing_element_print(stderr, &timing_info.total_read_map);
  orwl_timing_element_print(stderr, &timing_info.total_truncate);

  for (orwl_timing_element* el = head; el; el = el->next)
    orwl_timing_element_print(stderr, el);
}

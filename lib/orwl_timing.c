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
void orwl_timing_element_print(FILE *out, orwl_timing_element* el, int namelen, int nblen, bool human) {
  uint64_t nb = el->nb;
  if (nb) {
    atomic_float time = el->time;
    atomic_float mu = time/nb;
    atomic_float time2 = el->time2;
    atomic_float var = (time2 - (time * mu)) / nb;
    if (var < 0.0) var = 0.0;
    char const* name = el->name ? el->name : "<unamed>";
    if (human)
      fprintf(out,
            "TIMING: %-*s\t%*"PRIu64"\t%-8ss\t%-8ss\t%-8ss\n",
              namelen,
            name,
              nblen,
            nb,
            orwl_seconds2str(time),
            orwl_seconds2str(mu),
            orwl_seconds2str(sqrt(var))
           );
    else
      fprintf(out,
	    "TIMING: %-*s\t%*"PRIu64"\t%24.12f\t%24.12f\t%24.12f\n",
              namelen,
	    name,
              nblen,
	    nb,
	    time,
	    mu,
	    sqrt(var)
	    );
  }
}


#define O_RWL_TIMING_ELEMENT(NAME, X, I) .X = { .nb = 0, .name = P99_STRINGIFY(X) }

#define O_RWL_TIMING_ELEMENTS(...) P99_FOR(, P99_NARG(__VA_ARGS__), P00_SEQ, O_RWL_TIMING_ELEMENT, __VA_ARGS__)

static orwl_timing timing_info = {
  O_RWL_TIMING_ELEMENTS(ORWL_TIMING_LIST)
};

orwl_timing * orwl_timing_info(void) {
  return &timing_info;
}

#define O_RWL_PRINT(NAME, X, I) orwl_timing_element_print(stderr, &timing_info.X, namelen, nblen, human)
#define O_RWL_PRINTS(...) P99_FOR(, P99_NARG(__VA_ARGS__), P00_SEP, O_RWL_PRINT, __VA_ARGS__)

void orwl_timing_print_stats(void) {
  int namelen = 12;
  int nblen = 2;
  for (orwl_timing_element* el = head; el; el = el->next)
    if (el->name) {
      size_t l = strlen(el->name);
      if (l > namelen) namelen = l;
      size_t lg = (size_t)log10(el->nb);
      if (lg > nblen) nblen = lg;
    }
  ++nblen;
  char const* var = getenv("ORWL_TIMING");
  bool human = (var && var[0]);
  int slen =  human ? 9 : 24;
  fprintf(stderr,
          "TIMING: %-*s\t%*s\t%*s\t%*s\t%*s\n",
          namelen,
          "pt of measure",
          nblen,
          "n",
          slen,
          "time",
          slen,
          "time/n",
          slen,
          "dev"
         );
  O_RWL_PRINTS(ORWL_TIMING_LIST);
  for (orwl_timing_element* el = head; el; el = el->next)
    orwl_timing_element_print(stderr, el, namelen, nblen, human);
}

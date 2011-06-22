#ifndef   	ORWL_TIMING_H_
#define   	ORWL_TIMING_H_

#include "orwl_time.h"
#include "orwl_document.h"
#include "orwl_new.h"

P99_DECLARE_STRUCT(orwl_timing_element);

struct orwl_timing_element {
  uint64_t time;
  uint64_t nb;
  orwl_timing_element* next;
  char const* name;
};

void orwl_timing_element_insert(orwl_timing_element* );

P99_DECLARE_STRUCT(orwl_timing);

struct orwl_timing {
  orwl_timing_element total_acquire;
  orwl_timing_element wait_on_cond_acquire;

  orwl_timing_element total_release;
  orwl_timing_element copy_data_release;
  orwl_timing_element send_data_release;

  orwl_timing_element total_read_request;
  orwl_timing_element rpc_read_request;

  orwl_timing_element total_write_request;
  orwl_timing_element rpc_write_request;
};


orwl_timing * orwl_timing_info(void);

void orwl_timing_print_stats(void);

enum { orwl_timing_var = 0, orwl_timing_fetched = 0 };

/**
 ** @def ORWL_TIMING(NAME)
 ** @brief Timing of a statement or block
 **
 ** @remark Use this in inline functions that don't allow to declare
 ** @c static variables. For other places use ::ORWL_TIMER.
 **
 ** This macro is used as a prefix of a particular statement or block
 ** of code. @a NAME must correspond to one of the resources that are
 ** listed in the ::orwl_timing.
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
  P00_BLK_AFTER(atomic_fetch_add(&(orwl_timing_var->NAME.nb), 1))       \
  P00_BLK_AFTER(atomic_fetch_add(&(orwl_timing_var->NAME.time),         \
                                 timespec2useconds(timespec_diff(p00_start, p00_end)))) \
  P00_BLK_AFTER(p00_end = orwl_gettime())                               \
  P00_BLK_END
#else
# define ORWL_TIMING(NAME)
#endif

/**
 ** @def ORWL_TIMER(NAME)
 ** @brief Timing of a statement or block
 **
 ** @param NAME should be token but no identifier is declared or
 ** supposed to exist with that name.
 **
 ** This macro is used as a prefix of a particular statement or block
 ** of code.
 **
 ** Care is taken that the function calls are optimized
 ** - A static initialization is only run the first time that this
 **   place is encountered.
 ** - the two calls to ::orwl_gettime are as close as possible to the
 **   application code. I.e we have something like a sequence
 **   (1) ::orwl_gettime()
 **   (2) application code
 **   (3) ::orwl_gettime()
 **   (4) update of statistics
 **
 ** The update of the statistics are done with atomic operations such
 ** that all of this can be done without danger in a threaded
 ** environment.
 **
 ** @see ORWL_TIMING when the context doesn't allow to declare @c
 ** static variables, in particular @c inline functions.
 **/

#ifdef GETTIMING
# define ORWL_TIMER(NAME)                                               \
P00_BLK_START                                                           \
P00_BLK_BEFORE(register orwl_timing_element* elem = 0)                  \
P99_PREFER(                                                             \
           static orwl_timing_element p00_static_elem = { 0 };          \
           if (P99_UNLIKELY(!p00_static_elem.name)) {                   \
             ORWL_CRITICAL {                                            \
               if (P99_UNLIKELY(!p00_static_elem.name)) {               \
                 orwl_timing_element_insert(&p00_static_elem);          \
                 p00_static_elem.name = #NAME;                          \
               }                                                        \
             }                                                          \
           }                                                            \
           elem = &p00_static_elem;                                     \
           goto P99_LINEID(p00_label_, NAME);                           \
         ) P99_LINEID(p00_label_, NAME):                                \
  P00_BLK_DECL(struct timespec, p00_end)                                \
  P00_BLK_DECL(struct timespec, p00_start, orwl_gettime())              \
  P00_BLK_AFTER(atomic_fetch_add(&(elem->nb), 1))                       \
  P00_BLK_AFTER(atomic_fetch_add(&(elem->time),                         \
                                 timespec2useconds(timespec_diff(p00_start, p00_end)))) \
  P00_BLK_AFTER(p00_end = orwl_gettime())                               \
  P00_BLK_END
#else
# define ORWL_TIMER(NAME)
#endif


#endif 	    /* !ORWL_TIMING_H_ */

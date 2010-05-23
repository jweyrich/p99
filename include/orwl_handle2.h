/*
** orwl_handle2.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Fri May 21 22:22:06 2010 Jens Gustedt
** Last update Fri May 21 22:22:06 2010 Jens Gustedt
*/

#ifndef   	ORWL_HANDLE2_H_
# define   	ORWL_HANDLE2_H_

#include "orwl_remote_queue.h"

/**
 ** @brief A handle for periodic tasks
 **
 ** This knows basically two states, orwl_requested and orwl_acquired. If
 ** several handles are inserted into the same location they will
 ** always be acquired in the same cyclic order.
 ** @see orwl_acquire2 blocks until the state is orwl_acquired
 ** @see orwl_test2 can be used to know if an orwl_handle2 has been
 ** acquired asynchronously
 ** @see orwl_release2 inserts the handle at the end of the queue and
 ** frees the front of it.
 **/
struct orwl_handle2 {
  /** @brief is this an inclusive (read) request? */
  bool inclusive:1;
  /**
   ** @brief a time stamp to alternate according to its parity
   **
   ** This may just be an @c unsigned (and therefore a bit field)
   ** since we are usually only interested by the parity. Having it on
   ** some bits more might come handy when debugging, though.
   **/
  unsigned clock:(sizeof(unsigned)*8 - 1);
  /** the last known state of pair[i] */
  orwl_state state[2];
  /** the location to which this is inserted, if any */
  orwl_mirror* location;
  /** the two handles that are used in alternation */
  orwl_handle pair[2];
};

#ifndef __cplusplus
typedef struct orwl_handle2 orwl_handle2;
#endif

#define ORWL_HANDLE2_INITIALIZER                                \
{                                                               \
  .inclusive = false,                                           \
    .clock = 1,                                                 \
    .location = NULL,                                           \
    .state = { [0] = orwl_invalid, [1] = orwl_invalid },        \
    .pair = {                                                   \
    [0] = ORWL_HANDLE_INITIALIZER,                              \
    [1] = ORWL_HANDLE_INITIALIZER,                              \
  }                                                             \
}

inline
orwl_handle2 *orwl_handle2_init(orwl_handle2 *rh2, orwl_mirror* m, bool inc) {
  orwl_handle_init(&rh2->pair[0]);
  orwl_handle_init(&rh2->pair[1]);
  rh2->state[0] = orwl_invalid;
  rh2->state[1] = orwl_invalid;
  rh2->inclusive = false;
  rh2->clock = 1;
  rh2->location = NULL;
  return rh2;
}

inline
void orwl_handle2_destroy(orwl_handle2 *rh2) {
  orwl_handle_destroy(&rh2->pair[0]);
  orwl_handle_destroy(&rh2->pair[1]);
  rh2->inclusive = false;
  rh2->clock = 0;
  rh2->location = NULL;
}

inline
PROTOTYPE(orwl_handle2 *, orwl_handle2_init, orwl_handle2 *, orwl_mirror*, bool);

#define orwl_handle2_init(...) CALL_WITH_DEFAULTS(orwl_handle2_init, 3, __VA_ARGS__)

DECLARE_DEFARG(orwl_handle2_init, , NULL, false);

DECLARE_NEW_DELETE(orwl_handle2);

DECLARE_ORWL_TYPE_DYNAMIC(orwl_handle2);


orwl_state orwl_write_request2(orwl_mirror* location, /*!< [in,out] the location for the request */
                              orwl_handle2* handle,   /*!< [in,out] the handle for the request */
                              rand48_t* seed         /*!< [in] defaults to a thread local seed */
                              );

orwl_state orwl_read_request2(orwl_mirror* location, /*!< [in,out] the location for the request */
                             orwl_handle2* handle,   /*!< [in,out] the handle for the request */
                             rand48_t* seed         /*!< [in] defaults to a thread local seed */
                             );

orwl_state orwl_release2(orwl_handle2* handle,   /*!< [in,out] the handle to be released */
                        rand48_t* seed         /*!< [in] defaults to a thread local seed */
                        );

orwl_state orwl_cancel2(orwl_handle2* handle,   /*!< [in,out] the handle to be canceled */
                       rand48_t* seed         /*!< [in] defaults to a thread local seed */
                       );

orwl_state orwl_acquire2(orwl_handle2* handle,   /*!< [in,out] the handle to be acquired */
                         rand48_t* seed         /*!< [in] defaults to a thread local seed */
                         );
orwl_state orwl_test2(orwl_handle2* handle,   /*!< [in,out] the handle to be tested */
                      rand48_t* seed         /*!< [in] defaults to a thread local seed */
                      );

#ifndef DOXYGEN
PROTOTYPE(orwl_state, orwl_write_request2, orwl_mirror*, orwl_handle2*, rand48_t*);
#define orwl_write_request2(...)  CALL_WITH_DEFAULTS(orwl_write_request2, 3, __VA_ARGS__)
DECLARE_DEFARG(orwl_write_request2, , , seed_get());

PROTOTYPE(orwl_state, orwl_read_request2, orwl_mirror*, orwl_handle2*, rand48_t*);
#define orwl_read_request2(...)  CALL_WITH_DEFAULTS(orwl_read_request2, 3, __VA_ARGS__)
DECLARE_DEFARG(orwl_read_request2, , , seed_get());

PROTOTYPE(orwl_state, orwl_release2, orwl_handle2*, rand48_t*);
#define orwl_release2(...)  CALL_WITH_DEFAULTS(orwl_release2, 2, __VA_ARGS__)
DECLARE_DEFARG(orwl_release2, , seed_get());

PROTOTYPE(orwl_state, orwl_cancel2, orwl_handle2*, rand48_t*);
#define orwl_cancel2(...)  CALL_WITH_DEFAULTS(orwl_cancel2, 2, __VA_ARGS__)
DECLARE_DEFARG(orwl_cancel2, , seed_get());

PROTOTYPE(orwl_state, orwl_acquire2, orwl_handle2*, rand48_t*);
#define orwl_acquire2(...)  CALL_WITH_DEFAULTS(orwl_acquire2, 2, __VA_ARGS__)
DECLARE_DEFARG(orwl_acquire2, , seed_get());

PROTOTYPE(orwl_state, orwl_test2, orwl_handle2*, rand48_t*);
#define orwl_test2(...)  CALL_WITH_DEFAULTS(orwl_test2, 2, __VA_ARGS__)
DECLARE_DEFARG(orwl_test2, , seed_get());

#endif




#endif 	    /* !ORWL_HANDLE2_H_ */

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
#include P99_ADVANCE_ID

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
  /** the two handles that are used in alternation */
  orwl_handle pair[2];
};

#define ORWL_HANDLE2_INITIALIZER                                \
{                                                               \
  .inclusive = false,                                           \
    .clock = 1,                                                 \
    .state = { [0] = orwl_invalid, [1] = orwl_invalid },        \
    .pair = {                                                   \
    [0] = ORWL_HANDLE_INITIALIZER,                              \
    [1] = ORWL_HANDLE_INITIALIZER,                              \
  }                                                             \
}

inline
P99_PROTOTYPE(orwl_handle2 *, orwl_handle2_init, orwl_handle2 *, orwl_mirror*, bool);
#define orwl_handle2_init(...) P99_CALL_DEFARG(orwl_handle2_init, 3, __VA_ARGS__)
#define orwl_handle2_init_defarg_1() NULL
#define orwl_handle2_init_defarg_2() false


P99_DEFARG_DOCU(orwl_handle2_init)
DOCUMENT_INIT(orwl_handle2)
inline
orwl_handle2 *orwl_handle2_init(orwl_handle2 *rh2, orwl_mirror* m, bool inc) {
  if (!rh2) return NULL;
  orwl_handle_init(&rh2->pair[0]);
  orwl_handle_init(&rh2->pair[1]);
  rh2->state[0] = orwl_invalid;
  rh2->state[1] = orwl_invalid;
  rh2->inclusive = false;
  rh2->clock = 1;
  return rh2;
}

DOCUMENT_DESTROY(orwl_handle2)
inline
void orwl_handle2_destroy(orwl_handle2 *rh2) {
  orwl_handle_destroy(&rh2->pair[0]);
  orwl_handle_destroy(&rh2->pair[1]);
  rh2->inclusive = false;
  rh2->clock = 0;
  //rh2->location = NULL;
}


DECLARE_NEW_DELETE(orwl_handle2);

DECLARE_ORWL_TYPE_DYNAMIC(orwl_handle2);


P99_DEFARG_DOCU(orwl_write_request2)
orwl_state orwl_write_request2(orwl_mirror* location, /*!< [in,out] the location for the request */
                              orwl_handle2* rh2,   /*!< [in,out] the handle for the request */
                              rand48_t* seed         /*!< [in,out] defaults to a thread local seed */
                              );

P99_DEFARG_DOCU(orwl_read_request2)
orwl_state orwl_read_request2(orwl_mirror* location, /*!< [in,out] the location for the request */
                             orwl_handle2* rh2,   /*!< [in,out] the handle for the request */
                             rand48_t* seed         /*!< [in,out] defaults to a thread local seed */
                             );

P99_DEFARG_DOCU(orwl_release2)
orwl_state orwl_release2(orwl_handle2* rh2,   /*!< [in,out] the handle to be released */
                        rand48_t* seed         /*!< [in,out] defaults to a thread local seed */
                        );

P99_DEFARG_DOCU(orwl_cancel2)
orwl_state orwl_cancel2(orwl_handle2* rh2,   /*!< [in,out] the handle to be canceled */
                       rand48_t* seed         /*!< [in,out] defaults to a thread local seed */
                       );

P99_DEFARG_DOCU(orwl_acquire2)
orwl_state orwl_acquire2(orwl_handle2* rh2,   /*!< [in,out] the handle to be acquired */
                         rand48_t* seed         /*!< [in,out] defaults to a thread local seed */
                         );

P99_DEFARG_DOCU(orwl_test2)
orwl_state orwl_test2(orwl_handle2* rh2,   /*!< [in,out] the handle to be tested */
                      rand48_t* seed         /*!< [in,out] defaults to a thread local seed */
                      );

P99_DEFARG_DOCU(orwl_map2)
void orwl_map2(orwl_handle2* rh2,   /*!< [in,out] the handle to be queried */
               uint64_t** data,         /*!< [out] place a pointer to the
                                       data, here */
               size_t* data_len,    /*!< [out] length of @a data in
                                       number of elements */
               rand48_t* seed       /*!< [in,out] defaults to a thread local seed */
               );

P99_DEFARG_DOCU(orwl_resize2)
void orwl_resize2(orwl_handle2* rh2,   /*!< [in,out] the handle whos
                                          data to resize */
                  size_t data_len,     /*!< [in] future length of @a data in
                                         number of elements */
                  rand48_t* seed       /*!< [in,out] defaults to a thread local seed */
                  );




#ifndef DOXYGEN
P99_PROTOTYPE(orwl_state, orwl_write_request2, orwl_mirror*, orwl_handle2*, rand48_t*);
#define orwl_write_request2(...)  P99_CALL_DEFARG(orwl_write_request2, 3, __VA_ARGS__)
#define orwl_write_request2_defarg_2() seed_get()

P99_PROTOTYPE(orwl_state, orwl_read_request2, orwl_mirror*, orwl_handle2*, rand48_t*);
#define orwl_read_request2(...)  P99_CALL_DEFARG(orwl_read_request2, 3, __VA_ARGS__)
#define orwl_read_request2_defarg_2() seed_get()

P99_PROTOTYPE(orwl_state, orwl_release2, orwl_handle2*, rand48_t*);
#define orwl_release2(...)  P99_CALL_DEFARG(orwl_release2, 2, __VA_ARGS__)
#define orwl_release2_defarg_1() seed_get()

P99_PROTOTYPE(orwl_state, orwl_cancel2, orwl_handle2*, rand48_t*);
#define orwl_cancel2(...)  P99_CALL_DEFARG(orwl_cancel2, 2, __VA_ARGS__)
#define orwl_cancel2_defarg_1() seed_get()

P99_PROTOTYPE(orwl_state, orwl_acquire2, orwl_handle2*, rand48_t*);
#define orwl_acquire2(...)  P99_CALL_DEFARG(orwl_acquire2, 2, __VA_ARGS__)
#define orwl_acquire2_defarg_1() seed_get()

P99_PROTOTYPE(orwl_state, orwl_test2, orwl_handle2*, rand48_t*);
#define orwl_test2(...)  P99_CALL_DEFARG(orwl_test2, 2, __VA_ARGS__)
#define orwl_test2_defarg_1() seed_get()

#endif




#endif 	    /* !ORWL_HANDLE2_H_ */

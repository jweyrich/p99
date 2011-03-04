/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
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

#define ORWL_HANDLE2_INITIALIZER                               \
{                                                              \
  .inclusive = false,                                          \
    .clock = 1,                                                \
    .state = { [0] = orwl_invalid, [1] = orwl_invalid },       \
    .pair = {                                                  \
    [0] = ORWL_HANDLE_INITIALIZER,                             \
    [1] = ORWL_HANDLE_INITIALIZER,                             \
  }                                                            \
}

/**
 ** @brief Dynamically initialize a ::orwl_handle2
 **
 ** @param rh2 is the handle to be initialized
 ** @param m is an ::orwl_mirror that designates the location of the
 **        handle. It defaults to a null pointer, i.e no link.
 **/
DOCUMENT_INIT(orwl_handle2)
inline
orwl_handle2 *orwl_handle2_init(orwl_handle2 *rh2) {
  if (!rh2) return 0;
  *rh2 = (orwl_handle2 const)ORWL_HANDLE2_INITIALIZER;
  return rh2;
}

DOCUMENT_DESTROY(orwl_handle2)
inline
void orwl_handle2_destroy(orwl_handle2 *rh2) {
  orwl_handle_destroy(&rh2->pair[0]);
  orwl_handle_destroy(&rh2->pair[1]);
  rh2->inclusive = false;
  rh2->clock = 0;
}


DECLARE_NEW_DELETE(orwl_handle2);

DECLARE_ORWL_TYPE_DYNAMIC(orwl_handle2);


/**
 ** @memberof orwl_mirror
 **/
P00_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_write_request2)
orwl_state orwl_write_request2(orwl_mirror* location, /*!< [in,out] the location for the request */
                              orwl_handle2* rh2,   /*!< [in,out] the handle for the request */
                              rand48_t* seed         /*!< [in,out] defaults to a thread local seed */
                              );

/**
 ** @memberof orwl_mirror
 **/
P00_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_read_request2)
orwl_state orwl_read_request2(orwl_mirror* location, /*!< [in,out] the location for the request */
                             orwl_handle2* rh2,   /*!< [in,out] the handle for the request */
                             rand48_t* seed         /*!< [in,out] defaults to a thread local seed */
                             );

/**
 ** @memberof orwl_handle2
 **/
P00_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_release2)
orwl_state orwl_release2(orwl_handle2* rh2,   /*!< [in,out] the handle to be released */
                        rand48_t* seed         /*!< [in,out] defaults to a thread local seed */
                        );

/**
 ** @memberof orwl_handle2
 **/
P00_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_cancel2)
orwl_state orwl_cancel2(orwl_handle2* rh2,   /*!< [in,out] the handle to be canceled */
                       rand48_t* seed         /*!< [in,out] defaults to a thread local seed */
                       );

/**
 ** @memberof orwl_handle2
 **/
P00_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_acquire2)
orwl_state orwl_acquire2(orwl_handle2* rh2,   /*!< [in,out] the handle to be acquired */
                         rand48_t* seed         /*!< [in,out] defaults to a thread local seed */
                         );

/**
 ** @memberof orwl_handle2
 **/
P00_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_test2)
orwl_state orwl_test2(orwl_handle2* rh2,   /*!< [in,out] the handle to be tested */
                      rand48_t* seed         /*!< [in,out] defaults to a thread local seed */
                      );

/**
 ** @memberof orwl_handle2
 **/
P00_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_map2)
uint64_t* orwl_map2(orwl_handle2* rh2,   /*!< [in,out] the handle to be queried */
               size_t* data_len,    /*!< [out] length of @a data in
                                       number of elements */
               rand48_t* seed       /*!< [in,out] defaults to a thread local seed */
               );

/**
 ** @memberof orwl_handle2
 **/
P00_DOCUMENT_SEED
P99_DEFARG_DOCU(orwl_mapro2)
uint64_t const* orwl_mapro2(orwl_handle2* rh2,   /*!< [in,out] the handle to be queried */
               size_t* data_len,    /*!< [out] length of @a data in
                                       number of elements */
               rand48_t* seed       /*!< [in,out] defaults to a thread local seed */
               );

/**
 ** @memberof orwl_handle2
 **/
P00_DOCUMENT_SEED
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

P99_PROTOTYPE(uint64_t*, orwl_map2, orwl_handle2*, size_t*, rand48_t*);
#define orwl_map2(...)  P99_CALL_DEFARG(orwl_map2, 3, __VA_ARGS__)
#define orwl_map2_defarg_1() 0
#define orwl_map2_defarg_2() seed_get()

P99_PROTOTYPE(uint64_t const*, orwl_mapro2, orwl_handle2*, size_t*, rand48_t*);
#define orwl_mapro2(...)  P99_CALL_DEFARG(orwl_mapro2, 3, __VA_ARGS__)
#define orwl_mapro2_defarg_1() 0
#define orwl_mapro2_defarg_2() seed_get()

P99_PROTOTYPE(void, orwl_resize2, orwl_handle2*, size_t, rand48_t*);
#define orwl_resize2(...)  P99_CALL_DEFARG(orwl_resize2, 3, __VA_ARGS__)
#define orwl_resize2_defarg_2() seed_get()

#endif


DECLARE_ORWL_REGISTER_ALIAS(orwl_acquire2, orwl_handle2);
DECLARE_ORWL_REGISTER_ALIAS(orwl_release2, orwl_handle2);
DECLARE_ORWL_REGISTER_ALIAS(orwl_cancel2, orwl_handle2);


#endif 	    /* !ORWL_HANDLE2_H_ */

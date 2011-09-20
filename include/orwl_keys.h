/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2011 Jens Gustedt, INRIA, France                    */
/*                                                                           */
/* This file is part of the P99 project. You received this file as as        */
/* part of a confidential agreement and you may generally not                */
/* redistribute it and/or modify it, unless under the terms as given in      */
/* the file LICENSE.  It is distributed without any warranty; without        */
/* even the implied warranty of merchantability or fitness for a             */
/* particular purpose.                                                       */
/*                                                                           */
#include "orwl_once.h"
#include "p99_defarg.h"

/** @brief return the total number of locations that are reserved for
 ** this program
 **
 ** @see ORWL_KEYS on how to reserve global names of keys
 ** @see ORWL_KEY on how to obtain a particular key index
 **
 ** @warning You must have scaled all keys that need more than one
 ** location with by a call to #ORWL_KEY_SCALE before a call to this
 ** function.
 **
 ** Usually you will be only interested once in that number, namely as
 ** a third argument to #orwl_start.
 **/
size_t orwl_keys_total(void);

/** @brief Declare the series of ORWL keys for locations that is used
 ** in this executable
 **
 ** This must be used at most once in some header file. If #ORWL_KEYS
 ** is used there must a corresponding #ORWL_KEYS_DEFINE in exactly
 ** one .c file such that the necessary are implemented.
 ** @see ORWL_KEY_SCALE to have several locations reserved per key
 ** @see ORWL_KEY to obtain the key for a particular name and position
 **
 ** The names used in the argument list to this macro will be declared
 ** as global identifiers for the program, so you will not be able to
 ** use them for other purposes than key management.
 **
 ** E.g a number crunching program could use parameters, matrix blocks
 ** and results that it would want to manage with #orwl_mirror and
 ** access with #orwl_handle or #orwl_handle2.
 **
 ** @code
 ** ORWL_KEYS(parameters, block, results);
 ** @endcode
 **
 ** At the beginning of the @c main function it should then condition
 ** the number of locations that it wants under each key. In our
 ** example suppose that each process will need access to the
 ** parameters of all other processes at the same time, it will only
 ** hold one matrix block and there will be two different objects on
 ** each process that holds a result.
 **
 ** @code
 ** ORWL_KEY_SCALE(parameters, NbPE);
 ** ORWL_KEY_SCALE(block, 1);        // Optional is the default
 ** ORWL_KEY_SCALE(results, 2);
 ** orwl_server srv = P99_INIT;
 ** orwl_start(&srv, SOMAXCONN, orwl_keys_total());
 ** if (!orwl_alive(&srv)) return EXIT_FAILURE;
 ** @endcode
 **
 ** Now that the orwl server is up and running you may use #ORWL_KEY
 ** to connect an orwl_mirror to a particular location.
 **
 ** @code
 ** // First connect our matrix block locally to this server
 ** orwl_mirror block_location;
 ** orwl_endpoint here = srv.host.ep;
 ** orwl_mirror_connect(&block_location, &srv, here, ORWL_KEY(block));
 **
 ** // Now connect the parameters to their position on the central
 ** // process that is identified through argv[1]
 ** orwl_mirror param_location[NbPE];
 ** orwl_endpoint there = orwl_endpoint_get(argv[1]);
 ** for (size_t i = 0; i < NbPE; ++i) {
 **   orwl_mirror_connect(&param_location[i], &srv, there, ORWL_KEY(parameters, i));
 ** }
 ** @code
 **/
#define ORWL_KEYS(...)                                         \
DECLARE_ONCE(o_rwl_keys);                                      \
enum { O_RWL_KEYS_NB = P99_NARG(__VA_ARGS__), };               \
extern size_t o_rwl_key_offset[O_RWL_KEYS_NB + 1];             \
extern size_t o_rwl_key_nb[O_RWL_KEYS_NB];                     \
typedef enum o_rwl_key_enum { __VA_ARGS__ } o_rwl_key_enum

/** @brief Define the symbols that are necessary for #ORWL_KEYS
 **/
#define ORWL_KEYS_DEFINE()                                           \
DEFINE_ONCE(o_rwl_keys) {                                            \
  o_rwl_key_offset[0] = 0;                                           \
  for (size_t i = 0; i < O_RWL_KEYS_NB; ++i) {                       \
    if (!o_rwl_key_nb[i]) o_rwl_key_nb[i] = 1;                       \
    o_rwl_key_offset[i + 1] = o_rwl_key_offset[i] + o_rwl_key_nb[i]; \
  }                                                                  \
}                                                                    \
size_t orwl_keys_total(void) {                                       \
  INIT_ONCE(o_rwl_keys);                                             \
  return o_rwl_key_offset[O_RWL_KEYS_NB];                            \
}                                                                    \
size_t o_rwl_key_offset[O_RWL_KEYS_NB + 1];                          \
size_t o_rwl_key_nb[O_RWL_KEYS_NB]


/** @brief reserve @a SIZE slots for key @a NAME
 **/
#define ORWL_KEY_SCALE(NAME, SIZE) (o_rwl_key_nb[NAME] = (SIZE))

#define O_RWL_KEY2(NAME, I) (o_rwl_key_offset[NAME] + (I))
#define O_RWL_KEY1(NAME) (o_rwl_key_offset[NAME])

/** @brief obtain the key for name and position
 **
 ** position defaults to 0 if it is omitted
 **/
#define ORWL_KEY(...) P99_IF_LT(P99_NARG(__VA_ARGS__), 2)(O_RWL_KEY1(__VA_ARGS__))(O_RWL_KEY2(__VA_ARGS__))

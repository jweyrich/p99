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
#ifndef   	ORWL_NEW_H_
# define   	ORWL_NEW_H_

/**
 ** @file orwl_new.h
 **
 ** @brief implement poor mans constructors, destructors, @c new and
 ** @c delete.
 */

#include "p99_new.h"
#include "p99_defarg.h"

/**
 ** @brief Declare a `delete' operator for type @a T.
 **
 ** This supposes that type @a T has a `destructor', i.e a destroy
 ** function, that just takes a pointer to the element that is to be
 ** initialized.
 **
 ** Use the corresponding function from ::DECLARE_NEW to free a
 ** variable that you have such allocated.
 ** @see DECLARE_NEW_DELETE
 **/
#define DECLARE_DELETE(T)                                                                                   \
/*! @brief Operator @c delete for type T   **/                                                              \
  /*! @attention @ref T ## _destroy  is supposed to exist and to be callable with just one T * argument **/ \
  /*! @attention @a el show have been allocated through P99_NEW */                                          \
  /*! @see P99_NEW */                                                                                       \
  /*! @memberof T */                                                                                        \
inline                                                                                                      \
void P99_PASTE2(T, _delete)(T const*el) {                                                                   \
  if (el) {                                                                                                 \
    T* e = (T*)el;                                                                                          \
    P99_PASTE2(T, _destroy)(e);                                                                             \
    free((void*)e);                                                                                         \
  }                                                                                                         \
}

inline
size_t p00_blocks(size_t n, size_t stride) {
  return (n / stride) + ((n % stride) ? 1u : 0u);
}

inline
size_t p00_vnew_size_len(size_t n) {
  return p00_blocks(n, sizeof(size_t));
}

inline
void *p00_vnew(size_t n) {
  /* add one for the header */
  size_t *ret = (size_t*)calloc(p00_vnew_size_len(n) + 1, sizeof(size_t));
  if (!ret) return 0;
  ret[0] = n;
  return ret + 1;
}

inline
size_t const*p00_vfind(void const*p) {
  return ((size_t const*)p) - 1;
}

inline
size_t p00_vlen(void const*p) {
  return (p00_vfind(p))[0];
}

inline
void p00_vdelete(void const*p) {
  free((void*)p00_vfind(p));
}

/**
 ** @brief Declare a `new[]' operator for type @a T.
 **
 ** This supposes that type @a T has a `constructor', i.e and init
 ** function, that just takes a pointer to the element that is to be
 ** initialized.
 **/
#define DECLARE_VNEW(T)                                                                                 \
/*! @brief Operator @c new[] for type T   **/                                                           \
  /*! @attention @ref T ## _init  is supposed to exist and to be callable with just one T* argument **/ \
  /*! @attention @ref T ## _vdelete @b must be used to de-allocate such a variable **/                  \
  /*! @memberof T */                                                                                    \
inline                                                                                                  \
T *P99_PASTE2(T, _vnew)(size_t n) {                                                                     \
  size_t N = n*sizeof(T);                                                                               \
  T *ret = p00_vnew(N);                                                                                 \
  if (ret) {                                                                                            \
    for (size_t i = 0; i < n; ++i) {                                                                    \
      P99_PASTE2(T, _init)(ret + i);                                                                    \
    }                                                                                                   \
  }                                                                                                     \
  return ret;                                                                                           \
}

#define DECLARE_VDELETE(T)                                                                                 \
/*! @brief Operator @c delete[] for type T **/                                                             \
  /*! @attention @ref T ## _destroy  is supposed to exist and to be callable with just one T* argument **/ \
  /*! @attention @ref T ## _vnew @b must have been used to allocate this variable **/                      \
  /*! @memberof T */                                                                                       \
inline                                                                                                     \
void P99_PASTE2(T, _vdelete)(T const*vec) {                                                                \
  if (vec) {                                                                                               \
    size_t n = p00_vlen(vec) / sizeof(T);                                                                  \
    for (T *ve = (T*)vec, *stop = ve + n; ve < stop; ++ve) {                                               \
      P99_PASTE2(T, _destroy)(ve);                                                                         \
    }                                                                                                      \
    p00_vdelete(vec);                                                                                      \
  }                                                                                                        \
}

#define DECLARE_NEW_DELETE(T)                                                              \
DECLARE_DELETE(T)                                                                          \
DECLARE_VDELETE(T)                                                                         \
DECLARE_VNEW(T)                                                                            \
 enum P99_PASTE2(p00_tame_ansi_c_semicolon_message_, T) { P99_PASTE2(p00_new_delete_, T) }

#define DEFINE_DELETE(T) P99_INSTANTIATE(void, P99_PASTE2(T, _delete), T const*)
#define DEFINE_VNEW(T) P99_INSTANTIATE(T*, P99_PASTE2(T, _vnew), size_t)
#define DEFINE_VDELETE(T) P99_INSTANTIATE(void, P99_PASTE2(T, _vdelete), T const*)

#define DEFINE_NEW_DELETE(T)                                   \
DEFINE_VNEW(T);                                                \
DEFINE_DELETE(T);                                              \
DEFINE_VDELETE(T)


#endif 	    /* !ORWL_NEW_H_ */

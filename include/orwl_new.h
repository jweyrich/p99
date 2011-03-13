/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* all rights reserved,  2010-2011 Jens Gustedt, INRIA, France               */
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

P99_DECLARE_STRUCT(p00_vheader);
P99_DECLARE_UNION(p00_maxalign);

/* A union type to assure the largest possible alignment. */
union p00_maxalign {
  void* v;
  void (*f)(void);
  long double d;
  uintmax_t u;
};

/* A header structure for vector allocations. Align the flexible array
   to the largest possible alignment. */
struct p00_vheader {
  size_t len;
  p00_maxalign data[];
};

#define P00_VDATA(H) ((void*)((H)->data))
#define P00_VLENG(H) ((H)->len)

/**
 ** @brief Declare vector (de)allocation operators for type @a T.
 **
 ** @attention @ref A ..._destroy function is supposed to exist and to be
 ** callable with just one @a T* argument
 ** @attention @ref A ..._init function is supposed to exist and to be
 ** callable with just one @a T* argument.
 **
 ** This declares four functions for type @a T:
 **  - ..._vnew to allocate a vector of a given size and initialize
 **    the individual items
 **  - ..._vdelete to deallocate and destroy the items individually
 **  - ..._vrealloc that combines both in the spirit of @c realloc
 **  - ..._delete that deletes an individual item
 ** @see P99_NEW for a macro that allocates and intializes
 **/
#define DECLARE_NEW_DELETE(T)                                                                                        \
DECLARE_DELETE(T)                                                                                                    \
/*! @brief Reallocation operator for type T **/                                                                      \
/*! Semantics are similar as for @c realloc. That is **/                                                             \
/*!  - if @a n is @c 0, this frees the allocated space **/                                                           \
/*!  - if @a p is a null pointer constant this allocates the requested number of items **/                           \
/*!  - if none of the above, the array may be moved and the contents of the initial segment is preserved **/         \
/*! In addition to @c realloc semantics newly allocated items are initialized and discarded items are destroyed. **/ \
/*! @attention @ref T ## _destroy  is supposed to exist and to be callable with just one T* argument **/             \
/*! @attention @ref T ## _init  is supposed to exist and to be callable with just one T* argument **/                \
/*! @attention @ref T ## _realloc @b must have been used to allocate @a p if it is non-null **/                      \
/*! @memberof T */                                                                                                   \
inline                                                                                                               \
T *P99_PASTE2(T, _vrealloc)(T* p, size_t const n) {                                                                  \
  if (P99_UNLIKELY(!p && !n)) return 0;                                                                              \
  size_t o = 0;                                                                                                      \
  p00_vheader* head = 0;                                                                                             \
  if (p) {                                                                                                           \
    head = P99_FHEAD(p00_vheader, data, p);                                                                          \
    o = P00_VLENG(head) / sizeof(T);                                                                                 \
    for (size_t i = n; i < o; ++i)                                                                                   \
      P99_PASTE2(T, _destroy)(p + i);                                                                                \
    if (P99_LIKELY(!n)) {                                                                                            \
      free(head);                                                                                                    \
      return 0;                                                                                                      \
    }                                                                                                                \
  }                                                                                                                  \
  /* From here on n is not 0. */                                                                                     \
  size_t N = n*sizeof(T);                                                                                            \
  p00_vheader* headn = P00_FREALLOC(head, p00_vheader, data, N);                                                     \
  if (P99_LIKELY(!!headn)) {                                                                                         \
    head = headn;                                                                                                    \
    p = P00_VDATA(head);                                                                                             \
    for (size_t i = o; i < n; ++i)                                                                                   \
      P99_PASTE2(T, _init)(p + i);                                                                                   \
  } else                                                                                                             \
    /* even if realloc fails we still might have a good value for p if we were trying to shorten the array. */       \
    if (o < n) return 0;                                                                                             \
  P00_VLENG(head) = N;                                                                                               \
  return p;                                                                                                          \
}                                                                                                                    \
/*! @brief Operator @c new[] for type T   **/                                                                        \
  /*! @attention @ref T ## _init  is supposed to exist and to be callable with just one T* argument **/              \
  /*! @attention @ref T ## _vdelete @b must be used to de-allocate such a variable **/                               \
  /*! @memberof T */                                                                                                 \
inline                                                                                                               \
T *P99_PASTE2(T, _vnew)(size_t n) {                                                                                  \
  return P99_PASTE2(T, _vrealloc)(0, n);                                                                             \
}                                                                                                                    \
/*! @brief Operator @c delete[] for type T **/                                                                       \
  /*! @attention @ref T ## _destroy  is supposed to exist and to be callable with just one T* argument **/           \
  /*! @attention @ref T ## _vnew @b must have been used to allocate this variable **/                                \
  /*! @memberof T */                                                                                                 \
inline                                                                                                               \
void P99_PASTE2(T, _vdelete)(T const*vec) {                                                                          \
  P99_PASTE2(T, _vrealloc)((T*)vec, 0);                                                                              \
}                                                                                                                    \
P99_MACRO_END(DECLARE_NEW_DELETE_, T)

#define DEFINE_DELETE(T) P99_INSTANTIATE(void, P99_PASTE2(T, _delete), T const*)

#define DEFINE_NEW_DELETE(T)                                     \
P99_INSTANTIATE(T*, P99_PASTE2(T, _vnew), size_t);               \
P99_INSTANTIATE(T*, P99_PASTE2(T, _vrealloc), T* old, size_t n); \
P99_INSTANTIATE(void, P99_PASTE2(T, _vdelete), T const*);        \
DEFINE_DELETE(T)



#endif 	    /* !ORWL_NEW_H_ */

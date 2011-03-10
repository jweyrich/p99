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
  double d;
  uintmax_t u;
};

/* A header structure for vector allocations. Align the flexible array
   to the largest possible alignment. */
struct p00_vheader {
  size_t len;
  p00_maxalign data[];
};

/**
 ** @brief Declare a `new[]' operator for type @a T.
 **
 ** This supposes that type @a T has a `constructor', i.e and init
 ** function, that just takes a pointer to the element that is to be
 ** initialized.
 **/
#define DECLARE_VNEW(T)                                                                                 \
inline                                                                                                  \
T *P99_PASTE2(T, _vrealloc)(T* old, size_t n);                                                          \
/*! @brief Operator @c new[] for type T   **/                                                           \
  /*! @attention @ref T ## _init  is supposed to exist and to be callable with just one T* argument **/ \
  /*! @attention @ref T ## _vdelete @b must be used to de-allocate such a variable **/                  \
  /*! @memberof T */                                                                                    \
inline                                                                                                  \
T *P99_PASTE2(T, _vnew)(size_t n) {                                                                     \
  return P99_PASTE2(T, _vrealloc)(0, n);                                                                \
}

#define DECLARE_VDELETE(T)                                                                                           \
inline                                                                                                               \
T *P99_PASTE2(T, _vrealloc)(T* old, size_t n);                                                                       \
/*! @brief Operator @c delete[] for type T **/                                                                       \
  /*! @attention @ref T ## _destroy  is supposed to exist and to be callable with just one T* argument **/           \
  /*! @attention @ref T ## _vnew @b must have been used to allocate this variable **/                                \
  /*! @memberof T */                                                                                                 \
inline                                                                                                               \
void P99_PASTE2(T, _vdelete)(T const*vec) {                                                                          \
  P99_PASTE2(T, _vrealloc)((T*)vec, 0);                                                                              \
}                                                                                                                    \
                                                                                                                     \
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
T *P99_PASTE2(T, _vrealloc)(T* p, size_t n) {                                                                        \
  size_t o = 0;                                                                                                      \
  p00_vheader* head = 0;                                                                                             \
  if (p) {                                                                                                           \
    head = (p00_vheader*)((char*)p - offsetof(p00_vheader, data));                                                   \
    o = head->len / sizeof(T);                                                                                       \
    for (size_t i = n; i < o; ++i) {                                                                                 \
      P99_PASTE2(T, _destroy)(p + i);                                                                                \
    }                                                                                                                \
    if (P99_LIKELY(!n)) {                                                                                            \
      free(head);                                                                                                    \
      return 0;                                                                                                      \
    }                                                                                                                \
  }                                                                                                                  \
  if (n) {                                                                                                           \
    size_t N = n*sizeof(T);                                                                                          \
    /* re-assigning head is no pb, here, since we still have p */                                                    \
    head = realloc(head, sizeof(p00_vheader) + N);                                                                   \
    if (head) {                                                                                                      \
      *head = (p00_vheader){ .len = N };                                                                             \
      p = (T*)head->data;                                                                                            \
      for (size_t i = o; i < n; ++i) {                                                                               \
        P99_PASTE2(T, _init)(p + i);                                                                                 \
      }                                                                                                              \
    }                                                                                                                \
  }                                                                                                                  \
  return p;                                                                                                          \
}

#define DECLARE_NEW_DELETE(T)                                                              \
DECLARE_DELETE(T)                                                                          \
DECLARE_VDELETE(T)                                                                         \
DECLARE_VNEW(T)                                                                            \
 enum P99_PASTE2(p00_tame_ansi_c_semicolon_message_, T) { P99_PASTE2(p00_new_delete_, T) }

#define DEFINE_DELETE(T) P99_INSTANTIATE(void, P99_PASTE2(T, _delete), T const*)
#define DEFINE_VNEW(T) P99_INSTANTIATE(T*, P99_PASTE2(T, _vnew), size_t)
#define DEFINE_VDELETE(T)                                        \
P99_INSTANTIATE(T*, P99_PASTE2(T, _vrealloc), T* old, size_t n); \
P99_INSTANTIATE(void, P99_PASTE2(T, _vdelete), T const*)

#define DEFINE_NEW_DELETE(T)                                   \
DEFINE_VNEW(T);                                                \
DEFINE_DELETE(T);                                              \
DEFINE_VDELETE(T)


#endif 	    /* !ORWL_NEW_H_ */

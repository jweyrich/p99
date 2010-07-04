/*
** orwl_new.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Mon Feb 22 14:37:07 2010 Jens Gustedt
** Last update Mon Feb 22 14:37:07 2010 Jens Gustedt
*/

#ifndef   	ORWL_NEW_H_
# define   	ORWL_NEW_H_

/**
 ** @file orwl_new.h
 **
 ** @brief implement poor mans constructors, destructors, @c new and
 ** @c delete.
 */

#include <stddef.h>
#include <stdlib.h>
#include <inttypes.h>


/**
 ** @brief Zero out all bits in the object that @a X points to.
 **
 ** @a X can be a pointer or an array
 ** @code
 ** T *A;
 ** T B[3];
 ** PZERO(A, 29);
 ** PZERO(B, 2);
 ** @endcode
 **
 ** Here the first call zeroes out 29 elements of type T starting at
 ** the memory where @c A points to. The second zeroes out the first
 ** two elements of the array @c B.
 **
 ** This is a macro not a function, but anyhow @a X is evaluated only
 ** once.
 **/
#define PZERO(X, N) (memset((X), 0, sizeof(X[0]) * N))

/**
 ** @brief Zero out all bits in the object @a X.
 **
 ** @a X must be an object from which the address can be taken, such
 ** as a normal variable or an array.
 **
 ** This is a macro not a function, but anyhow @a X is evaluated only
 ** once.
 **/
#define TZERO(X) (memset(&(X), 0, sizeof(X)))

#define P99__NEW(T) PASTE2(T, _init)((T*)malloc(sizeof(T)))

#define P99__NEW_ARGS(T, ...) PASTE2(T, _init)((T*)malloc(sizeof(T)), __VA_ARGS__)


/**
 ** @brief Allocate an element of type @c T as given by the first
 ** argument and initialize it with the remaining arguments, if any.
 **
 ** This suppose that a function or macro named @c T_init exists and
 ** is ready to take the arguments that are provided in addition to
 ** the pointer of type T (which comes first).
 **
 ** @see CALL_WITH_DEFAULTS
 **/
#define NEW(...) IF_DEC_LT(NARG(__VA_ARGS__), 2)(P99__NEW(__VA_ARGS__))(P99__NEW_ARGS(__VA_ARGS__))


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
#define DECLARE_DELETE(T)                                               \
/*! @brief Operator @c delete for type T   **/                          \
  /*! @attention @ref T ## _destroy  is supposed to exist and to be callable with just one T* argument **/ \
  /*! @attention @a el show have been allocated through @ref NEW */     \
inline                                                                  \
void PASTE2(T, _delete)(T const*el) {                                   \
  if (el) {                                                             \
    PASTE2(T, _destroy)((T*)el);                                        \
    free((void*)el);                                                    \
  }                                                                     \
}

inline
void *p99__vnew(size_t n) {
  /* allocate in multiples of the alignment */
  size_t const ali = sizeof(uintmax_t);
  size_t N = (n / ali);
  /* add one if n doesn't fit uintmax_t boundary */
  if (n % ali) ++N;
  /* add one for the header */
  uintmax_t *ret = (uintmax_t*)calloc(N + 1, ali);
  ret[0] = n;
  return ret + 1;
}

inline
uintmax_t const*p99__vfind(void const*p) {
  uintmax_t const* ret = ((uintmax_t const*)p) - 1;
  return ret;
}

inline
size_t p99__vlen(void const*p) {
  uintmax_t const*ret = p99__vfind(p);
  return ret[0];
}

inline
void p99__vdelete(void const*p) {
  free((void*)p99__vfind(p));
}

/**
 ** @brief Declare a `new[]' operator for type @a T.
 **
 ** This supposes that type @a T has a `constructor', i.e and init
 ** function, that just takes a pointer to the element that is to be
 ** initialized.
 **/
#define DECLARE_VNEW(T)                                                 \
/*! @brief Operator @c new[] for type T   **/                           \
  /*! @attention @ref T ## _init  is supposed to exist and to be callable with just one T* argument **/ \
  /*! @attention @ref T ## _vdelete @b must be used to de-allocate such a variable **/ \
inline                                                                  \
T *PASTE2(T, _vnew)(size_t n) {                                         \
  size_t N = n*sizeof(T);                                               \
  T *ret = p99__vnew(N);                                                \
  if (ret) {                                                            \
    for (size_t i = 0; i < n; ++i) {                                    \
      PASTE2(T, _init)(ret + i);                                        \
    }                                                                   \
  }                                                                     \
  return ret;                                                           \
}

#define DECLARE_VDELETE(T)                                              \
/*! @brief Operator @c delete[] for type T **/                          \
  /*! @attention @ref T ## _destroy  is supposed to exist and to be callable with just one T* argument **/ \
  /*! @attention @ref T ## _vnew @b must have been used to allocate this variable **/ \
inline                                                                  \
void PASTE2(T, _vdelete)(T const*vec) {                                 \
  if (vec) {                                                            \
    size_t n = p99__vlen(vec) / sizeof(T);                              \
    for (size_t i = 0; i < n; ++i) {                                    \
      PASTE2(T, _destroy)((T*)(vec + i));                               \
    }                                                                   \
    p99__vdelete(vec);                                                  \
  }                                                                     \
}

#define DECLARE_NEW_DELETE(T)                                    \
DECLARE_DELETE(T)                                                \
DECLARE_VDELETE(T)                                               \
DECLARE_VNEW(T)                                                  \
 enum PASTE(p99__tame_ansi_c_semicolon_message_, T) { PASTE2(p99__new_delete_, T) }

#define DEFINE_DELETE(T) void PASTE2(T, _delete)(T const*el)
#define DEFINE_VNEW(T) T * PASTE2(T, _vnew)(size_t n)
#define DEFINE_VDELETE(T) void PASTE2(T, _vdelete)(T const*vec)

#define DEFINE_NEW_DELETE(T)                   \
DEFINE_VNEW(T);                                \
DEFINE_DELETE(T);                              \
DEFINE_VDELETE(T)


#endif 	    /* !ORWL_NEW_H_ */

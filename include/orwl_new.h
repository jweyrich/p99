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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "orwl_inline.h"


/**
 ** @brief Add some indications to a @c _init documentation.
 */
#define DOCUMENT_INIT(T) /*! @brief Initialize a variable of type T @see T ## _new needs a version of this that takes just the T* as argument. */

/**
 ** @brief Add some indications to a @c _destroy documentation.
 */
#define DOCUMENT_DESTROY(T) /*! @brief Destroy a variable of type T @see T ## _delete needs this. */

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

/**
 ** @brief Allocate and initialize an element of type @a T
 **/
#define NEW(T) T ## _init((T*)malloc(sizeof(T)))

/**
 ** @brief Allocate an element of type @a T and initialize it with the
 ** remaining arguments.
 **/
#define NEW_INIT(T, ...) T ## _init((T*)malloc(sizeof(T)), __VA_ARGS__)


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
/*! @brief Operator @c delete for class T   **/                         \
  /*! @see T ## _destroy  is supposed to exist and to be callable with just one T* argument **/ \
  /*! @see T ## _new                       **/                          \
inline                                                                  \
void T ## _delete(T *el) {                                              \
  if (el) {                                                             \
    T ## _destroy(el);                                                  \
    free(el);                                                           \
  }                                                                     \
}

inline
void *_vnew(size_t n) {
  /* allocate in multiples of the alignment */
  size_t const ali = sizeof(uintmax_t);
  size_t N = (n / ali);
  /* add one if n doesn't fit uintmax_t boundary */
  if (n % ali) ++N;
  /* add one for the header */
  size_t tot = (N + 1) * ali;
  uintmax_t *ret = (uintmax_t*)memset(malloc(tot), 0, tot);
  ret[0] = n;
  return ret + 1;
}

inline
uintmax_t *_vfind(void *p) {
  uintmax_t *ret = ((uintmax_t*)p) - 1;
  return ret;
}

inline
size_t _vlen(void *p) {
  uintmax_t *ret = _vfind(p);
  return ret[0];
}

inline
void _vdelete(void *p) {
  free(_vfind(p));
}

/**
 ** @brief Declare a `new[]' operator for type @a T.
 **
 ** This supposes that type @a T has a `constructor', i.e and init
 ** function, that just takes a pointer to the element that is to be
 ** initialized.
 **/
#define DECLARE_VNEW(T)                                                 \
/*! @brief Operator @c new[] for class T   **/                          \
  /*! @see T ## _init  is supposed to exist and to be callable with just one T* argument **/ \
  /*! @see T ## _vdelete                      **/                       \
inline                                                                  \
T *T ## _vnew(size_t n) {                                               \
  size_t N = n*sizeof(T);                                               \
  T *ret = _vnew(N);                                                    \
  if (ret) {                                                            \
    for (size_t i = 0; i < n; ++i) {                                    \
      T ## _init(ret + i);                                              \
    }                                                                   \
  }                                                                     \
  return ret;                                                           \
}

#define DECLARE_VDELETE(T)                                              \
/*! @brief Operator @c delete[] for class T   **/                       \
  /*! @see T ## _destroy  is supposed to exist and to be callable with just one T* argument **/ \
  /*! @see T ## _vnew                        **/                        \
inline                                                                  \
void T ## _vdelete(T *vec) {                                            \
  if (vec) {                                                            \
    size_t n = _vlen(vec) / sizeof(T);                                  \
    for (size_t i = 0; i < n; ++i) {                                    \
      T ## _destroy(vec + i);                                           \
    }                                                                   \
    _vdelete(vec);                                                      \
  }                                                                     \
}

#define DECLARE_NEW_DELETE(T)                                    \
  /*DECLARE_NEW(T) */                                            \
DECLARE_DELETE(T)                                                \
DECLARE_VDELETE(T)                                               \
DECLARE_VNEW(T)                                                  \
enum _tame_ansi_c_semicolon_message_ ## T { _new_delete_ ## T }

#define DEFINE_NEW(T) 

//T *T ## _new(void)

#define DEFINE_DELETE(T) void T ## _delete(T *el)
#define DEFINE_VNEW(T) T *T ## _vnew(size_t n)
#define DEFINE_VDELETE(T) void T ## _vdelete(T *vec)

#define DEFINE_NEW_DELETE(T)                   \
DEFINE_VNEW(T);                                \
DEFINE_DELETE(T);                              \
DEFINE_VDELETE(T)


#endif 	    /* !ORWL_NEW_H_ */

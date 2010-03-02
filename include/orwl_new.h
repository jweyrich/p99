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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DECLARE_NEW(T)                          \
inline                                          \
T *T ## _new(void) {                            \
  T *ret = (T*)malloc(sizeof(T));               \
  if (ret) T ## _init(ret);                     \
  return ret;                                   \
}

#define DECLARE_DELETE(T)                       \
inline                                          \
void T ## _delete(T *el) {                      \
  if (el) {                                     \
    T ## _destroy(el);                          \
    free(el);                                   \
  }                                             \
}

#define DECLARE_VNEW(T)                         \
inline                                          \
T *const*T ## _vnew(size_t n) {                 \
  size_t N = (n + 1)*sizeof(T*);                \
  T **ret = malloc(N);                          \
  if (ret) {                                    \
    memset(ret, 0, N);                          \
    assert(!ret[n]);                            \
    for (size_t i = 0; i < n; ++i) {            \
      ret[i] = T ## _new();                     \
      /* roll back if allocation failed */      \
      if (!ret[i]) {                            \
        T ## _vdelete(ret);                     \
        ret = NULL;                             \
        break;                                  \
      }                                         \
    }                                           \
  }                                             \
  return ret;                                   \
}

#define DECLARE_VDELETE(T)                      \
inline                                          \
void T ## _vdelete(T *const*vec) {              \
  if (vec) {                                    \
    for (T **v = (T**)vec; *v; ++v)             \
      T ## _delete(*v);                         \
    free((T**)vec);                             \
  }                                             \
}

#define DECLARE_NEW_DELETE(T)                                    \
DECLARE_NEW(T)                                                   \
DECLARE_DELETE(T)                                                \
DECLARE_VDELETE(T)                                               \
DECLARE_VNEW(T)                                                  \
enum { __tame_ansi_c_semicolon_message_for_new_delete_ ## T }

#define DEFINE_NEW(T) T *T ## _new(void)
#define DEFINE_DELETE(T) void T ## _delete(T *el)
#define DEFINE_VNEW(T) T *const*T ## _vnew(size_t n)
#define DEFINE_VDELETE(T) void T ## _vdelete(T *const*vec)

#define DEFINE_NEW_DELETE(T)                   \
DEFINE_NEW(T);                                 \
DEFINE_VNEW(T);                                \
DEFINE_DELETE(T);                              \
DEFINE_VDELETE(T)


#endif 	    /* !ORWL_NEW_H_ */

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

#define DEFINE_NEW(T, ...)                      \
inline                                          \
T *T ## _new(void) {                            \
  T *ret = (T*)malloc(sizeof(T));               \
  if (ret) T ## _init(ret, __VA_ARGS__);        \
  return ret;                                   \
}

#define DEFINE_DELETE(T)                        \
inline                                          \
void T ## _delete(T *el) {                      \
  if (el) {                                     \
    T ## _destroy(el);                          \
    free(el);                                   \
  }                                             \
}

#define DEFINE_VNEW(T)                          \
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

#define DEFINE_VDELETE(T)                       \
inline                                          \
void T ## _vdelete(T *const*vec) {              \
  if (vec) {                                    \
    for (T *v = *vec; v; ++v)                   \
      T ## _delete(v);                          \
    free((T**)vec);                             \
  }                                             \
}

#define DEFINE_NEW_DELETE(T, ...)                               \
  DECLARE_NEW_DELETE(T);                                        \
DEFINE_NEW(T, __VA_ARGS__)                                      \
DEFINE_VNEW(T)                                                  \
DEFINE_DELETE(T)                                                \
DEFINE_VDELETE(T)                                               \
enum { __tame_ansi_c_semicolon_message_for_new_delete_ ## T }

#define DECLARE_NEW(T) extern T *T ## _new(void)
#define DECLARE_DELETE(T) extern void T ## _delete(T *el)
#define DECLARE_VNEW(T) extern T *const*T ## _vnew(size_t n)
#define DECLARE_VDELETE(T) extern void T ## _vdelete(T *const*vec)

#define DECLARE_NEW_DELETE(T)                   \
DECLARE_NEW(T);                                 \
DECLARE_VNEW(T);                                \
DECLARE_DELETE(T);                              \
DECLARE_VDELETE(T)


#endif 	    /* !ORWL_NEW_H_ */

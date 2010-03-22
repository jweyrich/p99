/*
** orwl_socket.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Mar 21 21:46:01 2010 Jens Gustedt
** Last update Sun Mar 21 21:46:01 2010 Jens Gustedt
*/

#ifndef   	ORWL_SOCKET_H_
# define   	ORWL_SOCKET_H_

#include "orwl_macro.h"
#include "orwl_int.h"
#include "orwl_enum.h"

inline
void FUNC_DEFAULT(orwl_hton)(uint32_t *n, uint64_t const *h, size_t l) {
  for (size_t i = 0; i < l; ++i) {
    n[0] = htonl((uint32_t)h[0]);
    n[1] = htonl((uint32_t)(h[0] >> 32));
    h += 1;
    n += 2;
  }
}

#define orwl_hton(...) DEFINE_FUNC_DEFAULT(orwl_hton, 3, __VA_ARGS__)
declare_default_arg(orwl_hton, 2, size_t, 1);


inline
void FUNC_DEFAULT(orwl_ntoh)(uint64_t* h, uint32_t const *n, size_t l) {
  for (size_t i = 0; i < l; ++i) {
    h[0] = ((uint64_t)ntohl(n[0])) | (((uint64_t)ntohl(n[1])) << 32);
    n += 2;
    h += 1;
  }
}

#define orwl_ntoh(...) DEFINE_FUNC_DEFAULT(orwl_ntoh, 3, __VA_ARGS__)
declare_default_arg(orwl_ntoh, 2, size_t, 1);


#endif 	    /* !ORWL_SOCKET_H_ */

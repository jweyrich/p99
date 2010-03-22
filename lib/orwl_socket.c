/*
** orwl_socket.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Mar 21 22:00:19 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include <arpa/inet.h>
#include <stdio.h>

#include "orwl_socket.h"

static uint32_t mycode = 0;

typedef uint32_t U;
#define Code(C0, C1, C2, C3) ((((U)C0) << 0) | (((U)C1) << 8) | (((U)C2) << 16) | (((U)C3) << 24))

#define ifCode(C0, C1, C2, C3)                          \
do {                                                    \
  static uint32_t const c = Code(C0, C1, C2, C3);       \
  if (ntohl(c) == Code(0, 1, 2, 3)) {                   \
    mycode = c;                                         \
    goto END;                                           \
  }                                                     \
 } while (false)


DECLARE_ONCE(mycode);
DEFINE_ONCE(mycode) {
    ifCode(0, 1, 2, 3);
    ifCode(0, 1, 3, 2);
    ifCode(0, 2, 1, 3);
    ifCode(0, 2, 3, 1);
    ifCode(0, 3, 1, 2);
    ifCode(0, 3, 2, 1);

    ifCode(1, 0, 2, 3);
    ifCode(1, 0, 3, 2);
    ifCode(1, 2, 0, 3);
    ifCode(1, 2, 3, 0);
    ifCode(1, 3, 0, 2);
    ifCode(1, 3, 2, 0);

    ifCode(2, 0, 1, 3);
    ifCode(2, 0, 3, 1);
    ifCode(2, 1, 0, 3);
    ifCode(2, 1, 3, 0);
    ifCode(2, 3, 0, 1);
    ifCode(2, 3, 1, 0);

    ifCode(3, 0, 1, 2);
    ifCode(3, 0, 2, 1);
    ifCode(3, 1, 0, 2);
    ifCode(3, 1, 2, 0);
    ifCode(3, 2, 0, 1);
    ifCode(3, 2, 1, 0);
 END:;
}

bool same_endianess(uint32_t c) {
  INIT_ONCE(mycode);
  return c == mycode;
}

void FUNC_DEFAULT(orwl_hton)(uint32_t *n, uint64_t const *h, size_t l);
define_default_arg(orwl_hton, 2, size_t);
void FUNC_DEFAULT(orwl_ntoh)(uint64_t* h, uint32_t const *n, size_t l);
define_default_arg(orwl_ntoh, 2, size_t);

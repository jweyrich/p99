/*
** orwl_rand.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Mon Mar 22 23:23:58 2010 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "orwl_rand.h"
#include "orwl_posix_default.h"

rand48_t *rand48_t_init(rand48_t*, unsigned short, unsigned short, unsigned short);
DEFINE_DEFARG(rand48_t_init, , useconds(), getpid(), pthread_self());

void rand48_t_destroy(rand48_t* seed);

DEFINE_NEW_DELETE(rand48_t);

DEFINE_THREAD_VAR(rand48_t, seed_get);


uint32_t orwl_rand(rand48_t *xsubi);
double orwl_drand(rand48_t *xsubi);
uint64_t orwl_rand64(rand48_t *xsubi);

uint64_t useconds(void);

uint64_t orwl_mix(uint64_t a, uint64_t b) {
  uint16_t a0 = a >> 0;
  uint16_t a1 = a >> 16;
  uint16_t a2 = a >> 32;
  uint16_t a3 = a >> 48;
  uint16_t b0 = b >> 0;
  uint16_t b1 = b >> 16;
  uint16_t b2 = b >> 32;
  uint16_t b3 = b >> 48;
  rand48_t tmp[4] = {
    {{a0, b0, b2}},
    {{b1, a1, b3}},
    {{b2, b1, a2}},
    {{a3, b3, b0}},
  };
  return
    (uint64_t)(orwl_rand(&tmp[0]) ^ orwl_rand(&tmp[1]))
    | ((uint64_t)(orwl_rand(&tmp[2]) ^ orwl_rand(&tmp[3])) << 32);
}

static uint64_t secret = INITIALIZER;

static char const ENVVAR[] = "ORWL_SECRET";

DECLARE_ONCE_UPON(secret);
DEFINE_ONCE_UPON(secret) {
  char const *str = getenv(ENVVAR);
  if (str) secret = str2uint64_t(str);
}

uint64_t orwl_challenge(uint64_t a) {
  uint64_t ret = 0;
  INIT_ONCE_UPON(secret, secret);
  if (secret) {
    for (; !ret; ++a)
      ret = orwl_mix(a, secret);
  }
  return ret;
}

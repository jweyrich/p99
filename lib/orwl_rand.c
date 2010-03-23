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
#include "orwl_once.h"

uint32_t orwl_rand(rand48_t xsubi);

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
    {a0, b0, b2},
    {b1, a1, b3},
    {b2, b1, a2},
    {a3, b3, b0},
  };
  return
    (uint64_t)(orwl_rand(tmp[0]) ^ orwl_rand(tmp[1]))
    | ((uint64_t)(orwl_rand(tmp[2]) ^ orwl_rand(tmp[3])) << 32);
}

static uint64_t secret = INITIALIZER;

static char const ENVVAR[] = "ORWL_SECRET";

DECLARE_ONCE(secret);
DEFINE_ONCE(secret) {
  char const *str = getenv(ENVVAR);
  secret = strtoul(str, NULL, 0);
}

uint64_t orwl_challenge(uint64_t a) {
  INIT_ONCE(secret);
  return orwl_mix(a, secret);
}

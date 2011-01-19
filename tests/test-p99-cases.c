/*
** test_p99_cases.c
** 
** Made by (Jens Gustedt)
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Wed Jan 19 08:44:00 2011 Jens Gustedt
** Last update Sun May 12 01:17:25 2002 Speed Blue
*/

#include "p99_for.h"

int main(int argc, char*argv[]) {
  if (argc < 2) return -2;
  switch (argv[1][0]) {
    /* This line should produce a warning */
  P99_CASERANGE('\0', 0): return -1;
  P99_CASERANGE('0', 10): return 0;
  P99_CASERANGE('A', 25): return 1;
  P99_CASERANGE('a', 25, oioi): return 2;
  default: return 3;
  }
}

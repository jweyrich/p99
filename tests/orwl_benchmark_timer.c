#include "orwl.h"

int main(int argc, char **argv) {
  const size_t max = 1E+7;
  const struct timespec start = orwl_gettime();
  for (size_t i = 0; i < max; ++i) {
    orwl_gettime();
  }
  const struct timespec end = orwl_gettime();
  printf("Average time for orwl_gettime(): %f ns\n",
	 timespec2useconds(timespec_diff(start,end)) / (double)max * 1000);
  return(0);
}

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "benchmark.h"

/* usleep(): Sleep for the requested number of microseconds. */
int musleep(long usec) {
    struct timespec ts;
    int res;

    if (usec < 0) {
        return -1;
    }

    ts.tv_sec = usec / 1000000;
    ts.tv_nsec = (usec % 1000000) * 1000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}


void mindex_some_bmark_method() {
  // Intentionally empty
  musleep(100 + (rand() % 100));
}

int main() {

  // Seed random
  unsigned int seed;
  FILE* urandom = fopen("/dev/urandom", "r");
  fread(&seed, sizeof(int), 1, urandom);
  fclose(urandom);
  srand(seed);

  // Enqueue benchmarks
  BMARK(mindex_some_bmark_method);

  char percentiles[] = {
    1 ,  5, 20,
    50,
    80, 95, 99,
    0
  };

  // Run & return
  return bmark_run(100, percentiles);
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include "win32ports/time.h"
#else
#include <sys/time.h>
#endif

#include "benchmark.h"

struct bmark_queue_t {
  void *next;
  char *name;
  void (*fn)();
};
struct bmark_ptile_t {
  char n;
};

struct bmark_queue_t *bmark_queue   = NULL;

struct bmark_unit_entry {
  char *name;
  int   next;
};

struct bmark_unit_entry units[] = {
  { "us", 1000 },
  { "ms", 1000 },
  { "s" ,   60 },
  { "m" ,    0 },
};

const char *header_description = "Description";

void bmark_enqueue(char *name, void (*fn)()) {
  struct bmark_queue_t *q = calloc(1, sizeof(struct bmark_queue_t));
  q->name = name;
  q->fn   = fn;
  q->next = bmark_queue;
  bmark_queue = q;
}

// Example time numbers:  12.23 s
//                       900.00 ms
//                       123.45 us
int bmark_run(int run_count, char percentiles[]) {
  int header_desclen = strlen(header_description);
  int i, l, r, unit;
  struct timeval tv_start;
  struct timeval tv_end;

  uint64_t *runs    = calloc(run_count, sizeof(uint64_t));
  uint64_t  run_tmp = 0;
  double    run_f   = 0;

  // Get max description length
  struct bmark_queue_t *q_entry = bmark_queue;
  while(q_entry) {
    l = strlen(q_entry->name);
    if (l > header_desclen) header_desclen = l;
    q_entry = q_entry->next;
  }

  printf("\n");

  // Column names
  printf("| %-*s |", header_desclen, header_description);
  for(i = 0 ; percentiles[i] ; i++) printf(" %*d %% |", 7, percentiles[i]);
  printf("\n");

  // Separator
  printf("|:");
  for(i = 0 ; i < header_desclen ; i++) printf("-");
  printf(" |");
  for(i = 0 ; percentiles[i] ; i++) printf(" ---------:|");
  printf("\n");

  // Go over the whole queue
  q_entry = bmark_queue;
  while(q_entry) {
    printf("| %-*s |", header_desclen, q_entry->name);
    fflush(stdout);

    // Do the actual runs
    for(r = 0; r < run_count ; r++) {
      gettimeofday(&tv_start, NULL);
      q_entry->fn();
      gettimeofday(&tv_end, NULL);
      runs[r] = (tv_end.tv_sec*(uint64_t)1000000+tv_end.tv_usec) - (tv_start.tv_sec*(uint64_t)1000000+tv_start.tv_usec);
    }

    // Sort the runs
    // Basic bubble sort is fine here, we're not going for a speed record
    for(r = 0; r < (run_count-1) ; r++) {
      if (r < 0) continue;
      if (runs[r] > runs[r+1]) {
        run_tmp = runs[r];
        runs[r] = runs[r+1];
        runs[r+1] = run_tmp;
        r -= 2;
      }
    }

    // Print percentiles
    for(i = 0 ; percentiles[i] ; i++) {
      r     = percentiles[i] * (run_count-1) / 100;
      run_f = (double)runs[r];
      unit  = 0;
      while((run_f > units[unit].next) && units[unit].next) {
        run_f = run_f / units[unit].next;
        unit++;
      }
      printf(" %*.2lf %2s |", 6, run_f, units[unit].name);
    }

    printf("\n");

    q_entry = q_entry->next;
  }

  printf("\n");
  return 0;
}

#ifndef __FINWO_BENCHMARK_H__
#define __FINWO_BENCHMARK_H__


#define BMARK(fn) bmark_enqueue((#fn), (fn))

void bmark_enqueue(char *name, void (*fn)());
int bmark_run(int run_count, char percentiles[]);

#endif // __FINWO_BENCHMARK_H__

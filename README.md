benchmark
=========

Small benchmarking helper library

This library makes use of [dep](https://github.com/finwo/dep) to manage it's
dependencies and exports.

Installation
------------

```sh
dep add finwo/benchmark
dep install
```

After that, simply add `include lib/.dep/config.mk` in your makefile and include
the header file by adding `#include "finwo/benchmark.h`.

Features
--------

- Markdown-compatible output

Example
-------

```c
#include "finwo/benchmark.h"

static void some_function() {
  sleep(1);
}

int main() {
  BMARK(some_function);

  char percentiles[] = {
    1, 5, 50, 95, 99, 0
  };

  return bmark_run(100, percentiles);
}
```

API
---

```c
BMARK(fn)
```

Calls the `bmark_enqueue` method using the name of the function instead of a
custom name.

```c
void bmark_enqueue(char *name, void (*fn)());
```

| Parameter   | Description                                       |
| ----------- | ------------------------------------------------- |
| name        | The displayed name of the row in the output table |
| fn          | Function to call for making the time measurement  |

Marks a method to be run as benchmark entry.

```c
int bmark_run(int run_count, char percentiles[]);
```

Runs the benchmarks, returns non-zero on library error.

| Parameter   | Description                           |
| ----------- | ------------------------------------- |
| run_count   | How many times to run the methods for |
| percentiles | Which percentiles of the runs to show |

License
-------

benchmark source code is available under the MIT license.

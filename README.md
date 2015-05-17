# SYNOPSIS
This is an idea for some general purpose asynchronous control flow helpers
that write to concurrent data structures and return the same types that were
passed in.

# METHODS

## EACH PARALLEL
Run the tasks array of functions in parallel, each without waiting until 
the previous function has completed the calling thread waits until
all tasks have completed.
 
```cpp
#include "libsync.h"

//
// any arbitrary iterable
//
vector<int> arr { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

auto results = libasync.eachParallel(arr, [&](auto value, auto push) {
  // do something async
  push(value);
});

for(auto &el : results)
  cout << el << endl;
```

## EACH
Run the functions in the tasks array in series, each one running
once the previous function has completed. If `push` is not called,
the iteration will stop.

```cpp
vector<string> arr { "foo", "bar", "bazz" };

auto results = libasync.each(arr, [&](auto value, auto push) {
  push(value);
});

for (auto &el : results)
  cout << el << endl;
```

Methods can be wrapped in try catches! The thread will bubble the
exception back up to the caller.


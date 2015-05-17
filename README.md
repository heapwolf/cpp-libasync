# SYNOPSIS
This is an idea for some general purpose asynchronous control flow helpers
that write to a data structure of the given input type in a thread-safe way.

# BUILD
```
c++ test.cc -o test -std=c++1y
```

# METHODS

## EACH PARALLEL
Run the function in parallel on each of the array values, each without 
waiting until the previous function has completed. The calling thread 
waits until all tasks have completed.
 
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
Run the function on each item in the array in series, each one running
once the previous function has completed. If `push` is not called,
the iteration will stop. You can also terminate flow by pushing `nullptr`
as the value to `push`.

```cpp
vector<string> arr { "foo", "bar", "bazz" };

auto results = libasync.each(arr, [&](auto value, auto push) {
  push(value);
});

for (auto &el : results)
  cout << el << endl;
```

## ERRORS
Methods can be wrapped in try catches! The thread will bubble the
exception back up to the caller.

```cpp
try {

  vector<string> arr { "foo", "bar", "bazz" };

  libasync.each(arr, [&](auto value, auto push) {
    if (value == "bar") throw runtime_error("oops");
    push(value);
  });

}
catch(const std::exception& ex) {
  cout << ex.what() << endl;
}
```


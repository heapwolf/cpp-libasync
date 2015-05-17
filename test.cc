#include <vector>
#include <map>
#include <chrono>
#include <iostream>
#include <string>
#include <time.h>
#include <thread>
#include <mutex>

#include "asynclib.h"

int EXPECT = 0;

#define ASSERT(message, ...) do { \
  EXPECT = __COUNTER__; \
  if(!(__VA_ARGS__)) { \
    std::cerr << "FAIL: " << message << std::endl; \
  } \
  else { \
    std::cout << "OK: " << message << std::endl; \
  } \
} while(0);

#define EXPECTED(num) { \
  ASSERT("All tests", num == (EXPECT -1)); \
}

int main() {

  ASSERT("sanity: true is false", true == false);
  ASSERT("sanity: true is true", true == true);

  // TODO
  // refactor into real tests.

  using namespace Libasync;
  using namespace std;

  vector<string> arrs0 { "foo", "bar", "bazz" };

  auto results0 = libasync.each(arrs0, [&](auto value, auto push) {
    if (value == "bar") return push(nullptr);
    push(value);
  });

  ASSERT("size of new vector is correct", results0.size() == 1);
  ASSERT("first item in new vector has the right value", results0[0] == "foo");

  for(auto &n : results0)
    cout << n << endl;

  //srand (time(NULL));
  vector<int> arri { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

  auto results1 = libasync.eachParallel(arri, [&](int value, auto push) {

    this_thread::sleep_for(chrono::milliseconds(rand() % 100 + 1));
    push(value*4);
  });

  ASSERT("size of new vector is equal", results1.size() == arri.size());

  for(auto &n : results1)
    cout << "NUM" << n << endl;

  vector<string> arrs1 { "foo", "bar", "bazz" };

  auto results2 = libasync.each(arrs1, [&](auto value, auto push) {
    if (value == "bar") return push(nullptr);
    push(value + "X");
  });

  ASSERT("size of new vector is correct", results2.size() == 1);

  for (auto &n : results2)
    cout << n << endl;

  string exceptionText;

  try {

    vector<string> arrs2 { "foo", "bar", "bazz" };

    auto results3 = libasync.each(arrs2, [&](auto value, auto push) {
      // meanwhile, in a separete thread
      cout << value << endl;
      if (value == "bar") throw runtime_error("oops");
      push(value);
    });
  }
  catch(const std::exception& ex) {
    cout << ex.what() << endl;
    exceptionText = string(ex.what());
    ASSERT("an exception was thrown", true);
  }

  ASSERT("exception was handed back to the caller thread", exceptionText == "oops");
  EXPECTED(7);
}


#include <vector>
#include <map>
#include <chrono>
#include <iostream>
#include <string>
#include <time.h>
#include <thread>
#include <mutex>

#include "asynclib.h"

int main() {

  // TODO
  // refactor into real tests.

  using namespace Libasync;
  using namespace std;

  srand (time(NULL));
  vector<int> arri { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

  auto results1 = libasync.eachParallel(arri, [&](int value, auto push) {

    this_thread::sleep_for(chrono::milliseconds(rand() % 100 + 1));
    push(value + 20);
  });

  for(auto &n : results1)
    cout << "NUM" << n << endl;


  vector<string> arrs1 { "foo", "bar", "bazz" };

  auto results2 = libasync.each(arrs1, [&](auto value, auto push) {
    push(value + "X");
  });

  for (auto &n : results2)
    cout << n << endl;

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
  }
}


#ifndef LIBASYNC_H
#define LIBASYNC_H

#include <future>
#include <functional>
#include <iterator>

namespace Libasync {

  class Libasync {
   
    public: 
      template<typename Iterable, typename Callback>
      Iterable eachParallel(Iterable iterable, Callback cb);

      template<typename Iterable, typename Callback>
      Iterable each(Iterable iterable, Callback cb);

  } libasync;

  template <typename... Args>
    using Next = std::function<void(Args... params)>;

  template<class Type>
  class createContainer {
    std::mutex mutex;

    public:
      Type value;
      createContainer() {}

      template<typename Callback>
      void safely(Callback cb) {
        mutex.lock();
        cb(value);
        mutex.unlock();
      }
  };   

  template<typename Iterator, typename Callback, typename Results>
  void _eachParallel(Iterator first, Iterator last, Results results, Callback cb) {

    ptrdiff_t const len = last - first;

    if (!len) return;
    if (len == 1) {

      auto step = [&](auto val) {

        results->safely([&](auto &container) {
          container.push_back(*first);
        });
      };

      return cb(*first, step);
    }

    Iterator const middle = first + (len / 2);

    auto f = std::async(
      std::launch::async, 
      &_eachParallel<Iterator, Callback, Results>,
      first, 
      middle, 
      results,
      cb);

    try {
      _eachParallel(middle, last, results, cb);
    }
    catch(...) {
      f.wait();
      throw;
    }

    f.get();
  }

  template<typename Iterator, typename Callback, typename Results>
  void _each(Iterator first, Iterator last, Results results, Callback cb) {

    ptrdiff_t const len = last - first;

    if (!len) return;

    auto step = [&](auto val) {

      results->safely([&](auto &container) {
        container.push_back(*first);
      });

      Iterator const second = next(first);

      std::async(
        std::launch::async,
        &_each<Iterator, Callback, Results>,
        second,
        last,
        results,
        cb).get();
    };

    cb(*first, step);
  }

  template<typename Iterable, typename Callback>
  Iterable Libasync::each(Iterable iterable, Callback cb) {
    createContainer<Iterable> results;
    _each(iterable.begin(), iterable.end(), &results, cb);
    return results.value;
  }

  template <typename Iterable, typename Callback>
  Iterable Libasync::eachParallel(Iterable iterable, Callback cb) {

    createContainer<Iterable> results;
    _eachParallel(iterable.begin(), iterable.end(), &results, cb);
    return results.value;
  }

}

#endif


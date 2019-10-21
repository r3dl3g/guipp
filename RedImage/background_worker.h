
#pragma once

#include "base/blocking_queue.h"
#include <functional>
#include <thread>


template<typename T>
class background_worker {
public:
  typedef basepp::blocking_queue<T> queue;
  typedef std::function<void(queue&)> worker;

  background_worker () = default;
  background_worker (const background_worker&) = delete;
  background_worker (background_worker&&) = delete;

  ~background_worker () {
    join();
  }

  void start (worker w, unsigned count = std::thread::hardware_concurrency()) {
    for (unsigned i = 0; i < count; ++i) {
      threads.emplace_back([&] () { w(data_queue); });
    }
  }

  void add (T&& t) {
    data_queue.enqueue(std::move(t));
  }

  void add (const T& t) {
    data_queue.enqueue(t);
  }

  template<typename I>
  void add (I begin, I end) {
    data_queue.enqueue(begin, end);
  }

  void clear () {
    data_queue.clear();
  }

  void wait_until_empty (const std::chrono::milliseconds& timeout) {
    data_queue.wait_until_empty(timeout);
  }

  bool joinable () const {
    for (const std::thread& t : threads) {
      if (t.joinable()) {
        return true;
      }
    }
    return false;
  }

  void join () {
    for (std::thread& t : threads) {
      if (t.joinable()) {
        t.join();
      }
    }
  }

private:
  queue data_queue;
  std::vector<std::thread> threads;
};

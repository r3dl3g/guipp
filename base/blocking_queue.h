/**
* @copyright (c) 2015-2019 Ing. Buero Rothfuss
*                          Riedlinger Str. 8
*                          70327 Stuttgart
*                          Germany
*                          http://www.rothfuss-web.de
*
* @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
*
* Project    standard lib
*
* Customer   -
*
* @brief     C++ API: blocking queue
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <queue>
#include <condition_variable>
#include <mutex>


namespace basepp {

  template<typename T, std:: size_t S = 0xffffffff / sizeof(T)>
  class blocking_queue {
  public:

    /// Default constructor
    blocking_queue () {}

    /// Destructor
    ~blocking_queue () {}

    /// Enqueue an item and send signal to a waiting dequeuer.
    void enqueue (const T& t) {
      {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.size() == S) {
          m_queue.pop();
        }
        m_queue.push(t);
      }
      m_condition.notify_all();
    }

    /// Enqueue an item and send signal to a waiting dequeuer.
    void enqueue (T&& t) {
      {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.size() == S) {
          m_queue.pop();
        }
        m_queue.push(std::move(t));
      }
      m_condition.notify_all();
    }

    void wait_until_empty (const std::chrono::milliseconds& timeout) {
      std::unique_lock<std::mutex> lock(m_mutex);

      m_condition.wait_for(lock, timeout, [this] () {
        return m_queue.empty();
      });
    }

    void wait_until_not_empty (std::unique_lock<std::mutex> &lock,
                               const std::chrono::milliseconds maxWait) {
      m_condition.wait_for(lock, maxWait, [this] () -> bool {
        return !m_queue.empty();
      });
    }

    void wait_until_not_empty (const std::chrono::milliseconds maxWait) {
      std::unique_lock<std::mutex> lock(m_mutex);
      wait_until_not_empty(lock, maxWait);
    }

    void wait_until_not_empty (std::unique_lock<std::mutex> &lock) {
      m_condition.wait(lock, [this] () -> bool {
        return !m_queue.empty();
      });
    }

    void wait_until_not_empty () {
      std::unique_lock<std::mutex> lock(m_mutex);
      wait_until_not_empty(lock);
    }

    /// Dequeue the last item, if available, else waits until a new item is enqueued, clear queue after pop.
    T dequeue_back (const std::chrono::milliseconds maxWait) {
      std::unique_lock<std::mutex> lock(m_mutex);
      wait_until_not_empty(lock, maxWait);

      if (m_queue.empty()) {
        return T();
      }

      T item = m_queue.back();
      std::queue<T> tmp;
      m_queue.swap(tmp); // clear
      m_condition.notify_all();
      return item;
    }

    /// Dequeue an item if available, else waits until a new imitemage is enqueued.
    T dequeue (const std::chrono::milliseconds maxWait) {
      std::unique_lock<std::mutex> lock(m_mutex);

      wait_until_not_empty(lock, maxWait);

      if (m_queue.empty()) {
        return T();
      }

      T item = m_queue.front();
      m_queue.pop();
      m_condition.notify_all();
      return item;
    }

    /// Dequeue the last item, if available, else waits until a new item is enqueued, clear queue after pop.
    T dequeue_back () {
      std::unique_lock<std::mutex> lock(m_mutex);

      wait_until_not_empty(lock);

      if (m_queue.empty()) {
        return T();
      }

      T item = m_queue.back();
      std::queue<T> tmp;
      m_queue.swap(tmp); // clear
      m_condition.notify_all();
      return item;
    }

    /// Dequeue an item if available, else waits until a new item is enqueued.
    T dequeue () {
      std::unique_lock<std::mutex> lock(m_mutex);

      wait_until_not_empty(lock);

      if (m_queue.empty()) {
        return T();
      }

      T item = m_queue.front();
      m_queue.pop();
      m_condition.notify_all();
      return item;
    }

    /// Dequeue an item if available and return true, else return false.
    bool try_dequeue (T& t) {
      std::unique_lock<std::mutex> lock(m_mutex);

      if (m_queue.empty()) {
        return false;
      }

      t = m_queue.front();
      m_queue.pop();
      m_condition.notify_all();
      return true;
    }

    /// @return true, if the queue is empty.
    bool isEmpty () const {
      std::unique_lock<std::mutex> lock(m_mutex);
      return m_queue.empty();
    }

    /// Removes all items from the queue.
    void clear () {
      std::unique_lock<std::mutex> lock(m_mutex);
      std::queue<T> tmp;
      m_queue.swap(tmp); // clear
      m_condition.notify_all();
    }

  private:
    /// The queue to store the items in.
    std::queue<T> m_queue;

    /// Condition to signal new item to dequeuer.
    std::condition_variable m_condition;

    /// Mutex for thread safe access to the queue.
    mutable std::mutex m_mutex;

  };

  template<typename T, std:: size_t S>
  inline blocking_queue<T, S>& operator<< (blocking_queue<T, S>& queue, const T& t) {
    queue.enqueue(t);
    return queue;
  }

  template<typename T, std:: size_t S>
  inline blocking_queue<T, S>& operator>> (blocking_queue<T, S>& queue, T& t) {
    t = queue.dequeue();
    return queue;
  }

} // namespace basepp

// --------------------------------------------------------------------------

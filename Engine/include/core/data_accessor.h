#pragma once

#include <cstddef>
#include <mutex>

#include "core/synchronization_primitives.h"

template <typename T> class DataAccessor {
private:
  T &data;
  SynchronizationPrimitives &sync;

public:
  DataAccessor(T &data, SynchronizationPrimitives &sync_primitives)
      : data(data), sync(sync_primitives) {
    std::unique_lock<std::recursive_mutex> lock(sync.mutex);
    sync.condition_variable.wait(lock, [this] { return sync.data_ready; });
  }

  ~DataAccessor() {
    sync.mutex.unlock();
    sync.condition_variable.notify_one();
  }

  T &operator*() { return data; }

  T *operator->() { return &data; }

  void Unlock() { sync.mutex.unlock(); }

  static void *operator new(std::size_t) = delete;
  static void *operator new[](std::size_t) = delete;
  static void operator delete(void *) = delete;
  static void operator delete[](void *) = delete;
};

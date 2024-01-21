#pragma once

#include <cstddef>
#include <mutex>

#include "synchronization_primitives.h"

template <typename T> class DataAccessor {
 private:
  T &data;
  SynchronizationPrimitives &sync;
  std::unique_lock<std::recursive_mutex> lock;

 public:
  DataAccessor(T &data, SynchronizationPrimitives &sync_primitives)
      : data(data), sync(sync_primitives), lock(sync.mutex) {
    sync.condition_variable.wait(lock, [this] { return sync.data_ready; });
  }

  ~DataAccessor() {
    sync.condition_variable.notify_all();
  }

  T &operator*() { return data; }

  T *operator->() { return &data; }

  static void *operator new(std::size_t) = delete;
  static void *operator new[](std::size_t) = delete;
  static void operator delete(void *) = delete;
  static void operator delete[](void *) = delete;
};

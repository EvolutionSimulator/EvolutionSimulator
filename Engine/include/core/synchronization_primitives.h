#pragma once

#include <mutex>
#include <condition_variable>

struct SynchronizationPrimitives {
  std::recursive_mutex mutex;
  std::condition_variable_any condition_variable;
  bool data_ready = true;
};

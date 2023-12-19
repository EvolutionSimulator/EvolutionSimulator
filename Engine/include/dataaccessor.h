#pragma once

#include <cstddef>
#include <mutex>

template <typename T>
class DataAccessor {
 private:
  T& data;
  std::mutex& mtx;

 public:
  DataAccessor(T& data, std::mutex& mtx) : data(data), mtx(mtx) { mtx.lock(); }

  ~DataAccessor() { mtx.unlock(); }

  T& operator*() { return data; }

  T* operator->() { return &data; }

  void Unlock() { mtx.unlock(); }

  static void* operator new(std::size_t) = delete;
  static void* operator new[](std::size_t) = delete;
  static void operator delete(void*) = delete;
  static void operator delete[](void*) = delete;
};

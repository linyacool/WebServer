// @Author Lin Ya
// @Email xxbbb@vip.qq.com
#pragma once
#include <pthread.h>
#include <cstdio>
#include "noncopyable.h"

/**
 * 互斥锁
 * 提供lock、unlock、get获得互斥变量的能力
 * 
*/
class MutexLock : noncopyable {
 public:
  MutexLock() { pthread_mutex_init(&mutex, NULL); }
  ~MutexLock() {
    pthread_mutex_lock(&mutex);
    pthread_mutex_destroy(&mutex);
  }
  void lock() { pthread_mutex_lock(&mutex); }
  void unlock() { pthread_mutex_unlock(&mutex); }
  pthread_mutex_t *get() { return &mutex; }

 private:
  pthread_mutex_t mutex;

  // 友元类不受访问权限影响
 private:
  friend class Condition;
};
/**
 * 对互斥锁的封装，通过局部变量的构造自动加锁和自动析构实现解锁
*/
class MutexLockGuard : noncopyable {
 public:
  explicit MutexLockGuard(MutexLock &_mutex) : mutex(_mutex) { mutex.lock(); }
  ~MutexLockGuard() { mutex.unlock(); }

 private:
  MutexLock &mutex;
};
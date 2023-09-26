// @Author Lin Ya
// @Email xxbbb@vip.qq.com
#pragma once
#include <errno.h>
#include <pthread.h>
#include <pthread.h>
#include <time.h>
#include <cstdint>
#include "MutexLock.h"
#include "noncopyable.h"

/**
 * 条件变量 用于线程同步
 * 提供wait、notify、notifyAll和waitForSeconds的能力
 * 等待的时候必须要携带一个mutex，等待的函数会自动释放mutex并且阻塞条件变量（原子操作），当被唤醒之后会再次获得mutex：
 *    通常的应用场景下，当前线程执行pthread_cond_wait时，处于临界区访问共享资源，
 *    存在一个mutex与该临界区相关联。如果阻塞等待的时候没有释放mutex，那么另一个对象无法进入临界区，可能无发出释放信息，直接死锁
 * wait是无条件等待，一直不发生就一直等；waitForSeconds如果在给定时间内没有满足，那么就返回true
*/
class Condition : noncopyable {
 public:
  explicit Condition(MutexLock &_mutex) : mutex(_mutex) {
    pthread_cond_init(&cond, NULL);
  }
  ~Condition() { pthread_cond_destroy(&cond); }
  void wait() { pthread_cond_wait(&cond, mutex.get()); }
  void notify() { pthread_cond_signal(&cond); }
  void notifyAll() { pthread_cond_broadcast(&cond); }
  bool waitForSeconds(int seconds) {
    struct timespec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);
    abstime.tv_sec += static_cast<time_t>(seconds);
    return ETIMEDOUT == pthread_cond_timedwait(&cond, mutex.get(), &abstime);
  }

 private:
  MutexLock &mutex;
  pthread_cond_t cond;
};
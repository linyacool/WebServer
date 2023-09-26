// @Author Lin Ya
// @Email xxbbb@vip.qq.com
#pragma once
#include "Condition.h"
#include "MutexLock.h"
#include "noncopyable.h"

// CountDownLatch的主要作用是确保Thread中传进去的func真的启动了以后
// 外层的start才返回
/**
 * 计数器
 * 提供wait和countDown的能力
 * wait：如果count大于0，那么就一直循环（防止惊群现象）等待信号量
 * countDown：count-1， 如果count变成0，那么notifyAll
 * 
 * 例子：比如等待三件事做完之后主线程才做后续的事情，那么count可以设为3，
 *      主线程在wait，其他三件事做完之后分别调用countDown，三件事完成之后notifyAll就会唤醒等待的主线程保持同步了
*/
class CountDownLatch : noncopyable {
 public:
  explicit CountDownLatch(int count);
  void wait();
  void countDown();

 private:
  mutable MutexLock mutex_;
  Condition condition_;
  int count_;
};
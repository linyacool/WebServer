// @Author Lin Ya
// @Email xxbbb@vip.qq.com
#include "EventLoopThreadPool.h"
/**
 * 构造函数
 * 设置baseLoop，开始标志位为false，线程数量
*/
EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop, int numThreads)
    : baseLoop_(baseLoop), started_(false), numThreads_(numThreads), next_(0) {
  if (numThreads_ <= 0) {
    LOG << "numThreads_ <= 0";
    abort();
  }
}
/**
 * 启动线程池
 * 1. 判定在主线程 （baseLoop所在的线程）
 * 2. 设置开始标志位
 * 3. 创建numThreads个eventLoopThread加入的threads数组中
 * 4. 分别启动各个eventLoopThread并且加入loops数组中
*/
void EventLoopThreadPool::start() {
  baseLoop_->assertInLoopThread();
  started_ = true;
  for (int i = 0; i < numThreads_; ++i) {
    std::shared_ptr<EventLoopThread> t(new EventLoopThread());
    threads_.push_back(t);
    loops_.push_back(t->startLoop());
  }
}
/**
 * 获取下一个eventLoop
 * 1. 判定在主线程（baseLoop所在的线程）
 * 2. 判断开始标志位为true
 * 3. 如果loops数组不为空，返回loop数组的下一个eventLoop
 *    否则返回主线程
 * 
*/

EventLoop *EventLoopThreadPool::getNextLoop() {
  baseLoop_->assertInLoopThread();
  assert(started_);
  EventLoop *loop = baseLoop_;
  if (!loops_.empty()) {
    loop = loops_[next_];
    next_ = (next_ + 1) % numThreads_;
  }
  return loop;
}
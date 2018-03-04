# 遇到的困难

## 1. 如何设计各个线程个任务
其实我觉的实现上的困难都不算真正的困难吧，毕竟都能写出来，无非是解决bug花的时间的长短。  
我遇到的最大的问题是不太理解One loop per thread这句话吧，翻译出来不就是每个线程一个循环，我最开始写的也是一个线程一个循环啊，muduo的实现和我的有什么区别呢？还有怎么设计才能减少竞态？

带着这些问题我看了《Linux多线程服务端编程》，并看完了muduo的源码，这些问题自然而然就解决了


## 2. 异步Log几秒钟才写一次磁盘，要是coredump了，这段时间内产生的log我去哪找啊？

其实这个问题非常简单了，也没花多少时间去解决，但我觉的非常好玩。coredump了自然会保存在core文件里了，无非就是把它找出来的问题了，在这里记录一下。

当然这里不管coredump的原因是什么，我只想看丢失的log。所以模拟的话在某个地方abort()就行

多线程调试嘛，先看线程信息，info thread，找到我的异步打印线程，切换进去看bt调用栈，正常是阻塞在条件变量是wait条件中的，frame切换到threadFunc(这个函数是我的异步log里面的循环的函数名)，剩下的就是print啦～不过，我的Buffer是用智能指针shared_ptr包裹的，直接->不行，gdb不识别，优化完.get()不让用，可能被inline掉了，只能直接从shared_ptr源码中找到_M_ptr成员来打印。

![gdb](https://github.com/linyacool/WebServer/blob/master/datum/gdb.png)
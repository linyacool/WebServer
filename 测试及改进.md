# 测试及改进

## 测试环境
* OS：Ubuntu 14.04
* 内存：8G
* CPU：I7-4720HQ

## 测试方法
* 理想的测试环境是两台计算机，带宽无限，现在的网卡虽然都是千兆网卡，但是普通家用的网线都是5类双绞线，最高100Mbps,在linux下用ethtool可以看到网卡的速度被限制为100Mbsp，无法更改为更高的，经测试很容易跑满带宽，因此退而选择本地环境。
* 使用工具Webbench，开启1000客户端进程，时间为60s
* 分别测试短连接和长连接的情况
* 关闭所有的输出及Log
* 为避免磁盘IO对测试结果的影响，测试响应为内存中的"Hello World"字符加上必要的HTTP头
* 我的最终版本中很多方面借鉴了muduo的思路，muduo中也提供了一个简单的HTTP echo测试，因此我将与muduo进行一个小小的对比，我修改了muduo测试的代码，使其echo相同的内容，关闭muduo的所有输出及Log
* 线程池开启4线程
* 因为发送的内容很少，为避免发送可能的延迟，关闭Nagle算法


## 测试结果及分析
测试截图放在最后  

| 服务器 | 短连接QPS | 长连接QPS | 
| - | :-: | -: | 
| WebServer | 126798| 335338 | 
| Muduo | 88430 | 358302 | 

* 首先很明显的一点是长链接能处理的请求数是短连接的三四倍，因为没有了连接建立和断开的开销，不需要频繁accept和shutdown\close等系统调用，也不需要频繁建立和销毁对应的结构体。
* 我的服务器在最后的版本中，没有改进输入输出Buffer，用了效率低下的string，muduo用的是设计良好的vector<char>，我将在后续改进这一点。这也造成了在长连接的情况下，我的server逊于muduo。虽说边沿触发效率高一点，但是还是比不过在Buffer上性能的优化的。
* 短链接的情况下，我的服务器要超过Muduo很多。原因在于：Muduo采用水平触发方式(Linux下用epoll)，并且做法是每次Acceptor只accept一次就返回，面对突然的并发量，必然会因为频繁的epoll_wait耽误大量的时间，而我的做法是用while包裹accept，一直accept到不能再accept。当然，如果同时连接的请求很少，陈硕在书中也提到过，假如一次只有一个连接，那么我的方式就会多一次accpet才能跳出循环，但是这样的代价似乎微不足道啊，换来的效率却高了不少。
* 空闲时，Server几乎不占CPU，短连接时，各线程的CPU负载比较均衡，长连接时，主线程负载0，线程池的线程负载接近100%，因为没有新的连接需要处理。各种情况均正常。
* 没有严格的考证，测试时发现，HTTP的header解析的结果用map比用unordered_map快，网上的博客里有很多人做了测试，我在做实验的时候大致也发现了。主要是因为数据量太小，一个HTTP请求头才几个头部字段，建立unordered_map的成本要比map高，数据量小，复杂度根本体现不出来。



## 测试结果截图
* WebServer短连接测试  
![shortWeb](https://github.com/linyacool/WebServer/blob/master/datum/WebServer.png)
* muduo短连接测试  
![shortMuduo](https://github.com/linyacool/WebServer/blob/master/datum/muduo.png)
* WebServer长连接测试  
![keepWeb](https://github.com/linyacool/WebServer/blob/master/datum/WebServerk.png)
* muduo长连接测试  
![keepMuduo](https://github.com/linyacool/WebServer/blob/master/datum/muduok.png)
* WebServer空闲负载  
![idle](https://github.com/linyacool/WebServer/blob/master/datum/idle.png)
* WebServer短连接CPU负载  
![short](https://github.com/linyacool/WebServer/blob/master/datum/close.png)
* WebServer长连接CPU负载  
![keep](https://github.com/linyacool/WebServer/blob/master/datum/keepalive.png)


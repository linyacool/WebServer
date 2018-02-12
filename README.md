# A C++ High Performance Web Server
  
## Introduction  

* 本项目为C++11编写的Web服务器，解析了get、post请求，可处理静态资源，支持HTTP长连接，并实现了异步日志，记录服务器运行状态。  

| Part Ⅰ | Part Ⅱ | Part Ⅲ | Part Ⅳ | Part Ⅴ | Part Ⅵ |
| :--------: | :---------: | :---------: | :---------: | :---------: | :---------: |
|  [项目目的](https://github.com/linyacool/WebServer/blob/master/%E9%A1%B9%E7%9B%AE%E7%9B%AE%E7%9A%84.md)|[连接的维护](https://github.com/linyacool/WebServer/blob/master/连接的维护.md)|[版本历史](https://github.com/linyacool/WebServer/blob/master/%E7%89%88%E6%9C%AC%E5%8E%86%E5%8F%B2.md)| [遇到的困难]() |  [测试及改进](https://github.com/linyacool/WebServer/blob/master/测试及改进.md) | [背景知识]()| 

## Envoirment  
* OS: Ubuntu 14.04
* Complier: g++ 4.8

## Build

	./build.sh

## Usage

	./WebServer [-t thread numbers] [-p port] [-l log file path(begin with '/')]

## Technical points
* 使用Epoll边沿触发的IO多路复用技术，非阻塞IO，使用Reactor模型
* 使用多线程充分利用多核CPU，并使用线程池避免线程频繁创建销毁的开销
* 使用基于小根堆的定时器关闭超时请求
* 主线程只负责accept请求，并以Round Robin的方式分发给其它IO线程(兼计算线程)，锁的争用只会出现在主线程和某一特定线程中
* 使用eventfd实现了线程的异步唤醒
* 使用双缓冲区技术实现了简单的异步日志系统
* 为减少内存泄漏的可能，使用智能指针等RAII机制
* 使用状态机解析了HTTP请求

## Others
除了项目基本的代码，进服务器进行压测时，对开源测试工具Webbench增加了Keep-Alive选项和测试功能: 改写后的[Webbench](https://github.com/linyacool/WebBench)




---

[![Build Status](https://travis-ci.org/linyacool/WebServer.svg?branch=master)](https://travis-ci.org/linyacool/WebServer)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](https://opensource.org/licenses/MIT)

---
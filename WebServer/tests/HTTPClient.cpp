#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <iostream>


using namespace std;

#define MAXSIZE 1024
#define IPADDRESS "127.0.0.1"
#define SERV_PORT 8888
#define FDSIZE 1024
#define EPOLLEVENTS 20

// static void handle_connection(int sockfd);
// static void handle_events(int epollfd,struct epoll_event *events,int num,int
// sockfd,char *buf);
// static void do_read(int epollfd,int fd,int sockfd,char *buf);
// static void do_read(int epollfd,int fd,int sockfd,char *buf);
// static void do_write(int epollfd,int fd,int sockfd,char *buf);
// static void add_event(int epollfd,int fd,int state);
// static void delete_event(int epollfd,int fd,int state);
// static void modify_event(int epollfd,int fd,int state);

int setSocketNonBlocking1(int fd) {
  int flag = fcntl(fd, F_GETFL, 0);
  if (flag == -1) return -1;

  flag |= O_NONBLOCK;
  if (fcntl(fd, F_SETFL, flag) == -1) return -1;
  return 0;
}
int main(int argc, char *argv[]) {
  int sockfd;
  struct sockaddr_in servaddr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  inet_pton(AF_INET, IPADDRESS, &servaddr.sin_addr);
  char buff[4096];
  buff[0] = '\0';
  // 发空串
  const char *p = " ";
  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == 0) {
    setSocketNonBlocking1(sockfd);
    cout << "1:" << endl;
    ssize_t n = write(sockfd, p, strlen(p));
    cout << "strlen(p) = " << strlen(p) << endl;
    sleep(1);
    n = read(sockfd, buff, 4096);
    cout << "n=" << n << endl;
    printf("%s", buff);
    close(sockfd);
  } else {
    perror("err1");
  }
  sleep(1);

  // 发"GET  HTTP/1.1"
  p = "GET  HTTP/1.1";
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == 0) {
    setSocketNonBlocking1(sockfd);
    cout << "2:" << endl;
    ssize_t n = write(sockfd, p, strlen(p));
    cout << "strlen(p) = " << strlen(p) << endl;
    sleep(1);
    n = read(sockfd, buff, 4096);
    cout << "n=" << n << endl;
    printf("%s", buff);
    close(sockfd);
  } else {
    perror("err2");
  }
  sleep(1);

  // 发
  // GET  HTTP/1.1
  // Host: 192.168.52.135:8888
  // Content-Type: application/x-www-form-urlencoded
  // Connection: Keep-Alive
  p = "GET / HTTP/1.1\r\nHost: 192.168.52.135:8888\r\nContent-Type: "
      "application/x-www-form-urlencoded\r\nConnection: Keep-Alive\r\n\r\n";
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == 0) {
    setSocketNonBlocking1(sockfd);
    cout << "3:" << endl;
    ssize_t n = write(sockfd, p, strlen(p));
    cout << "strlen(p) = " << strlen(p) << endl;
    sleep(1);
    n = read(sockfd, buff, 4096);
    cout << "n=" << n << endl;
    printf("%s", buff);
    close(sockfd);
  } else {
    perror("err3");
  }
  return 0;
}

// static void handle_connection(int sockfd)
// {
//     int epollfd;
//     struct epoll_event events[EPOLLEVENTS];
//     char buf[MAXSIZE];
//     int ret;
//     epollfd = epoll_create(FDSIZE);
//     add_event(epollfd,STDIN_FILENO,EPOLLIN);
//     for ( ; ; )
//     {
//         ret = epoll_wait(epollfd,events,EPOLLEVENTS,-1);
//         handle_events(epollfd,events,ret,sockfd,buf);
//     }
//     close(epollfd);
// }

// static void
// handle_events(int epollfd,struct epoll_event *events,int num,int sockfd,char
// *buf)
// {
//     int fd;
//     int i;
//     for (i = 0;i < num;i++)
//     {
//         fd = events[i].data.fd;
//         if (events[i].events & EPOLLIN)
//             do_read(epollfd,fd,sockfd,buf);
//         else if (events[i].events & EPOLLOUT)
//             do_write(epollfd,fd,sockfd,buf);
//     }
// }

// static void do_read(int epollfd,int fd,int sockfd,char *buf)
// {
//     int nread;
//     nread = read(fd,buf,MAXSIZE);
//         if (nread == -1)
//     {
//         perror("read error:");
//         close(fd);
//     }
//     else if (nread == 0)
//     {
//         fprintf(stderr,"server close.\n");
//         close(fd);
//     }
//     else
//     {
//         if (fd == STDIN_FILENO)
//             add_event(epollfd,sockfd,EPOLLOUT);
//         else
//         {
//             delete_event(epollfd,sockfd,EPOLLIN);
//             add_event(epollfd,STDOUT_FILENO,EPOLLOUT);
//         }
//     }
// }

// static void do_write(int epollfd,int fd,int sockfd,char *buf)
// {
//     int nwrite;
//     nwrite = write(fd,buf,strlen(buf));
//     if (nwrite == -1)
//     {
//         perror("write error:");
//         close(fd);
//     }
//     else
//     {
//         if (fd == STDOUT_FILENO)
//             delete_event(epollfd,fd,EPOLLOUT);
//         else
//             modify_event(epollfd,fd,EPOLLIN);
//     }
//     memset(buf,0,MAXSIZE);
// }

// static void add_event(int epollfd,int fd,int state)
// {
//     struct epoll_event ev;
//     ev.events = state;
//     ev.data.fd = fd;
//     epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
// }

// static void delete_event(int epollfd,int fd,int state)
// {
//     struct epoll_event ev;
//     ev.events = state;
//     ev.data.fd = fd;
//     epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
// }

// static void modify_event(int epollfd,int fd,int state)
// {
//     struct epoll_event ev;
//     ev.events = state;
//     ev.data.fd = fd;
//     epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
// }
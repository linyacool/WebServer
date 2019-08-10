// @Author Lin Ya
// @Email xxbbb@vip.qq.com
#pragma once
#include "FileUtil.h"
#include "MutexLock.h"
#include "noncopyable.h"
#include <memory>
#include <string>

// TODO 提供自动归档功能
class LogFile : noncopyable
{
public:
    // 每被append flushEveryN次，flush一下，会往文件写，只不过，文件也是带缓冲区的
    LogFile(const std::string& basename, int flushEveryN = 1024);
    ~LogFile();

    void append(const char* logline, int len);
    void flush();
    void rollFile();

private:
    void append_unlocked(const char* logline, int len);
    // basename.年月日_时分秒.主机名.进程号.log
    std::string getFilename();
    
    static const int KMaxFileSize=1024*1024*1024; // 日志到达1G时切换

    const std::string basename_;
    const int flushEveryN_;

    int count_;
    int curSize_;
    std::unique_ptr<MutexLock> mutex_;
    std::unique_ptr<AppendFile> file_;
};

// @Author Lin Ya
// @Email xxbbb@vip.qq.com
#include "LogFile.h"
#include "FileUtil.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>

using namespace std;


LogFile::LogFile(const string& basename, int flushEveryN)
  : basename_(basename),
    flushEveryN_(flushEveryN),
    count_(0),
    mutex_(new MutexLock)
{
    //assert(basename.find('/') >= 0);
    rollFile();
}

LogFile::~LogFile()
{ }

void LogFile::append(const char* logline, int len)
{
    {
        MutexLockGuard lock(*mutex_);
        append_unlocked(logline, len);
    }
    
    curSize_ += len; 
    if(curSize_ >= KMaxFileSize)
    {
        file_->flush();
        rollFile();
    }
}

void LogFile::flush()
{
    MutexLockGuard lock(*mutex_);
    file_->flush();
}

void LogFile::rollFile()
{
    string filename=getFilename();
    file_.reset(new AppendFile(filename));
    curSize_=0;
    count_=0;  
}

void LogFile::append_unlocked(const char* logline, int len)
{
    file_->append(logline, len);
    ++count_;
    if (count_ >= flushEveryN_)
    {
        count_ = 0;
        file_->flush();
    }
}

std::string LogFile::getFilename()
{
    std::string filename;
    filename.reserve(64);
    filename = basename_;

    char timebuf[16];
    time_t now=time(NULL);
    struct tm *tm=localtime(&now);
    strftime(timebuf,sizeof timebuf,".%y%m%d_%H%M%S.",tm);

    char hostbuf[64]; // getconf HOST_NAME_MAX
    gethostname(hostbuf,sizeof hostbuf);

    filename+=timebuf;
    filename+=hostbuf;
    filename+=".";
    filename+=to_string(getpid());
    filename+=".log";

    return filename;
}

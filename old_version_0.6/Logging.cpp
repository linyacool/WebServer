#include "Logging.h"
#include "CurrentThread.hpp"
#include "Thread.h"
#include <assert.h>

#include "AsyncLogging.h"

#include <iostream>
using namespace std;


static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;
static AsyncLogging *AsyncLogger_;

void once_init()
{
    AsyncLogger_ = new AsyncLogging(std::string("/linya_web_server.log"));
    AsyncLogger_->start();
}

void output(const char* msg, int len)
{
    pthread_once(&once_control_, once_init);
    AsyncLogger_->append(msg, len);
}

Logger::Impl::Impl(const char *fileName, int line)
  : stream_(),
    basename_(fileName),
    line_(line)
{
  //formatTime();
  //CurrentThread::tid();
  //stream_ << T(CurrentThread::tidString(), CurrentThread::tidStringLength());
}

void Logger::Impl::formatTime()
{
  // int64_t microSecondsSinceEpoch = time_.microSecondsSinceEpoch();
  // time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / Timestamp::kMicroSecondsPerSecond);
  // int microseconds = static_cast<int>(microSecondsSinceEpoch % Timestamp::kMicroSecondsPerSecond);
  // if (seconds != t_lastSecond)
  // {
  //   t_lastSecond = seconds;
  //   struct tm tm_time;
  //   if (g_logTimeZone.valid())
  //   {
  //     tm_time = g_logTimeZone.toLocalTime(seconds);
  //   }
  //   else
  //   {
  //     ::gmtime_r(&seconds, &tm_time); // FIXME TimeZone::fromUtcTime
  //   }

  //   int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
  //       tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
  //       tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
  //   assert(len == 17); (void)len;
  // }

  // if (g_logTimeZone.valid())
  // {
  //   Fmt us(".%06d ", microseconds);
  //   assert(us.length() == 8);
  //   stream_ << T(t_time, 17) << T(us.data(), 8);
  // }
  // else
  // {
  //   Fmt us(".%06dZ ", microseconds);
  //   assert(us.length() == 9);
  //   stream_ << T(t_time, 17) << T(us.data(), 9);
  // }
}

Logger::Logger(const char *fileName, int line)
  : impl_(fileName, line)
{
}

Logger::~Logger()
{
  impl_.stream_ << " - " << impl_.basename_ << ':' << impl_.line_ << '\n';
  const LogStream::Buffer& buf(stream().buffer());
  output(buf.data(), buf.length());
}
#pragma once

#include <spdlog/spdlog.h>

#define tlog SPDLOG_TRACE
#define dlog SPDLOG_DEBUG
#define ilog SPDLOG_INFO
#define wlog SPDLOG_WARN
#define elog SPDLOG_ERROR
#define clog SPDLOG_CRITICAL

namespace spdlog
{
class logger;
}

namespace noble_steed
{
class Logger
{
  public:
    Logger();
    ~Logger();

    void initialize();

    void terminate();

  private:
    std::shared_ptr<spdlog::logger> logger;
};
} // namespace noble_steed
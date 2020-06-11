#pragma once

#include <spdlog/spdlog.h>
#include <noble_steed/core/common.h>
#include <spdlog/fmt/ostr.h>

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
const String IP_DEFAULT_LOGGER_INFO = "DLInfo";
// Log level for the 3 different sinks that are made per logger - a sink is just a place where the data gets logged to
// Setting any of these to Off will cause the "sink" to not be created - so for a logger that you don't want to output
// to the console in release for example, set console_release_level to Uber_Logger::Off.
struct Log_Level_Info
{
    // Console log level for debug build
    int console_debug_level;

    // Console log level for release build
    int console_release_level;

    // Log level of the daily file in debug mode.. the daily file is generated in the Logs folder for each day - consecutive executions of the program will add to the
    // day's log without truncating it - these logs will never be overwritten
    int daily_file_debug_level;

    // Log level of the daily file in release mode.. the daily file is generated in the Logs folder for each day - consecutive executions of the program will add to the
    // day's log without truncating it - these logs will never be overwritten. This is good for logging things that might need to be looked back on - such as warnings
    // or critical errors
    int daily_file_release_level;

    // Log level of the per execution log in debug mode.. the per execution file is overwritten every time the program is run again - this is really good for detailed
    // traces or debug statements trying to verify code is working correctly or for debugging stuff
    int per_exec_file_debug_level;

    // Log level of the per execution log in release mode.. the per execution file is overwritten every time the program is run again - this is really good for detailed
    // traces or debug statements trying to verify code is working correctly or for debugging stuff
    int per_exec_file_release_level;
};

// Enables logging easily - there is a default logger that saves the logged message to three different locations (sinks) - the console, a daily file, and a per execution file.
// The log levels can be specified with initialize function for the default logger, and in add_logger for all other loggers. If Uber_Logger::Off is specified as the log level
// for any of the sinks, then the correspoding sink will not be created and nothing logged to it (ie it wont create a new file if the sink is a file). The daily file sink
// persists between executions and a new file is created each day - the file is never truncated. The per execution file sink is truncated each execution of the program.
//
// The default logger can be used with macro xxxlog("message") macro where xxx is a single letter that specifies the log level of the message (ie ilog("message") for info log level).
// To add a new logger use add_logger and specify the name and the Log Levels for each sink. The logger can then be accessed anywhere using xxxlogc("logger_name","message")
// where xxx is a single letter that specifies the log level of the message.
//
// In the message you can use bracket substitution to specify value - for example ilog("This is a message with the number {}",i) where i is the value. Most types
// will work with this by default, but to get a custom type to work you just have to overload operator<< - see examples above. You can also specify and index in the
// brackets to specify which parameter goes in which brackets - ie ilog("This is a message with two numbers - the first is {1} and the second is {0}",second,first)
class Logger
{
  public:
    enum Level_Enum
    {
        Trace = SPDLOG_LEVEL_TRACE,
        Debug = SPDLOG_LEVEL_DEBUG,
        Info = SPDLOG_LEVEL_INFO,
        Warn = SPDLOG_LEVEL_WARN,
        Error = SPDLOG_LEVEL_ERROR,
        Critical = SPDLOG_LEVEL_CRITICAL,
        Off = SPDLOG_LEVEL_OFF
    };
    
    Logger();
    ~Logger();

    void add_logger(const String & logger_name, const Log_Level_Info & log_level_inf, const String & pattern="");

    void remove_logger(const String & logger_name);

    void initialize(const Variant_Map & init_params);

    void terminate();

  private:
    std::shared_ptr<spdlog::logger> default_logger_;
    std::vector<std::shared_ptr<spdlog::logger>> custom_loggers_;
};
} // namespace noble_steed
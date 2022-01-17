#include <chrono>
#include <ctime>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "logger.h"
#include "filesystem.h"

#include "../core/context.h"

namespace noble_steed
{
const String LOGS_FOLDER_PATH = "logs";

Logger::Logger() // : logger(nullptr)
{}

Logger::~Logger()
{}

void Logger::add_logger(const String & logger_name, const Log_Level_Info & log_level_inf, const String & pattern)
{
    std::list<spdlog::sink_ptr> multi_sink_list;

    std::time_t current_time = std::time(0);
    std::tm * local_time = std::localtime(&current_time);
    char str[20];
    String format = "%a %b %d";
    std::strftime(str, 20, format.c_str(), local_time);

    String cur_date = str;
    String app_name = fs::executable_name();
    String per_exec_name("Most Recent");

    String daily_file_path;
    String per_exec_file_path;

    if (logger_name.empty())
    {
        daily_file_path = (LOGS_FOLDER_PATH + app_name + " " + cur_date);
        per_exec_file_path = (LOGS_FOLDER_PATH + app_name + " " + per_exec_name);
    }
    else
    {
        daily_file_path = (LOGS_FOLDER_PATH + app_name + " " + logger_name + " " + cur_date);
        per_exec_file_path = (LOGS_FOLDER_PATH + app_name + " " + logger_name + " " + per_exec_name);
    }

    daily_file_path += ".log";
    per_exec_file_path += ".log";

#ifdef DEBUG_VERSION
    if (log_level_inf.console_debug_level != Off)
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(static_cast<spdlog::level::level_enum>(log_level_inf.console_debug_level));
        multi_sink_list.push_back(console_sink);
    }

    if (log_level_inf.daily_file_debug_level != Off)
    {
        auto daily_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(daily_file_path, false);
        daily_file_sink->set_level(static_cast<spdlog::level::level_enum>(log_level_inf.daily_file_debug_level));
        multi_sink_list.push_back(daily_file_sink);
    }

    if (log_level_inf.per_exec_file_debug_level != Off)
    {
        auto per_exec_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(per_exec_file_path, true);
        per_exec_file_sink->set_level(static_cast<spdlog::level::level_enum>(log_level_inf.per_exec_file_debug_level));
        multi_sink_list.push_back(per_exec_file_sink);
    }
#else
    if (log_level_inf.console_release_level != Off)
    {
        auto console_sink_deleter = [](spdlog::sinks::stdout_color_sink_mt * sink) { ns_ctxt.free(sink); };
        std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink(ns_ctxt.malloc<spdlog::sinks::stdout_color_sink_mt>(),
                                                                          console_sink_deleter);
        console_sink->set_level(static_cast<spdlog::level::level_enum>(log_level_inf.console_release_level));
        multi_sink_list.push_back(console_sink);
    }

    if (log_level_inf.daily_file_release_level != Off)
    {
        auto file_sink_deleter = [](spdlog::sinks::basic_file_sink_mt * sink) { ns_ctxt.free(sink); };
        std::shared_ptr<spdlog::sinks::basic_file_sink_mt> daily_file_sink(ns_ctxt.malloc<spdlog::sinks::basic_file_sink_mt>(daily_file_path, false),
                                                                           file_sink_deleter);
        daily_file_sink->set_level(static_cast<spdlog::level::level_enum>(log_level_inf.daily_file_release_level));
        multi_sink_list.push_back(daily_file_sink);
    }

    if (log_level_inf.per_exec_file_release_level != Off)
    {
        auto file_sink_deleter = [](spdlog::sinks::basic_file_sink_mt * sink) { ns_ctxt.free(sink); };
        std::shared_ptr<spdlog::sinks::basic_file_sink_mt> per_exec_file_sink(
            ns_ctxt.malloc<spdlog::sinks::basic_file_sink_mt>(per_exec_file_path, true), file_sink_deleter);
        per_exec_file_sink->set_level(static_cast<spdlog::level::level_enum>(log_level_inf.per_exec_file_release_level));

        multi_sink_list.push_back(per_exec_file_sink);
    }
#endif

    auto logger = std::make_shared<spdlog::logger>(logger_name, multi_sink_list.begin(), multi_sink_list.end());
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::info);

    if (!pattern.empty())
        logger->set_pattern(pattern);
    else
        logger->set_pattern("[%I:%M:%S.%e %p TID:%t] [%s:%#] %^[%l]%$ %v");

    if (logger_name.empty())
    {
        default_logger_ = logger;
        spdlog::set_default_logger(default_logger_);
    }
    else
    {
        custom_loggers_.push_back(logger);
        spdlog::register_logger(custom_loggers_.back());
    }
}

void Logger::remove_logger(const String & logger_name)
{
    if (logger_name.empty())
        return;

    spdlog::drop(logger_name);

    for (int i = 0; i < custom_loggers_.size(); ++i)
    {
        if (logger_name == custom_loggers_[i]->name())
        {
            custom_loggers_[i] = custom_loggers_.back();
            custom_loggers_.pop_back();
            return;
        }
    }
}

void Logger::initialize(const Variant_Map & init_params)
{
    Log_Level_Info def_inf;
    def_inf.console_debug_level = Logger::Debug;
    def_inf.console_release_level = Logger::Debug;
    def_inf.daily_file_debug_level = Logger::Off;
    def_inf.daily_file_release_level = Logger::Off;
    def_inf.per_exec_file_debug_level = Logger::Trace;
    def_inf.per_exec_file_release_level = Logger::Off;
    
    auto fiter = init_params.find(IP_DEFAULT_LOGGER_INFO);
    if (fiter != init_params.end())
        def_inf = fiter->second.get_custom_value<Log_Level_Info>();

    // Create dir if it doesn't exist
    fs::create_directories(LOGS_FOLDER_PATH);
    add_logger("", def_inf);
}

void Logger::terminate()
{
    spdlog::shutdown();
}
} // namespace noble_steed
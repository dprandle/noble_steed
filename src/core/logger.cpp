#include <noble_steed/core/logger.h>
#include <noble_steed/core/context.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace noble_steed
{
Logger::Logger()// : logger(nullptr)
{}

Logger::~Logger()
{}

void Logger::initialize()
{
    auto console_sink_deleter = [](spdlog::sinks::stdout_color_sink_mt * sink) { ns_ctxt.free(sink); };
    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink(
        ns_ctxt.malloc<spdlog::sinks::stdout_color_sink_mt>(),
        console_sink_deleter);
    console_sink->set_level(spdlog::level::debug);

    auto file_sink_deleter = [](spdlog::sinks::basic_file_sink_mt * sink) { ns_ctxt.free(sink); };
    std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink(
        ns_ctxt.malloc<spdlog::sinks::basic_file_sink_mt>("logs/multisink.txt", true),
        file_sink_deleter);
    file_sink->set_level(spdlog::level::info);

    spdlog::sinks_init_list sink_list = {file_sink, console_sink};

    auto log_deleter = [](spdlog::logger * lg) { ns_ctxt.free(lg); };
    logger = std::shared_ptr<spdlog::logger>(
        ns_ctxt.malloc<spdlog::logger>("multi_sink", sink_list.begin(), sink_list.end()),
        log_deleter);

    logger->set_level(spdlog::level::trace);
    logger->set_pattern("[%m/%d %X.%e TID:%t] [%s:%#] %^[%l]%$ %v");
    spdlog::set_default_logger(std::shared_ptr<spdlog::logger>(logger));
}

void Logger::terminate()
{
    spdlog::shutdown();
}
} // namespace noble_steed
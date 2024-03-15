#include "Logger.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "../common/misc.h"

#include <iostream>
#include <memory>
#include <string>

namespace Ironhead
{
namespace Utils
{
namespace Logger
{
  static const char *LOG_PATTERN = "%^[%H:%M:%S.%e] [%l] [%@ %!]: %v%$";
  std::vector<spdlog::sink_ptr> sinks;

  void init(std::string filename)
  {
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
#if defined(DEBUG)
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(widen(filename), true));
#endif

    spdlog::set_default_logger(std::make_shared<spdlog::logger>("ironhead logger", begin(sinks), end(sinks)));

    spdlog::set_pattern(LOG_PATTERN);

    spdlog::set_error_handler([](const std::string &msg) {
        std::cerr << "spdlog error: " << msg << std::endl;
    });

    spdlog::flush_on(spdlog::level::debug);
#if defined(DEBUG)
    spdlog::set_level(spdlog::level::trace);
#else
    spdlog::set_level(spdlog::level::info);
#endif
  }


} // namespace Logger
} // namespace Utils
} // namespace Ironhead

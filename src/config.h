#pragma once
#include <dpp/json_fwd.h>

namespace config
{
void init(const std::string &configFile);

dpp::json &get(const std::string &key = "");
} // namespace config

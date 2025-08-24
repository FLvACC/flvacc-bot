#pragma once
#include <dpp/dpp.h>

namespace listeners
{
void on_ready(const dpp::ready_t &event);
void grant_roles(dpp::cluster &bot);
} // namespace listeners

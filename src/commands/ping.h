#pragma once
#include "command.h"
#include <dpp/dpp.h>

struct PingCommand : public Command
{
	static dpp::slashcommand register_command(dpp::cluster &bot);
	static void callback(const dpp::slashcommand_t &event);
};

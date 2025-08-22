#pragma once
#include "command.h"
#include <dpp/dpp.h>

struct ApplyCommand : public AsyncCommand
{
	static dpp::slashcommand register_command(dpp::cluster &bot);
	static dpp::job callback(const dpp::slashcommand_t event);
};

#include "listeners.h"
#include "apply.h"
#include "ping.h"

void listeners::on_ready(const dpp::ready_t &event)
{
	dpp::cluster &bot = *event.owner;

	if (dpp::run_once<struct register_bot_commands>())
	{
		bot.global_bulk_command_create({ PingCommand::register_command(bot),
										 ApplyCommand::register_command(bot) });
	}
}

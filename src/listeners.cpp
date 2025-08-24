#include "listeners.h"
#include "apply.h"
#include "config.h"
#include "ping.h"
#include <dpp/dpp.h>
#include <fmt/core.h>
#include <pqxx/pqxx>
#include <vector>

void listeners::on_ready(const dpp::ready_t &event)
{
	dpp::cluster &bot = *event.owner;

	if (dpp::run_once<struct register_bot_commands>())
	{
		bot.global_bulk_command_create({ PingCommand::register_command(bot),
			ApplyCommand::register_command(bot) });

		bot.start_timer([&bot](dpp::timer t)
			{ grant_roles(bot); }, 1);
	}
}

void listeners::grant_roles(dpp::cluster &bot)
{
	pqxx::connection con(config::get("postgres-url"));

	con.listen("roles", [&bot](pqxx::notification notif)
		{ 
			bot.log(dpp::ll_info, fmt::format("Received roles notification: {}", notif.payload.c_str()));

			dpp::json data = dpp::json::parse(notif.payload);

			dpp::snowflake guildId(config::get("guild-id"));
			dpp::snowflake userId(data.at("userId"));

			dpp::json roleIds = data.at("roleIds");

			for (dpp::snowflake roleId : roleIds)
			{
				bot.guild_member_add_role(guildId, userId, roleId, [&bot, userId, roleId](dpp::confirmation_callback_t callback)
					{
						if (callback.is_error())
						{
							bot.direct_message_create(userId, dpp::message("It looks like I was unable to issue you a role. Please contact an ATC instructor for further assistance."));
							bot.log(dpp::ll_error, fmt::format("Unable to issue role {} to {}", roleId.str(), userId.str()));
						}
						else
						{
							bot.log(dpp::ll_info, fmt::format("Issued role {} to {}", roleId.str(), userId.str()));
						}
					});
			} });

	con.await_notification(1);
}

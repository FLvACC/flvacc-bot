#include "listeners.h"
#include "config.h"
#include "enroll.h"
#include "ping.h"
#include <dpp/dpp.h>
#include <fmt/core.h>
#include <pqxx/pqxx>
#include <vector>

namespace
{
std::unique_ptr<pqxx::connection> con;
};

void listeners::on_ready(const dpp::ready_t &event)
{
	dpp::cluster &bot = *event.owner;

	if (dpp::run_once<struct register_bot_commands>())
	{
		bot.global_bulk_command_create({ PingCommand::register_command(bot),
			EnrollCommand::register_command(bot) });

		grant_roles(bot);
	}
}

void listeners::grant_roles(dpp::cluster &bot)
{
	::con = std::make_unique<pqxx::connection>(pqxx::connection(config::get("postgres-url")));

	::con->listen("roles", [&bot](pqxx::notification notif)
		{
		bot.log(dpp::ll_info, fmt::format("Received roles notification: {}", notif.payload.c_str()));

		dpp::json data = dpp::json::parse(notif.payload);

		dpp::snowflake guildId(config::get("guild-id").get<std::string>());
		dpp::snowflake userId(data.at("userId").get<std::string>());

		dpp::json roleIds = data.at("roleIds");

		for (auto id : roleIds)
		{
			dpp::snowflake roleId(id.get<std::string>());

			bot.set_audit_reason("User passed ATC Basics Entrance Exam").guild_member_add_role(guildId, userId, roleId, [&bot, userId, roleId](dpp::confirmation_callback_t callback)
				{
						if (callback.is_error())
						{
							bot.direct_message_create(userId, dpp::message("It looks like I was unable to issue you a role. Please contact an ATC instructor or manager for further assistance."));
							bot.log(dpp::ll_error, fmt::format("Unable to issue role {} to {}", roleId.str(), userId.str()));
						}
						else
						{
							bot.log(dpp::ll_info, fmt::format("Issued role {} to {}", roleId.str(), userId.str()));
						} });
		}

		bot.direct_message_create(userId, dpp::message("Congratulations on passing your exam, and welcome to the Flightline vACC!\n\nAs a starting point, please read <#829473978078724136> to familiarize yourself with the training flow and standard procedures laid out here.")); });

	bot.start_timer([](dpp::timer)
		{ ::con->get_notifs(); }, 1);
}

#include "enroll.h"
#include "config.h"
#include <dpp/dpp.h>
#include <dpp/json.h>
#include <fmt/format.h>

dpp::slashcommand EnrollCommand::register_command(dpp::cluster &bot)
{
	bot.register_command("enroll", &EnrollCommand::callback);

	return dpp::slashcommand("enroll", "Enroll yourself for a publically available exam", bot.me.id)
		.add_option(dpp::command_option(dpp::co_sub_command, "atc", "Enroll yourself for the ATC Basics Entrance Exam"));
}

dpp::job EnrollCommand::callback(const dpp::slashcommand_t event)
{
	dpp::cluster &bot = *event.owner;
	auto subcommand = event.command.get_command_interaction().options[0];

	if (subcommand.name == "atc")
	{
		const dpp::guild_member &member = event.command.member;
		std::string atcRoleId = config::get("atc-role-id").get<std::string>();

		if (!atcRoleId.empty() && std::find(member.get_roles().begin(), member.get_roles().end(), dpp::snowflake(atcRoleId)) != member.get_roles().end())
		{
			co_await event.co_reply(dpp::message("Could not create exam: You are already an Air Traffic Controller.").set_flags(dpp::message_flags::m_ephemeral));

			co_return;
		}

		dpp::async thinking = event.co_thinking(true);
		dpp::snowflake examineeId = event.command.get_issuing_user().id;
		dpp::http_request_completion_t result;

		try
		{
			result = co_await bot.co_request("https://flvacc.com/api/exams/generate", dpp::m_get, "", "application/json",
				{
					{ "api-key", config::get("flvacc-api-key").get<std::string>() },
					{ "examinee-id", examineeId.str() },
					{ "type", "atc-basics" },
				});
		}
		catch (const std::exception &e)
		{
			bot.log(dpp::ll_error, e.what());

			event.edit_response("An error has occured while trying to create an exam, please try again later.");

			co_return;
		}

		dpp::json data = dpp::json::parse(result.body);

		co_await thinking;

		if (result.status == 200)
		{
			bot.log(dpp::ll_info, fmt::format("Created ATC Basics Exam for {} with exam id {}", examineeId.str(), data.at("id").get<std::string>()));

			dpp::message msg(event.command.channel_id, "Linked below is your ATC Basics Exam link.");

			msg.add_component(dpp::component()
					.add_component(
						dpp::component()
							.set_label("Exam Link")
							.set_type(dpp::cot_button)
							.set_url("https://flvacc.com/exams/" + data.at("id").get<std::string>())
							.set_style(dpp::cos_link)));

			dpp::confirmation_callback_t callback = co_await bot.co_direct_message_create(event.command.get_issuing_user().id, msg);

			if (callback.is_error())
			{
				msg.set_content("You have been assigned the ATC Basics Exam! However, I was unable to message you. Please save this exam link as you will not be able to see it again upon dismissing this message.");

				event.edit_response(msg);

				co_return;
			}

			event.edit_response("You have been assigned the ATC Basics Exam! I've sent the exam link through our direct messages.");
		}
		else
		{
			event.edit_response("Could not create exam: " + data.at("error").get<std::string>());
		}
	}
}

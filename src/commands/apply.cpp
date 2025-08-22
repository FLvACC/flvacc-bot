#include "apply.h"
#include "config.h"
#include <dpp/dpp.h>
#include <dpp/json.h>

dpp::slashcommand ApplyCommand::register_command(dpp::cluster &bot)
{
	bot.register_command("apply", &ApplyCommand::callback);

	return dpp::slashcommand("apply", "Enroll yourself for a publically available exam", bot.me.id)
		.add_option(dpp::command_option(dpp::co_sub_command, "atc", "ATC Basics Exam"));
}

dpp::job ApplyCommand::callback(const dpp::slashcommand_t event)
{
	auto subcommand = event.command.get_command_interaction().options[0];

	if (subcommand.name == "atc")
	{
		dpp::async thinking = event.co_thinking(true);

		// TODO: Make these all variable based on an upcoming API endpoint on flvacc.com to fetch exam data
		int duration = 1800;
		dpp::snowflake examineeId = event.command.get_issuing_user().id;
		std::string formId = "1J6pmoUGXcP-C8Sy5WVwojyH5VTStpjsl-Os8Uw9ey-g";

		dpp::http_request_completion_t result;

		try
		{
			result = co_await event.owner->co_request("https://flvacc.com/api/exams/generate", dpp::m_get, "", "application/json", {
																																	   { "X-Api-Key", config::get("flvacc-api-key").get<std::string>() },
																																	   { "duration", std::to_string(duration) },
																																	   { "examinee-id", examineeId.str() },
																																	   { "form-id", formId },
																																   });
		}
		catch (const std::exception &e)
		{
			event.owner->log(dpp::loglevel::ll_error, e.what());

			event.edit_response("An error has occured while trying to create an exam, please try again later.");

			co_return;
		}

		co_await thinking;

		dpp::json data = dpp::json::parse(result.body);

		if (result.status == 200)
		{
			dpp::message msg(event.command.channel_id, "Here is your ATC Basics Exam link!");

			msg.add_component(dpp::component()
								  .add_component(
									  dpp::component()
										  .set_label("Exam Link")
										  .set_type(dpp::cot_button)
										  .set_url("https://flvacc.com/exams/" + data.at("id").get<std::string>())
										  .set_style(dpp::cos_link)));

			dpp::confirmation_callback_t callback = co_await event.owner->co_direct_message_create(event.command.get_issuing_user().id, msg);

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

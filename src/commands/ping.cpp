#include "ping.h"

dpp::slashcommand PingCommand::register_command(dpp::cluster &bot)
{
	bot.register_command("ping", &PingCommand::callback);

	return dpp::slashcommand("ping", "Show bot latency", bot.me.id);
}

void PingCommand::callback(const dpp::slashcommand_t &event)
{
	dpp::cluster *bot = event.owner;

	dpp::embed embed = dpp::embed()
						   .set_color(dpp::colors::red_wine)
						   .set_title("Latency")
						   .set_thumbnail(bot->me.get_avatar_url());

	double restPing = bot->rest_ping * 1000;

	embed.add_field("REST API", std::to_string(restPing) + "ms");

	dpp::message msg(event.command.channel_id, embed);

	event.reply(msg);
}

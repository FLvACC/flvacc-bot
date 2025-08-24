#include "config.h"
#include "listeners.h"
#include <dpp/dpp.h>
#include <dpp/json.h>

int main(int argc, char **argv)
{
	config::init("config.json");

	dpp::cluster bot(config::get("token").get<std::string>(), dpp::i_default_intents);

	bot.on_log(dpp::utility::cout_logger());
	bot.on_ready(&listeners::on_ready);

	bot.start(dpp::st_wait);

	return 0;
}

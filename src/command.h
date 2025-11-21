#pragma once
#include <dpp/dpp.h>

/**
 * Base command struct for commands to inherit
 */
struct Command
{
	static dpp::slashcommand register_command(dpp::cluster &bot);
	static void callback(const dpp::slashcommand_t &event);
};

/**
 * Base command struct for asynchronous commands to inherit
 */
struct AsyncCommand
{
	static dpp::slashcommand register_command(dpp::cluster &bot);

	/** `event` needs to be passed by value to prevent coroutine issues */
	static dpp::job callback(const dpp::slashcommand_t event);
};

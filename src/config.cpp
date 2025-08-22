#include "config.h"
#include <dpp/json.h>
#include <fstream>

namespace
{
dpp::json configDocument;
};

void config::init(const std::string &configFile)
{
	std::ifstream f(configFile);

	f >> configDocument;
}

dpp::json &config::get(const std::string &key)
{
	if (key.empty())
	{
		return configDocument;
	}

	return configDocument.at(key);
}

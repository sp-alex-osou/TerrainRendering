#include "Config.h"

Config::ConfigCollection Config::config;


void Config::addFile(const std::string& path)
{
	std::string line, key, value;
	std::ifstream file;

	file.open(path.c_str());

	if (!file.is_open())
		throw std::exception();

	while (file.good())
	{
		std::getline(file, line);

		if (line.empty())
			continue;

		std::istringstream sstream = std::istringstream(line);

		std::getline(sstream, key, '=');
		std::getline(sstream, value, '=');

		config[key] = value;
	}

	file.close();
}


const std::string& Config::getValue(const std::string& key)
{
	ConfigCollection::const_iterator it;

	if ((it = config.find(key)) == config.end())
		throw std::exception();

	return it->second;
}

#include "INIReader.h"

#include "WPILIB.h"

#include <iostream>
#include <fstream>
#include <string>

#include "Logger.h"

INIReader::INIReader(std::string filename)
{
	std::ifstream iniFile(filename);
	if(iniFile.is_open())
	{
		std::string section = "";
		std::string line;
		while(getline(iniFile, line))
		{
			//remove whitespace
			line.erase(
				    std::remove_if(line.begin(), line.end(),
				        [](char c) -> bool
				        {
				            return std::isspace<char>(c, std::locale::classic());
				        }),
					line.end());

			std::string::size_type len = line.size();

			std::string::size_type equalpos = line.find('=');

			if(line[0] == ';')
				continue;

			if(line[0] == '[')
			{
				if(line[len-1] == ']')
				{
					section = line.substr(1, len-2);
					Logger::instance()->logInfo("INI INFO Switching section to: %s", section.c_str());
				}
			} else if(equalpos != std::string::npos){
				std::string key = line.substr(0,equalpos);
				std::string val = line.substr(equalpos+1, line.size());
				values[section][key] = val;
				Logger::instance()->logInfo("INI INFO REVIEVED -->:\n\tsection:%s\n\tkey:%s\n\tvalue:%s",
						section.c_str(),
						key.c_str(),
						val.c_str());
			}
		}
		iniFile.close();
	}
}


std::string INIReader::get(std::string sectionName, std::string keyName)
{
	return values[sectionName][keyName];
}

int INIReader::getInt(std::string sectionName, std::string keyName)
{
	int i = 0;
	std::string val = values[sectionName][keyName].c_str();
	try {
		i = std::stoi(val);
	} catch (std::invalid_argument &e)
	{
		Logger::instance()->logError("INI INFO CANNOT CONVERT TO _INT_-->: INVALID ARGUMENT\n\tsection:%s\n\tkey:%s\n\tvalue:%s",
				sectionName.c_str(),
				keyName.c_str(),
				val.c_str());
	} catch (std::out_of_range &e)
	{
		Logger::instance()->logError("INI INFO CANNOT CONVERT TO _INT_-->: OUT OF RANGE\n\tsection:%s\n\tkey:%s\n\tvalue:%s",
				sectionName.c_str(),
				keyName.c_str(),
				val.c_str());
	}
	return i;
}

bool INIReader::getBool(std::string sectionName, std::string keyName)
{
	std::string string = values[sectionName][keyName];
	std::transform(string.begin(), string.end(), string.begin(), ::tolower);
	    if (string == "true" || string == "yes" || string == "on" || string == "1")
	        return true;
	    return false;
}

float INIReader::getFloat(std::string sectionName, std::string keyName)
{
	float f = 0;
	std::string val = values[sectionName][keyName].c_str();
	try {
		f = std::stof(val);
	} catch (std::invalid_argument &e)
	{
		Logger::instance()->logError("INI INFO CANNOT CONVERT TO _INT_-->: INVALID ARGUMENT\n\tsection:%s\n\tkey:%s\n\tvalue:%s",
				sectionName.c_str(),
				keyName.c_str(),
				val.c_str());
	} catch (std::out_of_range &e)
	{
		Logger::instance()->logError("INI INFO CANNOT CONVERT TO _INT_-->: OUT OF RANGE\n\tsection:%s\n\tkey:%s\n\tvalue:%s",
				sectionName.c_str(),
				keyName.c_str(),
				val.c_str());
	}
	return f;
}

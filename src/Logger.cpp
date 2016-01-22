/*
 * Logger.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: feds
 */

#include <Logger.h>
#include <time.h>

Logger::Logger() {
	char timeString[64];
	strftime(timeString, sizeof(timeString), "%g%m%e_%H%M", 0);

	logFile.open("/users/lvuser/logFile." + (std::string)timeString + ".txt");
	csvFile.open("/users/lvuser/logFile." + (std::string)timeString + ".csv");
}

void Logger::logInfo(char *msg)
{
	logFile << "[INFO] " << msg << std::endl;
	save();
}

void Logger::logError(char *msg)
{
	logFile << "!!! [ERROR] " << msg << std::endl;
	std::cerr << "!!! [ERROR] " << msg << std::endl;
	save();
}

void Logger::logCSV(struct CSV *data)
{
	csvFile << data->totalCurrent << ','
			<< data->motorCurrent << std::endl;
	save();
}

void Logger::save()
{
	logFile.flush();
	csvFile.flush();
}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}


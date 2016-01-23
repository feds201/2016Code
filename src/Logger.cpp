/*
 * Logger.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: feds
 */

#include <Logger.h>
#include <time.h>
#include <wpilib.h>

Logger *Logger::singlton = 0;

Logger::Logger() {
	char timeString[64];
	time_t rawTime;
	struct tm * timeInfo;

	time(&rawTime);
	timeInfo = localtime (&rawTime);
	strftime(timeString, sizeof(timeString), "%g%m%e_%H%M", timeInfo);

	logFile.open("/home/lvuser/logFile" + (std::string)timeString + ".txt");
	csvFile.open("/home/lvuser/logFile" + (std::string)timeString + ".csv");
}

void Logger::logInfo(const char *msg)
{
	logFile << "[INFO] " << msg << std::endl;
	save();
}

void Logger::logError(const char *msg)
{
	logFile << "!!! [ERROR] " << msg << std::endl;
	std::cerr << "!!! [ERROR] " << msg << std::endl;
	save();
}

void Logger::logCSV(struct CSV *data)
{
	csvFile	<< data->voltage << ','
			<< data->totalCurrent << ','
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


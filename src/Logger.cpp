/*
 * Logger.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: feds
 */

#include <Logger.h>
#include <wpilib.h>
#include <MyTimer.h>

Logger *Logger::singlton = 0;

Logger::Logger() {
	gettimeofday(&startTime, 0);

	char timeString[64];
	time_t rawTime;
	struct tm * timeInfo;

	time(&rawTime);
	timeInfo = localtime (&rawTime);
	strftime(timeString, sizeof(timeString), "%g%m%e_%H%M", timeInfo);

	logFile.open("/home/lvuser/logFile" + (std::string)timeString + ".txt");
	csvFile.open("/home/lvuser/logFile" + (std::string)timeString + ".csv");

	csvFile
			<< "time" << ','
			<< "voltage" << ','
			<< "totalCurrent" << ','
			<< "driveCurrents0" << ','
			<< "driveCurrents1" << ','
			<< "driveCurrents2" << ','
			<< "driveCurrents3" << ','
			<< "driveSetpoint0" << ','
			<< "driveSetpoint1" << ','
			<< "driveSetpoint2" << ','
			<< "driveSetpoint3" << ','
			<< "driveSpeed0" << ','
			<< "driveSpeed1" << ','
			<< "driveSpeed2" << ','
			<< "driveSpeed3" << ','
			<< "shooterRPMActualL" << ','
			<< "shooterRPMActualR" << ','
			<< "shooterRPMSetpoint" << ','
			<< "shooterCylinderUp" << ','
			<< "lifterCurrent" << ','
			<< "pickupIsUp" << std::endl;
}

void Logger::logInfo(const char *msg, ... )
{
	va_list args;
	va_start(args,msg);

	char buffer[256];
	vsnprintf(buffer, sizeof(buffer), msg, args);

	logFile << timer.getTotalTime() << " - [INFO] " << buffer << std::endl;
	std::cout << timer.getTotalTime() << " - [INFO] " << buffer << std::endl;
	save();
}

void Logger::logError(const char *msg, ... )
{
	va_list args;
	va_start(args,msg);

	char buffer[256];
	vsnprintf(buffer, sizeof(buffer), msg, args);

	logFile << "[ERROR] " << buffer << std::endl;
	std::cerr << "[ERROR] " << buffer << std::endl;
	save();
}

void Logger::logCSV(struct CSVVals *data)
{
	double time = timer.getTotalTime();

	csvFile
			<< time << ','
			<< data->voltage << ','
			<< data->totalCurrent << ','
			<< data->driveCurrents[0] << ','
			<< data->driveCurrents[1] << ','
			<< data->driveCurrents[2] << ','
			<< data->driveCurrents[3] << ','
			<< data->driveSetpoints[0] << ','
			<< data->driveSetpoints[1] << ','
			<< data->driveSetpoints[2] << ','
			<< data->driveSetpoints[3] << ','
			<< data->shooterRPMActualL << ','
			<< data->shooterRPMActualR << ','
			<< data->shooterRPMSetpoint << ','
			<< (data->shooterCylinderUp ? 1 : 0) << ','
			<< data->lifterCurrent << ','
			<< (data->pickupIsUp ? 1 : 0) << std::endl;
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


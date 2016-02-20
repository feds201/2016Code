/*
 * Logger.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: feds
 */

#include <Logger.h>
#include <wpilib.h>

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
			<< "driveValues0" << ','
			<< "driveValues1" << ','
			<< "driveValues2" << ','
			<< "driveValues3" << ','
			<< "shooterRPMActual" << ','
			<< "shooterRPMSetpoint" << ','
			<< "shooterCylinderUp" << ','
			<< "pickupIsUp" << ','
			<< "psi" << ','
			<< "gear" << std::endl;
}

void Logger::logInfo(const char *msg, ... )
{
	va_list args;
	va_start(args,msg);

	char buffer[256];
	vsnprintf(buffer, sizeof(buffer), msg, args);

	logFile << "[INFO] " << buffer << std::endl;
	std::cout << "[INFO] " << buffer << std::endl;
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

void Logger::logCSV(struct CSV *data)
{
	struct timeval tv;
	gettimeofday(&tv, 0);

	double time = (tv.tv_sec - startTime.tv_sec) + (tv.tv_usec - startTime.tv_usec)/1000000.0;

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
			<< data->shooterRPMActual << ','
			<< data->shooterRPMSetpoint << ','
			<< (data->shooterCylinderUp ? 1 : 0) << ','
			<< (data->pickupIsUp ? 1 : 0) << ','
			<< data->psi << ','
			<< data->gear << std::endl;
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


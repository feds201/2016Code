/*
 * Logger.h
 *
 *  Created on: Jan 21, 2016
 *      Author: feds
 */

#ifndef SRC_LOGGER_H_
#define SRC_LOGGER_H_

#include <iostream>
#include <fstream>
#include <time.h>
#include <stdarg.h>
#include "MyTimer.h"

class Logger {
public:
	struct CSVVals {
		double voltage;
		double totalCurrent;

		double driveSetpoints[4];
		double driveSpeeds[4];
		double driveCurrents[4];

		double shooterRPMActualL;
		double shooterRPMActualR;
		double shooterRPMSetpoint;

		double lifterCurrent;

		bool shooterCylinderUp;
		bool pickupIsUp;
	};

	virtual ~Logger();

	void logError(const char *msg, ... );
	void logInfo(const char *msg, ... );

	void logCSV(struct CSVVals *data);

    static Logger *instance()
    {
        if (!singlton)
        	singlton = new Logger();
        return singlton;
    }
private:

    MyTimer timer;

    struct timeval startTime;

    std::ofstream logFile;
    std::ofstream csvFile;

    void save();

	Logger();
	static Logger *singlton;
};

#endif /* SRC_LOGGER_H_ */

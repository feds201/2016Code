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

class Logger {
public:
	struct CSV {
		double voltage;
		double totalCurrent;
		double driveSetpoints[4];
		double driveCurrents[4];
		double shooterRPMActual;
		double shooterRPMSetpoint;
		bool shooterCylinderUp;
		bool pickupIsUp;
		double psi;
		double gear;
	};

	virtual ~Logger();

	void logError(const char *msg, ... );
	void logInfo(const char *msg, ... );

	void logCSV(struct CSV *data);

    static Logger *instance()
    {
        if (!singlton)
        	singlton = new Logger();
        return singlton;
    }
private:

    struct timeval startTime;

    std::ofstream logFile;
    std::ofstream csvFile;

    void save();

	Logger();
	static Logger *singlton;
};

#endif /* SRC_LOGGER_H_ */

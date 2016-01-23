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

class Logger {
public:
	struct CSV {
		double voltage;
		double totalCurrent;
		double motorCurrent;
	};

	virtual ~Logger();

	void logError(const char *msg);
	void logInfo(const char *msg);

	void logCSV(struct CSV *data);

    static Logger *instance()
    {
        if (!singlton)
        	singlton = new Logger();
        return singlton;
    }
private:
    std::ofstream logFile;
    std::ofstream csvFile;

    void save();

	Logger();
	static Logger *singlton;
};

#endif /* SRC_LOGGER_H_ */

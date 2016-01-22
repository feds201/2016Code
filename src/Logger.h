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
		float totalCurrent;
		float motorCurrent;
	};

	virtual ~Logger();

	void logError(char *msg);
	void logInfo(char *msg);

	void logCSV(struct CSV *data);

    static Logger *instance()
    {
        if (!s_instance)
          s_instance = new Logger();
        return s_instance;
    }
private:
    std::ofstream logFile;
    std::ofstream csvFile;

    void save();

	Logger();
	static Logger *s_instance;
};

#endif /* SRC_LOGGER_H_ */

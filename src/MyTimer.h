/*
 * timer.h
 *
 *  Created on: Feb 20, 2016
 *      Author: feds
 */

#ifndef SRC_MYTIMER_H_
#define SRC_MYTIMER_H_

#include <sys/time.h>

class MyTimer{
public:
	MyTimer();
	double getTotalTime();
	void reset();
	double getDt();

	void diffBegin();
	double getDiff();
private:
		struct timeval startTime;
		struct timeval lastTime;
		struct timeval diffTime;
		double dt = 0;
};



#endif /* SRC_MYTIMER_H_ */

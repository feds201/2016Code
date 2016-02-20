/*
 * timer.cpp
 *
 *  Created on: Feb 20, 2016
 *      Author: feds
 */
#include <MyTimer.h>

MyTimer::MyTimer()
{
	gettimeofday(&startTime, 0);
	lastTime = startTime;
}

double MyTimer::getDt()
{
	struct timeval thisTime;
	gettimeofday(&thisTime, 0);

	return (thisTime.tv_sec-lastTime.tv_sec)+(thisTime.tv_usec-lastTime.tv_usec)/1000000.0;
}

double MyTimer::getTotalTime()
{
	struct timeval thisTime;
		gettimeofday(&thisTime, 0);
		return (thisTime.tv_sec-startTime.tv_sec)+(thisTime.tv_usec-startTime.tv_usec)/1000000.0;
}

void MyTimer::reset()
{
	gettimeofday(&startTime, 0);
	lastTime = startTime;
}


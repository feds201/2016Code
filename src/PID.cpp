/*
 * PID.cpp
 *
 *  Created on: Feb 29, 2016
 *      Author: feds
 */

#include <PID.h>

/**
 * PID!
 *
 * P is Proportional gain
 * I is Integral gain
 * D is Derivative gain
 * F is Feed forward (F * setpoint added to output)
 * iZone iAccumulator is resed if absError > iZone. 0 for disabled
 * iMax is the maximum value iAccum can be. 0 for disabled
 * rampRate the maximum speed the output will change in unitts of output per dt
 *
 */
PID::PID(float P, float I, float D, float F, float iZone, float iMax, float rampRate)
:P(P),
 I(I),
 D(D),
 F(F),
 iZone(iZone),
 iMax(iMax),
 rampRate(rampRate)
{

}

void PID::setOutputIsReversed(bool reversed)
{
	outputIsReversed = reversed;
}

void PID::setSensorIsReversed(bool reversed)
{
	sensorIsReversed = reversed;
}

float PID::update(float setpoint, float input, float dt)
{
	float err;
	float absErr;
	float dErr;
	float outBeforeRamp;

	if(sensorIsReversed)
		input = -input;

	err = setpoint - input;
	absErr = err;

	if(err < 0)
		absErr = -absErr;

	if(notFirst == false)
	{
		iAccum = 0;
		out = 0;//inital output for ramp rate
	} else if((iZone == 0) || (absErr < iZone))
	{
		iAccum += err;
		if(iMax != 0)
		{
			if(iAccum > iMax)
				iAccum = iMax;
			else if(iAccum < iMax)
				iAccum = -iMax;
		}
	} else {
		iAccum = 0;
	}

	if(notFirst)
		dErr = err - prevErr;
	else
		dErr = 0;

	//P Part
	outBeforeRamp = P * err;
	//I Part
	outBeforeRamp += I * iAccum;
	//D Part
	outBeforeRamp += D * dErr;
	//Feed Forward
	outBeforeRamp += F * setpoint;

	prevErr = err;
	notFirst = true;

	if(outputIsReversed)
		outBeforeRamp = -outBeforeRamp;

	if(rampRate != 0)
	{
		if(outBeforeRamp >= out)
		{
			//Rising
			float rate = (outBeforeRamp - out) / dt;
			if(rate > rampRate)
				rate = rampRate;
			out += rate*dt;
		} else {
			//Falling
			float rate = (out - outBeforeRamp) / dt;
			if(rate > rampRate)
				rate = rampRate;
			out -= rate*dt;
		}
	} else {
		out = outBeforeRamp;
	}

	return out;

}

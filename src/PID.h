/*
 * PID.h
 *
 *  Created on: Feb 29, 2016
 *      Author: feds
 */

#ifndef SRC_PID_H_
#define SRC_PID_H_

class PID {
public:
	PID(float P, float I, float D, float F, float IZone, float iMax, float rampRate);
	float update(float setpoint, float input, float dt);

	void setOutputIsReversed(bool reversed);
	void setSensorIsReversed(bool reversed);

private:
	float P, I, D, F, iZone, iMax, rampRate;
	float prevErr = 0;
	float iAccum = 0;
	bool notFirst = false;
	float out = 0;

	bool outputIsReversed = false;
	bool sensorIsReversed = false;
};

#endif /* SRC_PID_H_ */

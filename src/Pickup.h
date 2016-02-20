/*
 * Pickup.h
 *
 *  Created on: Feb 17, 2016
 *      Author: feds
 */

#ifndef SRC_PICKUP_H_
#define SRC_PICKUP_H_

#include "WPILIB.h"
#include "SolenoidControllers.h"

class Pickup {
public:
	struct LogVals {
		bool pickupIsUp;
	};

	Pickup(DoubleSolenoidController *solenoid, double downTime, DigitalInput *sensorA, DigitalInput *sensorB);

	struct Pickup::LogVals update(double dt, bool logThisTime);

	void setUp();
	void setDown();

	void pickupOnce();
	void pickupOnceSensored();

	bool getSensorIsReady();

private:
	DigitalInput *sensorA;
	DigitalInput *sensorB;
	double downTime;
	double countdown = 0;
	DoubleSolenoidController *solenoid;
};

#endif /* SRC_PICKUP_H_ */

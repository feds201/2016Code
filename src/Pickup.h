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
#include "INIReader.h"

class Pickup {
public:
	struct LogVals {
		bool pickupIsUp;
	};

	Pickup(INIReader *iniFile);

	struct Pickup::LogVals update(double dt, bool logThisTime);

	void setUp();
	void setDown();
	void togglePickup();

	void pickupOnce();
	void pickupOnceSensored();

	bool getSensorIsReady();

private:
	DigitalInput *sensorA;
	DigitalInput *sensorB;
	double downTime;
	double countdown = 0;
	DoubleSolenoidController *solenoid;
	bool pickupReady = true;
	bool pickupIsUp = true;
};

#endif /* SRC_PICKUP_H_ */

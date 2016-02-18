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
	Pickup(DoubleSolenoidController *solenoid, int timerMax, DigitalInput *sensorA, DigitalInput *sensorB);

	void update();

	void setUp();
	void setDown();

	void pickupOnce();
	void pickupOnceSensored();

private:
	bool getSensorIsReady();
	DigitalInput *sensorA;
	DigitalInput *sensorB;
	int timerMax;
	int countdown = 0;
	DoubleSolenoidController *solenoid;
	int pickupOnce_timer = 0;
};

#endif /* SRC_PICKUP_H_ */

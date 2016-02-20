/*
 * Shooter.h
 *
 *  Created on: Feb 9, 2016
 *      Author: feds
 */

#ifndef SRC_SHOOTER_H_
#define SRC_SHOOTER_H_

#include "MotorControllers.h"
#include "WPILIB.h"

class Shooter {
	SRXMotorController *shooterMotorList;
	DoubleSolenoid *solenoidTrigger;

	int countdown=0;
	float rpm=7.15;

	bool cylinderUp = false;
	bool running = false;
	float secsSinceStart = 0;
	float boostTime;
	float boostAmnt;

public:
	struct LogVals {
		int RPMSActual;
		int RPMSetpoint;
		bool cylinderUp;
	};

	Shooter(SRXMotorController *shooterMotors, DoubleSolenoid *trigger, float boostTime, float boostAmnt);
	void shoot();

	float modifyRPM(float delta);
	float setRPM(float rpm);

	void start();
	void stop();
	void toggle();

	struct Shooter::LogVals update(double dt, bool logThisTime);
};

#endif /* SRC_SHOOTER_H_ */

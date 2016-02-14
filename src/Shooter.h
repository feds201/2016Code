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

public:
	struct ShooterLogVals {
		int RPMSActual;
		int RPMSetpoint;
		bool cylinderUp;
	};

	Shooter(SRXMotorController *shooterMotors, DoubleSolenoid *trigger);
	void shoot();

	float modifyRPM(float delta);
	float setRPM(float rpm);

	struct Shooter::ShooterLogVals update(bool logThisTime);
};

#endif /* SRC_SHOOTER_H_ */

/*
 * Shooter.h
 *
 *  Created on: Feb 9, 2016
 *      Author: feds
 */

#ifndef SRC_SHOOTER_H_
#define SRC_SHOOTER_H_

#include "MotorController.h"
#include "WPILIB.h"

class Shooter {
	MotorController *shooterMotorList;
	Solenoid *solenoidTrigger;
	int countdown=0;

public:
	Shooter(MotorController *shooterMotors, Solenoid *trigger);
	void set(int rpm);
	void shoot();

	void update();
};

#endif /* SRC_SHOOTER_H_ */

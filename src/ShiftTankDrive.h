/*
 * ShiftTankDrive.h
 *
 *  Created on: Dec 3, 2015
 *      Author: feds201
 */

#ifndef SRC_SHIFTTANKDRIVE_H_
#define SRC_SHIFTTANKDRIVE_H_

#include "MotorController.h"
#include "SolenoidControllers.h"

class ShiftTankDrive {
public:
	ShiftTankDrive(MotorController *motors_left, MotorController *motors_right, DoubleSolenoidController *solenoids);
	virtual ~ShiftTankDrive();

	void setcontrol(float forward, float turn);

	void setGear(int);
	void setPercent(float p);

	void enable();
	void disable();

private:
	MotorController *motors_left;
	MotorController *motors_right;
	DoubleSolenoidController *solenoids;

	int gear=0;
	float percent=1.0f;
};

#endif /* SRC_SHIFTTANKDRIVE_H_ */

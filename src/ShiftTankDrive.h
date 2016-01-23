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
#include "EdgeDetection.h"

class ShiftTankDrive {
public:
	ShiftTankDrive(MotorController *motors_left, MotorController *motors_right, DoubleSolenoidController *solenoids);
	virtual ~ShiftTankDrive();

	void setControl(float forward, float turn, int gear);
	void setPercent(float p);

	void enable();
	void disable();

private:
	MotorController *motors_left;
	MotorController *motors_right;
	DoubleSolenoidController *solenoids;

	int numShifts = 0;
	EdgeDetection shiftEdge;

	float percent=1.0f;
};

#endif /* SRC_SHIFTTANKDRIVE_H_ */

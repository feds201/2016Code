/*
 * ShiftTankDrive.h
 *
 *  Created on: Dec 3, 2015
 *      Author: feds201
 */

#ifndef SRC_SHIFTTANKDRIVE_H_
#define SRC_SHIFTTANKDRIVE_H_

#include "MotorControllers.h"
#include "SolenoidControllers.h"
#include "EdgeDetection.h"

class ShiftTankDrive {
public:
	struct LogVals {
		double values[4];
	};

	ShiftTankDrive(SRXMotorController *motors_left, SRXMotorController *motors_right, DoubleSolenoidController *solenoids);
	virtual ~ShiftTankDrive();

	struct ShiftTankDrive::LogVals update(float forward, float turn, bool setHighGear, double dt, bool logThisTime);
	void setPercent(float p);

	void enable();
	void disable();

private:
	SRXMotorController *motors_left;
	SRXMotorController *motors_right;
	DoubleSolenoidController *solenoids;




	bool lastGear = false;
	double shiftTimer = 0;

	int numShifts = 0;
	EdgeDetection shiftEdge;

	float percent=1.0f;
};

#endif /* SRC_SHIFTTANKDRIVE_H_ */

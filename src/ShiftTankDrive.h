/*
 * ShiftTankDrive.h
 *
 *  Created on: Dec 3, 2015
 *      Author: feds201
 */

#ifndef SRC_SHIFTTANKDRIVE_H_
#define SRC_SHIFTTANKDRIVE_H_

#include "MotorControllers.h"
#include "EdgeDetection.h"
#include "INIReader.h"

class ShiftTankDrive {
public:
	struct LogVals {
		double values[4];
		double speeds[4];
	};

	ShiftTankDrive(INIReader *iniFile);
	virtual ~ShiftTankDrive();

	struct ShiftTankDrive::LogVals update(float forward, float turn, double dt, bool logThisTime);
	void setPercent(float p);

	void togglePIDMode();

	void enable();
	void disable();

private:
	SRXMotorController *motors_left;
	SRXMotorController *motors_right;

	float outputMux = 1000;

	bool PIDMode = true;

	float P, I, D, F, iZone, accumMax;

	float percent=1.0f;
};

#endif /* SRC_SHIFTTANKDRIVE_H_ */

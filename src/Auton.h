/*
 * Autonomous.h
 *
 *  Created on: Feb 20, 2016
 *      Author: feds
 */

#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_

#include "ShiftTankDrive.h"
#include "Shooter.h"
#include "Pickup.h"
#include "INIReader.h"
#include "MyTimer.h"

class Auton {
public:
	struct AutonVals {
		ShiftTankDrive::LogVals driveVals;
	};

	Auton(ShiftTankDrive *std, Shooter *shooter, Pickup *pickup, INIReader *iniFile);

	bool runAuton(double dt);
	void initAuton(std::string mode);

private:
	ShiftTankDrive *std;
	Shooter *shooter;
	Pickup *pickup;

	int autonMode = 0;
	MyTimer timer;

	//drivefwd
	float drivefwd_time;
	float drivefwd_speed = -.7;
};

#endif /* SRC_AUTONOMOUS_H_ */

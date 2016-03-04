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
		bool turnToGoalInError;
	};

	Auton(ShiftTankDrive *std, Shooter *shooter, Pickup *pickup, INIReader *iniFile);

	struct Auton::AutonVals turnToGoal(double dt, bool logThisTime);
	void update(double dt);

	bool runAuton(double dt);
	void initAuton(std::string mode);

private:
	ShiftTankDrive *std;
	Shooter *shooter;
	Pickup *pickup;

	std::string vision_horizOffsetName;
	std::string vision_vertOffsetName;
	std::string vision_flatDistName;
	std::string vision_isFreshName;
	float vision_turnMux;

	int autonMode = 0;
	MyTimer timer;

	float drivefwd_speed = -.7;

	//drivefwd
	float drivefwd_time;

	struct vision_vals {
		float vision_horizOffset;
		float vision_vertOffset;
		float vision_flatDist;
		bool vision_isFresh;
	};

	struct vision_vals getVisionVals();

	double vision_acceptableError;
};

#endif /* SRC_AUTONOMOUS_H_ */

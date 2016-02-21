/*
 * Autonomous.h
 *
 *  Created on: Feb 20, 2016
 *      Author: feds
 */

#ifndef SRC_AUTONOMOUS_H_
#define SRC_AUTONOMOUS_H_

#include "ShiftTankDrive.h"
#include "Shooter.h"
#include "Pickup.h"
#include "INIReader.h"

class Autonomous {
public:
	Autonomous(ShiftTankDrive *std, Shooter *shooter, Pickup *pickup, INIReader *iniFile);

	bool turnToGoal(double dt);
	bool shoot(double dt, double currTime);

	bool runAuton(double dt);
	void initAuton();

private:
	ShiftTankDrive *std;
	Shooter *shooter;
	Pickup *pickup;

	std::string vision_horizOffsetName;
	std::string vision_vertOffsetName;
	std::string vision_flatDistOffsetName;
	std::string vision_flatOffsetName;

	double vision_acceptableError;
};

#endif /* SRC_AUTONOMOUS_H_ */

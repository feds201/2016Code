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
	void update(double dt);

	bool runAuton(double dt);
	void initAuton();

private:
	ShiftTankDrive *std;
	Shooter *shooter;
	Pickup *pickup;

	std::string vision_horizOffsetName;
	std::string vision_vertOffsetName;
	std::string vision_flatDistName;
	std::string vision_isFreshName;

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

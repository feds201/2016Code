/*
 * Autonomous.cpp
 *
 *  Created on: Feb 20, 2016
 *      Author: feds
 */

#include <Autonomous.h>

#include "WPILIB.h"

Autonomous::Autonomous(ShiftTankDrive *std, Shooter *shooter, Pickup *pickup, INIReader *iniFile) :
std(std),
shooter(shooter),
pickup(pickup)
{
	vision_horizOffsetName = iniFile->get("Vision", "horizOffsetName");
	vision_vertOffsetName = iniFile->get("Vision", "vertOffsetName");
	vision_flatDistName = iniFile->get("Vision", "flatDistName");
	vision_isFreshName = iniFile->get("Vision", "isFreshName");

	vision_acceptableError = iniFile->getFloat("Vision", "acceptableError");
}

bool Autonomous::turnToGoal(double dt)
{
	struct Autonomous::vision_vals vision_vals = getVisionVals();

	if(vision_vals.vision_isFresh)
		std->update(0, -vision_vals.vision_horizOffset, 1, dt, false);
	else
		std->update(0, 0, 1, dt, false);

	return (vision_vals.vision_horizOffset < vision_acceptableError) && (vision_vals.vision_horizOffset > -vision_acceptableError);
}

void Autonomous::update(double dt)
{
}

bool Autonomous::runAuton(double dt)
{
}

void Autonomous::initAuton()
{

}

struct Autonomous::vision_vals Autonomous::getVisionVals()
{
	struct Autonomous::vision_vals ret;
	ret.vision_horizOffset = SmartDashboard::GetNumber(vision_horizOffsetName,0);
	ret.vision_vertOffset = SmartDashboard::GetNumber(vision_vertOffsetName, 0);
	ret.vision_flatDist = SmartDashboard::GetNumber(vision_flatDistName, 0);
	ret.vision_isFresh = SmartDashboard::GetNumber(vision_isFreshName, false);

	return ret;
}

/*
 * Autonomous.cpp
 *
 *  Created on: Feb 20, 2016
 *      Author: feds
 */

#include <Auton.h>
#include "WPILIB.h"

Auton::Auton(ShiftTankDrive *std, Shooter *shooter, Pickup *pickup, INIReader *iniFile) :
std(std),
shooter(shooter),
pickup(pickup)
{
	vision_horizOffsetName = iniFile->get("Vision", "horizOffsetName");
	vision_vertOffsetName = iniFile->get("Vision", "vertOffsetName");
	vision_flatDistName = iniFile->get("Vision", "flatDistName");
	vision_isFreshName = iniFile->get("Vision", "isFreshName");

	vision_acceptableError = iniFile->getFloat("Vision", "acceptableError");
	vision_turnMux = iniFile->getFloat("Vision", "turnMux");
}

struct Auton::AutonVals Auton::turnToGoal(double dt, bool logThisTime)
{
	struct Auton::vision_vals vision_vals = getVisionVals();
	struct Auton::AutonVals ret;

	if(vision_vals.vision_isFresh)
		ret.driveVals = std->update(0, -vision_vals.vision_horizOffset*vision_turnMux, 1, dt, logThisTime);
	else
		ret.driveVals = std->update(0, 0, 1, dt, false);

	ret.turnToGoalInError = (vision_vals.vision_horizOffset < vision_acceptableError) && (vision_vals.vision_horizOffset > -vision_acceptableError);

	return ret;
}

void Auton::update(double dt)
{
}

bool Auton::runAuton(double dt)
{
	return false;
}

void Auton::initAuton()
{

}

struct Auton::vision_vals Auton::getVisionVals()
{
	struct Auton::vision_vals ret;
	ret.vision_horizOffset = SmartDashboard::GetNumber(vision_horizOffsetName,0);
	ret.vision_vertOffset = SmartDashboard::GetNumber(vision_vertOffsetName, 0);
	ret.vision_flatDist = SmartDashboard::GetNumber(vision_flatDistName, 0);
	ret.vision_isFresh = SmartDashboard::GetNumber(vision_isFreshName, false);

	return ret;
}

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
	vision_horizOffsetName = iniFile->get("Vision", "vision_horizOffsetName");
	vision_vertOffsetName = iniFile->get("Vision", "vision_vertOffsetName");
	vision_flatDistOffsetName = iniFile->get("Vision", "vision_flatDistOffsetName");
	vision_flatOffsetName = iniFile->get("Vision", "vision_flatOffsetName");

	vision_acceptableError = iniFile->getFloat("Vision", "acceptableError");
}

bool Autonomous::turnToGoal(double dt)
{
	double offX = SmartDashboard::GetNumber(vision_horizOffsetName, 0);
	double offY = SmartDashboard::GetNumber(vision_vertOffsetName, 0);
	double dist = SmartDashboard::GetNumber(vision_flatDistOffsetName, 0);
	double stringDist = SmartDashboard::GetNumber(vision_flatOffsetName, 0);

	std->update(0,-offX, 1, dt, false);

	return (offX < vision_acceptableError) && (offX > -vision_acceptableError);
}

bool Autonomous::shoot(double dt, double currTime)
{
}

bool Autonomous::runAuton(double dt)
{
}

void Autonomous::initAuton()
{

}

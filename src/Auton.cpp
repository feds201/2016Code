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
	drivefwd_time = iniFile->getFloat("Auton", "driveFwdTime");
	drivefwd_speed = iniFile->getFloat("Auton", "driveFwdSpeed");
}

bool Auton::runAuton(double dt)
{
	if(timer.getTotalTime() < drivefwd_time)
	{
		std->update(drivefwd_speed, 0, dt, false);
		return true;
	} else {
		return false;
	}
}

void Auton::initAuton(std::string mode)
{
	if(mode == "none")
		autonMode = 0;
	else if(mode == "drivefwd")
		autonMode = 1;
	else
		autonMode = 0;

	timer.reset();
}

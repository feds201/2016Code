/*
 * Lifter.cpp
 *
 *  Created on: Feb 27, 2016
 *      Author: feds
 */

#include <Lifter.h>

Lifter::Lifter(INIReader *iniFile)
{
	motor = new Spark(iniFile->getInt("Lifter", "motorID"));
	reversed = iniFile->getBool("Lifter", "motorReversed");
	motorSpeed = iniFile->getFloat("Lifter", "motorPercentage");

	if(reversed)
		motorSpeed = -motorSpeed;

	lowerLimit = new DigitalInput(iniFile->getFloat("Lifter", "lowerLimitID"));
	middleLimit = new DigitalInput(iniFile->getFloat("Lifter", "middleLimitID"));
	topLimit = new DigitalInput(iniFile->getFloat("Lifter", "topLimitID"));

	motor->SetSafetyEnabled(false);
}

void Lifter::run(float val)
{
	motor->Set(val);
}

void Lifter::stop()
{
	//motor->Set(0);
}

bool Lifter::isManualMode()
{
	return true;
}

void Lifter::update(double dt)
{
	switch(lastKnownPosition)
	{
	case BOTTOM:
	{
		if(targetPosition!=BOTTOM)
			motor->Set(motorSpeed);
		break;
	}
	case MIDDLE:
	{
		if(targetPosition==TOP)
		{
			motor->Set(motorSpeed);
		} else if(targetPosition==BOTTOM)
		{
			motor->Set(-motorSpeed);
		} else {
			if(!lowerLimit->Get())
				motor->Set(motorSpeed);
		}
		break;
	}
	case TOP:
	{
		if(targetPosition!=TOP)
			motor->Set(-motorSpeed);
		break;
	}
	}
}

void Lifter::setPotition(enum Position targetPosition)
{
	this->targetPosition = targetPosition;
}

enum Lifter::Position Lifter::getPosition()
{
	return lastKnownPosition;
}

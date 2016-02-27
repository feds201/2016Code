/*
 * Lifter.cpp
 *
 *  Created on: Feb 27, 2016
 *      Author: feds
 */

#include <Lifter.h>

Lifter::Lifter(INIReader *iniFile)
{
	motor = new SparkMotorController(iniFile->getInt("Lifter", "MotorID"),
			iniFile->getBool("Lifter", "MotorReversed"));
	motorSpeed = iniFile->getFloat("Lifter", "MotorPercentage");

	lowerLimit = new DigitalInput(iniFile->getFloat("Lifter", "lowerLimitID"));
	middleLimit = new DigitalInput(iniFile->getFloat("Lifter", "middleLimitID"));
	topLimit = new DigitalInput(iniFile->getFloat("Lifter", "topLimitID"));
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

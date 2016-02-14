/*
 * ShiftTankDrive.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: feds201
 */

#include "ShiftTankDrive.h"
#include <cmath>
#include <iostream>
#include <string>
#include "Logger.h"

ShiftTankDrive::ShiftTankDrive(SRXMotorController *motors_left, SRXMotorController *motors_right, DoubleSolenoidController *solenoids)
:motors_left(motors_left),
 motors_right(motors_right),
 solenoids(solenoids),
 shiftEdge(false)

{
	motors_left->setControlMode(CANTalon::ControlMode::kPercentVbus);

	motors_left->enable();

	motors_right->setControlMode(CANTalon::ControlMode::kPercentVbus);
	motors_right->enable();
}

ShiftTankDrive::~ShiftTankDrive()
{
	delete motors_right;
	delete motors_left;
}

struct ShiftTankDrive::STDLogVals ShiftTankDrive::update(float forward, float turn, int gear, bool logThisTime)
{

	float l = forward - turn;
	float r = forward + turn;

	float m = fmax(fabs(l), fabs(r));
	if(m > 1)
	{
		l /= m;
		r /= m;
	}

	shiftEdge.update(gear==1);
	if(shiftEdge.isEdge())
	{
		numShifts++;
		char msg[64];
		snprintf(msg, sizeof(msg), "shift number: %i", numShifts);
		Logger::instance()->logInfo(msg);
	}


	if(gear != lastGear)
	{
		switch(gear)
		{
		case 0:
			solenoids->set(DoubleSolenoid::Value::kReverse);
			break;
		case 1:
			solenoids->set(DoubleSolenoid::Value::kForward);
			break;
		case 2: solenoids->set(DoubleSolenoid::Value::kOff);
			break;
		default:
			break;
		}
		shiftIterator = 0;
		lastGear = gear;
	} else if(shiftIterator < 200) {
		shiftIterator++;
		if(shiftIterator == 200)
			solenoids->set(DoubleSolenoid::Value::kOff);
	}


	l = l*percent;
	r = r*percent;

	motors_left->set(l);//because the motors are rotated 180deg
	motors_right->set(r);


	struct STDLogVals ret;
	if(logThisTime)
	{
		ret.values[0]=l;
		ret.values[1]=l;
		ret.values[2]=r;
		ret.values[3]=r;
	}
	return ret;
}

void ShiftTankDrive::enable()
{
	motors_left->enable();
	motors_right->enable();
}

void ShiftTankDrive::disable()
{
	motors_left->disable();
	motors_right->disable();
}

void ShiftTankDrive::setPercent(float p)
{
	percent = p > 1.0f ? 1.0f : (p < -1.0f ? -1.0f : p);

	Logger::instance()->logInfo("SDT Percent set to %f", percent);
}

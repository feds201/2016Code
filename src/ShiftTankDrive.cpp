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

ShiftTankDrive::ShiftTankDrive(INIReader *iniFile)
{
	motors_left = new SRXMotorController(
			iniFile->getInt("Drive", "leftPrimaryMotorID"),
			iniFile->getBool("Drive", "leftPrimaryMotorReversed"));
	motors_left->addMotor(
			iniFile->getInt("Drive", "leftSecondaryMotorID"),
			iniFile->getBool("Drive", "leftSecondaryMotorReversed"));
	motors_right = new SRXMotorController(
			iniFile->getInt("Drive", "rightPrimaryMotorID"),
			iniFile->getBool("Drive", "rightPrimaryMotorReversed"));
	motors_right->addMotor(
			iniFile->getInt("Drive", "rightSecondaryMotorID"),
			iniFile->getBool("Drive", "rightSecondaryMotorReversed"));

	motors_left->SetControlMode(CANTalon::ControlMode::kSpeed);
	motors_left->SetVoltageRampRate(iniFile->getFloat("Drive", "rampRate"));

	motors_right->SetControlMode(CANTalon::ControlMode::kSpeed);
	motors_right->SetVoltageRampRate(iniFile->getFloat("Drive", "rampRate"));

	motors_left->SetSensorDirection(iniFile->getBool("Drive", "reverseLeftEnc"));
	motors_right->SetSensorDirection(iniFile->getBool("Drive", "reverseRightEnc"));

	P= iniFile->getFloat("Drive", "P");
	I= iniFile->getFloat("Drive", "I");
	D= iniFile->getFloat("Drive", "D");
	F= iniFile->getFloat("Drive", "F");
	iZone = iniFile->getFloat("Drive", "iZone");

	motors_left->SetIzone(iZone);
	motors_right->SetIzone(iZone);

	motors_left->SetPID(P, I, D, F);
	motors_right->SetPID(P, I, D, F);

	outputMux = iniFile->getFloat("Drive", "mux");
	accumMax =  iniFile->getFloat("Drive", "iMax");
}

void ShiftTankDrive::togglePIDMode()
{
	if(PIDMode)
	{
		motors_left->SetControlMode(CANTalon::ControlMode::kVoltage);
		motors_right->SetControlMode(CANTalon::ControlMode::kVoltage);

		PIDMode = false;
	} else {
		motors_left->SetControlMode(CANTalon::ControlMode::kSpeed);
		motors_right->SetControlMode(CANTalon::ControlMode::kSpeed);

		motors_left->SetIzone(iZone);
		motors_right->SetIzone(iZone);

		motors_left->SetPID(P, I, D, F);
		motors_right->SetPID(P, I, D, F);

		PIDMode = true;
	}

	SmartDashboard::PutBoolean("*** PID MODE: ***", PIDMode);
}

ShiftTankDrive::~ShiftTankDrive()
{
	delete motors_right;
	delete motors_left;
}

struct ShiftTankDrive::LogVals ShiftTankDrive::update(float forward, float turn, double dt, bool logThisTime)
{
	float l = forward - turn;
	float r = forward + turn;

	float m = fmax(fabs(l), fabs(r));
	if(m > 1)
	{
		l /= m;
		r /= m;
	}

	float ia = motors_left->GetIaccum();
	if(ia > accumMax)
		motors_left->ClearIaccum();
	else if(ia < -accumMax)
		motors_left->ClearIaccum();
	ia = motors_right->GetIaccum();
	if(ia > accumMax)
		motors_right->ClearIaccum();
	else if(ia < -accumMax)
		motors_right->ClearIaccum();

	l = l*percent;
	r = r*percent;

	if(PIDMode)
	{
		l *= outputMux;
		r *= outputMux;

		float max = l;
		if(r > l)
			max = r;

		if(max > 4000.0f)
		{
			l *= 4000.0f / max;
			r *= 4000.0f / max;
		}
	} else {
		l *= 12;
		r *= 12;
	}

	enable();

	motors_left->Set(l);
	motors_right->Set(r);


	struct LogVals ret;
	if(logThisTime)
	{
		ret.values[0]=l;
		ret.values[1]=l;
		ret.values[2]=r;
		ret.values[3]=r;

		ret.speeds[0]=motors_left->GetSpeed();
		ret.speeds[1]=motors_left->GetSpeed();
		ret.speeds[2]=motors_right->GetSpeed();
		ret.speeds[3]=motors_right->GetSpeed();
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

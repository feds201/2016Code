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
:shiftEdge(false)
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
	solenoids = new DoubleSolenoidController(
			iniFile->getInt("Drive", "PCMID"),
			iniFile->getInt("Drive", "shiftSolenoidChannelA"),
			iniFile->getInt("Drive", "shiftSolenoidChannelB"));

	motors_left->SetControlMode(CANTalon::ControlMode::kSpeed);
	motors_left->SetVoltageRampRate(iniFile->getFloat("Drive", "rampRate"));

	motors_right->SetControlMode(CANTalon::ControlMode::kSpeed);
	motors_right->SetVoltageRampRate(iniFile->getFloat("Drive", "rampRate"));

	motors_left->SetSensorDirection(iniFile->getBool("Drive", "reverseLeftEnc"));
	motors_right->SetSensorDirection(iniFile->getBool("Drive", "reverseRightEnc"));
	motors_left->SetIzone(iniFile->getBool("Drive", "iZone"));
	motors_right->SetIzone(iniFile->getBool("Drive", "iZone"));

	motors_left->SetPID(
			iniFile->getFloat("Drive", "P"),
			iniFile->getFloat("Drive", "I"),
			iniFile->getFloat("Drive", "D"),
			iniFile->getFloat("Drive", "F")
			);
	motors_right->SetPID(
			iniFile->getFloat("Drive", "P"),
			iniFile->getFloat("Drive", "I"),
			iniFile->getFloat("Drive", "D"),
			iniFile->getFloat("Drive", "F")
			);

	outputMux = iniFile->getFloat("Drive", "mux");
	accumMax =  iniFile->getFloat("Drive", "iMax");
}

ShiftTankDrive::~ShiftTankDrive()
{
	delete motors_right;
	delete motors_left;
	delete solenoids;
}

struct ShiftTankDrive::LogVals ShiftTankDrive::update(float forward, float turn, bool setHighGear, double dt, bool logThisTime)
{
	float l = forward - turn;
	float r = forward + turn;

	float m = fmax(fabs(l), fabs(r));
	if(m > 1)
	{
		l /= m;
		r /= m;
	}

	shiftEdge.update(setHighGear);
	if(shiftEdge.isEdge())
	{
		numShifts++;
		char msg[64];
		snprintf(msg, sizeof(msg), "shift number: %i", numShifts);
		Logger::instance()->logInfo(msg);
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


	if(setHighGear != lastGear)
	{
		switch(setHighGear)
		{
		case true:
			solenoids->set(DoubleSolenoid::Value::kReverse);
			break;
		case false:
			solenoids->set(DoubleSolenoid::Value::kForward);
			break;
		default:
			break;
		}
		shiftTimer = 0;
		lastGear = setHighGear;
	} else if(shiftTimer < 1) {
		shiftTimer += dt;
		if(shiftTimer >= 1)
			solenoids->set(DoubleSolenoid::Value::kOff);
	}


	l = l*percent*outputMux;
	r = r*percent*outputMux;

	enable();

	motors_left->Set(l);//because the motors are rotated 180deg
	motors_right->Set(r);


	struct LogVals ret;
	if(logThisTime)
	{
		ret.values[0]=l;
		ret.values[1]=l;
		ret.values[2]=r;
		ret.values[3]=r;

		SmartDashboard::PutNumber("getencvelL", motors_left->GetSpeed());
		SmartDashboard::PutNumber("getencvelR", motors_right->GetSpeed());
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

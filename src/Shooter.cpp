/*
 * Shooter.cpp
 *
 *  Created on: Feb 9, 2016
 *      Author: feds
 */

#include <Shooter.h>
#include "Logger.h"

Shooter::Shooter(SRXMotorController *shooterMotors, DoubleSolenoid *trigger, float boostTime, float boostAmnt) :
shooterMotorList(shooterMotors),
solenoidTrigger(trigger),
boostTime(boostTime),
boostAmnt(boostAmnt)
{
	shooterMotorList->SetControlMode(CANTalon::ControlMode::kVoltage);
	shooterMotorList->SetVoltageRampRate(10000);
	shooterMotorList->enable();
}

void Shooter::shoot()
{
	this->cylinderUp = true;
	solenoidTrigger->Set(DoubleSolenoid::Value::kForward);
	countdown=100;
}

float Shooter::modifyRPM(float delta)
{
	rpm += delta;
	Logger::instance()->logInfo("Shooter RPM set to %f", rpm);
	return rpm;
}

float Shooter::setRPM(float rpm)
{
	this->rpm = rpm;
	Logger::instance()->logInfo("Shooter RPM set to %f", rpm);
	return rpm;
}

struct Shooter::LogVals Shooter::update(double dt, bool logThisTime)
{
	if(countdown != 0)
	{
		countdown--;
		if(countdown == 0)
		{
			this->cylinderUp = false;
			solenoidTrigger->Set(DoubleSolenoid::Value::kReverse);
		}
	}

	float setpoint = rpm;
	if(running)
	{
		if(secsSinceStart < boostTime)
		{
			secsSinceStart += dt;
			setpoint += boostAmnt;
		}
	} else {
		setpoint = 0;
	}
	shooterMotorList->Set(setpoint);

	shooterMotorList->enable();
	struct LogVals ret;
	if(logThisTime)
	{
		ret.RPMSetpoint = rpm;
		ret.cylinderUp = this->cylinderUp;
	}
	return ret;
}



void Shooter::start()
{
	running = true;
}
void Shooter::stop()
{
	running = false;
}

void Shooter::toggle()
{
	running = !running;
}

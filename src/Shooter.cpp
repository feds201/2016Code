/*
 * Shooter.cpp
 *
 *  Created on: Feb 9, 2016
 *      Author: feds
 */

#include <Shooter.h>
#include "Logger.h"

Shooter::Shooter(SRXMotorController *shooterMotors, DoubleSolenoid *trigger, float rampRate, float boostTime, float boostAmnt) :
shooterMotorList(shooterMotors),
solenoidTrigger(trigger),
rampRate(rampRate),
boostTime(boostTime),
boostAmnt(boostAmnt)
{
	shooterMotorList->setControlMode(CANTalon::ControlMode::kVoltage);
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

	if(running)
	{
		float setpoint = rpm;
		float realRampRate = rampRate;
		if(secsSinceStart < boostTime)
		{
			secsSinceStart += dt;
			setpoint += boostAmnt;
			realRampRate *= 20;
		}
		SmartDashboard::PutNumber("SSS:", secsSinceStart);
		SmartDashboard::PutNumber("setpoint:", setpoint);
		SmartDashboard::PutNumber("rv:", runningVoltage);
		if(setpoint > runningVoltage)
		{
			runningVoltage += realRampRate*dt;
			if(runningVoltage > setpoint)
				runningVoltage = setpoint;
		} else if(setpoint < runningVoltage)
		{
			runningVoltage -= realRampRate*dt;
			if(runningVoltage < setpoint)
				runningVoltage = setpoint;
		}
	} else {
		if(runningVoltage > 0)
		{
			runningVoltage -= rampRate*dt;
			if(runningVoltage < 0)
				runningVoltage = 0;
		}
	}

	shooterMotorList->enable();
	shooterMotorList->set(runningVoltage);
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

/*
 * Shooter.cpp
 *
 *  Created on: Feb 9, 2016
 *      Author: feds
 */

#include <Shooter.h>
#include "Logger.h"

void initMotor(CANTalon *m)
{
	m->SetControlMode(CANTalon::ControlMode::kVoltage);
	m->SetSafetyEnabled(false);
	m->Enable();
}

Shooter::Shooter(SRXMotorController *shooterMotors, DoubleSolenoid *trigger) :
shooterMotorList(shooterMotors),
solenoidTrigger(trigger)
{
	shooterMotorList->runFunctionOnAll(initMotor);
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

struct Shooter::ShooterLogVals Shooter::update(bool logThisTime)
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

	shooterMotorList->set(rpm);

	struct ShooterLogVals ret;
	if(logThisTime)
	{
		ret.RPMSActual = shooterMotorList->getEncoderInfo_Vel().avg;
		ret.RPMSetpoint = rpm;
		ret.cylinderUp = this->cylinderUp;
	}
	return ret;
}

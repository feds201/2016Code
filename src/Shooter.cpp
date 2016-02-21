/*
 * Shooter.cpp
 *
 *  Created on: Feb 9, 2016
 *      Author: feds
 */

#include <Shooter.h>
#include "Logger.h"

Shooter::Shooter(INIReader *iniFile)
{
	motorL = new CANTalon(iniFile->getInt("Shooter", "leftMotorID"));
	motorL->SetInverted(iniFile->getBool("Shooter", "leftMotorReversed"));

	motorR = new CANTalon(iniFile->getInt("Shooter", "rightMotorID"));
	motorR->SetInverted(iniFile->getBool("Shooter", "rightMotorReversed"));

	motorR->SetSensorDirection(iniFile->getBool("Shooter", "rightMotorReversed"));
	motorL->SetSensorDirection(iniFile->getBool("Shooter", "leftMotorReversed"));

	solenoidTrigger = new DoubleSolenoid(
			iniFile->getInt("Shooter", "PCMID"),
			iniFile->getInt("Shooter", "triggerSolenoidChannelA"),
			iniFile->getInt("Shooter", "triggerSolenoidChannelB"));

	shooterPusherTime = iniFile->getFloat("Shooter", "shooterPusherTime");

	encCodesPerRevNotQuadrature = iniFile->getInt("Shooter", "encCodesPerRev");
	motorL->ConfigEncoderCodesPerRev(encCodesPerRevNotQuadrature*4);
	motorR->ConfigEncoderCodesPerRev(encCodesPerRevNotQuadrature*4);

	motorL->SetControlMode(CANTalon::ControlMode::kSpeed);
	motorR->SetControlMode(CANTalon::ControlMode::kSpeed);

	motorL->SetPID(iniFile->getFloat("Shooter", "P"),
			iniFile->getFloat("Shooter", "I"),
			iniFile->getFloat("Shooter", "D"));
	motorR->SetPID(iniFile->getFloat("Shooter", "P"),
			iniFile->getFloat("Shooter", "I"),
			iniFile->getFloat("Shooter", "D"));
}

void Shooter::shoot()
{
	this->cylinderUp = true;
	solenoidTrigger->Set(DoubleSolenoid::Value::kForward);
	shooterPusherCountdown=shooterPusherTime;
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
	if(shooterPusherCountdown > 0)
	{
		shooterPusherCountdown -= dt;
		if(shooterPusherCountdown <= 0)
		{
			this->cylinderUp = false;
			solenoidTrigger->Set(DoubleSolenoid::Value::kReverse);
		}
	}

	float setpoint = rpm;
	if(running)
	{
		setpoint = rpm * ((4.0*(double)encCodesPerRevNotQuadrature) / (600.0)) * (2.0 / 3.0);
	} else {
		setpoint = 0;
	}

	motorL->Enable();
	motorR->Enable();
	motorL->Set(setpoint);
	motorR->Set(setpoint);

	struct LogVals ret;
	if(logThisTime)
	{
		SmartDashboard::PutNumber("shoot enc speed L: ", motorL->GetEncVel() * (600.0 / ((double)encCodesPerRevNotQuadrature*4.0)));
		SmartDashboard::PutNumber("shoot enc speed R: ", motorR->GetEncVel() * (600.0 / ((double)encCodesPerRevNotQuadrature*4.0)));
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

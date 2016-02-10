/*
 * Shooter.cpp
 *
 *  Created on: Feb 9, 2016
 *      Author: feds
 */

#include <Shooter.h>

void initMotor(CANTalon *m)
{
	m->SetControlMode(CANTalon::ControlMode::kPercentVbus);
	m->SetSafetyEnabled(false);
	m->Enable();
}

Shooter::Shooter(MotorController *shooterMotors, Solenoid *trigger) :
shooterMotorList(shooterMotors),
solenoidTrigger(trigger)
{
	shooterMotorList->runFunctionOnAll(initMotor);
}

void Shooter::set(int rpm)
{
	float value = (float)rpm / 10000.0f;
	shooterMotorList->set(value);
}

void Shooter::shoot()
{
	solenoidTrigger->Set(true);
	countdown=100;
}

void Shooter::update()
{
	if(countdown != 0)
	{
		countdown--;
		if(countdown == 0)
			solenoidTrigger->Set(false);
	}

}

/*
 * Pickup.cpp
 *
 *  Created on: Feb 17, 2016
 *      Author: feds
 */

#include <Pickup.h>

Pickup::Pickup(DoubleSolenoidController *solenoid, int timerMax, DigitalInput *sensorA, DigitalInput *sensorB) :
sensorA(sensorA),
sensorB(sensorB),
timerMax(timerMax),
solenoid(solenoid)
{
	solenoid->set(DoubleSolenoid::Value::kReverse);
	countdown = timerMax;
}

void Pickup::update()
{
	if(countdown > 0)
	{
		countdown--;
		if(countdown == 0)
			solenoid->set(DoubleSolenoid::Value::kOff);
	}
	if(pickupOnce_timer > 0)
	{
		pickupOnce_timer--;
		if(pickupOnce_timer == 0)
		{
			solenoid->set(DoubleSolenoid::Value::kReverse);
			countdown = timerMax;
		}
	}
}

void Pickup::setUp()
{
	solenoid->set(DoubleSolenoid::Value::kReverse);
	countdown = timerMax;
}

void Pickup::setDown()
{
	solenoid->set(DoubleSolenoid::Value::kForward);
	countdown = timerMax;
}

void Pickup::pickupOnce()
{
	solenoid->set(DoubleSolenoid::Value::kForward);
	pickupOnce_timer = timerMax;
}

void Pickup::pickupOnceSensored()
{
	if(getSensorIsReady())
	{
		solenoid->set(DoubleSolenoid::Value::kForward);
		pickupOnce_timer = timerMax;
	}
}

bool Pickup::getSensorIsReady()
{
	return (sensorA->Get() && sensorB->Get());
}

/*
 * Pickup.cpp
 *
 *  Created on: Feb 17, 2016
 *      Author: feds
 */

#include <Pickup.h>

Pickup::Pickup(DoubleSolenoidController *solenoid, double downTime, DigitalInput *sensorA, DigitalInput *sensorB) :
sensorA(sensorA),
sensorB(sensorB),
downTime(downTime),
solenoid(solenoid)
{
	solenoid->set(DoubleSolenoid::Value::kReverse);
	countdown = 0;
}

struct Pickup::LogVals Pickup::update(double dt, bool logThisTime)
{
	if(countdown > 0)
	{
		countdown -= dt;
		if(countdown <= 0)
			solenoid->set(DoubleSolenoid::Value::kReverse);
	}
	struct Pickup::LogVals ret;
	ret.pickupIsUp = true;
	return ret;
}

void Pickup::setUp()
{
	solenoid->set(DoubleSolenoid::Value::kReverse);
}

void Pickup::setDown()
{
	solenoid->set(DoubleSolenoid::Value::kForward);
}

void Pickup::pickupOnce()
{
	solenoid->set(DoubleSolenoid::Value::kForward);
	countdown = downTime;
}

void Pickup::pickupOnceSensored()
{
	if(getSensorIsReady())
	{
		solenoid->set(DoubleSolenoid::Value::kForward);
		countdown = downTime;
	}
}

bool Pickup::getSensorIsReady()
{
	return (sensorA->Get() && sensorB->Get());
}

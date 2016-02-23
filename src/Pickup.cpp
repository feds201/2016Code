/*
 * Pickup.cpp
 *
 *  Created on: Feb 17, 2016
 *      Author: feds
 */

#include <Pickup.h>

Pickup::Pickup(INIReader *iniFile)
{
	solenoid = new DoubleSolenoidController(
			iniFile->getInt("Pickup", "PCMID"),
			iniFile->getInt("Pickup", "solenoid1ChannelA"),
			iniFile->getInt("Pickup", "solenoid1ChannelB"));
	solenoid->addSolenoid(
			iniFile->getInt("Pickup", "PCMID"),
			iniFile->getInt("Pickup", "solenoid2ChannelA"),
			iniFile->getInt("Pickup", "solenoid2ChannelB"));
	sensorA = new DigitalInput(iniFile->getInt("Pickup", "sensorAID"));
	sensorB = new DigitalInput(iniFile->getInt("Pickup", "sensorBID"));

	downTime = iniFile->getFloat("Pickup", "downTime");

	countdown = 0;
	setUp();
}

struct Pickup::LogVals Pickup::update(double dt, bool logThisTime)
{
	if(countdown > 0)
	{
		countdown -= dt;
		if(countdown <= 0)
		{
			setUp();
			pickupReady = true;
		}
	}
	struct Pickup::LogVals ret;
	ret.pickupIsUp = pickupIsUp;
	return ret;
}

void Pickup::setUp()
{
	solenoid->set(DoubleSolenoid::Value::kReverse);
	pickupIsUp=true;
}

void Pickup::setDown()
{
	solenoid->set(DoubleSolenoid::Value::kForward);
	pickupIsUp=false;
}

void Pickup::togglePickup()
{
	if(pickupIsUp)
		setDown();
	else
		setUp();
}

void Pickup::pickupOnce()
{
	if(!pickupIsUp)
	{
		setUp();
		return;
	}
	if(!pickupReady)
		return;
	setDown();
	countdown = downTime;
	pickupReady = false;
}

void Pickup::pickupOnceSensored()
{
	if(!pickupIsUp)
	{
		setUp();
		return;
	}
	if(getSensorIsReady())
	{
		pickupOnce();
	}
}

bool Pickup::getSensorIsReady()
{
	return (sensorA->Get() && sensorB->Get());
}

/*
 * Lifter.cpp
 *
 *  Created on: Feb 27, 2016
 *      Author: feds
 */

#include <Lifter.h>

Lifter::Lifter(INIReader *iniFile)
{
	motor = new Spark(iniFile->getInt("Lifter", "motorID"));
	reversed = iniFile->getBool("Lifter", "motorReversed");
	motor->SetSafetyEnabled(true);
	motor->Set(0);
	lock = new DoubleSolenoidController(
			iniFile->getInt("Lifter", "PCMID"),
			iniFile->getInt("Lifter", "solenoid1ChannelA"),
			iniFile->getInt("Lifter", "solenoid1ChannelB"));
	lock->set(DoubleSolenoid::Value::kReverse);
}

void Lifter::run(float val)
{
	if(reversed)
		val = -val;
	motor->Set(val);
}

void Lifter::stop()
{
	motor->Set(0);
}

void Lifter::toggleLock()
{
	if(lockstate)
	{
		lockstate = false;
		lock->set(DoubleSolenoid::Value::kReverse);
	} else {
		lockstate = true;
		lock->set(DoubleSolenoid::Value::kForward);
	}
}

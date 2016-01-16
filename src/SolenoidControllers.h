/*
 * SolenoidController.h
 *
 *  Created on: Nov 25, 2015
 *      Author: mitch
 */

#ifndef SOLENOIDCONTROLLERS_H
#define SOLENOIDCONTROLLERS_H

#include "WPILib.h"

class SolenoidController {
public:
	SolenoidController(uint8_t canid, uint32_t channel);
	void addSolenoid(uint8_t canid, uint32_t channel);
	void removeSolenoid(uint32_t channel);

	void set(bool on);
	bool get();

private:
	struct sclist {
		Solenoid *s = 0;
		uint32_t m_channel;

		struct sclist *next = 0;
		struct sclist *prev = 0;
	};
	struct sclist *list;

	bool state=false;
};

class DoubleSolenoidController {
public:
	DoubleSolenoidController(uint8_t canid, uint32_t channel1, uint32_t channel2);

	void addSolenoid(uint8_t canid, uint32_t channel1, uint32_t channel2);
	void removeSolenoid(uint32_t channel);//either channel

	void set(DoubleSolenoid::Value);
	DoubleSolenoid::Value get();

private:
	struct sclist {
		DoubleSolenoid *s = 0;
		uint32_t m_channel1;
		uint32_t m_channel2;

		struct sclist *next = 0;
		struct sclist *prev = 0;
	};
	struct sclist *list;

	DoubleSolenoid::Value state = DoubleSolenoid::Value::kOff;
};

#endif //SOLENOIDCONTROLLERS_H

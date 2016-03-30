/*
 * Lifter.h
 *
 *  Created on: Feb 27, 2016
 *      Author: feds
 */

#ifndef SRC_LIFTER_H_
#define SRC_LIFTER_H_

#include "INIReader.h"
#include "MotorControllers.h"
#include "SolenoidControllers.h"

class Lifter {
public:
	Lifter(INIReader *iniFile);

	void run(float val);
	void stop();
	void toggleLock();

private:
	bool reversed = false;
	bool lockstate = false;

	DoubleSolenoidController *lock;
	Spark *motor;
};

#endif /* SRC_LIFTER_H_ */

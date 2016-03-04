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

class Lifter {
public:
	enum Position {BOTTOM, MIDDLE, TOP};

	Lifter(INIReader *iniFile);
	void update(double dt);

	void setPotition(enum Position targetPosition);
	enum Position getPosition();

private:
	enum Position lastKnownPosition = BOTTOM;
	enum Position targetPosition = BOTTOM;

	SparkMotorController *motor;
	float motorSpeed;

	DigitalInput *lowerLimit;
	DigitalInput *middleLimit;
	DigitalInput *topLimit;
};

#endif /* SRC_LIFTER_H_ */

/*
 * Shooter.h
 *
 *  Created on: Feb 9, 2016
 *      Author: feds
 */

#ifndef SRC_SHOOTER_H_
#define SRC_SHOOTER_H_

#include "MotorControllers.h"
#include "INIReader.h"
#include "WPILIB.h"

class Shooter {
	CANTalon *motorL;
	CANTalon *motorR;
	DoubleSolenoid *solenoidTrigger;

	double shooterPusherCountdown=0;
	double shooterPusherTime = 0;
	float rpm=8100;
	int encCodesPerRevNotQuadrature;

	bool cylinderUp = false;
	bool running = false;

public:
	struct LogVals {
		int RPMSActualL;
		int RPMSActualR;
		int RPMSetpoint;
		bool cylinderUp;
	};

	Shooter(INIReader *iniFile);
	void shoot();

	float modifyRPM(float delta);
	float setRPM(float rpm);

	void start();
	void stop();
	void toggleWheels();

	struct Shooter::LogVals update(double dt, bool logThisTime);
};

#endif /* SRC_SHOOTER_H_ */

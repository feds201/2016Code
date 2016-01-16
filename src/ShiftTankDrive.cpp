/*
 * ShiftTankDrive.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: feds201
 */

#include "ShiftTankDrive.h"
#include "cmath"

ShiftTankDrive::ShiftTankDrive(MotorController *motors_left, MotorController *motors_right, DoubleSolenoidController *solenoids)
:motors_left(motors_left),
 motors_right(motors_right),
 solenoids(solenoids)
{
	printf("SIX!\n");
	motors_left->setControlMode(CANTalon::ControlMode::kPercentVbus);

	printf("SEVEN!\n");
	motors_left->enable();

	printf("EIGHT!\n");
	motors_right->setControlMode(CANTalon::ControlMode::kPercentVbus);
	motors_right->enable();
}

ShiftTankDrive::~ShiftTankDrive()
{
	delete motors_right;
	delete motors_left;
}

void ShiftTankDrive::setcontrol(float forward, float turn)
{

	float l = forward - turn;
	float r = forward + turn;

	float m = fmax(fabs(l), fabs(r));
	if(m > 1)
	{
		l /= m;
		r /= m;
	}

	switch(gear)
	{
	case 0:
		solenoids->set(DoubleSolenoid::Value::kReverse);
		break;
	case 1:
		solenoids->set(DoubleSolenoid::Value::kForward);
		break;
	default:
		break;
	}

	motors_left->set(-l*percent);//because the motors are rotated 180deg
	motors_right->set(r*percent);
}

void ShiftTankDrive::enable()
{
	motors_left->enable();
	motors_right->enable();
}

void ShiftTankDrive::disable()
{
	motors_left->disable();
	motors_right->disable();
}

void ShiftTankDrive::setGear(int gear)
{
	this->gear = gear;

	std::cout << "[INFO] Setting ShiftTankDrive gear to: " << this->gear << std::endl;
}

void ShiftTankDrive::setPercent(float p)
{
	percent = p > 1.0f ? 1.0f : (p < -1.0f ? -1.0f : p);

	std::cout << "[INFO] Setting ShiftTankDrive percent to: " << percent << std::endl;
}

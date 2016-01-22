#include "MotorController.h"

MotorController::MotorController(uint8_t canid)
{
	list = new struct mclist;
	list->motor = new CANTalon(canid);
}

void MotorController::addMotor(uint8_t canid)
{
	struct mclist *l=list;
	while(l->next)
		l = l->next;

	l->next = new struct mclist;

	l->next->prev = l;
	l=l->next;

	l->motor = new CANTalon(canid);
	l->canid = canid;
}

void MotorController::removeMotor(uint8_t canid)
{
	struct mclist *l=list;
	while(l->canid != canid)
		l = l->next;

	if(l == list)
		list = list->next;


	if(l->prev)
		l->prev->next = l->next;
	if(l->next)
		l->next->prev = l->prev;


	delete l->motor;
	delete l;
}

void MotorController::set(float value)
{
	struct mclist *l=list;
	l->motor->Set(value);
	while(l->next)
	{
		l = l->next;
		l->motor->Set(value);
	}
}

void MotorController::enable()
{
	struct mclist *l=list;
	l->motor->EnableControl();
	l->motor->SetSafetyEnabled(false);
	while(l->next)
	{
		l = l->next;
		l->motor->EnableControl();
		l->motor->SetSafetyEnabled(false);
	}
}

void MotorController::setControlMode(CANTalon::ControlMode controlmode)
{
	struct mclist *l=list;
	l->motor->SetControlMode(controlmode);
	while(l->next)
	{
		l = l->next;
		l->motor->SetControlMode(controlmode);
	}
}

void MotorController::disable()
{
	struct mclist *l=list;
	l->motor->Disable();
	while(l->next)
	{
		l = l->next;
		l->motor->Disable();
	}
}
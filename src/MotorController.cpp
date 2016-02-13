#include "MotorController.h"

MotorController::MotorController(uint8_t canid, bool reverse)
{
	list = new struct mclist;
	list->motor = new CANTalon(canid);
	list->reversed = reverse;
}

void MotorController::addMotor(uint8_t canid, bool reverse)
{
	struct mclist *l=list;
	while(l->next)
		l = l->next;

	l->next = new struct mclist;

	l->next->prev = l;
	l=l->next;

	l->motor = new CANTalon(canid);
	l->reversed = reverse;
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
	l->motor->Set(l->reversed ? -value : value);
	while(l->next)
	{
		l = l->next;
		l->motor->Set(l->reversed ? -value : value);
	}
}

void MotorController::runFunctionOnAll(void (*func)(CANTalon *))
{
	struct mclist *l=list;
	func(l->motor);
	while(l->next)
	{
		l = l->next;
		func(l->motor);
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

SparkMotorController::SparkMotorController(uint32_t channel, bool inverted){
	sparkList->inverted = inverted;
	sparkList = new struct mclist;
	sparkList->sparkMotor = new Spark(channel);
	sparkList->sparkMotor->SetInverted(inverted);
}
void SparkMotorController::addSpark(uint32_t channel, bool inverted){

	struct mclist *l=sparkList;
		while(l->next)
			l = l->next;
		l->inverted = inverted;
		l->next = new struct mclist;
		l->next->prev = l;
		l=l->next;
		l->sparkMotor = new Spark(channel);
		l->sparkMotor->SetInverted(inverted);


}
void SparkMotorController::setSpark(float value, uint8_t syncGroup){

	struct mclist *l=sparkList;
		l->sparkMotor->Set(value, syncGroup);
		while(l->next)
		{
			l = l->next;
			l->sparkMotor->Set(value, syncGroup);
		}
}

void SparkMotorController::stopSpark()
{
	struct mclist *l=sparkList;
	l->sparkMotor->StopMotor();
	while(l->next)
	{
		l = l->next;
		l->sparkMotor->StopMotor();
	}
}

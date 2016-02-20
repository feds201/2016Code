#include <MotorControllers.h>

SRXMotorController::SRXMotorController(uint8_t canid, bool reverse)
{
	list = new struct mclist;
	list->motor = new CANTalon(canid);
	list->canid = canid;
	if(reverse)
		list->motor->SetInverted(true);
	list->motor->SetSafetyEnabled(true);
}

void SRXMotorController::addMotor(uint8_t canid, bool reverse)
{
	struct mclist *l=list;
	while(l->next)
		l = l->next;

	l->next = new struct mclist;

	l->next->prev = l;
	l=l->next;

	l->motor = new CANTalon(canid);
	l->motor->SetControlMode(CANTalon::ControlMode::kFollower);
	l->motor->Set(list->canid);
	l->canid = canid;
}

void SRXMotorController::removeMotor(uint8_t canid)
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

void SRXMotorController::set(float value)
{
	list->motor->Set(value);
}

void SRXMotorController::enable()
{
	struct mclist *l=list;
	l->motor->Enable();
	while(l->next)
	{
		l = l->next;
		l->motor->Enable();
	}
}

void SRXMotorController::setControlMode(CANTalon::ControlMode controlmode)
{
	struct mclist *l=list;
	l->motor->SetControlMode(controlmode);
}

void SRXMotorController::syncEncoders()
{
	struct mclist *l=list;
	int encPos = l->motor->GetEncPosition();
	while(l->next)
	{
		l = l->next;
		l->motor->SetEncPosition(encPos);
	}
}

struct SRXMotorController::EncoderInfo_Vel SRXMotorController::getEncoderInfo_Vel()
{
	struct SRXMotorController::EncoderInfo_Vel ret;

	int num=1;
	struct mclist *l=list;
	float sum = l->motor->GetEncVel();
	while(l->next)
	{
		l = l->next;
		sum += l->motor->GetEncVel();
		num++;
	}

	float avg = sum/(float)num;
	ret.avg = avg;
	return ret;
}

void SRXMotorController::disable()
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

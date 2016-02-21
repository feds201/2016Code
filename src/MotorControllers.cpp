#include <MotorControllers.h>

SRXMotorController::SRXMotorController(uint8_t canid, bool reverse)
:CANTalon(canid)
{
	list = new struct mclist;
	list->motor = 0;
	list->canid = canid;
	list->reversed = reverse;
	if(reverse)
		CANTalon::SetInverted(true);
	CANTalon::SetSafetyEnabled(true);
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
	l->reversed = reverse;
	l->motor->SetClosedLoopOutputDirection(reverse != list->reversed);

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


	l->motor->Disable();
	delete l->motor;
	delete l;
}

void SRXMotorController::enable()
{
	struct mclist *l=list;
	CANTalon::Enable();
	while(l->next)
	{
		l = l->next;
		l->motor->Enable();
	}
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

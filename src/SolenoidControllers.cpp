#include "SolenoidControllers.h"

SolenoidController::SolenoidController(uint8_t canid, uint32_t channel)
{
	list = new struct sclist;
	list->s = new Solenoid(canid, channel);
}

void SolenoidController::addSolenoid(uint8_t canid, uint32_t channel)
{
	struct sclist *l=list;
	while(l->next)
		l = l->next;

	l->next = new struct sclist;

	l->next->prev = l;
	l=l->next;

	l->s = new Solenoid(canid, channel);
	l->m_channel = channel;
}

void SolenoidController::removeSolenoid(uint32_t channel)
{
	struct sclist *l=list;
	while(l->m_channel != channel)
		l = l->next;

	if(l == list)
		list = list->next;


	if(l->prev)
		l->prev->next = l->next;
	if(l->next)
		l->next->prev = l->prev;


	delete l->s;
	delete l;
}

void SolenoidController::set(bool on)
{
	struct sclist *l=list;
	l->s->Set(on);
	while(l->next)
	{
		l = l->next;
		l->s->Set(on);
	}
}

bool SolenoidController::get()
{
	return state;
}


DoubleSolenoidController::DoubleSolenoidController(uint8_t canid, uint32_t channel1, uint32_t channel2)
{
	list = new struct sclist;
	list->s = new DoubleSolenoid(canid, channel1, channel2);
}

void DoubleSolenoidController::addSolenoid(uint8_t canid, uint32_t channel1, uint32_t channel2)
{
	struct sclist *l=list;
	while(l->next)
		l = l->next;

	l->next = new struct sclist;

	l->next->prev = l;
	l=l->next;

	l->s = new DoubleSolenoid(canid, channel1, channel2);
	l->m_channel1 = channel1;
	l->m_channel2 = channel2;
}

void DoubleSolenoidController::removeSolenoid(uint32_t channel)
{
	struct sclist *l=list;
	while(l->m_channel1 != channel && l->m_channel2 != channel)
		l = l->next;

	if(l == list)
		list = list->next;


	if(l->prev)
		l->prev->next = l->next;
	if(l->next)
		l->next->prev = l->prev;


	delete l->s;
	delete l;
}

void DoubleSolenoidController::set(DoubleSolenoid::Value v)
{
	struct sclist *l=list;
	l->s->Set(v);
	while(l->next)
	{
		l = l->next;
		l->s->Set(v);
	}
}

DoubleSolenoid::Value DoubleSolenoidController::get()
{
	return state;
}

#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include "CANTalon.h"
#include "WPILib.h"

class MotorController {
public:
	MotorController(uint8_t canid);
	void addMotor(uint8_t canid);
	void removeMotor(uint8_t canid);

	void set(float value);
	void enable();
	void disable();
	void setControlMode(CANTalon::ControlMode);

private:
	struct mclist {
		CANTalon *motor;

		struct mclist *next = 0;
		struct mclist *prev = 0;

		uint8_t canid;
	};
	struct mclist *list;
};

#endif //MOTORCONTROLLER_H

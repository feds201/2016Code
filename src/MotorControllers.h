#ifndef MOTORCONTROLLERS_H
#define MOTORCONTROLLERS_H

#include "CANTalon.h"
#include "WPILib.h"

class SRXMotorController {
public:
	struct EncoderInfo_Vel {
		float avg;
		float standardDiff;
	};

	SRXMotorController(uint8_t canid, bool reverse);
	void addMotor(uint8_t canid, bool reverse);
	void removeMotor(uint8_t canid);

	void set(float value);
	void enable();
	void disable();
	void setControlMode(CANTalon::ControlMode);
	void syncEncoders();
	struct SRXMotorController::EncoderInfo_Vel getEncoderInfo_Vel();
	void runFunctionOnAll(void (*func)(CANTalon *motor));

private:
	struct mclist {
		CANTalon *motor;
		bool reversed;

		struct mclist *next = 0;
		struct mclist *prev = 0;

		uint8_t canid;
	};
	struct mclist *list;
};

class SparkMotorController {
public:
	SparkMotorController(uint32_t channel, bool inverted);
	void addSpark(uint32_t channel, bool inverted);
	void stopSpark();
	void setSpark(float value, uint8_t syncGroup);
private:
	struct mclist {
		Spark *sparkMotor;
		bool inverted;
		float value;
		uint32_t channel;
		uint8_t syncGroup;

		struct mclist *next = 0;
		struct mclist *prev = 0;
	};
	struct mclist *sparkList;

};

#endif //MOTORCONTROLLERS_H

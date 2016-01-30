#include "WPILib.h"

#include "SolenoidControllers.h"
#include "MotorController.h"
#include "ShiftTankDrive.h"
#include "EdgeDetection.h"
#include "Logger.h"

/**
 * This is a demo program showing the use of the RobotDrive class.
 * The SampleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 *
 * WARNING: While it may look like a good choice to use for your code if you're inexperienced,
 * don't. Unless cyou know what you are doing, complex code will be much more difficult under
 * this system. Use IterativeRobot or Command-Based instead if you're new.
 */
class Robot: public SampleRobot
{
	Joystick controller_driver;
	Joystick controller_operator;

	PowerDistributionPanel pdp;
	Compressor compressor;

	MotorController *motors_left;
	MotorController *motors_right;
	DoubleSolenoidController *shifter;
	ShiftTankDrive *std;

public:
	Robot() :
			controller_driver(5),
			controller_operator(1),
			compressor(5)
	{
		shifter = new DoubleSolenoidController(5, 0, 1);
		shifter->addSolenoid(5,2,3);

		motors_left = new MotorController(1);
		motors_left->addMotor(2);
		motors_right = new MotorController(3);
		motors_right->addMotor(4);

		std = new ShiftTankDrive(motors_left, motors_right, shifter);

		// compressor.SetClosedLoopControl(true);

		Logger::instance()->logInfo("Init complete");
	}

	float deadband(float f)
	{
		if(fabs(f)<.15)
			return 0.0f;
		else
		{
			if(f>0)
				return (f-.15)/(1-.15);
			else
				return (f+.15)/(1-.15);
		}
	}

	void Autonomous()
	{
	}

	void OperatorControl()
	{
		EdgeDetection btn_A(false);
		EdgeDetection btn_B(false);
		EdgeDetection btn_X(false);
		EdgeDetection btn_Y(false);

		float percent=1.0f;

		int gear=0;

		while (IsOperatorControl() && IsEnabled())
		{
			btn_A.update(controller_driver.GetRawButton(1));
			btn_B.update(controller_driver.GetRawButton(2));
			btn_X.update(controller_driver.GetRawButton(3));

			if(btn_B.isRising())
			{
				percent += .05;
				percent = percent > 1.0f ? 1.0f : (percent < 0.0f ? 0.0f : percent);

				std->setPercent(percent);
			}
			if(btn_X.isRising())
			{
				percent -= .05;
				percent = percent > 1.0f ? 1.0f : (percent < 0.0f ? 0.0f : percent);

				std->setPercent(percent);
			}
			if(btn_A.isRising())
			{
				if(gear==0)
					gear=1;
				else
					gear=0;
			}

			float forward = deadband(-controller_driver.GetRawAxis(1));
			float rot = deadband(-controller_driver.GetRawAxis(4));

			forward *= fabs(forward);
			rot *= fabs(rot);

			struct Logger::CSV csvData;

			std->setControl(forward, rot, gear,
					&(csvData.driveValues[0]),
					&(csvData.driveValues[1]),
					&(csvData.driveValues[2]),
					&(csvData.driveValues[3]));

			//Logging
			csvData.voltage = pdp.GetVoltage();
			csvData.totalCurrent = pdp.GetTotalCurrent();
			csvData.driveCurrents[0] = pdp.GetCurrent(12);
			csvData.driveCurrents[1] = pdp.GetCurrent(13);
			csvData.driveCurrents[2] = pdp.GetCurrent(14);
			csvData.driveCurrents[3] = pdp.GetCurrent(15);
			csvData.psi = -1;
			csvData.gear = gear;

			Logger::instance()->logCSV(&csvData);
			Wait(0.03);
		}
	}

	void Test()
	{
	}
};

START_ROBOT_CLASS(Robot);

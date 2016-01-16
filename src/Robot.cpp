#include "WPILib.h"

#include "SolenoidControllers.h"
#include "MotorController.h"
#include "ShiftTankDrive.h"
#include "EdgeDetection.h"

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

	MotorController *motors_left;
	MotorController *motors_right;

	DoubleSolenoidController *shifter;

	ShiftTankDrive *std;

	Compressor *compressor;

public:
	Robot() :
			controller_driver(5),
			controller_operator(1)
	{
		shifter = new DoubleSolenoidController(5, 0, 1);
		shifter->addSolenoid(5,2,3);

		motors_left = new MotorController(1);
		motors_left->addMotor(2);
		motors_right = new MotorController(3);
		motors_right->addMotor(4);

		std = new ShiftTankDrive(motors_left, motors_right, shifter);

		compressor = new Compressor(5);
		compressor->SetClosedLoopControl(true);
	}

	void Autonomous()
	{
	}

	void OperatorControl()
	{
		EdgeDetection btn_A(false);
		EdgeDetection btn_B(false);
		EdgeDetection btn_X(false);

		int gear=0;
		float percent=.5;


		int timerUp = 0;
		int timerDown = 0;
		int currGear = 0;
		double su = 0.60;
		double sl = 0.40;
		double alpha = 1.65;

		double phi = (alpha - su)/(alpha*(1 - su));
		double gamma = (su - su*alpha)/(alpha*(1 - su));

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

			double forward = -controller_driver.GetAxis(Joystick::AxisType::kYAxis);
			if(currGear == 0)
			{
				if(fabs(forward) >= su)
				{
					timerDown = 0;
					timerUp++;
					if(timerUp >= 0)
					{
						std->setGear(1);
						currGear = 1;
						timerUp = 0;
					}
				}
				else
				{
					timerUp = 0;
					timerDown = 0;
				}
			}
			else
			{
				if(fabs(forward) <= sl)
				{
					timerUp = 0;
					timerDown++;
					if(timerDown >= 0)
					{
						std->setGear(0);
						currGear = 0;
						timerDown = 0;
					}
				}
				else
				{
					timerDown = 0;
					timerUp = 0;
				}
			}

			if(currGear == 0)
			{
				std->setcontrol(-controller_driver.GetAxis(Joystick::AxisType::kYAxis),-controller_driver.GetAxis(Joystick::AxisType::kTwistAxis));
			}
			if(currGear == 1)
			{
				double forward = controller_driver.GetAxis(Joystick::AxisType::kYAxis);
				if(fabs(forward) <= su)
				{
					forward /= alpha;
				}
				else
				{
					if(forward >= 0)
					{
						forward *= phi;
						forward += gamma;
					}
					else
					{
						forward *= phi;
						forward -= gamma;
					}
				}
				std->setcontrol(-forward,-controller_driver.GetAxis(Joystick::AxisType::kTwistAxis));
			}

			Wait(0.03);
		}
	}

	void Test()
	{
	}
};

START_ROBOT_CLASS(Robot);

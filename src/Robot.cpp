#include "WPILib.h"
#include "MotorControllers.h"
#include "SolenoidControllers.h"
#include "ShiftTankDrive.h"
#include "EdgeDetection.h"
#include "Logger.h"
#include "Shooter.h"

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
	ShiftTankDrive *std;
	Shooter *shooter;

	int logTicker=0;

public:
	Robot() :
			controller_driver(5),
			controller_operator(1),
			compressor(5)

	{
		//SHIFT DRIVE TRAIN
		SRXMotorController *motors_left = new SRXMotorController(3, true);
		motors_left->addMotor(4, true);
		SRXMotorController *motors_right = new SRXMotorController(1, false);
		motors_right->addMotor(2, false);
		DoubleSolenoidController *shifter = new DoubleSolenoidController(10, 0, 1);
		std = new ShiftTankDrive(motors_left, motors_right, shifter);

		//SHOOTER
		SRXMotorController *motors_shooter = new SRXMotorController(5, true);
		motors_shooter->addMotor(6, false);
		DoubleSolenoid *trigger = new DoubleSolenoid(10, 2, 3);
		shooter = new Shooter(motors_shooter, trigger);

		//LOGGER
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
		EdgeDetection btn_Back(false);
		EdgeDetection btn_Start(false);

		bool reverseMode = false;

		int gear=0;

		while (IsOperatorControl() && IsEnabled())
		{
			bool logThisTime = false;
			logTicker++;
			if(logTicker == 20)
			{
				logTicker = 0;
				logThisTime = true;
			}

			btn_A.update(controller_driver.GetRawButton(1));
			btn_B.update(controller_driver.GetRawButton(2));
			btn_X.update(controller_driver.GetRawButton(3));
			btn_Y.update(controller_driver.GetRawButton(4));

			float forward = deadband(-controller_driver.GetRawAxis(1));
			float rot = deadband(-controller_driver.GetRawAxis(4));

			forward *= fabs(forward) * (reverseMode ? -1 : 1);
			rot *= fabs(rot);

			if(btn_A.isRising())
			{
				shooter->shoot();
			}
			if(btn_B.isRising())
			{
				reverseMode = !reverseMode;
			}
			if(btn_X.isRising())
			{
			}
			if(btn_Y.isRising())
			{
				if(gear==0)
					gear=1;
				else
					gear=0;
			}
			if(btn_Back.isRising())
				shooter->modifyRPM(-0.01);
			if(btn_Start.isRising())
				shooter->modifyRPM(+0.01);

			struct ShiftTankDrive::STDLogVals driveVals = std->update(forward, rot, gear, logThisTime);
			struct Shooter::ShooterLogVals shooterVals = shooter->update(logThisTime);

			//Logging
			if(logThisTime)
			{
				logTicker = 0;
				SmartDashboard::PutNumber("Total Amps:", pdp.GetTotalCurrent());
				SmartDashboard::PutNumber("Gear:", gear+1);

				struct Logger::CSV csvData;

				csvData.driveSetpoints[0] = driveVals.values[0];
				csvData.driveSetpoints[1] = driveVals.values[1];
				csvData.driveSetpoints[2] = driveVals.values[2];
				csvData.driveSetpoints[3] = driveVals.values[3];

				csvData.voltage = pdp.GetVoltage();
				csvData.totalCurrent = pdp.GetTotalCurrent();
				csvData.driveCurrents[0] = pdp.GetCurrent(12);
				csvData.driveCurrents[1] = pdp.GetCurrent(13);
				csvData.driveCurrents[2] = pdp.GetCurrent(14);
				csvData.driveCurrents[3] = pdp.GetCurrent(15);
				csvData.shooterRPMActual = shooterVals.RPMSActual;
				csvData.shooterRPMSetpoint = shooterVals.RPMSetpoint;
				csvData.shooterCylinderUp = shooterVals.cylinderUp;
				csvData.psi = -1;
				csvData.gear = gear + 1;

				Logger::instance()->logCSV(&csvData);
			}
			Wait(0.005);
		}
	}

	void Test()
	{
	}
};

START_ROBOT_CLASS(Robot);

#include "WPILib.h"
#include "MotorControllers.h"
#include "SolenoidControllers.h"
#include "ShiftTankDrive.h"
#include "EdgeDetection.h"
#include "Logger.h"
#include "Shooter.h"
#include "INIReader.h"
#include "Pickup.h"
#include "MyTimer.h"

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
	MyTimer timer;

	Joystick controller_driver;
	Joystick controller_operator;

	PowerDistributionPanel pdp;
	Compressor *compressor;
	ShiftTankDrive *std;
	Shooter *shooter;
	Pickup *pickerUpper;

	//INI FILE
	INIReader iniFile;

	int logTicker=0;
	int logInterval;

public:
	Robot() :
			controller_driver(5),
			controller_operator(1),
			iniFile("/config.ini")
	{
		std = new ShiftTankDrive(&iniFile);
		shooter = new Shooter(&iniFile);
		pickerUpper = new Pickup(&iniFile);

		compressor = new Compressor(iniFile.getInt("Pneumatics", "compressorPCMID"));
		compressor->SetClosedLoopControl(true);

		//LOGGER
		logInterval = iniFile.getInt("Logger", "interval");
		Logger::instance()->logInfo("Init complete");
	}

	void RobotInit()
	{
		SmartDashboard::PutString("Robot Name:", iniFile.get("Robot", "name"));
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
		EdgeDetection btn_LBumper(false);
		EdgeDetection btn_RBumper(false);
		EdgeDetection btn_Back(false);
		EdgeDetection btn_Start(false);

		bool reverseMode = false;
		double reverseDisabledCounter;
		bool reverseDisabled = false;

		int gear=0;

		while (IsOperatorControl() && IsEnabled())
		{
			double dt = timer.getDt();

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
			btn_LBumper.update(controller_driver.GetRawButton(5));
			btn_RBumper.update(controller_driver.GetRawButton(6));
			btn_Back.update(controller_driver.GetRawButton(7));
			btn_Start.update(controller_driver.GetRawButton(8));

			float forward = deadband(-controller_driver.GetRawAxis(1));
			float rot = deadband(-controller_driver.GetRawAxis(4));

			forward *= fabs(forward) * (reverseMode ? -1 : 1);
			rot *= fabs(rot);

			if(btn_A.isRising())
			{
				if(btn_LBumper.getState())
					shooter->toggle();
				else
					shooter->shoot();
			}
			if(btn_B.isRising())
			{
				reverseDisabled = true;
				reverseDisabledCounter = -.2;
			}
			if(btn_RBumper.isRising())
			{
			}
			if(btn_X.getState())
			{
				if(btn_LBumper.getState())
					pickerUpper->pickupOnce();
				else
					pickerUpper->pickupOnceSensored();
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

			if(reverseDisabled)
			{
				if(reverseDisabledCounter <= 0)
				{
					reverseDisabledCounter += dt;
					if(reverseDisabledCounter > 0)
						reverseMode = !reverseMode;
				} else
					reverseDisabledCounter += dt;
				if(reverseDisabledCounter >= .2)
					reverseDisabled = false;

				forward = 0;
			}

			struct ShiftTankDrive::LogVals driveVals = std->update(forward, rot, gear, dt, logThisTime);
			struct Shooter::LogVals shooterVals = shooter->update(dt, logThisTime);
			struct Pickup::LogVals pickupVals = pickerUpper->update(dt, logThisTime);

			//Logging
			if(logThisTime)
			{
				logTicker = 0;
				SmartDashboard::PutNumber("Total Amps:", pdp.GetTotalCurrent());
				SmartDashboard::PutNumber("Gear:", gear+1);
				SmartDashboard::PutNumber("dt:", dt);
				SmartDashboard::PutBoolean("would pickup ball:", pickerUpper->getSensorIsReady());

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
				csvData.pickupIsUp = pickupVals.pickupIsUp;

				Logger::instance()->logCSV(&csvData);
			}
			Wait(0.005);
		}
	}

	void Disabled()
	{

	}

	void Test()
	{
	}
};

START_ROBOT_CLASS(Robot);

#include <Auton.h>
#include <math.h>
#include "WPILib.h"
#include "MotorControllers.h"
#include "SolenoidControllers.h"
#include "ShiftTankDrive.h"
#include "EdgeDetection.h"
#include "Logger.h"
#include "Shooter.h"
#include "Lifter.h"
#include "INIReader.h"
#include "Pickup.h"
#include "MyTimer.h"
#include "XboxButtons.h"

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
	Auton *auton;

	SendableChooser *chooser = 0;
	Lifter *lifter;

	//INI FILE
	INIReader iniFile;

	int logTicker=0;
	int logInterval;

	int PDPID_leftPrimaryMotor;
	int PDPID_leftSecondaryMotor;
	int PDPID_rightPrimaryMotor;
	int PDPID_rightSecondaryMotor;
	int PDPID_lifterMotorA;
	int PDPID_lifterMotorB;

public:
	Robot() :
			controller_driver(0),
			controller_operator(1),
			iniFile("/config.ini")
	{
		std = new ShiftTankDrive(&iniFile);
		shooter = new Shooter(&iniFile);
		pickerUpper = new Pickup(&iniFile);
		lifter = new Lifter(&iniFile);

		auton = new Auton(std, shooter, pickerUpper, &iniFile);

		compressor = new Compressor(iniFile.getInt("Pneumatics", "compressorPCMID"));
		compressor->SetClosedLoopControl(true);

		PDPID_leftPrimaryMotor = iniFile.getInt("PDP", "leftPrimaryMotor");
		PDPID_leftSecondaryMotor = iniFile.getInt("PDP", "leftSecondaryMotor");
		PDPID_rightPrimaryMotor = iniFile.getInt("PDP", "rightPrimaryMotor");
		PDPID_rightSecondaryMotor = iniFile.getInt("PDP", "rightSecondaryMotor");
		PDPID_lifterMotorA = iniFile.getInt("PDP", "lifterMotorA");
		PDPID_lifterMotorB = iniFile.getInt("PDP", "lifterMotorB");

		//LOGGER
		logInterval = iniFile.getInt("Logger", "interval");
		Logger::instance()->logInfo("Init complete");
	}

	void RobotInit()
	{
		SmartDashboard::PutString("Robot Name:", iniFile.get("Robot", "name"));

		std::string none = "none";
		std::string drivefwd = "none";

		chooser = new SendableChooser();
		chooser->AddDefault(none, (void*)&none);
		chooser->AddObject(drivefwd, (void*)&drivefwd);
		chooser->AddObject(none, (void*)&none);
		SmartDashboard::PutData("Auto Modes", chooser);
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
		std::string autoSelected = *((std::string*)chooser->GetSelected());
		auton->initAuton(autoSelected);


		double dt = timer.getDt();
		while (IsAutonomous() && IsEnabled() && auton->runAuton(dt))
			dt = timer.getDt();
	}

	void OperatorControl()
	{
		EdgeDetection btns_driver[10];
		EdgeDetection btns_operator[10];

		bool reverseMode = true;
		double reverseDisabledCounter;
		bool reverseDisabled = false;

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

			for(int i=0; i < 10; i++)
				btns_driver[i].update(controller_driver.GetRawButton(i+1));
			for(int i=0; i < 10; i++)
				btns_operator[i].update(controller_operator.GetRawButton(i+1));

			float forward = deadband(-controller_driver.GetRawAxis(1));
			float rot = deadband(-controller_driver.GetRawAxis(4));

			forward *= fabs(forward) * (reverseMode ? -1 : 1);
			rot *= fabs(rot);

			//SHOOTER
			if(btns_driver[XBOX_BTN_A].isRising() && btns_driver[XBOX_BTN_LB].getState())
					shooter->shoot();
			if(btns_operator[XBOX_BTN_A].isRising())
			{
				if(btns_operator[XBOX_BTN_LB].getState())
					shooter->toggleWheels();
				else
					shooter->shoot();
			}

			if(btns_operator[XBOX_BTN_BACK].isRising())
				shooter->modifyRPM(-100);
			if(btns_operator[XBOX_BTN_START].isRising())
				shooter->modifyRPM(+100);

			//TOGGLE PID MODE
			if(btns_driver[XBOX_BTN_RB].isRising() || (btns_operator[XBOX_BTN_RB].isRising() && !btns_operator[XBOX_BTN_LB].getState()))
			{
				std->togglePIDMode();
			}

			//REVERSE MODE
			if(btns_driver[XBOX_BTN_B].isRising())
			{
				reverseDisabled = true;
				reverseDisabledCounter = -.2;
			}

			//LIFTER
			if(btns_operator[XBOX_BTN_LB].getState())
			{
				float val = controller_operator.GetRawAxis(2) - controller_operator.GetRawAxis(3);
				if (logThisTime)
					SmartDashboard::PutNumber("Lifter PWM", val);
				lifter->run(val);
				if(btns_operator[XBOX_BTN_RB].isRising())
					lifter->toggleLock();
			} else if(btns_operator[XBOX_BTN_LB].isFalling())
			{
				lifter->stop();
			}

			//PICKUP
			if(btns_operator[XBOX_BTN_X].getState() && !btns_operator[XBOX_BTN_LB].getState())
				pickerUpper->pickupOnce();

			if(btns_operator[XBOX_BTN_X].isRising() && btns_operator[XBOX_BTN_LB].getState())
				pickerUpper->togglePickup();

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

			struct ShiftTankDrive::LogVals driveVals = std->update(forward, rot, dt, logThisTime);
			struct Shooter::LogVals shooterVals = shooter->update(dt, logThisTime);
			struct Pickup::LogVals pickupVals = pickerUpper->update(dt, logThisTime);

			//Logging
			if(logThisTime)
			{
				logTicker = 0;
				SmartDashboard::PutNumber("Total Amps:", pdp.GetTotalCurrent());
				SmartDashboard::PutNumber("dt:", dt);

				struct Logger::CSVVals csvData;

				csvData.driveSetpoints[0] = driveVals.values[0];
				csvData.driveSetpoints[1] = driveVals.values[1];
				csvData.driveSetpoints[2] = driveVals.values[2];
				csvData.driveSetpoints[3] = driveVals.values[3];

				csvData.driveSpeeds[0] = driveVals.speeds[0];
				csvData.driveSpeeds[1] = driveVals.speeds[1];
				csvData.driveSpeeds[2] = driveVals.speeds[2];
				csvData.driveSpeeds[3] = driveVals.speeds[3];

				csvData.driveCurrents[0] = pdp.GetCurrent(PDPID_leftPrimaryMotor);
				csvData.driveCurrents[1] = pdp.GetCurrent(PDPID_leftSecondaryMotor);
				csvData.driveCurrents[2] = pdp.GetCurrent(PDPID_rightPrimaryMotor);
				csvData.driveCurrents[3] = pdp.GetCurrent(PDPID_rightSecondaryMotor);

				csvData.lifterCurrent = std::max(pdp.GetCurrent(PDPID_lifterMotorA), pdp.GetCurrent(PDPID_lifterMotorB));

				csvData.voltage = pdp.GetVoltage();
				csvData.totalCurrent = pdp.GetTotalCurrent();

				csvData.shooterRPMActualL = shooterVals.RPMSActualL;
				csvData.shooterRPMActualR = shooterVals.RPMSActualR;
				csvData.shooterRPMSetpoint = shooterVals.RPMSetpoint;
				csvData.shooterCylinderUp = shooterVals.cylinderUp;

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

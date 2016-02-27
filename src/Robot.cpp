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
	Lifter *lifter;

	//INI FILE
	INIReader iniFile;

	int logTicker=0;
	int logInterval;

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

		compressor = new Compressor(iniFile.getInt("Pneumatics", "compressorPCMID"));
		compressor->SetClosedLoopControl(true);

		//LOGGER
		logInterval = iniFile.getInt("Logger", "interval");
		Logger::instance()->logInfo("Init complete");
	}

	void RobotInit()
	{
		//SmartDashboard::PutString("Robot Name:", iniFile.get("Robot", "name"));
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
		EdgeDetection btns_driver[10];
		EdgeDetection btns_operator[10];

		bool reverseMode = false;
		double reverseDisabledCounter;
		bool reverseDisabled = false;

		bool highGear=false;

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

			for(int i=0; i < sizeof(btns_driver); i++)
				btns_driver[i].update(controller_driver.GetRawButton(i+1));
			for(int i=0; i < sizeof(btns_operator); i++)
				btns_driver[i].update(controller_operator.GetRawButton(i+1));

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

			//REVERSE MODE
			if(btns_driver[XBOX_BTN_B].isRising())
			{
				reverseDisabled = true;
				reverseDisabledCounter = -.2;
			}

			if(btns_operator[XBOX_BTN_X].getState() && !btns_operator[XBOX_BTN_LB].getState())
				pickerUpper->pickupOnceSensored();

			if(btns_operator[XBOX_BTN_X].isRising() && btns_operator[XBOX_BTN_LB].getState())
				pickerUpper->togglePickup();

			if(btns_driver[XBOX_BTN_A].isRising())
				highGear = !highGear;

			if(btns_operator[XBOX_BTN_BACK].isRising())
				shooter->modifyRPM(-100);
			if(btns_operator[XBOX_BTN_BACK].isRising())
				shooter->modifyRPM(+100);

			//LIFTER
			if(controller_driver.GetPOV(0)==0)
			{
				if(btns_driver[XBOX_BTN_LB])
					lifter->setPotition(Lifter::Position::TOP);
				else
					lifter->setPotition(Lifter::Position::MIDDLE);
			} else if(controller_driver.GetPOV(0)==180)
				lifter->setPotition(Lifter::Position::BOTTOM);
			if(controller_operator.GetPOV(0)==0)
			{
				if(btns_operator[XBOX_BTN_LB])
					lifter->setPotition(Lifter::Position::TOP);
				else
					lifter->setPotition(Lifter::Position::MIDDLE);
			} else if(controller_operator.GetPOV(0)==180)
				lifter->setPotition(Lifter::Position::BOTTOM);

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

			struct ShiftTankDrive::LogVals driveVals = std->update(forward, rot, highGear, dt, logThisTime);
			struct Shooter::LogVals shooterVals = shooter->update(dt, logThisTime);
			struct Pickup::LogVals pickupVals = pickerUpper->update(dt, logThisTime);

			//Logging
			if(logThisTime)
			{
				logTicker = 0;
				SmartDashboard::PutNumber("Total Amps:", pdp.GetTotalCurrent());
				SmartDashboard::PutBoolean("Gear:", highGear);
				SmartDashboard::PutNumber("dt:", dt);
				SmartDashboard::PutBoolean("would pickup ball:", pickerUpper->getSensorIsReady());

				struct Logger::CSVVals csvData;

				csvData.driveSetpoints[0] = driveVals.values[0];
				csvData.driveSetpoints[1] = driveVals.values[1];
				csvData.driveSetpoints[2] = driveVals.values[2];
				csvData.driveSetpoints[3] = driveVals.values[3];

				csvData.voltage = pdp.GetVoltage();
				csvData.totalCurrent = pdp.GetTotalCurrent();
				csvData.driveCurrents[0] = pdp.GetCurrent(0);
				csvData.driveCurrents[1] = pdp.GetCurrent(1);
				csvData.driveCurrents[2] = pdp.GetCurrent(2);
				csvData.driveCurrents[3] = pdp.GetCurrent(3);
				csvData.shooterRPMActualL = shooterVals.RPMSActualL;
				csvData.shooterRPMActualR = shooterVals.RPMSActualR;
				csvData.shooterRPMSetpoint = shooterVals.RPMSetpoint;
				csvData.shooterCylinderUp = shooterVals.cylinderUp;
				csvData.psi = -1;
				csvData.gear = highGear ? 2 : 1;
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

#ifndef _JETCAT_H_
#define _JETCAT_H_


typedef enum{
	OFF = 0,			//Engine can be start
	WAIT_for_RPM, 
	IGNITE, 
	ACCELERATE,
	STABILIZE,
	
	NOT_USED_01,	//5
	LEARN_LO, 
	OFF_COOLING,			//Engine can be start
	SLOW_DOWN,
	NOT_USED_03,
	
	AUTO_OFF,		//10
	RUN,
	ACCELATION_DELAY,
	SPEED_REG,
	TWO_SHAFT_REGULATE,

	PRE_HEAT_1,		//15
	PRE_HEAT_2,
	AUTO_BLEED,
	NOT_USED_04,
	KEROS_FULL_ON,

	TURBINE_STATE_NUM
} TURBINE_STATE_TYPE;


typedef struct{
	float TurbineRPM;
	float EGT;
	float PumpVoltage;
	unsigned int TurbineState;
	float ThrottlePosition;
	float EngineCurrent;
}P250_RAC;


typedef struct{
	unsigned char firmware_s[256];
	unsigned int firmware;
	unsigned int LastRunTime;
	unsigned int TotalOperationTime;
	unsigned int SerialNo;
	unsigned int TurbineType;
}P250_RTY;


typedef struct{
	unsigned int OffCondition;
	float AmbientTemp;
	float MinPumpVoltage;
	float MaxPumpVoltage;
}P250_RA1;


typedef struct{
	unsigned int RunOK;
	unsigned int StartFail;
	unsigned int TotalOperationTime;
	unsigned int LastRunTime;
	unsigned int SystemTime;
}P250_RI1;


typedef struct{
	float ActualFuelFlow;
	unsigned int RestVolumeinTank;
	float setRPM;
	float ActualBatteryVoltage;
	unsigned int LastRunTime;
	float FuelConsumedonActualRun;
}P250_RFI;


typedef struct{
	unsigned int ThrottlePos;
	unsigned int ThrottlePulse;
	unsigned int AUXPos;
	unsigned int AUXPulse;
	unsigned int FailSafeCount;
	unsigned int FailSafeTime;
}P250_RRC;
#endif
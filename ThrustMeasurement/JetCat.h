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

typedef enum{
	NORMAL_OP = 0,
	ECU_CAN_FLASH,
	ECU_RS232_FLASH
} TURBINE_OP_MODE;

typedef enum{
	No_OFF_Condition_Defined = 0,
	SHUT_DOWN_via_RC,
	OVER_TEMPERATURE,
	IGNITION_TIMEOUT,
	ACC_TIME_OUT, 
	ACC_TOO_SLOW,				//5
	OVER_RPM, 
	LOW_RPM_OFF, 
	LOW_BATTERY, 
	ENGINE_AUTO_OFF, 
	LOW_TEMP_OFF,				//10
	HIGH_TEMP_OFF, 
	GLOW_PLUG_DEFECTIVE,
	WATCHDOG_TIMER,
	FAIL_SAFE_OFF,
	MANUAL_OFF_via_GSU,			//15
	POWER_FAIL, 
	TEMP_SENSOR_FAIL, 
	FUEL_FAIL, 
	PROP_FAIL, 
	SND_ENGINE_FAIL,			//20
	SND_ENGINE_DIFF_TO_HIGH, 
	SND_ENGINE_NO_COMM, 
	NO_OIL, 
	OVER_CURRENT, 
	NO_FUEL_PUMP_CONNECT,		//25
	WRONG_FUEL_PUMP_CONNECT,
	FUEL_PUMP_COMM_ERROR, 
	OUT_OF_FUEL_SHUT_DOWN, 
	LOW_RPM_SHUT_DOWN_PUMP_FAIL, 
	LOW_RPM_SHUT_DOWN_FRONTBOARD_FAIL,	//30
	CLUTCH_FAIL, 
	ECU_REBOOT, 
	ENGINE_SHUT_DOWN_CAN,
	NO_RC_PULSE,
	ROTOR_BLOCKED
} ENGINE_OFF_CONDITION;

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
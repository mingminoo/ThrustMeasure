

//0x100 (10Hz)
typedef struct
{
	unsigned int	SetRPM;
	unsigned int	RealRPM;
	float			EGT;
	unsigned char	State;
	float			Pump_Voltage;
}Read_Actual_Value;

//0x101 (10Hz)
typedef struct
{
	float			Battery_Voltage;
	float			Engine_Current;
	float			Generator_Voltage;
	float			Generator_Current;
	unsigned char	Flag;
}Read_Voltage_Current;

//0x102 (5Hz)
typedef struct
{
	unsigned int	Fuel_Flow;
	unsigned int	Fuel_Consumed;
	float			Air_Pressure;
	float			Ambient_Temperature;
}Read_Fuel_Ambient;

//0x103 (1Hz)
typedef struct
{
	unsigned int	Runs_OK;
	unsigned int	Runs_Aborted;
	unsigned long int	Total_Run_Time;
}Read_Statistics;

//0x104 (1Hz)
typedef struct
{
	unsigned int	Last_Run_Time;
	unsigned int	Last_Off_RPM;
	float			Last_Off_EGT;
	float			Last_Off_PumpVoltage;
	unsigned char	Last_Off_Condition;
}Read_Last_Run_Info;

//0x105 (1Hz)
typedef struct
{
	unsigned int	Serial_Number;
	unsigned int	Firmware_Version;
	unsigned char	Engine_Type;
	unsigned char	Engine_Sub_Type;
	char			OpMode;
}Read_System_Info;
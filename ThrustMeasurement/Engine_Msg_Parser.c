#include <stdio.h>      // Include file for printf
#include "nixnet.h"     // Include file for NI-XNET functions and constants
#include "Engine_Msg_Parser.h"

//=============================================================================  
// Parsing_Read_Actual_Value (0x100)
//============================================================================= 
void Parsing_Read_Actual_Value(Read_Actual_Value* var, nxFrameVar_t* l_MyFramePtr){
	unsigned int temp1 = 0;
	unsigned int temp2 = 0;
	unsigned int temp3 = 0;

	temp1 = (l_MyFramePtr->Payload[0] & 0xFF) << 8;
	temp1 = (unsigned int)(temp1 | (l_MyFramePtr->Payload[1] & 0xFF));
	var->SetRPM = temp1 * 10;
	//printf("\tSetRPM : %d\n",var->SetRPM);

	temp2 = (l_MyFramePtr->Payload[2] & 0xFF) << 8;
	temp2 = (unsigned int)(temp2 | (l_MyFramePtr->Payload[3] & 0xFF));
	var->RealRPM = temp2 * 10;
	//printf("\tRealRPM : %d\n",var->RealRPM);

	temp3 = (l_MyFramePtr->Payload[4] & 0xFF) << 8;
	temp3 = (unsigned int)(temp3 | (l_MyFramePtr->Payload[5] & 0xFF));
	var->EGT = (float)(temp3 * 0.1);
	//printf("\tEGT : %f\n",var->EGT);

	var->State = (unsigned char)(l_MyFramePtr->Payload[6] & 0xFF);
	//printf("\tState : %x\n",var->State);

	var->Pump_Voltage = (float)((l_MyFramePtr->Payload[7] & 0xFF) * 0.05);
	//printf("\tPump_Voltage : %f\n",var->Pump_Voltage);
	//printf("\n");
}

//=============================================================================  
// Parsing_Read_Voltage_Current (0x101)
//============================================================================= 
void Parsing_Read_Voltage_Current(Read_Voltage_Current* var, nxFrameVar_t* l_MyFramePtr){
	var->Battery_Voltage = (float)((l_MyFramePtr->Payload[0] & 0xFF) * 0.1);
	//printf("\tBattery_Voltage : %f\n",var->Battery_Voltage);
	var->Engine_Current = (float)((l_MyFramePtr->Payload[1] & 0xFF) * 0.1);
	//printf("\tEngine_Current : %f\n",var->Engine_Current);
	var->Generator_Voltage = (float)((l_MyFramePtr->Payload[2] & 0xFF) * 0.2);
	//printf("\tGenerator_Voltage : %f\n",var->Generator_Voltage);
	var->Generator_Current = (float)((l_MyFramePtr->Payload[3] & 0xFF) * 0.1);
	//printf("\tGenerator_Current : %f\n",var->Generator_Current);
	var->Flag = (unsigned char)(l_MyFramePtr->Payload[4] & 0xFF);
	//printf("\tFlag : %x\n",var->Flag);
	//printf("\n");
}

//=============================================================================  
// Parsing_Read_Fuel_Ambient (0x102)
//============================================================================= 
void Parsing_Read_Fuel_Ambient(Read_Fuel_Ambient* var, nxFrameVar_t* l_MyFramePtr){
	unsigned int temp1 = 0;
	unsigned int temp2 = 0;
	unsigned int temp3 = 0;

	temp1 = (l_MyFramePtr->Payload[0] & 0xFF) << 8;
	temp1 = (unsigned int)(temp1 | (l_MyFramePtr->Payload[1] & 0xFF));
	var->Fuel_Flow = temp1;
	//printf("\tFuel_Flow : %d\n",var->Fuel_Flow);
	temp2 = (l_MyFramePtr->Payload[2] & 0xFF) << 8;
	temp2 = (unsigned int)(temp2 | (l_MyFramePtr->Payload[3] & 0xFF));
	var->Fuel_Consumed = temp2 * 10;
	//printf("\tFuel_Consumed : %d\n",var->Fuel_Consumed);
	temp3 = (l_MyFramePtr->Payload[4] & 0xFF) << 8;
	temp3 = temp3 | (l_MyFramePtr->Payload[5] & 0xFF);
	var->Air_Pressure = (float)(temp3 * 0.02);
	//printf("\tAir_Pressure : %f\n",var->Air_Pressure);
	var->Ambient_Temperature = (float)((l_MyFramePtr->Payload[6] & 0xFF) * 0.5);
	//printf("\tAmbient_Temperature : %f\n",var->Ambient_Temperature);
	//printf("\n");
}

//=============================================================================  
// Parsing_Read_Statistics (0x103)
//============================================================================= 
void Parsing_Read_Statistics(Read_Statistics* var, nxFrameVar_t* l_MyFramePtr){
	unsigned int temp1 = 0;
	unsigned int temp2 = 0;
	unsigned int temp3 = 0;

	temp1 = (l_MyFramePtr->Payload[0] & 0xFF) << 8;
	temp1 = (unsigned int)(temp1 | (l_MyFramePtr->Payload[1] & 0xFF));
	var->Runs_OK = temp1;
	//printf("\tRuns_OK : %d\n",var->Runs_OK);
	temp2 = (l_MyFramePtr->Payload[2] & 0xFF) << 8;
	temp2 = (unsigned int)(temp2 | (l_MyFramePtr->Payload[3] & 0xFF));
	var->Runs_Aborted = temp2;
	//printf("\tRuns_Aborted : %d\n",var->Runs_Aborted);
	temp3 = (l_MyFramePtr->Payload[4] & 0xFF) << 24;
	temp3 = (temp3 | (l_MyFramePtr->Payload[5] & 0xFF) << 16);
	temp3 = (temp3 | (l_MyFramePtr->Payload[6] & 0xFF) << 8);
	temp3 = (temp3 | (l_MyFramePtr->Payload[7] & 0xFF));
	var->Total_Run_Time = (unsigned long)temp3;
	//printf("\tTotal_Run_Time : %d\n",var->Total_Run_Time);
	//printf("\n");
}

//=============================================================================  
// Parsing_Read_Last_Run_Info (0x104)
//============================================================================= 
void Parsing_Read_Last_Run_Info(Read_Last_Run_Info* var, nxFrameVar_t* l_MyFramePtr){
	unsigned int temp1 = 0;
	unsigned int temp2 = 0;
	unsigned int temp3 = 0;

	temp1 = (l_MyFramePtr->Payload[0] & 0xFF) << 8;
	temp1 = (unsigned int)(temp1 | (l_MyFramePtr->Payload[1] & 0xFF));
	var->Last_Run_Time = temp1;
	//printf("\tLast_Run_Time : %d\n",var->Last_Run_Time);
	temp2 = (l_MyFramePtr->Payload[2] & 0xFF) << 8;
	temp2 = (unsigned int)(temp2 | (l_MyFramePtr->Payload[3] & 0xFF));
	var->Last_Off_RPM = temp2;
	//printf("\tLast_Off_RPM : %d\n",var->Last_Off_RPM);
	temp3 = (l_MyFramePtr->Payload[4] & 0xFF) << 8;
	temp3 = (unsigned int)(temp3 | (l_MyFramePtr->Payload[5] & 0xFF));
	var->Last_Off_EGT = (float)temp3;
	//printf("\tLast_Off_EGT : %f\n",var->Last_Off_EGT);
	var->Last_Off_PumpVoltage = (float)((l_MyFramePtr->Payload[6] & 0xFF) * 0.05);
	//printf("\tLast_Off_PumpVoltage : %f\n",var->Last_Off_PumpVoltage);
	var->Last_Off_Condition = (unsigned char)(l_MyFramePtr->Payload[7] & 0xFF);
	//printf("\tLast_Off_Condition : %x\n",var->Last_Off_Condition);
	//printf("\n");
}

//=============================================================================  
// Parsing_Read_System_Info (0x105)
//============================================================================= 
void Parsing_Read_System_Info(Read_System_Info* var, nxFrameVar_t* l_MyFramePtr){
	unsigned int temp1 = 0;
	unsigned int temp2 = 0;

	temp1 = (l_MyFramePtr->Payload[0] & 0xFF) << 8;
	temp1 = (unsigned int)(temp1 | (l_MyFramePtr->Payload[1] & 0xFF));
	var->Serial_Number = temp1;
	//printf("\tSerial_Number : %x\n",var->Serial_Number);
	temp2 = (l_MyFramePtr->Payload[2] & 0xFF) << 8;
	temp2 = (unsigned int)(temp2 | (l_MyFramePtr->Payload[3] & 0xFF));
	var->Firmware_Version = temp2;
	//printf("\tFirmware_Version : %x\n",var->Firmware_Version);
	var->Engine_Type = (unsigned char)(l_MyFramePtr->Payload[4] & 0xFF);
	//printf("\tEngine_Type : %x\n",var->Engine_Type);
	var->Engine_Sub_Type = (unsigned char)(l_MyFramePtr->Payload[5] & 0xFF);
	//printf("\tEngine_Sub_Type : %x\n",var->Engine_Sub_Type);
	var->OpMode = (unsigned char)(l_MyFramePtr->Payload[6] & 0xFF);
	//printf("\tOpMode : %x\n",var->OpMode);
	//printf("\n");
}

#include "nixnet.h"     // Include file for NI-XNET functions and constants


void Engine_On_Off_Control(nxFrameVar_t * l_MyFramePtr, unsigned int value){
	//DLC = 1
	//On	= 1
	//Off	= 0
	l_MyFramePtr->Timestamp		=0;
	l_MyFramePtr->Flags			=0;
	l_MyFramePtr->Identifier	=0x120; 
	l_MyFramePtr->Type			=nxFrameType_CAN_Data;
	l_MyFramePtr->PayloadLength	=1;
	if (value == 1){
		l_MyFramePtr->Payload[0]=1;	
	}
	else if (value == 0){
		l_MyFramePtr->Payload[0]=0;	
	}
	else{
		// Do nothing
	}
}

void Engine_Throttle_Control(nxFrameVar_t * l_MyFramePtr, float value){
	//DLC = 2
	//Byte0 = HighByte
	//Byte1 = LowByte
	//0.0% ~ 100.0%
	unsigned int temp = 0;
	
	l_MyFramePtr->Timestamp		=0;
	l_MyFramePtr->Flags			=0;
	l_MyFramePtr->Identifier	=0x121; 
	l_MyFramePtr->Type			=nxFrameType_CAN_Data;
	l_MyFramePtr->PayloadLength	=2;

	if (value >= 0 && value <= 100){
		temp = (unsigned int)(value * 10);
		l_MyFramePtr->Payload[0] = (unsigned char)((temp & 0xFF00) >> 8);
		l_MyFramePtr->Payload[1] = (unsigned char)(temp & 0x00FF);
	}

	

}

void Engine_RPM_Control(nxFrameVar_t * l_MyFramePtr, unsigned int value){
	//DLC = 2
	//Byte0 = HighByte
	//Byte1 = LowByte
	//0 ~ 250000
	unsigned int temp = 0;

	l_MyFramePtr->Timestamp		=0;
	l_MyFramePtr->Flags			=0;
	l_MyFramePtr->Identifier	=0x122; 
	l_MyFramePtr->Type			=nxFrameType_CAN_Data;
	l_MyFramePtr->PayloadLength	=2;

	if (value >= 0 && value <= 115000){
		temp = (unsigned int)value / 10;
		l_MyFramePtr->Payload[0] = (unsigned char)((temp & 0xFF00) >> 8);
		l_MyFramePtr->Payload[1] = (unsigned char)(temp & 0x00FF);
	}
}

void Engine_Test_Function(nxFrameVar_t * l_MyFramePtr){
	//DLC = 6
	//0 : Fuel Pump Test
	//1 : Smoke Pump Test
	//2 : Main Valve Test
	//3 : Starter Valve Test
	//4 : Igniter Test
	//5 : Starter Test
	
}
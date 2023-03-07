#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "visa.h"
//#include "RdWrtSrl.h"
#include "Header.h"

extern ViSession	instr_ch4;
extern ViStatus		status;
extern ViUInt32		writeCount;

void sendJetCatCMD_RAC(){
	// Read Actual Values
	static char stringinput[512];

	strcpy_s(stringinput, 8, "1,RAC,1\r");

	status = viWrite (instr_ch4, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
	if (status < VI_SUCCESS)    
	{
		//printf ("Error writing to the device.\n");
		//goto Close;
	}

}

void sendJetCatCMD_RTY(){
	// Read Information
	static char stringinput[512];

	strcpy_s(stringinput, 8, "1,RTY,1\r");

	status = viWrite (instr_ch4, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
	if (status < VI_SUCCESS)    
	{
		//printf ("Error writing to the device.\n");
		//goto Close;
	}
}

void sendJetCatCMD_RA1(){
	// Read
	static char stringinput[512];

	strcpy_s(stringinput, 8, "1,RA1,1\r");

	status = viWrite (instr_ch4, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
	if (status < VI_SUCCESS)    
	{
		//printf ("Error writing to the device.\n");
		//goto Close;
	}
}

void sendJetCatCMD_RI1(){
	// Read Statistic Info
	static char stringinput[512];

	strcpy_s(stringinput, 8, "1,RI1,1\r");

	status = viWrite (instr_ch4, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
	if (status < VI_SUCCESS)    
	{
		//printf ("Error writing to the device.\n");
		//goto Close;
	}
}

void sendJetCatCMD_RFI(){
	// Read Fuel Info
	static char stringinput[512];

	strcpy_s(stringinput, 8, "1,RFI,1\r");

	status = viWrite (instr_ch4, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
	if (status < VI_SUCCESS)    
	{
		//printf ("Error writing to the device.\n");
		//goto Close;
	}
}

void sendJetCatCMD_RRC(){
	// Read RC Data Info
	static char stringinput[512];

	strcpy_s(stringinput, 8, "1,RRC,1\r");

	status = viWrite (instr_ch4, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
	if (status < VI_SUCCESS)    
	{
		//printf ("Error writing to the device.\n");
		//goto Close;
	}
}

void sendJetCatCMD_TCO(){
	// Turbine Control
	static char stringinput[512];

	strcpy_s(stringinput, 8, "1,TCO,1\r");

	status = viWrite (instr_ch4, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
	if (status < VI_SUCCESS)    
	{
		//printf ("Error writing to the device.\n");
		//goto Close;
	}
}

void sendJetCatCMD_WRP(){
	// Set Turbine RPM
	static char stringinput[512];

	strcpy_s(stringinput, 8, "1,WRP,1\r");

	status = viWrite (instr_ch4, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
	if (status < VI_SUCCESS)    
	{
		//printf ("Error writing to the device.\n");
		//goto Close;
	}
}

void sendJetCatCMD_WPE(){
	// Set Turbine Thrust in %
	static char stringinput[512];

	strcpy_s(stringinput, 8, "1,WPE,1\r");

	status = viWrite (instr_ch4, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
	if (status < VI_SUCCESS)    
	{
		//printf ("Error writing to the device.\n");
		//goto Close;
	}
}

void sendJetCatCMD_SVC(){
	// Smoker Valve Control
	static char stringinput[512];

	strcpy_s(stringinput, 8, "1,SVC,1\r");

	status = viWrite (instr_ch4, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
	if (status < VI_SUCCESS)    
	{
		//printf ("Error writing to the device.\n");
		//goto Close;
	}
}
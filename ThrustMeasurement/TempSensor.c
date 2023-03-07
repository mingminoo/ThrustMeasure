#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include "RdWrtSrl.h"
#include "Header.h"

/* Get Temperature Result */
void get_Temp_Result(SaveParameters* vSave, const char* buffer){
	float result = 0.0;

	// Check sign
	if (buffer[0] == 0x2D){
		result = (float)atof(&buffer[1]);
		result = (float)(result * -1.0);
	}
	else{
		result = (float)atof(&buffer[0]);
	}

	vSave->Temp = result;
}

/************************************************************************/
/* Temperature Sensor #1                                                */
/************************************************************************/
void rs232TempRecv(SaveParameters* vSaveP, const char* buffer){
	int len = 0;
	int idx = 0;

	for(idx = 0; idx < TEMPSENSOR_READ_NUM; idx++){
		// Check LF
		if (buffer[idx+5] == 0x0A){
			get_Temp_Result(vSaveP, &buffer[idx]);
		}
	}

	//printf ("\nData read: %f\n", vSaveP->Temp);
}
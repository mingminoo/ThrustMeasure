#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include "RdWrtSrl.h"
#include "Header.h"

/* Get LoadCell Result #1 */
void get_LoadCell_1_Result(SaveParameters* vSave, const char* buffer){
	float result = 0.0;

	result = (float)atof(&buffer[4]);

	if (buffer[3] == 0x2B){
		result = (float)(result * 1.0);
	}
	else if (buffer[3] == 0x2D){
		result = (float)(result * -1.0);
	}
	else{
		result = 0;
	}

	vSave->LoadCell01 = result;

}
/* Get LoadCell Result #1 */
void get_LoadCell_2_Result(SaveParameters* vSave, const char* buffer){
	float result = 0.0;

	result = (float)atof(&buffer[4]);

	if (buffer[3] == 0x2B){
		result = (float)(result * 1.0);
	}
	else if (buffer[3] == 0x2D){
		result = (float)(result * -1.0);
	}
	else{
		result = 0;
	}

	vSave->LoadCell02 = result;

}


/************************************************************************/
/* Load Cell #1                                                         */
/************************************************************************/
void rs232LoadCell_1_Recv(SaveParameters* vSaveP, const char* buffer){
	int len = 0;
	int idx = 0;

	for(idx = 0; idx < LOADCELL_READ_NUM; idx++){
		// Check STX
		if (buffer[idx] == 0x02){
			// Check ID '0'
			if (buffer[idx+1] == 0x30){
				// Check ID '1'
				if (buffer[idx+2] == 0x31){
					// Check ETX
					if (buffer[idx+12] == 0x03){
						get_LoadCell_1_Result(vSaveP, &buffer[idx]);
						break;
					}
				}
			}
		}
	}
	//printf ("\nData read: %*s\n", retCount, buffer);
	//printf ("\nData read: %f\n", vSaveP->LoadCell01);

}

/************************************************************************/
/* Load Cell #2                                                         */
/************************************************************************/
void rs232LoadCell_2_Recv(SaveParameters* vSaveP, const char* buffer){
	int len = 0;
	int idx = 0;

	for(idx = 0; idx < LOADCELL_READ_NUM; idx++){
		// Check STX
		if (buffer[idx] == 0x02){
			// Check ID '0'
			if (buffer[idx+1] == 0x30){
				// Check ID '1'
				if (buffer[idx+2] == 0x31){
					// Check ETX
					if (buffer[idx+12] == 0x03){
						get_LoadCell_2_Result(vSaveP, &buffer[idx]);
						break;
					}
				}
			}
		}
	}
	//printf ("\nData read: %f\n", vSaveP->LoadCell01);

}

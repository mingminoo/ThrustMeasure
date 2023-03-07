#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "visa.h"
//#include "RdWrtSrl.h"
#include "Header.h."

void get_RAC_Result(SaveParameters* vSave, const char* buffer){
	int i=0;
	int seperator = 0;
	int offset = 0;
	float tempEGT = 0.0;

	// Turbine RPM
	vSave->Turbine_RPM = (float)atof(&buffer[ENGINE_DATA_OFFSET]);
	
	for (i = 0; i < 10; i++){
		if (buffer[ENGINE_DATA_OFFSET+i] == 0x2C){
			seperator = i;
			break;
		}
	}
	offset = offset+seperator+1;

	// EGT
	vSave->EGT = (float)atof(&buffer[ENGINE_DATA_OFFSET+offset]);

	for (i = 0; i < 10; i++){
		if (buffer[ENGINE_DATA_OFFSET+offset+i] == 0x2C){
			seperator = i;
			break;
		}
	}
	offset = offset+seperator+1;


}

void get_RTY_Result(SaveParameters* vSave, const char* buffer){
	int i=0;
	int seperator = 0;
	int offset = 0;
}

void get_RA1_Result(SaveParameters* vSave, const char* buffer){
	int i=0;
	int seperator = 0;
	int offset = 0;

	for (i = 0; i < 10; i++){
		if (buffer[ENGINE_DATA_OFFSET+i] == 0x2C){
			seperator = i;
			break;
		}
	}
	offset = offset+seperator+1;

	// Ambient Temperature
	vSave->Ambient_Temp = (float)atof(&buffer[ENGINE_DATA_OFFSET+offset]);
}

void get_RI1_Result(SaveParameters* vSave, const char* buffer){
	int i=0;
	int seperator = 0;
	int offset = 0;
}

void get_RFI_Result(SaveParameters* vSave, const char* buffer){
	int i=0;
	int seperator = 0;
	int offset = 0;

	// Fuel Flow
	vSave->FF = (float)atof(&buffer[ENGINE_DATA_OFFSET]);

	for (i = 0; i < 10; i++){
		if (buffer[ENGINE_DATA_OFFSET+i] == 0x2C){
			seperator = i;
			break;
		}
	}
	offset = offset+seperator+1;

	for (i = 0; i < 10; i++){
		if (buffer[ENGINE_DATA_OFFSET+offset+i] == 0x2C){
			seperator = i;
			break;
		}
	}
	offset = offset+seperator+1;

	for (i = 0; i < 10; i++){
		if (buffer[ENGINE_DATA_OFFSET+offset+i] == 0x2C){
			seperator = i;
			break;
		}
	}
	offset = offset+seperator+1;

	for (i = 0; i < 10; i++){
		if (buffer[ENGINE_DATA_OFFSET+offset+i] == 0x2C){
			seperator = i;
			break;
		}
	}
	offset = offset+seperator+1;

	for (i = 0; i < 10; i++){
		if (buffer[ENGINE_DATA_OFFSET+offset+i] == 0x2C){
			seperator = i;
			break;
		}
	}
	offset = offset+seperator+1;

	// Fuel Consumed
	vSave->FuelConsumed = (float)atof(&buffer[ENGINE_DATA_OFFSET+offset]);

}

void get_RRC_Result(SaveParameters* vSave, const char* buffer){
	int i=0;
	int seperator = 0;
	int offset = 0;
}






void recvJetCatCMD_RAC(SaveParameters* vSaveP, const char* buffer){
	// Read Actual Values
	// [1] Turbine RPM,			*
	// [2] EGT,					* (minus)
	// [3] Pump Voltage,
	// [4] Turbine State,
	// [5] Throttle Position,	*
	// [6] Engine Current
	int len = 0;
	int idx = 0;

	for (idx = 0; idx< ENGINE_RAC_NUM; idx++){
		// Check CMD Echo (1)
		if (buffer[idx] == 0x31){
			// Check CMD Echo (,)
			if (buffer[idx+1] == 0x2C){
				// Check CMD Echo (R)
				if (buffer[idx+2] == 0x52){
					// Check CMD Echo (A)
					if (buffer[idx+3] == 0x41){
						// Check CMD Echo (C)
						if (buffer[idx+4] == 0x43){
							// Check CMD Echo (,)
							if (buffer[idx+5] == 0x2C){
								// Check CMD Echo (1)
								if (buffer[idx+6] == 0x31){
									// Check CMD Echo (CR)
									if (buffer[idx+7] == 0x0D){
										// Check HS (H)
										if (buffer[idx+10] == 0x48){
											// Check HS (O)
											if (buffer[idx+13] == 0x4F){
												get_RAC_Result(vSaveP, &buffer[idx]);
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void recvJetCatCMD_RTY(SaveParameters* vSaveP, const char* buffer){
	// Read Information
	// [1] Firmware Version, 
	// [2] Firmware Version,
	// [3] Last Run Time, 
	// [4] Total Operation Time, 
	// [5] Serial No, 
	// [6] Turbine Type
	int len = 0;
	int idx = 0;

	for (idx = 0; idx< ENGINE_RAC_NUM; idx++){
		// Check CMD Echo (1)
		if (buffer[idx] == 0x31){
			// Check CMD Echo (,)
			if (buffer[idx+1] == 0x2C){
				// Check CMD Echo (R)
				if (buffer[idx+2] == 0x52){
					// Check CMD Echo (T)
					if (buffer[idx+3] == 0x54){
						// Check CMD Echo (Y)
						if (buffer[idx+4] == 0x59){
							// Check CMD Echo (,)
							if (buffer[idx+5] == 0x2C){
								// Check CMD Echo (1)
								if (buffer[idx+6] == 0x31){
									// Check CMD Echo (CR)
									if (buffer[idx+7] == 0x0D){
										// Check HS (H)
										if (buffer[idx+10] == 0x48){
											// Check HS (O)
											if (buffer[idx+13] == 0x4F){
												get_RAC_Result(vSaveP, &buffer[idx]);
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void recvJetCatCMD_RA1(SaveParameters* vSaveP, const char* buffer){
	// Read
	// [1] Off Condition,
	// [2] Ambient Temp,		* (minus)
	// [3] Min Pump Voltage, 
	// [4] Max Pump Voltage
	int len = 0;
	int idx = 0;

	for (idx = 0; idx< ENGINE_RAC_NUM; idx++){
		// Check CMD Echo (1)
		if (buffer[idx] == 0x31){
			// Check CMD Echo (,)
			if (buffer[idx+1] == 0x2C){
				// Check CMD Echo (R)
				if (buffer[idx+2] == 0x52){
					// Check CMD Echo (A)
					if (buffer[idx+3] == 0x41){
						// Check CMD Echo (1)
						if (buffer[idx+4] == 0x31){
							// Check CMD Echo (,)
							if (buffer[idx+5] == 0x2C){
								// Check CMD Echo (1)
								if (buffer[idx+6] == 0x31){
									// Check CMD Echo (CR)
									if (buffer[idx+7] == 0x0D){
										// Check HS (H)
										if (buffer[idx+10] == 0x48){
											// Check HS (O)
											if (buffer[idx+13] == 0x4F){
												get_RA1_Result(vSaveP, &buffer[idx]);
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void recvJetCatCMD_RI1(SaveParameters* vSaveP, const char* buffer){
	// Read Statistic Info
	// [1] Run is OK, 
	// [2] Start Failed, 
	// [3] Total Operation Time, 
	// [4] Last Run Time, 
	// [5] System Time
	int len = 0;
	int idx = 0;

	for (idx = 0; idx< ENGINE_RA1_NUM; idx++){
		// Check CMD Echo (1)
		if (buffer[idx] == 0x31){
			// Check CMD Echo (,)
			if (buffer[idx+1] == 0x2C){
				// Check CMD Echo (R)
				if (buffer[idx+2] == 0x52){
					// Check CMD Echo (I)
					if (buffer[idx+3] == 0x49){
						// Check CMD Echo (1)
						if (buffer[idx+4] == 0x31){
							// Check CMD Echo (,)
							if (buffer[idx+5] == 0x2C){
								// Check CMD Echo (1)
								if (buffer[idx+6] == 0x31){
									// Check CMD Echo (CR)
									if (buffer[idx+7] == 0x0D){
										// Check HS (H)
										if (buffer[idx+10] == 0x48){
											// Check HS (O)
											if (buffer[idx+13] == 0x4F){
												get_RI1_Result(vSaveP, &buffer[idx]);
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void recvJetCatCMD_RFI(SaveParameters* vSaveP, const char* buffer){
	// Read Fuel Info
	// [1] Actual Fuel Flow,				*
	// [2] Rest Volume in tank, 
	// [3] set RPM, 
	// [4] Actual Battery Voltage, 
	// [5] Last Run Time, 
	// [6] Fuel Consumed on actual run		*
	int len = 0;
	int idx = 0;

	for (idx = 0; idx< ENGINE_RAC_NUM; idx++){
		// Check CMD Echo (1)
		if (buffer[idx] == 0x31){
			// Check CMD Echo (,)
			if (buffer[idx+1] == 0x2C){
				// Check CMD Echo (R)
				if (buffer[idx+2] == 0x52){
					// Check CMD Echo (F)
					if (buffer[idx+3] == 0x46){
						// Check CMD Echo (I)
						if (buffer[idx+4] == 0x49){
							// Check CMD Echo (,)
							if (buffer[idx+5] == 0x2C){
								// Check CMD Echo (1)
								if (buffer[idx+6] == 0x31){
									// Check CMD Echo (CR)
									if (buffer[idx+7] == 0x0D){
										// Check HS (H)
										if (buffer[idx+10] == 0x48){
											// Check HS (O)
											if (buffer[idx+13] == 0x4F){
 												get_RFI_Result(vSaveP, &buffer[idx]);
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void recvJetCatCMD_RRC(SaveParameters* vSaveP, const char* buffer){
	// Read RC Data Info
	// Throttle Position %
	// Throttle Pulse Length
	// AUX Position %
	// AUX Pulse Length
	// Fail Safe Counts
	// Fail Safe Time
	int len = 0;
	int idx = 0;

	for (idx = 0; idx< ENGINE_RRC_NUM; idx++){
		// Check CMD Echo (1)
		if (buffer[idx] == 0x31){
			// Check CMD Echo (,)
			if (buffer[idx+1] == 0x2C){
				// Check CMD Echo (R)
				if (buffer[idx+2] == 0x52){
					// Check CMD Echo (R)
					if (buffer[idx+3] == 0x52){
						// Check CMD Echo (C)
						if (buffer[idx+4] == 0x43){
							// Check CMD Echo (,)
							if (buffer[idx+5] == 0x2C){
								// Check CMD Echo (1)
								if (buffer[idx+6] == 0x31){
									// Check CMD Echo (CR)
									if (buffer[idx+7] == 0x0D){
										// Check HS (H)
										if (buffer[idx+10] == 0x48){
											// Check HS (O)
											if (buffer[idx+13] == 0x4F){
												get_RRC_Result(vSaveP, &buffer[idx]);
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void recvJetCatCMD_TCO(){
	// Turbine Control without Parameters

}
void recvJetCatCMD_tco(){
	// Turbine Control

}

void recvJetCatCMD_WRP(){
	// Set Turbine RPM without Parameters

}
void recvJetCatCMD_wrp(){
	// Set Turbine RPM

}

void recvJetCatCMD_WPE(){
	// Set Turbine Thrust in % without Parameters

}
void recvJetCatCMD_wpe(){
	// Set Turbine Thrust in %

}

void recvJetCatCMD_SVC(){
	// Smoker Valve Control

}
#ifndef _HEADER_H_
#define _HEADER_H_

#include "visa.h"		// Include file for VISA functions and constants
#include "nixnet.h"     // Include file for NI-XNET functions and constants
#include "Engine_Msg_Parser.h"
#include "JetCat.h"


#define	LOADCELL_READ_NUM	20
#define TEMPSENSOR_READ_NUM	8
#define ENGINE_READ_NUM		50
#define ENGINE_RAC_NUM		40
#define ENGINE_RTY_NUM		41
#define ENGINE_RA1_NUM		27
#define ENGINE_RI1_NUM		35
#define ENGINE_RFI_NUM		53
#define ENGINE_RRC_NUM		46
#define ENGINE_DATA_OFFSET	16

#define NUMFRAMES 2

typedef struct{
	/* LoadCell Data */
	float LoadCell01;
	float LoadCell02;

	/*Temperature Sensor Data */
	float Temp;

	/* Engine Data */
	float Turbine_RPM;
	float EGT;
	float Ambient_Temp;
	float FF;
	float FuelConsumed;
	float Throttle_Position;
} SaveParameters;

#define MAX_FILENAME_LENGTH	64

static ViSession		defaultRM; 
static ViSession		instr_ch1;
static ViSession		instr_ch2;
static ViSession		instr_ch3;
static ViSession		instr_ch4;
static ViUInt32			retCount;
static ViUInt32			writeCount;
static ViStatus			status;
static unsigned char	buffer1[20];
static unsigned char	buffer2[20];
static unsigned char	buffer3[8];
static unsigned char	buffer4[100];
static char				stringinput[512];

static SaveParameters	vSaveParameters		= {0};
static P250_RAC			vSaveRAC			= {0};
static P250_RFI			vSaveRFI			= {0};
static P250_RRC			vSaveRRC			= {0};
static P250_RTY			vSaveRTY			= {0};
static P250_RA1			vSaveRA1			= {0};
static P250_RI1			vSaveRI1			= {0};


//=============================================================================
// Static global variables .
//=============================================================================  
static nxSessionRef_t  g_SessionRef;
static nxSessionRef_t  g_SessionRef_Output;




/************************************************************************/
/*  Test Input                                                          */
/************************************************************************/
static unsigned char loadcellset1[20]	=	{0x02, '0', '1', '-', '1', '2', '3', '.', '4', '5', '6', 'L', 0x03, 'A', 'B', \
	'C', 'D', 'E', 'F', 'G'}; //-123.456
static unsigned char loadcellset2[20]	=	{'F', 'G', 0x02, '0', '1', '-', '1', '2', '3', '.', '4', '5', '6', 'L', 0x03, \
	'A', 'B', 'C', 'D', 'E'}; //-123.456

static unsigned char temptestset1[8]	=	{'5', '1', '0', '5', '.', '7', 0xA, '1'}; //105.7
static unsigned char temptestset2[8]	=	{'1', '0', '.', '7', '5', 0xA, '1', '2'}; //10.75
static unsigned char temptestset3[8]	=	{'-', '1', '0', '.', '7', 0xA, '1', '2'}; //-10.7
static unsigned char temptestset4[8]	=	{'1', '-', '1', '0', '.', '7', 0xA, '2'}; //-10.7

static unsigned char rac_testset1[45]	=	{'1', ',', 'R', 'A', 'C', ',', '1', 0x0D, '1', ',', 'H', 'S', ',', 'O', 'K', ',', \
	'1', '2', '3', '4', '5', '6', ',', '1', '2', '6', '8', ',', '1', '.', '3', '2', \
	'1', '9', ',', '3', '0', '.', '1', ',', '0', '.', '1', '2', 0x0D};
static unsigned char rac_testset2[46]	=	{'1', ',', 'R', 'A', 'C', ',', '1', 0x0D, '1', ',', 'H', 'S', ',', 'O', 'K', ',', \
	'1', '2', '3', '4', '5', '6', ',', '-', '2', '0', '.', '8', ',', '1', '.', '3', '2', \
	'1', '9', ',', '3', '0', '.', '1', ',', '0', '.', '1', '2', 0x0D};

static unsigned char ra1_testset1[27]	=	{'1', ',', 'R', 'A', '1', ',', '1', 0x0D, '1', ',', 'H', 'S', ',', 'O', 'K', ',', \
	'2', '2', ',', '-', '2', '0', ',', '7', ',', '7', 0x0D};
static unsigned char ra1_testset2[27]	=	{'1', ',', 'R', 'A', '1', ',', '1', 0x0D, '1', ',', 'H', 'S', ',', 'O', 'K', ',', \
	'2', '2', ',', '7', '0', ',', '7', ',', '7', 0x0D};

static unsigned char rfi_testset1[40]	=	{'1', ',', 'R', 'F', 'I', ',', '1', 0x0D, '1', ',', 'H', 'S', ',', 'O', 'K', ',', \
	'1', '0', '0', '0', '0', '0', ',', '0', ',', '0', ',', '0', ',', '0', ',',  '1', '0', '0', '0', '0', '0', '0', 0x0D};

static unsigned char rty_testset1[40]	=	{'1', ',', 'R', 'T', 'Y', ',', '1', 0x0D, '1', ',', 'H', 'S', ',', 'O', 'K', ',', \
};

//static unsigned char 


#endif
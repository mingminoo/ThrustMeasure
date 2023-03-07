#include <Windows.h>
#include <tchar.h>
#include <CommCtrl.h>
#include <commdlg.h>
#include <stdio.h>

#include "resource.h"
#include "Header.h"

#pragma comment(linker, \
	"\"/manifestdependency:type='Win32' "\
	"name='Microsoft.Windows.Common-Controls' "\
	"version='6.0.0.0' "\
	"processorArchitecture='*' "\
	"publicKeyToken='6595b64144ccf1df' "\
	"language='*'\"")

#pragma comment(lib, "ComCtl32.lib")

extern void rs232LoadCell_1_Recv(SaveParameters* vSaveP, const char* buffer);
extern void rs232LoadCell_2_Recv(SaveParameters* vSaveP, const char* buffer);
extern void rs232TempRecv(SaveParameters* vSaveP, const char* buffer);
extern void recvJetCatCMD_RAC(SaveParameters* vSaveP, const char* buffer);
extern void recvJetCatCMD_RTY(SaveParameters* vSaveP, const char* buffer);
extern void recvJetCatCMD_RA1(SaveParameters* vSaveP, const char* buffer);
extern void recvJetCatCMD_RI1(SaveParameters* vSaveP, const char* buffer);
extern void recvJetCatCMD_RFI(SaveParameters* vSaveP, const char* buffer);
extern void recvJetCatCMD_RRC(SaveParameters* vSaveP, const char* buffer);
extern void sendJetCatCMD_RAC();
extern void sendJetCatCMD_RTY();
extern void sendJetCatCMD_RA1();
extern void sendJetCatCMD_RI1();
extern void sendJetCatCMD_RFI();
extern void sendJetCatCMD_RRC();
extern void sendJetCatCMD_TCO();
extern void sendJetCatCMD_WRP();
extern void sendJetCatCMD_WPE();

extern void PrintStat(nxStatus_t l_status, char *source);
extern void PrintTimestamp(nxTimestamp_t * time);

extern void Parsing_Read_Actual_Value(Read_Actual_Value* var, nxFrameVar_t* l_MyFramePtr);
extern void Parsing_Read_Voltage_Current(Read_Voltage_Current* var, nxFrameVar_t* l_MyFramePtr);
extern void Parsing_Read_Fuel_Ambient(Read_Fuel_Ambient* var, nxFrameVar_t* l_MyFramePtr);
extern void Parsing_Read_Statistics(Read_Statistics* var, nxFrameVar_t* l_MyFramePtr);
extern void Parsing_Read_Last_Run_Info(Read_Last_Run_Info* var, nxFrameVar_t* l_MyFramePtr);
extern void Parsing_Read_System_Info(Read_System_Info* var, nxFrameVar_t* l_MyFramePtr);
extern void Engine_On_Off_Control(nxFrameVar_t * l_MyFramePtr, unsigned int value);
extern void Engine_Throttle_Control(nxFrameVar_t * l_MyFramePtr, float value);
extern void Engine_RPM_Control(nxFrameVar_t * l_MyFramePtr, unsigned int value);

extern void engineStart(HWND hDlg);
extern void engineStop(HWND hDlg);
extern void checkThrottle(HWND hDlg);

extern void logfileheadersave(FILE*	file);
extern void logfilesave(FILE*	file, nxFrameVar_t * l_MyFramePtr,Read_Actual_Value*	\
						vRAV, Read_Voltage_Current* vRVC, Read_Fuel_Ambient* vRFA, \
						Read_Statistics* vRS, Read_Last_Run_Info* vRLRI, Read_System_Info* vRSI,\
						SaveParameters* vSaveP);
extern void logfilesave2(FILE*	file, Read_Actual_Value*	\
						vRAV, Read_Voltage_Current* vRVC, Read_Fuel_Ambient* vRFA, \
						Read_Statistics* vRS, Read_Last_Run_Info* vRLRI, Read_System_Info* vRSI,\
						SaveParameters* vSaveP);

OPENFILENAME filename;
char tfilename[256] = {0};
char tfilenamendirectory[256] = {0};
char fileextension[10] = {0};

// Declare all variables for function 
int				i						= 0; 
int				j						= 0;
char			*l_pSelectedInterface	= "CAN1";
char			*l_pFrameArray			= NULL;
char			*l_pSelectedDatabase	= ":memory:";
char			*l_pSelectedCluster		= ""; 
u8				l_MyBuffer[4000];
nxFrameVar_t *	l_MyFramePtr;
int				l_NumBytes;
u32				l_Baud					= 250000;	//250Kbps
u32				l_MyNumberOfBytesForFrames	= 0;
u8				l_MyBufferOutput1[40];
u8				l_MyBufferOutput2[40];
u8				l_MyBufferOutput3[100];
nxFrameVar_t *	l_MyFramePtrOutput;

u8				OK_flag = 0;
//u8				RS232setup_flag = 0;
UINT32			counter = 0;

FILE*			file					= NULL;

// Error Status 
nxStatus_t g_Status = 0;

// String used for displaying timestamp
char	*g_timeString;

char	timeinfo[MAX_FILENAME_LENGTH]	= "";

static Read_Actual_Value		vRAV	= {0};
static Read_Voltage_Current		vRVC	= {0};
static Read_Fuel_Ambient		vRFA	= {0};
static Read_Statistics			vRS		= {0};
static Read_Last_Run_Info		vRLRI	= {0};
static Read_System_Info			vRSI	= {0};


void onCancel(HWND hDlg)
{
	SendMessage(hDlg, WM_CLOSE, 0, 0);
	memset(&filename, 0, sizeof(OPENFILENAME));
}

void onClose(HWND hDlg)
{
	/*
	if(MessageBox(hDlg, TEXT("Close the program?"), TEXT("Close"),
		MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		DestroyWindow(hDlg);
	}
	*/

	// Clear the xnet session Input
	g_Status = nxClear(g_SessionRef);
	g_Status = nxClear(g_SessionRef_Output);

	free(l_pFrameArray);

	if (file != NULL){
		fclose(file);
	}

	KillTimer(hDlg, 1);
	KillTimer(hDlg, 2);

	status = viClose (instr_ch1);
	status = viClose (instr_ch2);

	DestroyWindow(hDlg);
}

void onInit(HWND hDlg){
	// OK Button Disable when it Start
	EnableWindow(GetDlgItem(hDlg,IDOK), FALSE);
	EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_FILENAME), FALSE);
	EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1), FALSE);
	CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
}

void onStart(HWND hDlg)
{
	if (OK_flag == 0){
		//First
		SetTimer(hDlg, 1, 20, NULL);
		SetTimer(hDlg, 2, 20, NULL);
		GetDlgItemText(hDlg, IDC_EDIT_Log, filename.lpstrFile, 256);
		//wsprintf(tfilename,"%s",filename.lpstrFile);
		fopen_s(&file, tfilename, "wt");
		logfileheadersave(file);
		SetDlgItemText(hDlg, IDOK, "Log Stop!!!");
		OK_flag = 1;
	}
	else if(OK_flag == 1){
		//on going
		KillTimer(hDlg, 1);
		KillTimer(hDlg, 2);
		status = viClose (instr_ch1);
		status = viClose (instr_ch2);
		fclose(file);
		SetDlgItemText(hDlg, IDOK, "Log Start!!!");
		OK_flag = 0;
	}
	
}

void onSerialConnect(HWND hDlg){
	// Serial Comm Initialization
	status=viOpenDefaultRM (&defaultRM);
	if (status < VI_SUCCESS){
		exit (EXIT_FAILURE);
	}
	status = viOpen (defaultRM, "ASRL5::INSTR", VI_NULL, VI_NULL, &instr_ch1);
	if (status < VI_SUCCESS){
	}
	status = viOpen (defaultRM, "ASRL4::INSTR", VI_NULL, VI_NULL, &instr_ch2);
	if (status < VI_SUCCESS){
	
	}
	/* Set the timeout to 5 seconds (5000 milliseconds). */
	status = viSetAttribute (instr_ch1, VI_ATTR_TMO_VALUE, 500);
	status = viSetAttribute (instr_ch2, VI_ATTR_TMO_VALUE, 500);
	/* Set the baud rate to 9600. */
	status = viSetAttribute (instr_ch1, VI_ATTR_ASRL_BAUD, 9600);
	status = viSetAttribute (instr_ch2, VI_ATTR_ASRL_BAUD, 9600);
	/* Set the number of data bits contained in each frame (from 5 to 8). 
    * The data bits for  each frame are located in the low-order bits of
    * every byte stored in memory.    
    */
	status = viSetAttribute (instr_ch1, VI_ATTR_ASRL_DATA_BITS, 8);
	status = viSetAttribute (instr_ch2, VI_ATTR_ASRL_DATA_BITS, 8);
	/* Specify parity. Options: 
    * VI_ASRL_PAR_NONE  - No parity bit exists, 
    * VI_ASRL_PAR_ODD   - Odd parity should be used, 
    * VI_ASRL_PAR_EVEN  - Even parity should be used,
    * VI_ASRL_PAR_MARK  - Parity bit exists and is always 1,
    * VI_ASRL_PAR_SPACE - Parity bit exists and is always 0.
    */
	status = viSetAttribute (instr_ch1, VI_ATTR_ASRL_PARITY, VI_ASRL_PAR_NONE);
	status = viSetAttribute (instr_ch2, VI_ATTR_ASRL_PARITY, VI_ASRL_PAR_NONE);
	/* Specify stop bit. Options:
    * VI_ASRL_STOP_ONE   - 1 stop bit is used per frame,
    * VI_ASRL_STOP_ONE_5 - 1.5 stop bits are used per frame,
    * VI_ASRL_STOP_TWO   - 2 stop bits are used per frame.
    */
	status = viSetAttribute (instr_ch1, VI_ATTR_ASRL_STOP_BITS, VI_ASRL_STOP_ONE);
	status = viSetAttribute (instr_ch2, VI_ATTR_ASRL_STOP_BITS, VI_ASRL_STOP_ONE);
	/* Specify that the read operation should terminate when a termination 
     * character is received.
     */
	status = viSetAttribute (instr_ch1, VI_ATTR_TERMCHAR_EN, VI_TRUE); 
	status = viSetAttribute (instr_ch2, VI_ATTR_TERMCHAR_EN, VI_TRUE); 
	/* Set the termination character to 0xA */
	/************************************************************************/
	/* We should check this termination character to stop read function.    */
	/************************************************************************/
	/************************************************************************/
	/* Load Cell :		STX(02) ETX(03)                                     */
	/* Temperature :	LF(0A)                                              */
	/* Engine :         (Default)*/
	/************************************************************************/
	status = viSetAttribute (instr_ch1, VI_ATTR_TERMCHAR, 0x03);
	status = viSetAttribute (instr_ch1, VI_ATTR_TERMCHAR_EN, VI_TRUE);
	status = viSetAttribute (instr_ch2, VI_ATTR_TERMCHAR, 0x0A);
	status = viSetAttribute (instr_ch2, VI_ATTR_TERMCHAR_EN, VI_TRUE);
}

void onCANConnect(HWND hDlg){
	// CAN Comm Initialization

	// Create an xnet session for Signal Input 
	g_Status = nxCreateSession(l_pSelectedDatabase,l_pSelectedCluster,l_pFrameArray,l_pSelectedInterface,nxMode_FrameInStream,&g_SessionRef);
	if(nxSuccess != g_Status){
		PrintStat(g_Status,"nxCreateSession Input");  
	}
	g_Status = nxSetProperty(g_SessionRef,nxPropSession_IntfBaudRate,sizeof(u32),&l_Baud);


	// Create an xnet session for Signal Output 
	g_Status = nxCreateSession (l_pSelectedDatabase,l_pSelectedCluster,"", l_pSelectedInterface,nxMode_FrameOutStream,&g_SessionRef_Output);
	if(nxSuccess != g_Status){
		PrintStat(g_Status,"nxCreateSession");  
	}
	g_Status = nxSetProperty(g_SessionRef_Output,nxPropSession_IntfBaudRate, sizeof(u32),&l_Baud);
	if(nxSuccess != g_Status){
		PrintStat(g_Status,"nxSetProperty Output");  
	}
}

void onDecideFilename(HWND hDlg){
	filename.lStructSize = sizeof(OPENFILENAME);
	filename.hwndOwner = hDlg;
	filename.lpstrTitle = TEXT("Log 파일을 저장할 위치를 선택해 주세요.");
	filename.lpstrFileTitle = tfilename;			// 파일명
	filename.lpstrFile = tfilenamendirectory;		// 전체 경로와 파일명
	filename.lpstrFilter = TEXT("*.txt");
	filename.nMaxFile = MAX_PATH;
	filename.nMaxFileTitle = MAX_PATH;

	if (GetOpenFileName(&filename) != 0)
	{
		/*
		switch (filename.nFilterIndex)
		{
		case 1: wsprintf(fileextension, TEXT("%s"), TEXT("bmp"));
			break;
		case 2: wsprintf(fileextension, TEXT("%s"), TEXT("jpg"));
			break;
		}
		*/
		//wsprintf(tfilename,"%s 파일을 선택했습니다.",filename.lpstrFile);
		sprintf(tfilename,"%s.txt",filename.lpstrFile);

		//if (SetDlgItemText(hDlg, IDC_EDIT_Log, filename.lpstrFile)){
		if (SetDlgItemText(hDlg, IDC_EDIT_Log, tfilename)){
			EnableWindow(GetDlgItem(hDlg,IDOK), TRUE);
		}
		
		InvalidateRect(hDlg, NULL, TRUE);
	}

}

void onTimer232(){
	// Read RS232 Rx Data

	status = viRead (instr_ch1, buffer1, LOADCELL_READ_NUM, &retCount);
	if (status < VI_SUCCESS) {
	}
	else{
		rs232LoadCell_1_Recv(&vSaveParameters, &buffer1);
	}
	/*
	status = viRead (instr_ch2, buffer2, LOADCELL_READ_NUM, &retCount);
	if (status < VI_SUCCESS) 
	{
		rs232LoadCell_2_Recv(&vSaveParameters, &buffer2);
		//printf ("Error reading a response from the device.\n");
	}
	else
	{
		//printf ("\nData read: %*s\n", retCount, buffer2);
	}
	*/
}

void onTimerCAN(){
	
	//Read CAN RX Frames
	g_Status = nxReadFrame(g_SessionRef,l_MyBuffer,sizeof(l_MyBuffer),0,&l_NumBytes);
	if(nxSuccess != g_Status){
 
	}
	l_MyFramePtr = (nxFrameVar_t *)(l_MyBuffer);
	// myFramePtr iterates to each frame. When it increments
	// past the number of bytes returned, we reached the end.
	while ( ((u8 *)(l_MyFramePtr) - (u8 *)l_MyBuffer) < l_NumBytes){
		if (l_MyFramePtr->Identifier == 0x100){
			Parsing_Read_Actual_Value(&vRAV, l_MyFramePtr);
		}
		else if(l_MyFramePtr->Identifier == 0x101){
			Parsing_Read_Voltage_Current(&vRVC, l_MyFramePtr);
		}
		else if(l_MyFramePtr->Identifier == 0x102){
			Parsing_Read_Fuel_Ambient(&vRFA, l_MyFramePtr);
		}
		else if(l_MyFramePtr->Identifier == 0x103){
			Parsing_Read_Statistics(&vRS, l_MyFramePtr);
		}
		else if(l_MyFramePtr->Identifier == 0x104){
			Parsing_Read_Last_Run_Info(&vRLRI, l_MyFramePtr);
		}
		else if(l_MyFramePtr->Identifier == 0x105){
			Parsing_Read_System_Info(&vRSI, l_MyFramePtr);
		}
		else{
		}
		//log file save
		logfilesave(file, l_MyFramePtr, &vRAV,&vRVC,&vRFA,&vRS,&vRLRI,&vRSI, &vSaveParameters);
		// Go to next variable-payload frame.
		l_MyFramePtr = nxFrameIterate(l_MyFramePtr);
	}
}

void engineStart(HWND hDlg){
	//Output progress
	l_MyFramePtrOutput = (nxFrameVar_t *)(l_MyBufferOutput1);

	Engine_On_Off_Control(l_MyFramePtrOutput, 1);

	//Iterate the pointer to the next frame
	l_MyFramePtrOutput = nxFrameIterate(l_MyFramePtrOutput);

	//Get the actual size of the buffer used
	l_MyNumberOfBytesForFrames = ((u8 *)l_MyFramePtrOutput - (u8 *)l_MyBufferOutput1);

	g_Status = nxWriteFrame(g_SessionRef_Output,l_MyBufferOutput1,l_MyNumberOfBytesForFrames,10);

}

void engineStop(HWND hDlg){
	//Output progress
	l_MyFramePtrOutput = (nxFrameVar_t *)(l_MyBufferOutput2);

	Engine_On_Off_Control(l_MyFramePtrOutput, 0);

	//Iterate the pointer to the next frame
	l_MyFramePtrOutput = nxFrameIterate(l_MyFramePtrOutput);

	//Get the actual size of the buffer used
	l_MyNumberOfBytesForFrames = ((u8 *)l_MyFramePtrOutput - (u8 *)l_MyBufferOutput2);

	g_Status = nxWriteFrame(g_SessionRef_Output,l_MyBufferOutput2,l_MyNumberOfBytesForFrames,10);
}

void checkThrottle(HWND hDlg){
	int		i = 0;
	u8		count = 0;
	char	lpstring[10]= {0};
	float	fvalue = 0.0f;
	int		ivalue = 0;

	//GetDlgItemText(hDlg, IDC_EDIT1, lpstring, NULL);

	//count = strlen(lpstring);

	//for (i=0; i<count;i++){
	//	if ((lpstring[i] <= 48) && (lpstring[i] <= 58)){
	//		MessageBox(hDlg, "Out of Range", NULL, MB_OK);
	//		lpstring[i] = NULL;
	//		break;
	//	}
	//}

	//Output progress
	l_MyFramePtrOutput = (nxFrameVar_t *)(l_MyBufferOutput3);

	if (IsDlgButtonChecked(hDlg, IDC_RADIO1)){
		//MessageBox(hDlg, "Out of Range", NULL, MB_OK);
		GetDlgItemText(hDlg, IDC_EDIT1, lpstring, 10);

		fvalue = atof(lpstring);

		Engine_Throttle_Control(l_MyFramePtrOutput, fvalue);
	}
	else if (IsDlgButtonChecked(hDlg, IDC_RADIO2)){
		//MessageBox(hDlg, "Out of Range", NULL, MB_OK);
		GetDlgItemText(hDlg, IDC_EDIT1, lpstring, 10);

		ivalue = atoi(lpstring);

		Engine_RPM_Control(l_MyFramePtrOutput, ivalue);
	}
	else{
		//Do Nothing
	}
	//Iterate the pointer to the next frame
	l_MyFramePtrOutput = nxFrameIterate(l_MyFramePtrOutput);

	//Get the actual size of the buffer used
	l_MyNumberOfBytesForFrames = ((u8 *)l_MyFramePtrOutput - (u8 *)l_MyBufferOutput3);

	g_Status = nxWriteFrame(g_SessionRef_Output,l_MyBufferOutput3,l_MyNumberOfBytesForFrames,10);
}

void onPaint(HWND hDlg){
	float area;
	char mtext[100] = {0};
	//sprintf(mtext, "%f", area);

	//Engine Data
	SetDlgItemInt(hDlg, IDC_EDIT4, vRAV.SetRPM, FALSE);
	SetDlgItemInt(hDlg, IDC_EDIT11, vRAV.RealRPM, FALSE);
	//SetDlgItemInt(hDlg, IDC_EDIT5, vRAV.EGT, TRUE);				//float
	sprintf(mtext, "%f", vRAV.EGT);
	SetDlgItemText(hDlg, IDC_EDIT5, mtext);
	//SetDlgItemInt(hDlg, IDC_EDIT12, vRAV.Pump_Voltage, FALSE);	//float
	memset(mtext, 0x00, sizeof(mtext));
	sprintf(mtext, "%f", vRAV.Pump_Voltage);
	SetDlgItemText(hDlg, IDC_EDIT12, mtext);
	//SetDlgItemInt(hDlg, IDC_EDIT13, vRAV.State, FALSE);
	memset(mtext, 0x00, sizeof(mtext));
	if (OFF==vRAV.State){
		sprintf(mtext, "OFF");
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1), TRUE);
	}
	else if (WAIT_for_RPM==vRAV.State){
		sprintf(mtext, "WAIT_for_RPM");
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1), FALSE);
	}
	else if (IGNITE==vRAV.State){
		sprintf(mtext, "IGNITE");
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1), FALSE);
	}
	else if (ACCELERATE==vRAV.State){
		sprintf(mtext, "ACCELERATE");
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1), FALSE);
	}
	else if (STABILIZE==vRAV.State){
		sprintf(mtext, "STABILIZE");
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1), FALSE);
	}
	else if (LEARN_LO==vRAV.State){
		sprintf(mtext, "LEARN_LO");
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1), FALSE);
	}
	else if (OFF_COOLING==vRAV.State){
		sprintf(mtext, "OFF_COOLING");
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1), TRUE);
	}
	else if (SLOW_DOWN==vRAV.State){
		sprintf(mtext, "SLOW_DOWN");
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1), FALSE);
	}
	else if (AUTO_OFF==vRAV.State){
		sprintf(mtext, "AUTO_OFF");
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1), FALSE);
	}
	else if (RUN==vRAV.State){
		sprintf(mtext, "RUN");
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1), FALSE);
	}
	else if (ACCELATION_DELAY==vRAV.State){
		sprintf(mtext, "ACCELATION_DELAY");
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1), FALSE);
	}
	else if (SPEED_REG==vRAV.State){
		sprintf(mtext, "SPEED_REG");
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1), FALSE);
	}
	else if (TWO_SHAFT_REGULATE==vRAV.State){
		sprintf(mtext, "TWO_SHAFT_REGULATE");
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON1), FALSE);
	}
	SetDlgItemText(hDlg, IDC_EDIT13, mtext);

	//SetDlgItemInt(hDlg, IDC_EDIT16, vRVC.Battery_Voltage, FALSE);	//float
	memset(mtext, 0x00, sizeof(mtext));
	sprintf(mtext, "%f", vRVC.Battery_Voltage);
	SetDlgItemText(hDlg, IDC_EDIT16, mtext);
	//SetDlgItemInt(hDlg, IDC_EDIT10, vRVC.Engine_Current, FALSE);	//float
	memset(mtext, 0x00, sizeof(mtext));
	sprintf(mtext, "%f", vRVC.Engine_Current);
	SetDlgItemText(hDlg, IDC_EDIT10, mtext);
	//SetDlgItemInt(hDlg, IDC_EDIT18, vRVC.Generator_Voltage, FALSE);	//float
	memset(mtext, 0x00, sizeof(mtext));
	sprintf(mtext, "%f", vRVC.Generator_Voltage);
	SetDlgItemText(hDlg, IDC_EDIT18, mtext);
	//SetDlgItemInt(hDlg, IDC_EDIT15, vRVC.Generator_Current, FALSE);	//float
	memset(mtext, 0x00, sizeof(mtext));
	sprintf(mtext, "%f", vRVC.Generator_Current);
	SetDlgItemText(hDlg, IDC_EDIT15, mtext);
	SetDlgItemInt(hDlg, IDC_EDIT20, vRVC.Flag, FALSE);
	
	SetDlgItemInt(hDlg, IDC_EDIT7, vRFA.Fuel_Flow, FALSE);
	SetDlgItemInt(hDlg, IDC_EDIT6, vRFA.Fuel_Consumed, FALSE);
	SetDlgItemInt(hDlg, IDC_EDIT14, vRFA.Air_Pressure, FALSE);
	SetDlgItemInt(hDlg, IDC_EDIT8, vRFA.Ambient_Temperature, FALSE);

	SetDlgItemInt(hDlg, IDC_EDIT9, vRS.Runs_OK, FALSE);
	SetDlgItemInt(hDlg, IDC_EDIT17, vRS.Runs_Aborted, FALSE);
	SetDlgItemInt(hDlg, IDC_EDIT19, vRS.Total_Run_Time, FALSE);

	SetDlgItemInt(hDlg, IDC_EDIT22, vRLRI.Last_Run_Time, FALSE);
	SetDlgItemInt(hDlg, IDC_EDIT21, vRLRI.Last_Off_RPM, FALSE);
	//SetDlgItemInt(hDlg, IDC_EDIT23, vRLRI.Last_Off_EGT, TRUE);	//float
	memset(mtext, 0x00, sizeof(mtext));
	sprintf(mtext, "%f", vRLRI.Last_Off_EGT);
	SetDlgItemText(hDlg, IDC_EDIT23, mtext);
	SetDlgItemInt(hDlg, IDC_EDIT24, vRLRI.Last_Off_PumpVoltage, FALSE);
	SetDlgItemInt(hDlg, IDC_EDIT26, vRLRI.Last_Off_Condition, FALSE);

	SetDlgItemInt(hDlg, IDC_EDIT30, vRSI.Serial_Number, FALSE);
	SetDlgItemInt(hDlg, IDC_EDIT29, vRSI.Firmware_Version, FALSE);
	SetDlgItemInt(hDlg, IDC_EDIT28, vRSI.Engine_Type, FALSE);
	SetDlgItemInt(hDlg, IDC_EDIT27, vRSI.Engine_Sub_Type, FALSE);
	SetDlgItemInt(hDlg, IDC_EDIT25, vRSI.OpMode, FALSE);
	
	//Load Cell Data
	//SetDlgItemInt(hDlg, IDC_EDIT_LC, vSaveParameters.LoadCell01, FALSE);
	memset(mtext, 0x00, sizeof(mtext));
	sprintf(mtext, "%f", vSaveParameters.LoadCell01);
	SetDlgItemText(hDlg, IDC_EDIT_LC, mtext);

	//Temperature Data
	//SetDlgItemInt(hDlg, IDC_EDIT3, vSaveParameters.Temp, TRUE);
	memset(mtext, 0x00, sizeof(mtext));
	sprintf(mtext, "%f", vSaveParameters.Temp);
	SetDlgItemText(hDlg, IDC_EDIT3, mtext);

	//Counter
	SetDlgItemInt(hDlg, IDC_EDIT_Counter, counter, FALSE);
}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg) /* more compact, each "case" through a single line, easier on the eyes */
	{
	case WM_INITDIALOG: onInit(hDlg); return TRUE; 		
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDCANCEL: onCancel(hDlg); return TRUE; /* call subroutine */
		case IDOK: onStart(hDlg); return TRUE;
		case IDC_BUTTON_FILENAME: onDecideFilename(hDlg); return TRUE;
		case IDC_BUTTON_CO: 
			onSerialConnect(hDlg);
			onCANConnect(hDlg); 
			EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_FILENAME), TRUE);
			return TRUE;
		case IDC_BUTTON1:	engineStart(hDlg);		return TRUE;
		case IDC_BUTTON2:	engineStop(hDlg);		return TRUE;
		case IDC_BUTTON3:	checkThrottle(hDlg);	return TRUE;
		}
		break;
	case WM_TIMER:
		switch(wParam)
		{
		case 1: 
			onTimerCAN(); 
			onTimer232();
			//logfilesave2(file, &vRAV,&vRVC,&vRFA,&vRS,&vRLRI,&vRSI, &vSaveParameters);
			return TRUE;
		case 2: onPaint(hDlg); return TRUE;
		}
		break;
	//case WM_PAINT:	checkThrottle(hDlg);	return TRUE;
	case WM_CLOSE:   onClose(hDlg); return TRUE; /* call subroutine */
	case WM_DESTROY: PostQuitMessage(0); return TRUE;
	}

	return FALSE;
}

//=============================================================================  
// Display Error Function 
//============================================================================= 
void PrintStat(nxStatus_t l_status, char *source) 
{
	char statusString[1024];

	if (nxSuccess != l_status) 
	{  
		nxStatusToString (l_status,sizeof(statusString),statusString);
		//printf("NI-XNET Status: %s",statusString);
		nxClear(g_SessionRef);
		//printf("\nPress any key to quit\n");
		//_getch();
		exit(1);
	}
}

//=============================================================================  
// Print a proper timestamp  
//============================================================================= 
void PrintTimestamp(nxTimestamp_t * time) 
{
   SYSTEMTIME              stime;
   FILETIME                localftime;
   //char *l_timeString;

  /* This Win32 function converts from UTC (international) time
  to the local time zone.  The NI-XNET card keeps time in UTC
  format (refer to the description of nxTimestamp_t in
  the NI-XNET reference for Read functions).  */
   FileTimeToLocalFileTime((FILETIME *)(time), &localftime);

  /* This Win32 function converts an absolute time (FILETIME) 
  into SYSTEMTIME, a structure with fields for year, month, day,
  and so on.  */
   FileTimeToSystemTime(&localftime, &stime);
   g_timeString = (char *) malloc (256); 
   
   sprintf(g_timeString,"%02d/%02d/%04d %02d:%02d:%02d.%03d",stime.wMonth, 
      stime.wDay, stime.wYear,stime.wHour, stime.wMinute, stime.wSecond, 
      stime.wMilliseconds);
}


//=============================================================================  
// Log File Header Save
//============================================================================= 
void logfileheadersave(FILE*	file){
	//log file header
	fprintf(file, "[Time]\t");
	//fprintf(file, "[CAN Msg ID]\t ");
	fprintf(file, "Load Cell\t Temper.\t ");
	fprintf(file, "Set_RPM\t Real_RPM\t EGT\t Pump_Voltage\t State\t ");										//0x100
	fprintf(file, "Battery_Voltage\t Engine_Current\t Generator_Voltage\t Generator_Current\t Flag\t ");		//0x101
	fprintf(file, "Fuel_Flow\t Fuel_Consumed\t Air_Pressure\t Ambient_Temp.\t ");								//0x102
	fprintf(file, "Run_OK\t Run_Aborted\t Total_Run_Time\t ");													//0x103
	fprintf(file, "Last_Run_Time\t Last_Off_RPM\t Last_Off_EGT\t Last_Off_PumpVoltage\t Last_Off_Condition\t ");//0x104
	fprintf(file, "Serial_Number\t Firmware_Version\t Engine_Type\t Engine_Sub_Type\t OpMode\t ");				//0x105
	fprintf(file, "\n\n");
}

//=============================================================================  
// Log File Save
//============================================================================= 
void logfilesave(FILE*	file, nxFrameVar_t * l_MyFramePtr, Read_Actual_Value* vRAV, Read_Voltage_Current* vRVC, Read_Fuel_Ambient* vRFA, Read_Statistics* vRS, Read_Last_Run_Info* vRLRI, Read_System_Info* vRSI, SaveParameters* vSaveP){
	PrintTimestamp(&l_MyFramePtr->Timestamp);
	fprintf(file, "%s\t", g_timeString);
	//fprintf(file, "%x\t ",l_MyFramePtr->Identifier);
	fprintf(file, "%f\t %f\t ",vSaveP->LoadCell01, vSaveP->Temp);
	fprintf(file, "%d\t %d\t %f\t %f\t %X\t ",vRAV->SetRPM, vRAV->RealRPM, vRAV->EGT, vRAV->Pump_Voltage, vRAV->State);													//0x100
	fprintf(file, "%f\t %f\t %f\t %f\t %X\t ",vRVC->Battery_Voltage, vRVC->Engine_Current, vRVC->Generator_Voltage, vRVC->Generator_Current, vRVC->Flag);				//0x101
	fprintf(file, "%d\t %d\t %f\t %f\t ",vRFA->Fuel_Flow, vRFA->Fuel_Consumed, vRFA->Air_Pressure, vRFA->Ambient_Temperature);											//0x102
	fprintf(file, "%d\t %d\t %d\t ",vRS->Runs_OK, vRS->Runs_Aborted, vRS->Total_Run_Time);																				//0x103
	fprintf(file, "%d\t %d\t %f\t %f\t %X\t ",vRLRI->Last_Run_Time, vRLRI->Last_Off_RPM, vRLRI->Last_Off_EGT, vRLRI->Last_Off_PumpVoltage, vRLRI->Last_Off_Condition);	//0x104
	fprintf(file, "%X\t %X\t %X\t %X\t %X\t ",vRSI->Serial_Number, vRSI->Firmware_Version, vRSI->Engine_Type, vRSI->Engine_Sub_Type, vRSI->OpMode);						//0x105
	fprintf(file, "\n");
}

//=============================================================================  
// Log File Save
//============================================================================= 
void logfilesave2(FILE*	file, Read_Actual_Value* vRAV, Read_Voltage_Current* vRVC, Read_Fuel_Ambient* vRFA, Read_Statistics* vRS, Read_Last_Run_Info* vRLRI, Read_System_Info* vRSI, SaveParameters* vSaveP){
	//PrintTimestamp(&l_MyFramePtr->Timestamp);
	//fprintf(file, "%s\t", g_timeString);
	//fprintf(file, "%x\t ",l_MyFramePtr->Identifier);
	fprintf(file, "%f\t %f\t ",vSaveP->LoadCell01, vSaveP->Temp);
	fprintf(file, "%d\t %d\t %f\t %f\t %X\t ",vRAV->SetRPM, vRAV->RealRPM, vRAV->EGT, vRAV->Pump_Voltage, vRAV->State);													//0x100
	fprintf(file, "%f\t %f\t %f\t %f\t %X\t ",vRVC->Battery_Voltage, vRVC->Engine_Current, vRVC->Generator_Voltage, vRVC->Generator_Current, vRVC->Flag);				//0x101
	fprintf(file, "%d\t %d\t %f\t %f\t ",vRFA->Fuel_Flow, vRFA->Fuel_Consumed, vRFA->Air_Pressure, vRFA->Ambient_Temperature);											//0x102
	fprintf(file, "%d\t %d\t %d\t ",vRS->Runs_OK, vRS->Runs_Aborted, vRS->Total_Run_Time);																				//0x103
	fprintf(file, "%d\t %d\t %f\t %f\t %X\t ",vRLRI->Last_Run_Time, vRLRI->Last_Off_RPM, vRLRI->Last_Off_EGT, vRLRI->Last_Off_PumpVoltage, vRLRI->Last_Off_Condition);	//0x104
	fprintf(file, "%X\t %X\t %X\t %X\t %X\t ",vRSI->Serial_Number, vRSI->Firmware_Version, vRSI->Engine_Type, vRSI->Engine_Sub_Type, vRSI->OpMode);						//0x105
	fprintf(file, "\n");
}

int _tWinMain(HINSTANCE hInst, HINSTANCE h0, LPCTSTR lpCmdLine, int nCmdShow){
	HWND hDlg;
	BOOL ret;
	MSG msg;


	hDlg = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc, 0);

	ShowWindow(hDlg, nCmdShow);

	//Edit Control에 뿌리기
	//SetDlgItemText(hDlg, IDC_EDIT3, (LPCSTR)"기본 입력 문장");

	while((ret = GetMessage(&msg, 0, 0, 0)) != 0) {
		if(ret == -1) /* error found */
			return -1;

		//Dialog Message Capture
		if(!IsDialogMessage(hDlg, &msg)) {
			TranslateMessage(&msg); /* translate virtual-key messages */
			DispatchMessage(&msg); /* send it to dialog procedure */
		}
		counter++;
	}
	return 0;
}
#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <windows.h>
#include <wincon.h>
#include <string>
#include <iostream>
#include <time.h>
#include "PDI.h"
#include "mex.h"

using namespace std;
typedef basic_string<TCHAR> tstring;
#if defined(UNICODE) || defined(_UNICODE)
#define tcout std::wcout
#else
#define tcout std::cout
#endif

CPDIfastrak	g_pdiDev;
CPDImdat    g_pdiMDat;
CPDIser		g_pdiSer;
DWORD		g_dwFrameSize;
BOOL		g_bCnxReady;
DWORD		g_dwStationMap;
time_t  g_beginTime = 0, g_endTime = 0;
//#define BUFFER_SIZE 0x1FA400   // 30 seconds of xyzaer+fc 8 sensors at 240 hz 
#define BUFFER_SIZE 0x1DA9C000   // 2 hour of xyzaer+fc 8 sensors at 240 hz 
//BYTE	g_pMotionBuf[0x0800];  // 2K worth of data.  == 73 frames of XYZAER
BYTE	g_pMotionBuf[BUFFER_SIZE] = {0};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

typedef enum
{
	CHOICE_CONT
	, CHOICE_SINGLE
	, CHOICE_QUIT

	, CHOICE_NONE = -1
} eMenuChoice;
#define ESC	0x1b

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
BOOL		Initialize			( VOID );
BOOL		Connect				( VOID );
VOID		Disconnect			( VOID );
BOOL		SetupDevice			( VOID );
VOID		UpdateStationMap	( VOID );
VOID		AddMsg				( tstring & );
VOID		AddResultMsg		( LPCTSTR );
VOID		ShowMenu			( VOID );	
eMenuChoice Prompt				( VOID );
BOOL		StartCont			( VOID );
BOOL		StopCont			( VOID );	
VOID		DisplayCont			( VOID );
VOID		DisplaySingle		( VOID );
VOID		DisplayFrame		( PBYTE, DWORD );
VOID		GetBeginTime(VOID);
VOID		GetEndTime(VOID);
VOID		ResetData(VOID);
VOID		GetData(int & nrhs, mxArray *plhs[]);
VOID		GetAFrame(int & nrhs, mxArray *plhs[]);
VOID		GetTime(time_t &usTime);

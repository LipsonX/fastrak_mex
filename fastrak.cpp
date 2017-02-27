#include "fastrack.h"

BOOL Initialize( VOID )
{
	BOOL	bRet = TRUE;

	//g_pdiDev.Trace(TRUE, 7);
	g_pdiMDat.Empty();
	g_pdiMDat.Append( PDI_MODATA_POS );
	g_pdiMDat.Append( PDI_MODATA_ORI );
	g_dwFrameSize = 8+12+12;

	g_bCnxReady = FALSE;
	g_dwStationMap = 0;

	return bRet;
}

BOOL Connect( VOID )
{
	//tstring msg;
	if (!(g_pdiDev.CnxReady()))
	{
		g_pdiDev.SetSerialIF( &g_pdiSer );

		ePiCommType eType = g_pdiDev.DiscoverCnx();
		switch (eType)
		{
		case PI_CNX_USB:
			//msg = _T("USB Connection: ") + tstring( g_pdiDev.GetLastResultStr() ) + _T("\r\n");
			break;
		case PI_CNX_SERIAL:
			//msg = _T("Serial Connection: ") + tstring( g_pdiDev.GetLastResultStr() ) + _T("\r\n");
			break;
		default:
			//msg = _T("DiscoverCnx result: ") + tstring( g_pdiDev.GetLastResultStr() ) + _T("\r\n");
			break;
		}
		g_bCnxReady = g_pdiDev.CnxReady();
		//AddMsg( msg );

	}
	else
	{
		//msg = _T("Already connected\r\n");
		g_bCnxReady = TRUE;
		//AddMsg( msg );
	}

	return g_bCnxReady;
}

VOID Disconnect()
{
	//tstring msg;
	if (!(g_pdiDev.CnxReady()))
	{
		//msg = _T("Already disconnected\r\n");
	}
	else
	{
		g_pdiDev.Disconnect();
		//msg = _T("Disconnect result: ") + tstring(g_pdiDev.GetLastResultStr()) + _T("\r\n");
	}
	//AddMsg( msg );
}

BOOL SetupDevice( VOID )
{
	g_pdiDev.SetPnoBuffer( g_pMotionBuf, BUFFER_SIZE );
	//AddResultMsg(_T("SetPnoBuffer") );

	g_pdiDev.StartPipeExport();
	//AddResultMsg(_T("StartPipeExport") );

	tstring msg;

	g_pdiDev.SetSDataList( -1, g_pdiMDat );
	//AddResultMsg(_T("SetSDataList") );

	TCHAR	szBE[200];
	CPDIbiterrFT cBE;
	g_pdiDev.GetBITErr( cBE );
	//AddResultMsg(_T("GetBITErr"));

	cBE.Parse( szBE, 200 );

	//AddMsg(tstring(szBE));

	UpdateStationMap();

	return TRUE;
}

VOID UpdateStationMap( VOID )
{
	g_pdiDev.GetStationMap( g_dwStationMap );
	//AddResultMsg(_T("GetStationMap") );

	//TCHAR szMsg[100];
	//_stprintf_s(szMsg, _countof(szMsg), _T("ActiveStationMap: %#x\r\n"), g_dwStationMap );

	//AddMsg( tstring(szMsg) );
}

VOID AddMsg( tstring & csMsg )
{
	tcout << csMsg ;
}

VOID AddResultMsg( LPCTSTR szCmd )
{
	tstring msg;

	//msg.Format("%s result: %s\r\n", szCmd, m_pdiDev.GetLastResultStr() );
	msg = tstring(szCmd) + _T(" result: ") + tstring( g_pdiDev.GetLastResultStr() ) + _T("\r\n");
	AddMsg( msg );
}

VOID ShowMenu( VOID )
{
	return;
	tcout << _T("\n\nPlease enter select a data command option: \n\n");
	tcout << _T("C or c:  Continuous Motion Data Display\n");
	tcout << _T("P or p:  Single Motion Data Frame Display\n");
	tcout << endl;
	tcout << _T("ESC:     Quit\n");
}

eMenuChoice Prompt( VOID )
{
	eMenuChoice eRet = CHOICE_NONE;
	INT			ch;

	do
	{
		tcout << _T("\nCc/Pp/ESC>> ");
		ch = _getche();
		ch = toupper( ch );

		switch (ch)
		{
		case 'C':
			eRet = CHOICE_CONT;
			break;
		case 'P':
			eRet = CHOICE_SINGLE;
			break;
		case ESC: // ESC
			eRet = CHOICE_QUIT;
			break;
		default:
			break;
		}
	} while (eRet == CHOICE_NONE);

	return eRet;
}

BOOL StartCont( VOID )
{
	BOOL bRet = FALSE;

	GetBeginTime();
	if (!(g_pdiDev.StartContPno(0)))
	{
	}
	else
	{
		bRet = TRUE;
		Sleep(1000);  
	}
	AddResultMsg(_T("\nStartContPno") );

	return bRet;
}

BOOL StopCont( VOID )
{
	BOOL bRet = FALSE;

	if (!(g_pdiDev.StopContPno()))
	{
	}
	else
	{
		bRet = TRUE;
		Sleep(1000);
	}
	AddResultMsg(_T("StopContPno") );

	return bRet;
}

VOID DisplayCont( VOID )
{
	BOOL bExit = FALSE;

	PBYTE pBuf;
	DWORD dwSize;
	PBYTE pLastBuf = 0;
	DWORD dwLastSize = 0;

	tcout << _T("\nPress any key to start continuous display\r\n");
	tcout << _T("\nPress ESC to stop.\r\n");
	tcout << _T("\nReady? ");
	_getche();
	tcout << endl;

	do
	{

		if (!(g_pdiDev.LastPnoPtr(pBuf, dwSize)))
		{
			AddResultMsg(_T("LastPnoPtr") );
			bExit = TRUE;
		}
		else if ((pBuf == 0) || (dwSize == 0))
		{
		}
		else if (pLastBuf && (pBuf > pLastBuf))
		{
			DisplayFrame( pLastBuf+dwLastSize, (DWORD)(dwSize+(pBuf-pLastBuf-dwLastSize)) );
			pLastBuf = pBuf;
			dwLastSize = dwSize;
		}
		else if (pBuf != pLastBuf) // it wrapped in buffer
		{
			pLastBuf = pBuf;
			dwLastSize = dwSize;
			DisplayFrame( pBuf, dwSize );
		}
		if ( _kbhit() && ( _getch() == ESC ) ) 
		{
			bExit = TRUE;
		}

	} while (!bExit);

}

VOID DisplaySingle( VOID )
{
	BOOL bExit = FALSE;

	PBYTE pBuf;
	DWORD dwSize;

	cout << endl;

	if (!(g_pdiDev.ReadSinglePnoBuf(pBuf, dwSize)))
	{
		AddResultMsg(_T("ReadSinglePno") );
		bExit = TRUE;
	}
	else if ((pBuf == 0) || (dwSize == 0))
	{
	}
	else 
	{
		DisplayFrame( pBuf, dwSize );
	}
}

VOID DisplayFrame( PBYTE pBuf, DWORD dwSize )
{
	TCHAR	szFrame[200];
	DWORD		i = 0;

	while ( i<dwSize)
	{
		FT_BINHDR *pHdr = (FT_BINHDR*)(&pBuf[i]);
		CHAR cSensor = pHdr->station;
		SHORT shSize = 6*(sizeof(FLOAT));

		// skip rest of header
		i += sizeof(FT_BINHDR);

		PFLOAT pPno = (PFLOAT)(&pBuf[i]);

		_stprintf_s( szFrame, _countof(szFrame), _T("%2c   %+011.6f %+011.6f %+011.6f   %+011.6f %+011.6f %+011.6f\r\n"), 
			cSensor, pPno[0], pPno[1], pPno[2], pPno[3], pPno[4], pPno[5] );

		AddMsg( tstring( szFrame ) );

		i += shSize;
	}
}

VOID ResetData() {
	GetBeginTime();
	g_pdiDev.ResetPnoPtr();
}

VOID GetData ( int & nrhs, mxArray *plhs[] ) {
	GetEndTime();;

	PBYTE pBuf = g_pMotionBuf;
	DWORD dwSize = 0;
	if (!(g_pdiDev.LastPnoPtr(pBuf, dwSize)))
		AddResultMsg(_T("LastPnoPtr") );
	else if ((pBuf == g_pMotionBuf) || (dwSize == 0))
		AddResultMsg(_T("Read ERROR") );

	int frameCount = 0;
	int bufSizeCount = static_cast<int>(pBuf - g_pMotionBuf);
	frameCount = bufSizeCount > 0 ? bufSizeCount / dwSize : 0;

	double *p1, *p2;
	plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
	plhs[1] = mxCreateDoubleMatrix(1, 1, mxREAL);
	plhs[2] = mxCreateDoubleMatrix(frameCount, 6, mxREAL);
	p1 = mxGetPr( plhs[0]);
	p2 = mxGetPr( plhs[1]);
	double *resultP = mxGetPr(plhs[2]);

	*p1 = static_cast<double>(g_beginTime);
	*p2 = static_cast<double>(g_endTime);
	PBYTE itrP = g_pMotionBuf;
	for (int i = 0; i < frameCount; i += 1) {
		itrP = i * dwSize + g_pMotionBuf;
		FT_BINHDR *pHdr = (FT_BINHDR*)(itrP);
		itrP += sizeof(FT_BINHDR);
		PFLOAT pPno = (PFLOAT)(itrP);
		for (int j = 0; j < 6; j++) {
			resultP[j * frameCount + i] = pPno[j];
		}
	}
}

VOID GetAFrame(int & nrhs, mxArray *plhs[])
{
	PBYTE pBuf = g_pMotionBuf;
	DWORD dwSize = 0;
	if (!(g_pdiDev.LastPnoPtr(pBuf, dwSize)))
		AddResultMsg(_T("LastPnoPtr"));
	else if ((pBuf == g_pMotionBuf) || (dwSize == 0))
		AddResultMsg(_T("Read ERROR"));

	plhs[0] = mxCreateDoubleMatrix(1, 6, mxREAL);
	double *resultP = mxGetPr(plhs[2]);

	FT_BINHDR *pHdr = (FT_BINHDR*)(pBuf);
	pBuf += sizeof(FT_BINHDR);
	PFLOAT pPno = (PFLOAT)(pBuf);
	for (int j = 0; j < 6; j++) {
		resultP[j] = pPno[j];
	}
}

static const unsigned __int64 epoch = ((unsigned __int64)116444736000000000ULL);
int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
	FILETIME    file_time;
	SYSTEMTIME  system_time;
	ULARGE_INTEGER ularge;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	ularge.LowPart = file_time.dwLowDateTime;
	ularge.HighPart = file_time.dwHighDateTime;

	tp->tv_sec = (long)((ularge.QuadPart - epoch) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);

	return 0;
}

VOID GetBeginTime(VOID)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	g_beginTime = static_cast<INT64>(tv.tv_sec) * 1000 + tv.tv_usec / 1000;
}

VOID GetEndTime(VOID)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	g_endTime = static_cast<INT64>(tv.tv_sec) * 1000 + tv.tv_usec / 1000;
}

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
    /* Check for proper number of input and output arguments */
    if (nrhs != 1) {
        mexErrMsgIdAndTxt( "MATLAB:fastrak:invalidNumInputs",
                           "One input argument required.");
    }
    
    /* Check to be sure input is of type char */
    if (!(mxIsChar(prhs[0]))){
        mexErrMsgIdAndTxt( "MATLAB:fastrak:inputNotString",
                           "Input must be of type string.\n.");
    }

    char *str;
    str = mxArrayToString(prhs[0]);

    if (!strcmp(str, "initAll")) {
        mexPrintf("init\n");
        Initialize();
        Connect();
        SetupDevice();
    }
    if (!strcmp(str, "init")) {
        mexPrintf("init\n");
        Initialize();
    }
    if (!strcmp(str, "connect")) {
        mexPrintf("connect\n");
        Connect();
    }
    if (!strcmp(str, "config")) {
        mexPrintf("config\n");
        SetupDevice();
    }
    if (!strcmp(str, "start")) {
        mexPrintf("start\n");
        StartCont();
    }
    if (!strcmp(str, "reset")) {
        mexPrintf("reset data\n");
        ResetData();
    }
    if (!strcmp(str, "getData")) {
		if (nlhs != 3) {
			mexErrMsgIdAndTxt( "MATLAB:fastrak:maxlhs",
				"Three arguments required.");
		}

        mexPrintf("getData");
        GetData(nrhs, plhs);
    }
    if (!strcmp(str, "stop")) {
        mexPrintf("stop and disconnect\n");
        StopCont();
        Disconnect();
    }

	if (!strcmp(str, "test")) {
		GetBeginTime();
		GetEndTime();;

		double *p1, *p2;
		plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
		plhs[1] = mxCreateDoubleMatrix(1, 1, mxREAL);
		plhs[2] = mxCreateDoubleMatrix(2, 6, mxREAL);
		p1 = mxGetPr( plhs[0]);
		p2 = mxGetPr( plhs[1]);
		double *resultP = mxGetPr(plhs[2]);

		*p1 = static_cast<double>(g_beginTime);
		*p2 = static_cast<double>(g_endTime);

		mexPrintf("%d\n", g_beginTime);
		
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 6; j++) {
				resultP[j * 2 + i] = 1;
			}
		}
	}

    mxFree(str);
}
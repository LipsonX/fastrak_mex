/////////////////////////////////////////////////////////////////////
// Polhemus  www.polhemus.com
// ?2009-14 Polhemus, All Rights Reserved
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//
//  Filename:           $Workfile: PDIg4defs.h $
//
//  Project Name:       Polhemus Developer Interface  
//
//  Description:        Definitions file
//
//  VSS $Header: /PiDevTools11/Inc/PDIg4defs.h 8     1/09/14 1:05p Suzanne $  
//
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#ifndef _PDIG4DEFS_H_
#define _PDIG4DEFS_H_

// 8-byte header for use with legacy-formatted G4 P&O output
typedef struct 
{
	unsigned char sysID;
	unsigned char hubID;
	unsigned char snsID;
	unsigned char cmd; //cmd
	unsigned char res2;
	unsigned char res3;
	short		  length;
}*LPG4HDR, G4HDR;

#define G4_MAX_SENSORS_PER_HUB	3
#define G4_MAX_HUB_COUNT		256
#define G4_STAMAP_BLOCK_COUNT	16	
#define G4_LEGACY_FRAMEWORK_MAX_SENSORS 30

typedef struct {				// per sensor P&O data
	UINT32 nSnsID;
	float pos[3];
	float ori[4];
}*LPG4_SENSORDATA,G4_SENSORDATA;//32 bytes

typedef struct {				// per hub P&O data
	UINT32 nHubID;
	UINT32 nFrameCount;
	UINT32 dwSensorMap;	
	UINT32 dwDigIO;
	G4_SENSORDATA sd[G4_MAX_SENSORS_PER_HUB];
}*LPG4_HUBDATA,G4_HUBDATA;		// 112 bytes					

typedef struct {
	UINT32 nSrcID;
	float pos[3];
	float ori[4];
}*LPG4_SRCDATA,G4_SRCDATA;

#define G4_FILT_F_HEAVY		0.02f
#define G4_FILT_F_MEDIUM	0.05f
#define G4_FILT_F_LIGHT		0.2f
#define	G4_FILT_FLOW_HEAVY	0.02f
#define G4_FILT_FLOW_MEDIUM	0.05f
#define G4_FILT_FLOW_LIGHT	0.2f
#define G4_FILT_FHI_HEAVY	0.8f
#define G4_FILT_FHI_MEDIUM	0.8f
#define G4_FILT_FHI_LIGHT	0.8f
#define G4_FILT_MAXT_HEAVY	0.95f
#define G4_FILT_MAXT_MEDIUM	0.95f
#define G4_FILT_MAXT_LIGHT	0.95f
#endif // _PDIG4DEFS_H_

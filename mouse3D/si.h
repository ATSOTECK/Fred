/*----------------------------------------------------------------------
 *  si.h -- SpaceWare input library header
 *
 *  SpaceWare input library
 *
 *----------------------------------------------------------------------
 *
 * (C) 1998-2012 3Dconnexion. All rights reserved. 
 * Permission to use, copy, modify, and distribute this software for all
 * purposes and without fees is hereby grated provided that this copyright
 * notice appears in all copies.  Permission to modify this software is granted
 * and 3Dconnexion will support such modifications only is said modifications are
 * approved by 3Dconnexion.
 *
 */


#ifndef _SI_H_
#define _SI_H_

static char incFileNameCvsId[]="(C) 1996-2012 3Dconnexion: $Id: si.h 7481 2012-02-06 23:40:55Z jim_wick $";

#ifndef __cplusplus
#ifdef _WIN32
#include <windows.h>
#endif
#endif

#include <stdio.h>

// This fixes up structure packing differences between x64 and x86
#pragma pack(push, 4)

#include "spwmacro.h"
#include "spwdata.h"
#include "siSync.h"

#include "spwerror.h"

#include "siDefines.h"

typedef int SiDevID;          /* Device ID */
typedef void *SiHdl;          /* SpaceWare handle */
typedef void *SiTransCtl;     /* SpaceWare transport control handle */

#ifdef _WIN32
typedef struct                /* Open data */
   {
   HWND hWnd;               /* Window handle for SpaceWare messages.        */
   SiTransCtl transCtl;     /* SpaceWare transport control handle. Reserved */
                            /* for the s80 transport mechanism.             */
   DWORD processID;         /* The process ID for this application.         */
   char exeFile['k'];  /* The executable name of the process.          */
   SPWint32 libFlag;        /* Library version flag.                        */
   } SiOpenData;
#endif

#ifdef _WIN32
typedef struct                /* Get event Data */
   {
   UINT msg;
   WPARAM wParam;
   LPARAM lParam;
   } SiGetEventData;
#endif

typedef struct                /* Device type mask */
   {
   unsigned char mask[8];
   } SiTypeMask;

typedef struct                /* Device port information */
   {
   SiDevID devID;             /* Device ID */
   int devType;               /* Device type */
   int devClass;              /* Device class */
   char devName[SI_STRSIZE];  /* Device name */
   char portName[SI_MAXPORTNAME]; /* Port name */
   } SiDevPort;

typedef struct                /* Device information */
   {
   char firmware[SI_STRSIZE]; /* Firmware version */
   int devType;               /* Device type */
   int numButtons;            /* Number of buttons */
   int numDegrees;            /* Number of degrees of freedom */
   SPWbool canBeep;           /* Device beeps */
   int majorVersion;          /* Major version number */
   int minorVersion;          /* Minor version number */
   } SiDevInfo;

typedef struct                /* Button information */
	{
	char name[SI_STRSIZE];	  /* Contains the name of a button for display in an app's GUI */
	} SiButtonName;

typedef struct                /* Button information */
	{
	char name[SI_STRSIZE];	  /* Contains the name of a device for display in an app's GUI */
	} SiDeviceName;

typedef struct                /* Port information */
	{
	char name[SI_MAXPATH];	  /* The name of a port the device is located on */
	} SiPortName;

typedef struct                /* Version information */
   {
   int major;                 /* Major version number */
   int minor;                 /* Minor version number */
   int build;                 /* Build number */
   char version[SI_STRSIZE];  /* Version string */
   char date[SI_STRSIZE];     /* Date string */
   } SiVerInfo;

typedef struct                /* Sensitivity parameters */
   {
   char dummy;
   } SiSensitivity;

typedef struct                /* Tuning parameters */
   {
   char dummy;
   } SiTuning;

// Turn off "nonstandard extension used : nameless struct/union" warning
#pragma warning ( disable : 4201 )

typedef struct
   {
   SPWuint8 code;                 /* Out of band message code */
   union {
		SPWuint8 message[SI_MAXBUF-1];  /* The actual message       */
		SPWint32 messageAsLongs[SI_MAXBUF/4]; /* Access for longs/DWORDs  */
		void    *pvoid[SI_MAXBUF/8];    /* void ptrs.  Enough room for 64bit ptrs */
   };
   } SiSpwOOB;

typedef struct
   {
   union {
		SPWuint8 string[SI_KEY_MAXBUF];  /* No longer used, but it establishes the total size of SiSpwEvent, so keep it around in case anyone cares about the old size. */
		struct {
			SiKeyboardEventType type;
			union {
				struct keyData { int VirtualKeyCode; int ScanCode; } keyData; // Data for KeyPress and KeyRelease SiKeyboardEventType
			};
		} keyboardEvent;
   };
   } SiKeyboardData;

// Turn warning back on
#pragma warning ( default : 4201 )

typedef struct                /* Bitmasks of button states */
   {
   SPWuint32 last;            /* Buttons pressed as of last event */
   SPWuint32 current;         /* Buttons pressed as of this event */
   SPWuint32 pressed;         /* Buttons pressed this event */
   SPWuint32 released;        /* Buttons released this event */
   } SiButtonData;

/*
 * SI_BUTTON_PRESS_EVENT & SI_BUTTON_RELEASE_EVENT are hardware button 
 * events.  Meaning that they are meant to be sent when a specific hardware
 * button is pressed.  The correlation between the actual hardware button
 * and the resulting button number could be broken by careful editing of
 * a config file, but it is intended that the correlation be intact.
 * This is basically the same as SI_BUTTON_EVENT, but allows
 * more than 29 buttons because it isn't limited to a 32-bit mask.
 * Different entries in the config file determine whether SI_BUTTON_PRESS/RELEASE_EVENTs
 * or SI_BUTTON_EVENTs will be generated.
 * This event was introduced in 3DxWare 5.2, but not implemented until 3DxWare 7.0.
 */
typedef struct				  /* Data for SI_BUTTON_PRESS/RELEASE_EVENT */
	{
	SPWuint32 buttonNumber;	  /* The button number that went down/up in a  *
							   * SI_BUTTON_PRESS/RELEASE_EVENT event       */
	} SiHWButtonData;

typedef struct				  /* Data for SI_APP_EVENT */
	{
	SPWuint32 functionNumber; /* The Application-specific function number 
							   * invoked by the user in a SI_APP_EVENT     */
	} SiAppEventData;

typedef struct				  /* Data for SI_DEVICE_CHANGE_EVENT */
	{
	SiDeviceChangeType type;  /* The type of event that happened */
	SiDevID devID;			  /* The device ID effected */
	SiPortName portName;      /* The device path that changed */
	} SiDeviceChangeEventData;

typedef struct                /* SpaceWare data */
   {
   SiButtonData bData;        /* Button data */
   long mData[6];             /* Motion data (index via SI_TX, etc) */
   long period;               /* Period (milliseconds) */
   } SiSpwData;

typedef struct                /* SpaceWare event */
   {
   int type;                  /* Event type */
   union
      {
      SiSpwData spwData;           /* Button, motion, or combo data        */
      SiSpwOOB spwOOB;             /* Out of band message                  */
      SiOrientation spwOrientation;/* Which hand orientation is the device */
      char exData[SI_MAXBUF];      /* Exception data                       */
      SiKeyboardData spwKeyData;   /* String for keyboard data             */
	  SiSyncPacket siSyncPacket;   /* GUI SyncPacket sent to applications  */
	  SiHWButtonData hwButtonEvent;/* ButtonNumber that goes with          *
	                                * SI_BUTTON_PRESS/RELEASE_EVENT        */
	  SiAppEventData appEventData; /* Application event function data that *
                                    * goes with an SI_APP_EVENT event      */
	  SiDeviceChangeEventData deviceChangeEventData;    /* Data for connecting/disconnecting devices     */
      } u;
   } SiSpwEvent;

#ifdef _WIN32
typedef struct                /* Event handler (for SiDispatch) */
   {
   int (*func) (SiOpenData *, SiGetEventData *, SiSpwEvent *, void *);
   void *data;
   } SiEventHandler;
#endif

#ifdef _WIN32
typedef struct                /* SpaceWare event handlers */
   {
   SiEventHandler button;     /* Button event handler */
   SiEventHandler motion;     /* Motion event handler */
   SiEventHandler combo;      /* Combo event handler */
   SiEventHandler zero;       /* Zero event handler */
   SiEventHandler exception;  /* Exception event handler */
   } SiSpwHandlers;
#endif

// Reset packing to default so don't effect including file
#pragma pack(pop)


#endif   /* _SI_H_ */

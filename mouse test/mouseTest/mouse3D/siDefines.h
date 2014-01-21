/*----------------------------------------------------------------------
 *  siDefines.h -- SpaceWare input library defines and enums
 *
 *  SpaceWare Input library constants, no data structures
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


#ifndef _SIDEFINES_H_
#define _SIDEFINES_H_

static char siDefinesCvsId[]="(C) 2012 3Dconnexion: $Id: siDefines.h 7190 2012-02-06 22:40:22Z jim_wick $";

/*
 * UI modes 
 */
#define SI_UI_ALL_CONTROLS    0xffffffffL
#define SI_UI_NO_CONTROLS     0x00000000L

/*
 * Device types and classes 
 */
typedef enum
   {
   SI_ALL_TYPES                 = -1,
   SI_UNKNOWN_DEVICE            =  0,
   SI_SPACEBALL_2003            =  1,
   SI_SPACEBALL_3003            =  2,
   SI_SPACE_CONTROLLER          =  3,
   SI_SPACEEXPLORER             =  4,
   SI_SPACENAVIGATOR_FOR_NOTEBOOKS =  5,
   SI_SPACENAVIGATOR            =  6,
   SI_SPACEBALL_2003A           =  7,
   SI_SPACEBALL_2003B           =  8,
   SI_SPACEBALL_2003C           =  9,
   SI_SPACEBALL_3003A           =  10,
   SI_SPACEBALL_3003B           =  11,
   SI_SPACEBALL_3003C           =  12,
   SI_SPACEBALL_4000            =  13,
   SI_SPACEMOUSE_CLASSIC        =  14, 
   SI_SPACEMOUSE_PLUS           =  15,
   SI_SPACEMOUSE_XT             =  16,
   SI_CYBERMAN                  =  17,
   SI_CADMAN                    =  18,
   SI_SPACEMOUSE_CLASSIC_PROMO  =  19,
   SI_SERIAL_CADMAN             =  20,
   SI_SPACEBALL_5000            =  21,
   SI_TEST_NO_DEVICE            =  22,
   SI_3DX_KEYBOARD_BLACK        =  23,
   SI_3DX_KEYBOARD_WHITE        =  24,
   SI_TRAVELER                  =  25,
   SI_TRAVELER1                 =  26,
   SI_SPACEBALL_5000A           =  27,
   SI_SPACEDRAGON               =  28,
   SI_SPACEPILOT                =  29,
   SI_MB                        =  30,
   SI_SPACEPILOT_PRO            =  0xc629,
   SI_SPACEMOUSE_PRO            =  0xc62b
} SiDevType;

/*
 * Data retrieval mode, only SI_EVENT is currently supported.
 */
#define SI_EVENT              0x0001
#define SI_POLL               0x0002
#define SI_NEUTERED           0x0004 // A connection that doesn't get events

/*
 * Get event flags
 */
#define SI_AVERAGE_EVENTS     0x0001

/*
 * This is an INTERNAL flag used by the polling mechanism, user applications
 * should NOT send this flag.
 */
#define SI_POLLED_REQUEST     0x0100 

/*
 * 3DxWare event types
 */
typedef enum
   {
   SI_BUTTON_EVENT = 1,
   SI_MOTION_EVENT,
   SI_COMBO_EVENT, /* Not implemented */
   SI_ZERO_EVENT,
   SI_EXCEPTION_EVENT,
   SI_OUT_OF_BAND,
   SI_ORIENTATION_EVENT,
   SI_KEYBOARD_EVENT,
   SI_LPFK_EVENT,
   SI_APP_EVENT,			/* Application functions */
   SI_SYNC_EVENT,			/* GUI synchronization events */
   SI_BUTTON_PRESS_EVENT,	/* Single button events (replace SI_BUTTON_EVENT) */
   SI_BUTTON_RELEASE_EVENT,	/* Single button events (replace SI_BUTTON_EVENT) */
   SI_DEVICE_CHANGE_EVENT,
   SI_MOUSE_EVENT,
   SI_JOYSTICK_EVENT
} SiEventType;

/*
 * SI_DEVICE_CHANGE_EVENT type
 */
typedef enum
{
	SI_DEVICE_CHANGE_CONNECT,
	SI_DEVICE_CHANGE_DISCONNECT
} SiDeviceChangeType;

/* 
 * SI_KEYBOARD_EVENT type
 */
typedef enum 
{
	SI_KEYBOARD_EVENT_KEYPRESS,
	SI_KEYBOARD_EVENT_KEYRELEASE
} SiKeyboardEventType;


/*
 * Motion data offsets
 */
#define SI_TX                 0              /* Translation X value */
#define SI_TY                 1              /* Translation Y value */
#define SI_TZ                 2              /* Translation Z value */
#define SI_RX                 3              /* Rotation X value */
#define SI_RY                 4              /* Rotation Y value */
#define SI_RZ                 5              /* Rotation Z value */

/*
 * Reserved buttons
 */

#define SI_RESET_DEVICE_BIT   0x00000001L
#define SI_APP_FIT_BIT        0x80000000L
#define SI_APP_DIALOG_BIT     0x40000000L

#define SI_RESET_DEVICE_BUTTON     0
#define SI_APP_FIT_BUTTON         31
#define SI_APP_DIALOG_BUTTON      30
#define SI_APP_POPUPMENU_BUTTON   29

/*
 * Miscellaneous
 */
#define SI_END_ARGS           0
#define SI_NO_HANDLE          ((SiHdl) NULL)
#define SI_ALL_HANDLES        ((SiHdl) NULL)
#define SI_ANY_HANDLE         ((SiHdl) NULL)
#define SI_NO_TRANSCTL        ((SiTransCtl) NULL)
#define SI_NO_MASK            ((SiTypeMask *) NULL)
#define SI_ANY_DEVICE         -1
#define SI_NOTIFICATION_DEVICE 0
#define SI_NO_DEVICE          -1
#define SI_NO_TYPE            -1
#define SI_NO_LIST            -1
#define SI_NO_BUTTON          -1
#define SI_STRSIZE            128
#define SI_MAXBUF             128
#define SI_MAXPORTNAME		  260
#define SI_MAXPATH			  512
#define SI_KEY_MAXBUF         5120


typedef enum
   {
   SI_LEFT = 0,
   SI_RIGHT
   } SiOrientation;

#endif   /* _SIDEFINES_H_ */

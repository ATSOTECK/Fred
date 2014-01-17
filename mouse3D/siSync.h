/*----------------------------------------------------------------------
 *  siSync.h -- 3DxWare GUI Synchronization header
 *
 * Written:     September 2004
 * Author:      Jim Wick
 *
 *----------------------------------------------------------------------
 *
 * (c) 1998-2012 3Dconnexion. All rights reserved. 
 * Permission to use, copy, modify, and distribute this software for all
 * purposes and without fees is hereby grated provided that this copyright
 * notice appears in all copies.  Permission to modify this software is granted
 * and 3Dconnexion will support such modifications only is said modifications are
 * approved by 3Dconnexion.
 *
 */


#ifndef _SISYNC_H_
#define _SISYNC_H_

#include "siSyncDefines.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
	int state; /* VURZYX (Tx = LSB (& 1<<0) */
} SiSyncAxesState;


/*
 * Private / implementation structures
 * 
 * We suggest you leave these hidden and use the accessor functions rather than
 * directly accessing the structures.
 */
#include "siSyncPriv.h"


/*
 * Accessor Function headers 
 */
SPWuint32 SiSyncGetSize(SiSyncPacket p);
void      SiSyncSetSize(SiSyncPacket *p, SPWuint32 size);

SPWuint32 SiSyncGetHashCode(SiSyncPacket p);
void      SiSyncSetHashCode(SiSyncPacket *p, SPWuint32 hashCode);

SiSyncOpCode SiSyncGetOpCode(SiSyncPacket p);
void         SiSyncSetOpCode(SiSyncPacket *p, SPWuint32 opCode);

SiSyncItemCode SiSyncGetItemCode(SiSyncPacket p);
void           SiSyncSetItemCode(SiSyncPacket *p, SPWuint32 itemCode);

#ifdef __cplusplus
}
#endif

#endif   /* _SI_SYNC_H_ */

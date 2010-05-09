/*
=================================================================================================
Filename: TmpBufferHandler.h
Desciption:  
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/
#ifndef _TMP_BUFFER_HANDLER_H
#define	_TMP_BUFFER_HANDLER_H
#include "mmclib.h"
#include "stdio.h"
#include "common.h"

typedef struct _tmpHandleStruct TmpHandleStruct, *TmpHandleStructPtr;
struct _tmpHandleStruct{
	int id;
};

typedef struct _TmpFileStruct TmpFileStruct,*TmpFileStructPtr;
struct _TmpFileStruct{
	TempHandle hndl;
	HANDLE* m_pHFile;
	int bFirstRead;
	TmpFileStructPtr m_pNext;
};

int InitTempBufferHandler();
int GetNewTempHandle(TempHandle* hndl);
int SaveTempBuffer(TempHandle hndl,const unsigned char* buffer, unsigned int bufferSize);
int GetTempBuffer(TempHandle hndl,unsigned char* buffer, unsigned int bufferSize, unsigned int* bytesWrote);
int CloseTempBuffer(TempHandle hndl);
int DropTempHandle(TempHandle hndl);
int UnInitTempBufferHandler();

#endif


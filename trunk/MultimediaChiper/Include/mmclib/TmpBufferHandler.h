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

typedef struct _TmpFileStruct TmpFileStruct,*TmpFileStructPtr;
struct _TmpFileStruct{
	Filter m_pFlt;
	HANDLE* m_pHFile;
	int bFirstRead;
	TmpFileStructPtr m_pNext;
};

int InitTempBufferHandler();
int GetNewTempHandle(TempHandle* handle);
int SaveTempBuffer(Filter fltr,const unsigned char* buffer, unsigned int bufferSize);
int GetTempBuffer(Filter fltr,unsigned char* buffer, unsigned int bufferSize, unsigned int* bytesWrote);
int CloseTempBuffer(Filter fltr);
int UnInitTempBufferhandler();

#endif


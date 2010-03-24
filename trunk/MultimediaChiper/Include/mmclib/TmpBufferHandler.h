/*
=================================================================================================
Filename: TmpBufferHandler.h
Desciption:  
Author: $Author: claudiu.guiman $
Last changed by:    $Author: claudiu.guiman $
Last changed date:    $Date: 2010-03-14 19:33:19 +0200 (D, 14 mar. 2010) $
ID:            $Id: mmc.h 14 2010-03-14 17:33:19Z claudiu.guiman $
=================================================================================================
*/
#ifndef _TMP_BUFFER_HANDLER_H
#define	_TMP_BUFFER_HANDLER_H
#include "mmclib.h"
#include "stdio.h"

typedef struct _TmpFileStruct TmpFileStruct,*TmpFileStructPtr;
struct _TmpFileStruct{
	Filter m_pFlt;
	HANDLE* m_pHFile;
	int bFirstRead;
	TmpFileStructPtr m_pNext;
};

int InitTempBufferHandler();
int SaveTempBuffer(Filter fltr,const unsigned char* buffer, unsigned int bufferSize);
int GetTempBuffer(Filter fltr,unsigned char* buffer, unsigned int bufferSize, unsigned int* bytesWrote);
int CloseTempBuffer(Filter fltr);
int UnInitTempBufferhandler();

#endif


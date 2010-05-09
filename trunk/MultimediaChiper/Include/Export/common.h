/*
=================================================================================================
Filename: common.h
Desciption: Common definitions
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/
#ifndef _COMMON_DEF
#define _COMMON_DEF 1

#include <windows.h>

typedef void* TempHandle;

typedef int (*getTempHandleFn) (TempHandle*);
typedef int (*saveTempBufferFn) (TempHandle,const unsigned char*,unsigned int);
typedef int (*getTempBufferFn) (TempHandle,unsigned char*,unsigned int,unsigned int*);
typedef int (*closeTempHandleFn) (TempHandle);
typedef int (*dropTempHandleFn) (TempHandle);

typedef struct _tempHandlerAPI TempHandlerAPI, *TempHandlerAPIPtr;
typedef struct _tempHandlerAPI{
	getTempHandleFn		m_lpfnGetTempHandle;
	saveTempBufferFn	m_lpfnSaveTempBuffer;
	getTempBufferFn		m_lpfnGetTempBuffer;
	closeTempHandleFn	m_lpfnCloseTempHandle;
	dropTempHandleFn	m_lpfnDropTempHandle;
};

#endif
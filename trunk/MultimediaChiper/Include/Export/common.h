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

// these 3 funcs templates are used by mmclib to store temporary information for filters 
typedef void* TempHandle;

typedef int (*getNewHandleFn) (TempHandle*);
typedef int (*saveTempBufferFn) (TempHandle,const unsigned char*,unsigned int);
typedef int (*getTempBufferFn) (TempHandle,unsigned char*,unsigned int,unsigned int*);
typedef int (*closeTempBufferFn) (TempHandle);



#endif
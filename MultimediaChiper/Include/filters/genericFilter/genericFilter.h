/*
=================================================================================================
Filename: genericFilter.h
Desciption: 
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/
#include "filter.h"

#define FILTER_VERSION  L"1.0.0"
#define FILTER_NAME		L"Generic Filter"
#define FILTER_UID		0x01
#define FILTER_TYPE		L"generic"
#define FILTER_USE_INPUT FALSE

FilterStructPtr m_pInternalStruct = NULL;
TempHandlerAPIPtr pTmpApi = NULL;
TempHandle	bufferHandle = NULL;
int m_bActionFilter = TRUE;

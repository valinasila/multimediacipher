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

FilterStructPtr m_pInternalStruct = NULL;
saveTempBufferFn	m_lpfnSaveTempBuffer = NULL;
getTempBufferFn		m_lpfnGetTempBuffer = NULL;
closeTempBufferFn	m_lpfnCloseTempBuffer = NULL;

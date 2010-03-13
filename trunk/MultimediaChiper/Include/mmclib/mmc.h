/*
=================================================================================================
Filename: mmc.h
Desciption:  aditional functions and define, not needed for export are added here
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/
#include "mmclib.h"
#define DEFAULT_ENCODERS_DIR L"encoders"
#define DEFAULT_FILTERS_DIR	 L"filters"

typedef struct _encodersNode EncodersNode, *EncodersNodePtr;
struct _encodersNode{
	HMODULE*		m_dllHandle;
	EncoderAPI  	m_API;
	Encoder			m_pEncoder;
	EncodersNodePtr	m_pNext;
};

LPWSTR encodersDir = NULL;
LPWSTR filtersDir = NULL;
EncodersNodePtr encodersList = NULL;

int ScanEncoders();
int ScanFilters();
int ReleaseEncoders();
int ReleaseFilters();
int LoadEncodersAPI(HMODULE* dll, EncoderAPI* api);
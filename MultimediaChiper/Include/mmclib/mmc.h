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

#ifndef _MMC_H_ONCE
#define _MMC_H_ONCE 1

#include "mmclib.h"
#include "TmpBufferhandler.h"
#define DEFAULT_ENCODERS_DIR L"\\encoders"
#define DEFAULT_FILTERS_DIR	 L"\\filters"

typedef struct _encodersNode EncodersNode, *EncodersNodePtr;
struct _encodersNode{
	HMODULE*		m_dllHandle;
	EncoderAPI  	m_API;
	Encoder			m_pEncoder;
	EncodersNodePtr	m_pNext;
};
typedef struct _filtersNode FiltersNode, *FiltersNodePtr;
struct _filtersNode{
	HMODULE*		m_dllHandle;
	FilterAPI		m_API;
	Filter			m_pFilter;
	FiltersNodePtr	m_pNext;
};

LPWSTR encodersDir = NULL;
LPWSTR filtersDir = NULL;
EncodersNodePtr encodersList = NULL;
FiltersNodePtr  filtersList = NULL;
Encoder* m_Encoders = NULL;
unsigned int EncodersSize = 0;
Filter* m_Filters = NULL;
unsigned int FiltersSize = 0;

int ScanEncoders();
int ScanFilters();
int ReleaseEncoders();
int ReleaseFilters();
int LoadEncodersAPI(HMODULE* dll, EncoderAPI* api);
int LoadFiltersAPI(HMODULE* dll, FilterAPI* api);

#endif
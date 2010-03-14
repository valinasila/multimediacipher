/*
=================================================================================================
Filename: mmclib.h
Desciption: Exported API for mmclib
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/
#ifndef _MMCLIB_DEF
#define _MMCLIB_DEF
#include <windows.h>
#include "encoder.h"
#include "filter.h"

# ifdef __cplusplus
#  define API_EXPORT	extern "C"
# else
#  define API_EXPORT
#endif

API_EXPORT int Init();
typedef int (* initMmCFn) ();

API_EXPORT int UnInit();
typedef int (* uninitMmCFn) ();

API_EXPORT int ChangeEncodersFolder(LPCWSTR path);
typedef int (*changeEncFoldersFn) (LPCWSTR);

API_EXPORT int ChangeFiltersFolder(LPCWSTR path);
typedef int (*changeFilFoldersFn) (LPCWSTR);

API_EXPORT int EnumerateEncoders(Encoder** encoders, unsigned int* size);
typedef int (*enumerateEncodersFn) (Encoder**, unsigned int*);

API_EXPORT int EnumerateFilters(Filter** filters, unsigned int* size);
typedef int (*enumerateFiltersFn) (Filter**, unsigned int*);

typedef struct _MmCAPI{
	initMmCFn				m_lpfnInit;	
	uninitMmCFn				m_lpfnUnInit;
	changeEncFoldersFn		m_lpfnChangeEncodersFolder;
	changeFilFoldersFn		m_lpfnChangeFiltersFolder;
	enumerateEncodersFn		m_lpfnEnumerateEncoders;
	enumerateFiltersFn		m_lpfnEnumerateFilters;
} MmCAPI;


#define MMC_OK 0
#define MMC_MEMORY_ERROR 0x1000
#define MMC_WRONG_ENCODERS_FOLDER 0x0100
#define MMC_WRONG_ENCODER_LIBRARY 0x0101
#define MMC_WRONG_FILTERS_FOLDER  0x0200
#define MMC_WRONG_FILTER_LIBRARY  0x0201

#endif
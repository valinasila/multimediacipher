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
#define _MMCLIB_DEF 1
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

API_EXPORT int EncodeFile(LPCWSTR sourceFile, LPCWSTR* destFiles, int nDestFiles, const Filter* useFilters, int nFilters);
typedef int (*encodeFileFn) (LPCWSTR, LPCWSTR*, int, const Filter*, int);

API_EXPORT int DecodeFiles(LPCWSTR* sourceFiles, int nSourceFiles);
typedef int (*decodeFilesFn) (LPCWSTR*, int);

API_EXPORT int GetEncoderForFile(LPCWSTR filePath, Encoder* encoder);
typedef int (*getEncoderForFileFn) (LPCWSTR,Encoder*);

typedef struct _MmCAPI{
	initMmCFn				m_lpfnInit;	
	uninitMmCFn				m_lpfnUnInit;
	changeEncFoldersFn		m_lpfnChangeEncodersFolder;
	changeFilFoldersFn		m_lpfnChangeFiltersFolder;
	enumerateEncodersFn		m_lpfnEnumerateEncoders;
	enumerateFiltersFn		m_lpfnEnumerateFilters;
	encodeFileFn			m_lpfnEncodeFile;
	decodeFilesFn			m_lpfnDecodeFiles;
	getEncoderForFileFn		m_lpfnGetEncoderForFile;
} MmCAPI;


#define MMC_OK						0
#define MMC_WRONG_ARGUMENTS			1

#define MMC_MEMORY_ERROR			0x0A00

#define MMC_ENCODER_NOT_FOUND		0x0B00	

#define MMC_WRONG_ENCODERS_FOLDER	0x0C00
#define MMC_WRONG_ENCODER_LIBRARY	0x0C01

#define MMC_WRONG_FILTERS_FOLDER	0x0D00
#define MMC_WRONG_FILTER_LIBRARY	0x0D01

#define MMC_READ_FILE_ERROR			0x0E00
#define MMC_WRITE_FILE_ERROR		0x0E01

#endif
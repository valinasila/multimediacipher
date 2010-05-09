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

/**
       * Init func for mmclib
       * @return MMC_RETURN_CODE
       */
API_EXPORT int Init();
typedef int (* initMmCFn) ();

/**
       * UnInit func for mmclib
       * @return MMC_RETURN_CODE
       */
API_EXPORT int UnInit();
typedef int (* uninitMmCFn) ();


/**
       * Sets the folder for encoder .dll files. When this function is called,the encoder engines are reloaded.
	   * If not called, the default path is used on init
       * @param path Path to folder      
	   * @return MMC_OK if all is ok
       */
API_EXPORT int ChangeEncodersFolder(LPCWSTR path);
typedef int (*changeEncFoldersFn) (LPCWSTR);


/**
       * Sets the folder for filter .dll files.
	   * @see ChangeEncodersFolder(LPCWSTR path)   
	   * @return MMC_OK if all is ok
       */
API_EXPORT int ChangeFiltersFolder(LPCWSTR path);
typedef int (*changeFilFoldersFn) (LPCWSTR);


/**
       * Sets a list with all available encoders.
       * @param encoders  Pointer to a list of encoders
	   * @param size	Pointer to a type int paramater. returns the size of the encoder list.
	   * @return MMC_OK if all is ok
       */
API_EXPORT int EnumerateEncoders(Encoder** encoders, unsigned int* size);
typedef int (*enumerateEncodersFn) (Encoder**, unsigned int*);


/**
       * Sets a list with all available filters.
       * @param encoders  Pointer to a list of filters
	   * @param size	Pointer to a type int paramater. returns the size of the filter list.
	   * @return MMC_OK if all is ok
       */
API_EXPORT int EnumerateFilters(Filter** filters, unsigned int* size);
typedef int (*enumerateFiltersFn) (Filter**, unsigned int*);


/**
       * Encodes a filtered source file using one or more mediaFiles and outputs the buffer into destination file(s) 
	   * This version of mmclib only supports one media file and one destination file
       * @param sourceFile  Path to source file
	   * @param mediaFiles	List to  paths of media files
	   * @param nMediaFiles	Number of media files
	   * @param	destFile	List of paths to destination files
	   * @param nDestFiles	Number of destination files
	   * @param useFilters  List of filters to use. These will be applied in same order as they are found in the list.
	   * @param nFilters	Number of filters
	   * @return MMC_OK if all is ok
       */
API_EXPORT int EncodeFile(LPCWSTR sourceFile, LPCWSTR* mediaFiles, int nMediaFiles, LPCWSTR* destFiles, int nDestFiles, const Filter* useFilters, int nFilters);
typedef int (*encodeFileFn) (LPCWSTR, LPCWSTR*, int , LPCWSTR*, int, const Filter*, int);


/**
       * Decodes a list of files.Information about decoded files are kept inside the encoded files
       * @param sourceFile		Pointer to a list of source file paths 
	   * @param nSourceFiles	List size
	   * @return MMC_OK if all is ok
       */
API_EXPORT int DecodeFiles(LPCWSTR* sourceFiles, int nSourceFiles);
typedef int (*decodeFilesFn) (LPCWSTR*, int);


/**
       * Get encoder for a file
       * @param filePath		Path to file
	   * @param encoder			Pointer to an encoder structure. This will contain the pointer for the specific encoder that can be used with the file provided.
	   * @return MMC_OK if all is ok
       */
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
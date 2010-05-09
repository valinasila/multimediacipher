/*
=================================================================================================
Filename: filter.h
Desciption: The default API for a filter
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/
#ifndef _FILTER_DEF
#define _FILTER_DEF 1

#include <windows.h>
#include "common.h"

# ifdef __cplusplus
#  define API_EXPORT	extern "C"
# else
#  define API_EXPORT
#endif

typedef void* Filter;
typedef struct _filterStruct FilterStruct, * FilterStructPtr;

struct _filterStruct {
	FilterStructPtr	    m_pThis;		/*!< pointer to self */
	LPWSTR				m_szName;		/*!< filter name */
	LPWSTR				m_szVersion;	/*!< filter version */	
	LPWSTR				m_szType;		/*!< filter type */
	int					m_bUseinputForFilter; /*!< a filter may need a password or some input to filter the buffer (crypt filters)
												generaly this is FALSE; if TRUE, mmclib must set the input buffer with SetFilterInputBuffer()
												*@see SetFilterInputBuffer */	
	unsigned long long	m_ulUid;		/*!< filter GUID */
};

typedef enum _filterRet {
	FIL_RET_OK					= 0,
	FIL_RET_UnknownError		= -500,
	FIL_RET_WrongArgument		= -10,
	FIL_RET_NotImplemented		= -15,
	FIL_RET_IsFilter            = 4243,
} FilterRet;

/********************************************//**
       * Checks if the library is correctly loaded
       * @return FIL_RET_IsFilter if everything is ok
 ***********************************************/
API_EXPORT FilterRet IsFilter();
/**	*@see IsFilter	*/
typedef FilterRet (*isFilterFn)();


/********************************************//**
       * Init       
	   * @return FIL_RET_OK	 if all is ok
 ***********************************************/
API_EXPORT FilterRet Init();
/**	*@see Init	*/
typedef FilterRet (*initFilterFn)();


/********************************************//**
       * Get pointer to filter struct      
	   * @return pointer to filter. Must check if return value is NULL
 ***********************************************/
API_EXPORT Filter GetFilter();
/**	*@see GetFilter	*/
typedef Filter (*getFilterFn) ();


/********************************************//**
       * UnInit       
	   * @return FIL_RET_OK	 if all is ok
 ***********************************************/
API_EXPORT FilterRet UnInit();
/**	*@see UnInit	*/
typedef FilterRet (*uninitFilterFn) ();


/********************************************//**
       * Sets pointer to SaveTempBuffer function
       * @param [in] func  Pointer to SaveTempBuffer()  
	   * @see saveTempBufferFn
	   * @return FIL_RET_OK if all is ok
 ***********************************************/
API_EXPORT FilterRet SetSaveTempBufferFn(saveTempBufferFn func);
/**	*@see SetSaveTempBufferFn	*/
typedef  FilterRet (*setSaveTempBufferFn) (saveTempBufferFn);


/********************************************//**
       * Sets pointer to GetTempBuffer function
       * @param [in] func  Pointer to GetTempBuffer()
	   * @see getTempBufferFn
	   * @return FIL_RET_OK if all is ok
 ***********************************************/
API_EXPORT FilterRet SetGetTempBufferFn(getTempBufferFn func);
/**	*@see SetGetTempBufferFn	*/
typedef FilterRet (*setGetTempBufferFn) (getTempBufferFn);


/********************************************//**
       * Sets pointer to CloseTempBuffer function
       * @param [in] func  Pointer to CloseTempBuffer()	
	   * @see closeTempBufferFn
	   * @return FIL_RET_OK if all is ok
 ***********************************************/ 
API_EXPORT FilterRet SetCloseTempBufferFn(closeTempBufferFn func);
/**	*@see SetCloseTempBufferFn */
typedef FilterRet (*setCloseTempBufferFn)(closeTempBufferFn);


/********************************************//**
       * Set filter action
       * @param [in] bFilter if TRUE -> the filter encodes, else it decodes	   
	   * @return FIL_RET_OK if all is ok
 ***********************************************/ 
API_EXPORT FilterRet SetFilterAction(int bFilter);
/**	*@see SetFilterAction */
typedef FilterRet (*setFilterActionFn) (int);

/********************************************//**
       * Set input buffer. 
	   * This buffer can be a buffer from a file, or a buffer output from another filter (mmclib sets the buffer source)
	   * mmclib will send the entire source buffer; the filter can save the source buffer/output buffer in memory or in a temp file
	   * (there are 3 function sent by mmclib that help saving information in temp files)	   
	   * After all of the input is sent mmclib will read the processed output using  GetFilterBuffer()
	   * @see GetFilterBuffer
       * @param [in] buffer
	   * @param [in] bufferSize  size of buffer
	   * @param [in] bLastBuffer If TRUE, this is the last buffer sent
	   * @return FIL_RET_OK if all is ok
 ***********************************************/ 
API_EXPORT FilterRet SetFilterBuffer(const unsigned char* buffer,unsigned int bufferSize,int bLastBuffer);
/**	*@see SetFilterBuffer */
typedef FilterRet (*setFilterBufferFn) (const unsigned char*,unsigned int,int);


/********************************************//**
       * Get output buffer. 
	   * Writes into buffer size chunks the filter output for a specific input sent by mmclib using SetFilterBuffer()
	   * @see SetFilterBuffer
       * @param [in,out]	buffer
	   * @param [in]		bufferSize  size of buffer
	   * @param [out]		bytesWrote  bytes wrote in supplied buffer. If this is 0, then there is no more output to send.
	   * @return FIL_RET_OK if all is ok
 ***********************************************/ 
API_EXPORT FilterRet GetFilterBuffer(unsigned char* buffer,unsigned int bufferSize,unsigned int* bytesWrote);
/**	*@see GetFilterBuffer */
typedef FilterRet (*getFilterBufferFn) (unsigned char*,unsigned int,unsigned int*);


/********************************************//**
       * Set filter input 
	   * If filter has m_bUseinputForFilter = TRUE, then mmclib will require a user input, which will be provided to the filter using this function 
	   * @see FilterStruct.m_bUseinputForFilter
       * @param [in] buffer
	   * @param [in] bufferSize  size of buffer	  
	   * @return FIL_RET_OK if all is ok
 ***********************************************/ 
API_EXPORT FilterRet SetFilterInputBuffer(unsigned char* buffer, unsigned int bufferSize);
/**	*@see SetFilterInputBuffer */
typedef FilterRet (*setFilterInputBufferFn) (unsigned char*,unsigned int);


/********************************************//**
       * Resets the filter 
	   * After each use of the filter, use this function to reset the filter	   
	   * @return FIL_RET_OK if all is ok
 ***********************************************/ 
API_EXPORT FilterRet ReloadFilter();
/**	*@see ReloadFilter */
typedef FilterRet (*reloadFilterFn)();

typedef struct _filterAPI{
	isFilterFn				m_lpfnIsFilter	;	
	initFilterFn			m_lpfnInit;
	getFilterFn				m_lpfnGetFilter;
	uninitFilterFn			m_lpfnUnInit;
	setSaveTempBufferFn		m_lpfnSetSaveTempBufferFn;
	setGetTempBufferFn		m_lpfnSetGetTempBufferFn;
	setCloseTempBufferFn	m_lpfnSetCloseTempBufferFn;
	setFilterActionFn		m_lpfnSetAction;
	setFilterBufferFn		m_lpfnSetBuffer;
	getFilterBufferFn		m_lpfnGetBuffer;
	setFilterInputBufferFn  m_lpfnSetInputBuffer;
	reloadFilterFn			m_lpfnReloadFilter;
} FilterAPI;

#endif
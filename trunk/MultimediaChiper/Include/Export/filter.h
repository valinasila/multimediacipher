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

# ifdef __cplusplus
#  define API_EXPORT	extern "C"
# else
#  define API_EXPORT
#endif

typedef void* Filter;
typedef struct _filterStruct FilterStruct, * FilterStructPtr;
struct _filterStruct {
	FilterStructPtr	    m_pThis;
	LPWSTR				m_szName;
	LPWSTR				m_szVersion;	
	LPWSTR				m_szType;
	int					m_bUseinputForFilter; // a filter may need a password or some input to filter the buffer (crypt filters)
											  // generaly this is FALSE; if TRUE, mmclib must set the input buffer with SetFilterInputBuffer()
	unsigned long long	m_ulUid;
};
typedef enum _filterRet {
	FIL_RET_OK					= 0,
	FIL_RET_UnknownError		= -500,
	FIL_RET_WrongArgument		= -10,
	FIL_RET_NotImplemented		= -15,
	FIL_RET_IsFilter            = 4243,
} FilterRet;

// these 3 funcs templates are used by mmclib to store temporary information for filters 
typedef int (*saveTempBufferFn) (Filter,const unsigned char*,unsigned int);
typedef int (*getTempBufferFn) (Filter,unsigned char*,unsigned int,unsigned int*);
typedef int (*closeTempBufferFn) (Filter);

API_EXPORT FilterRet IsFilter();
typedef FilterRet (*isFilterFn)();

API_EXPORT FilterRet Init();
typedef FilterRet (*initFilterFn)();

API_EXPORT Filter GetFilter();
typedef Filter (*getFilterFn) ();

API_EXPORT FilterRet UnInit();
typedef FilterRet (*uninitFilterFn) ();

API_EXPORT FilterRet SetSaveTempBufferFn(saveTempBufferFn func);
typedef  FilterRet (*setSaveTempBufferFn) (saveTempBufferFn);

API_EXPORT FilterRet SetGetTempBufferFn(getTempBufferFn func);
typedef FilterRet (*setGetTempBufferFn) (getTempBufferFn);

API_EXPORT FilterRet SetCloseTempBufferFn(closeTempBufferFn func);
typedef FilterRet (*setCloseTempBufferFn)(closeTempBufferFn);

API_EXPORT FilterRet SetFilterAction(int bFilter);
typedef FilterRet (*setFilterActionFn) (int);

API_EXPORT FilterRet SetFilterBuffer(const unsigned char* buffer,unsigned int bufferSize,int bLastBuffer);
typedef FilterRet (*setFilterBufferFn) (const unsigned char*,unsigned int,int);

API_EXPORT FilterRet GetFilterBuffer(unsigned char* buffer,unsigned int bufferSize,unsigned int* bytesWrote);
typedef FilterRet (*getFilterBufferFn) (unsigned char*,unsigned int,unsigned int*);

API_EXPORT FilterRet SetFilterInputBuffer(unsigned char* buffer, unsigned int bufferSize);
typedef FilterRet (*setFilterInputBufferFn) (unsigned char*,unsigned int);

API_EXPORT FilterRet ReloadFilter();
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
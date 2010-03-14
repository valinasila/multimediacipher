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
#define _FILTER_DEF
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
	unsigned long long	m_ulUid;
};
typedef enum _filterRet {
	FIL_RET_OK					= 0,
	FIL_RET_UnknownError		= -500,
	FIL_RET_WrongArgument		= -10,
	FIL_RET_IsFilter            = 4243,
} FilterRet;

API_EXPORT FilterRet IsFilter();
typedef FilterRet (*isFilterFn)();

API_EXPORT FilterRet Init();
typedef FilterRet (*initFilterFn)();

API_EXPORT Filter GetEncoder();
typedef Filter (*getFilterFn) ();

API_EXPORT FilterRet UnInit();
typedef FilterRet (*uninitFilterFn) ();


typedef struct _filterAPI{
	isFilterFn				m_lpfnIsFilter	;	
	initFilterFn			m_lpfnInit;
	getFilterFn				m_lpfnGetFilter;
	uninitFilterFn			m_lpfnUnInit;
} FilterAPI;

#endif
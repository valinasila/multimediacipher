/*
=================================================================================================
Filename: main.c
Desciption: Main code implementation for genericFilter
Author: $Author: claudiu.guiman $
Last changed by:    $Author: claudiu.guiman $
Last changed date:    $Date: 2010-03-12 10:09:18 +0200 (V, 12 mar. 2010) $
ID:            $Id: main.c 6 2010-03-12 08:09:18Z claudiu.guiman $
=================================================================================================
*/

#include "genericFilter.h"

FilterRet IsFilter()
{
	return FIL_RET_IsFilter;
}
FilterRet Init()
{
	m_pInternalStruct = (FilterStructPtr) malloc( sizeof(FilterStruct) );
	m_pInternalStruct->m_pThis = m_pInternalStruct;
	m_pInternalStruct->m_ulUid = FILTER_UID;
	m_pInternalStruct->m_bUseinputForFilter = FILTER_USE_INPUT;
	m_pInternalStruct->m_szName = (LPWSTR) malloc( sizeof(WCHAR) * ( wcslen(FILTER_NAME) + 1 ) );
	memcpy( m_pInternalStruct->m_szName , FILTER_NAME , sizeof(WCHAR) * wcslen(FILTER_NAME) );
	m_pInternalStruct->m_szName[wcslen(FILTER_NAME)] = L'\0';
	m_pInternalStruct->m_szType = (LPWSTR) malloc( sizeof(WCHAR) * (wcslen(FILTER_TYPE) + 1 ) );
	memcpy( m_pInternalStruct->m_szType , FILTER_TYPE , sizeof(WCHAR) * wcslen(FILTER_TYPE) );
	m_pInternalStruct->m_szType[wcslen(FILTER_TYPE)] = L'\0';
	m_pInternalStruct->m_szVersion = (LPWSTR) malloc( sizeof(WCHAR) * (wcslen(FILTER_VERSION) + 1 ) );	
	memcpy( m_pInternalStruct->m_szVersion , FILTER_VERSION , sizeof(WCHAR) * wcslen(FILTER_VERSION) );
	m_pInternalStruct->m_szVersion[wcslen(FILTER_VERSION)] = L'\0';
	return FIL_RET_OK;
}
Filter GetFilter()
{
	return (Filter) m_pInternalStruct;
}
FilterRet	UnInit()
{
	if(m_pInternalStruct)
	{
		free(m_pInternalStruct->m_szName);
		free(m_pInternalStruct->m_szType);
		free(m_pInternalStruct->m_szVersion);
		free(m_pInternalStruct);
	}
	return FIL_RET_OK;
}
FilterRet SetSaveTempBufferFn(saveTempBufferFn func)
{
	m_lpfnSaveTempBuffer = func;
	return FIL_RET_OK;
}
FilterRet SetGetTempBufferFn(getTempBufferFn func)
{
	m_lpfnGetTempBuffer = func;
	return FIL_RET_OK;
}
FilterRet SetCloseTempBufferFn(closeTempBufferFn func)
{
	m_lpfnCloseTempBuffer = func;
	return FIL_RET_OK;
}
FilterRet SetFilterAction(int bFilter)
{
	m_bActionFilter = bFilter;
	return FIL_RET_OK;
}
FilterRet SetFilterBuffer(const unsigned char* buffer,unsigned int bufferSize,int bLastBuffer)
{
	if(NULL == m_lpfnSaveTempBuffer)
		return FIL_RET_WrongArgument;

	if( 0 != m_lpfnSaveTempBuffer( (Filter) m_pInternalStruct, buffer, bufferSize) )
		return FIL_RET_UnknownError;

	return FIL_RET_OK;
}
FilterRet GetFilterBuffer(unsigned char* buffer,unsigned int bufferSize,unsigned int* bytesWrote)
{
	if(NULL == m_lpfnGetTempBuffer)
		return FIL_RET_WrongArgument;

	if( 0 != m_lpfnGetTempBuffer( (Filter) m_pInternalStruct, buffer,bufferSize,bytesWrote) )
		return FIL_RET_UnknownError;

	return FIL_RET_OK;
}
FilterRet SetFilterInputBuffer(unsigned char* buffer, unsigned int bufferSize)
{
	return FIL_RET_NotImplemented; // this filter doesn't use user input to filter the buffer
}
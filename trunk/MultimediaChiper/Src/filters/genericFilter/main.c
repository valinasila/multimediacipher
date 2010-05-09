/*
=================================================================================================
Filename: main.c
Desciption: Main code implementation for genericFilter
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
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
FilterRet UnInit()
{
	if(NULL != pTmpApi->m_lpfnDropTempHandle)
		pTmpApi->m_lpfnDropTempHandle(bufferHandle);

	if(m_pInternalStruct)
	{
		free(m_pInternalStruct->m_szName);
		free(m_pInternalStruct->m_szType);
		free(m_pInternalStruct->m_szVersion);
		free(m_pInternalStruct);
	}	
	return FIL_RET_OK;
}
FilterRet SetTempFn(TempHandlerAPIPtr api)
{
	pTmpApi = api; 

	if(NULL == pTmpApi->m_lpfnGetTempHandle)
		return FIL_RET_WrongArgument;

	pTmpApi->m_lpfnGetTempHandle(&bufferHandle);

	return FIL_RET_OK;
}
FilterRet SetFilterAction(int bFilter)
{
	m_bActionFilter = bFilter;
	return FIL_RET_OK;
}
FilterRet SetFilterBuffer(const unsigned char* buffer,unsigned int bufferSize,int bLastBuffer)
{
	if(NULL == pTmpApi->m_lpfnSaveTempBuffer)
		return FIL_RET_WrongArgument;

	if( 0 != pTmpApi->m_lpfnSaveTempBuffer( bufferHandle, buffer, bufferSize) )
		return FIL_RET_UnknownError;

	return FIL_RET_OK;
}
FilterRet GetFilterBuffer(unsigned char* buffer,unsigned int bufferSize,unsigned int* bytesWrote)
{
	if(NULL == pTmpApi->m_lpfnGetTempBuffer)
		return FIL_RET_WrongArgument;

	if( 0 != pTmpApi->m_lpfnGetTempBuffer( bufferHandle, buffer,bufferSize,bytesWrote) )
		return FIL_RET_UnknownError;

	return FIL_RET_OK;
}
FilterRet SetFilterInputBuffer(unsigned char* buffer, unsigned int bufferSize)
{
	return FIL_RET_NotImplemented; // this filter doesn't use user input to filter the buffer
}
FilterRet ReloadFilter()
{
	pTmpApi->m_lpfnCloseTempHandle( bufferHandle);
	return FIL_RET_OK;
}
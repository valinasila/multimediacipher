/*
=================================================================================================
Filename: main.c
Desciption:				Main file for mmclib
Author:					$Author$
Last changed by:		$Author$
Last changed date:		$Date$
ID:						$Id$
=================================================================================================
*/
#include "mmc.h"

int Init()
{
	int ret;
	ret = ChangeEncodersFolder(DEFAULT_ENCODERS_DIR);
	if(ret != MMC_OK)
		return ret;

	ret = ChangeFiltersFolder(DEFAULT_FILTERS_DIR);
	if(ret != MMC_OK)
		return ret;

	return MMC_OK;
}
int UnInit()
{
	if(encodersDir)
		free(encodersDir);

	if(filtersDir)
		free(filtersDir);

	ReleaseEncoders();
	ReleaseFilters();
	return MMC_OK;
}
int ChangeEncodersFolder(LPCWSTR path)
{
	if(encodersDir)
		free(encodersDir);
	if(	path[wcslen(path) - 1] == L'\\' )
		encodersDir = (LPWSTR) malloc( sizeof(WCHAR) * (wcslen(path) + 1) );
	else
		encodersDir = (LPWSTR) malloc( sizeof(WCHAR) * (wcslen(path) + 2) );

	if(encodersDir == NULL)
		return MMC_MEMORY_ERROR;

	memcpy(encodersDir , path , sizeof(WCHAR) * wcslen(path));

	if(	path[wcslen(path) - 1] == L'\\' )	
	{		
		encodersDir[wcslen(path)] = L'\0';
	}
	else
	{
		encodersDir[wcslen(path)]	  =	L'\\';		
		encodersDir[wcslen(path) + 1] = L'\0';
	}
	
	return ScanEncoders();
	
}
int ChangeFiltersFolder(LPCWSTR path)
{
	if(filtersDir)
		free(filtersDir);

	if(	path[wcslen(path) - 1] == L'\\' )
		filtersDir = (LPWSTR) malloc( sizeof(WCHAR) * (wcslen(path) + 1) );
	else
		filtersDir = (LPWSTR) malloc( sizeof(WCHAR) * (wcslen(path) + 2) );

	if(filtersDir == NULL)
		return MMC_MEMORY_ERROR;

	memcpy(filtersDir , path , sizeof(WCHAR) * wcslen(path));

	if(	path[wcslen(path) - 1] == L'\\' )	
	{		
		filtersDir[wcslen(path)] = L'\0';
	}
	else
	{
		filtersDir[wcslen(path)]	 = L'\\';		
		filtersDir[wcslen(path) + 1] = L'\0';
	}
	
	return ScanFilters();
}
int ScanEncoders()
{
	int bRunning = 1;	
	HANDLE hFile = NULL;	
	WIN32_FIND_DATA wfd;
	HMODULE* tmpDll;	
	LPWSTR dllPath = (LPWSTR) malloc(sizeof(WCHAR) * MAX_PATH);
	LPWSTR dllDir = (LPWSTR) malloc(sizeof(WCHAR) * ( wcslen(encodersDir) + 6 ) );

	memcpy(dllDir , encodersDir , sizeof(WCHAR) * wcslen(encodersDir) );
	memcpy(dllDir + wcslen(encodersDir),L"*.dll", sizeof(WCHAR) * 5);
	dllDir[wcslen(encodersDir) + 5] = L'\0';
	memcpy(dllPath , dllDir , sizeof(WCHAR) * wcslen(dllDir) );

	ReleaseEncoders();
	
	do{
		if(hFile == NULL)
		{
			hFile = FindFirstFile(dllDir,&wfd);
			if(INVALID_HANDLE_VALUE == hFile)
			{
				free(dllPath);	
				free(dllDir);
				return MMC_WRONG_ENCODERS_FOLDER;
			}
		}
		else
		{
			if( ! FindNextFile(hFile,&wfd) )
				break;
		}				
		memcpy(dllPath + wcslen(encodersDir) , wfd.cFileName , sizeof(WCHAR) * wcslen(wfd.cFileName)); 
		dllPath[wcslen(encodersDir) + wcslen(wfd.cFileName)] = L'\0';	
		tmpDll = (HMODULE*) malloc(sizeof(HMODULE));
		if( NULL ==(*tmpDll = LoadLibrary(dllPath)) )
		{
			free(tmpDll);
		}
		else
		{
			EncodersNodePtr node = (EncodersNodePtr) malloc(sizeof(EncodersNode));
			node->m_dllHandle = tmpDll;
			if( MMC_OK !=  LoadEncodersAPI(tmpDll,&node->m_API ) )
			{
				free(node);
			}
			else
			{
				node->m_API.m_lpfnInit();
				node->m_pEncoder = node->m_API.m_lpfnGetEncoder();
				node->m_pNext = encodersList;
				if(NULL != encodersList)
					encodersList->m_pNext = node;
				else
					encodersList = node;
			}
		}
	} while(1);

	free(dllPath);	
	free(dllDir);
	return MMC_OK;
}
int ScanFilters()
{
	return MMC_OK;
}
int ReleaseEncoders()
{
	EncodersNodePtr node = encodersList;
	while(node)
	{
		encodersList = node->m_pNext;
		node->m_API.m_lpfnUnInit();		
		FreeLibrary(*node->m_dllHandle);
		free(node->m_dllHandle);
		free(node);
		node = encodersList;
	}
	return MMC_OK;
}
int ReleaseFilters()
{
	return MMC_OK;
}
int LoadEncodersAPI(HMODULE* dll, EncoderAPI* api)
{
#define CHECK_ENC_DLL(x) if(NULL == x) return MMC_WRONG_ENCODER_LIBRARY;
	api->m_lpfnIsEncoder = (isEncoderFn) GetProcAddress(*dll,"IsEncoder"); CHECK_ENC_DLL(api->m_lpfnIsEncoder)
	api->m_lpfnInit = (initEncoderFn) GetProcAddress(*dll,"Init"); CHECK_ENC_DLL(api->m_lpfnInit)
	api->m_lpfnUnInit = (uninitEncoderFn) GetProcAddress(*dll,"UnInit"); CHECK_ENC_DLL(api->m_lpfnUnInit)
	api->m_lpfnGetEncoder = (getEncoderFn) GetProcAddress(*dll,"GetEncoder"); CHECK_ENC_DLL(api->m_lpfnGetEncoder)
	if(ENC_RET_IsEncoder  != api->m_lpfnIsEncoder() )
		return MMC_WRONG_ENCODER_LIBRARY;
	
	return MMC_OK;
}
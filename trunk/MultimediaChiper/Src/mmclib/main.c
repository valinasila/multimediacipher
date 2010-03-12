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

	ReleaseDllList(&dllEncoders , &dllEncodersSize);
	ReleaseDllList(&dllFilters , &dllFiltersSize);
	return MMC_OK;
}
int ChangeEncodersFolder(LPCWSTR path)
{
	if(encodersDir)
		free(encodersDir);
	if(	path[wcslen(path) - 1] == L'\\' )
		encodersDir = (LPWSTR) malloc( sizeof(WCHAR) * (wcslen(path) + 6) );
	else
		encodersDir = (LPWSTR) malloc( sizeof(WCHAR) * (wcslen(path) + 7) );

	if(encodersDir == NULL)
		return MMC_MEMORY_ERROR;
	memcpy(encodersDir , path , sizeof(WCHAR) * wcslen(path));

	if(	path[wcslen(path) - 1] == L'\\' )	
	{
		memcpy(encodersDir + wcslen(path) ,L"*.dll",11);
		encodersDir[wcslen(path)+ 5] = L'\0';
	}
	else
	{
		memcpy(encodersDir + wcslen(path) ,L"\\*.dll",12);		
		encodersDir[wcslen(path) + 6] = L'\0';
	}
	
	return ScanEncoders();
	
}
int ChangeFiltersFolder(LPCWSTR path)
{
	if(filtersDir)
		free(filtersDir);
	if(	path[wcslen(path) - 1] == L'\\' )
		filtersDir = (LPWSTR) malloc( sizeof(WCHAR) * (wcslen(path) + 6) );
	else
		filtersDir = (LPWSTR) malloc( sizeof(WCHAR) * (wcslen(path) + 7) );

	if(filtersDir == NULL)
		return MMC_MEMORY_ERROR;
	memcpy(filtersDir , path , sizeof(WCHAR) * wcslen(path));

	if(	path[wcslen(path) - 1] == L'\\' )	
	{
		memcpy(filtersDir + wcslen(path) ,L"*.dll",11);
		filtersDir[wcslen(path)+ 5] = L'\0';
	}
	else
	{
		memcpy(filtersDir + wcslen(path) ,L"\\*.dll",12);		
		filtersDir[wcslen(path) + 6] = L'\0';
	}
	
	return ScanFilters();
}
int ReleaseDllList(HMODULE** dllList,int* size)
{
	int i;
	if(size > 0)
	{
		for(i = 0;  i < *size; i++)
		{
			FreeLibrary(*(*dllList + i));
		}
		free(*dllList);
		*dllList = NULL;
		*size = 0;
	}
	return MMC_OK;
}
int ScanEncoders()
{
	int i,bRunning = 1;
	LPWSTR dir;
	DWORD q;
	HANDLE hFile = NULL;
	HMODULE tmpDll;
	LPWSTR dllPath = (LPWSTR) malloc(sizeof(WCHAR) * MAX_PATH);
	WIN32_FIND_DATA wfd;

	ReleaseDllList(&dllEncoders , &dllEncodersSize);
	dir = (LPWSTR) malloc(MAX_PATH);
	q = GetCurrentDirectory(MAX_PATH,dir);
	do{
		if(hFile == NULL)
		{
			hFile = FindFirstFile(L".\\encoders\\*.dll"/*encodersDir*/,&wfd);
			if(INVALID_HANDLE_VALUE == hFile)
			{
				DWORD d = GetLastError();
				free(dllPath);
				return MMC_WRONG_ENCODERS_FOLDER;
			}
		}
		else
		{
			if( ! FindNextFile(hFile,&wfd) )
				break;
		}
		if(  wcscmp(wfd.cFileName,L".") != 0 && wcscmp(wfd.cFileName,L"..") != 0 )
		{
			memcpy(dllPath , encodersDir , sizeof(WCHAR) * wcslen(encodersDir) );
			/*memcpy(dllPath + wcslen(encodersDir) , wfd.cFileName , 
			wcscat_s(
			tmpDll = LoadLibrary(wfd.*/
		}
	} while(1);

	free(dllPath);
	return MMC_OK;
}
int ScanFilters()
{
	return MMC_OK;
}
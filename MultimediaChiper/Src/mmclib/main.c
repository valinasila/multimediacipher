/*
=================================================================================================
Filename: main.c
Desciption: 
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
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
		encodersDir[wcslen(path)] = L'\0';
	else
	{
		encodersDir[wcslen(path)] = L'\\';
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
	memcpy(filtersDir  ,path , sizeof(WCHAR) * wcslen(path));

	if(	path[wcslen(path) - 1] == L'\\' )	
		filtersDir[wcslen(path)] = L'\0';
	else
	{
		filtersDir[wcslen(path)] = L'\\';
		filtersDir[wcslen(path) + 1] = L'\0';
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
	HANDLE hFile = NULL;
	HMODULE tmpDll;
	LPWSTR dllPath = (LPWSTR) malloc(sizeof(WCHAR) * MAX_PATH);
	WIN32_FIND_DATA wfd;

	ReleaseDllList(&dllEncoders , &dllEncodersSize);

	do{
		if(hFile == NULL)
		{
			hFile = FindFirstFile(encodersDir,&wfd);
			if(INVALID_HANDLE_VALUE == hFile)
			{
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
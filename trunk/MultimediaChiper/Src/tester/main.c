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
#include "mmclib.h"
#include <stdio.h>
MmCAPI api_mmc;
HMODULE hDll;
void InitAPI()
{
	hDll = LoadLibrary(L"mmclib.dll");
	api_mmc.m_lpfnInit = (initMmCFn)GetProcAddress(hDll,"Init");
	api_mmc.m_lpfnUnInit = (uninitMmCFn) GetProcAddress(hDll,"UnInit");
	api_mmc.m_lpfnChangeEncodersFolder = (changeEncFoldersFn) GetProcAddress(hDll,"ChangeEncodersFolder");
	api_mmc.m_lpfnChangeFiltersFolder = (changeFilFoldersFn) GetProcAddress(hDll,"ChangeFiltersFolder");
}
int main()
{
	int ret;
	InitAPI();
	ret = api_mmc.m_lpfnInit();
	printf("%d",ret);
	api_mmc.m_lpfnUnInit();
	FreeLibrary(hDll);	
	system("pause");
	return 0;
}
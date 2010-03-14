/*
=================================================================================================
Filename: main.c
Desciption: teste.exe code
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/
#include "vld.h" // visual leak detector  http://www.codeproject.com/KB/applications/visualleakdetector.aspx
#include "mmclib.h"
#include <stdio.h>
MmCAPI api_mmc;
HMODULE hDll;
Encoder* encoderList = NULL;
unsigned int encoderListSize = 0;
Filter* filterList = NULL;
unsigned int filterListSize = 0;

void InitAPI()
{
	hDll = LoadLibrary(L"mmclib.dll");
	api_mmc.m_lpfnInit = (initMmCFn)GetProcAddress(hDll,"Init");
	api_mmc.m_lpfnUnInit = (uninitMmCFn) GetProcAddress(hDll,"UnInit");
	api_mmc.m_lpfnChangeEncodersFolder = (changeEncFoldersFn) GetProcAddress(hDll,"ChangeEncodersFolder");
	api_mmc.m_lpfnChangeFiltersFolder = (changeFilFoldersFn) GetProcAddress(hDll,"ChangeFiltersFolder");
	api_mmc.m_lpfnEnumerateEncoders = (enumerateEncodersFn) GetProcAddress(hDll,"EnumerateEncoders");
	api_mmc.m_lpfnEnumerateFilters = (enumerateFiltersFn) GetProcAddress(hDll,"EnumerateFilters");
}
int main()
{
	int ret;
	unsigned int i;
	InitAPI();
	ret = api_mmc.m_lpfnInit();
	if(ret != MMC_OK)
	{
		printf("Error: %d",ret);
		api_mmc.m_lpfnUnInit();
		FreeLibrary(hDll);
		system("pause");
		return 1;
	}
	
	api_mmc.m_lpfnEnumerateEncoders(&encoderList,&encoderListSize);
	printf("Encoders:\n");
	for(i = 0; i < encoderListSize; i++)
	{
		EncoderStructPtr tmpEnc = (EncoderStructPtr) *(encoderList + i);		
		wprintf(L"[%lld] %s %s  - %s\n",tmpEnc->m_ulUid,tmpEnc->m_szName,tmpEnc->m_szVersion,tmpEnc->m_szType);
	}

	api_mmc.m_lpfnEnumerateFilters(&filterList,&filterListSize);
	printf("Filters:\n");
	for(i = 0; i < filterListSize; i++)
	{
		FilterStructPtr tmpEnc = (FilterStructPtr) *(filterList + i);		
		wprintf(L"[%lld] %s %s  - %s\n",tmpEnc->m_ulUid,tmpEnc->m_szName,tmpEnc->m_szVersion,tmpEnc->m_szType);
	}

	api_mmc.m_lpfnUnInit();
	FreeLibrary(hDll);	
	system("pause");
	
	return 0;
}
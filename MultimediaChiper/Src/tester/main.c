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
	api_mmc.m_lpfnEncodeFile =(encodeFileFn) GetProcAddress(hDll,"EncodeFile");
	api_mmc.m_lpfnDecodeFiles = (decodeFilesFn) GetProcAddress(hDll,"DecodeFiles");	
	api_mmc.m_lpfnGetEncoderForFile = (getEncoderForFileFn) GetProcAddress(hDll,"GetEncoderForFile");
}
int main()
{
	int ret;
	unsigned int i;
	WCHAR path[MAX_PATH] = {0};
	WCHAR path2[MAX_PATH] = {0};
	WCHAR path3[MAX_PATH] = {0};
	LPWSTR* files = NULL; 
	LPWSTR* mediaFiles = NULL;
	Filter* filters = NULL;
	int filtersCount = 0;
	Encoder encoder = NULL;
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
		wprintf(L"%d. [%lld] %s %s  - %s\n",i+1,tmpEnc->m_ulUid,tmpEnc->m_szName,tmpEnc->m_szVersion,tmpEnc->m_szType);
	}

	api_mmc.m_lpfnEnumerateFilters(&filterList,&filterListSize);
	printf("\n\nFilters:\n");
	for(i = 0; i < filterListSize; i++)
	{
		FilterStructPtr tmpFil = (FilterStructPtr) *(filterList + i);		
		wprintf(L"%d. [%lld] %s %s  - %s\n",i+1,tmpFil->m_ulUid,tmpFil->m_szName,tmpFil->m_szVersion,tmpFil->m_szType);
	}

	printf("\n\nPath to file:");
	wscanf_s(L"%s",path,MAX_PATH-1);
	ret =  api_mmc.m_lpfnGetEncoderForFile(path,&encoder);
	if(MMC_OK == ret)
	{
		EncoderStructPtr tmpEnc = (EncoderStructPtr) encoder;
		wprintf(L"\n\nFound encoder:\n[%lld] %s %s  - %s\n",tmpEnc->m_ulUid,tmpEnc->m_szName,tmpEnc->m_szVersion,tmpEnc->m_szType);		
	}
	else
	{
		wprintf(L"\n\nNo encoder found for that file\n");
		api_mmc.m_lpfnUnInit();
		FreeLibrary(hDll);	
		system("pause");
		return 0;
	}
	printf("\nSource file path:");
	wscanf_s(L"%s",path3,MAX_PATH-1);

	printf("\nEncoded file path:");
	wscanf_s(L"%s",path2,MAX_PATH-1);
	printf("\nChoose filters: ");	
	do{
		scanf_s("%d",&i);
		if( (i > 0) && ( i <= filterListSize ) )
		{
			filters = (Filter*) realloc(filters,sizeof(Filter) * (filtersCount + 1) );
			*(filters + filtersCount) = *(filterList + i - 1);
			filtersCount ++;
		}
	} while( (i > 0) && ( i <= filterListSize ) );

	files = (LPWSTR*) malloc(sizeof(LPWSTR));
	*files = path2;
	mediaFiles = (LPWSTR*) malloc(sizeof(LPWSTR));
	*mediaFiles = path3;

	ret = api_mmc.m_lpfnEncodeFile(path,mediaFiles,1,files,1,filters,filtersCount);

	free(mediaFiles);
	free(files);
	api_mmc.m_lpfnUnInit();
	FreeLibrary(hDll);	
	system("pause");
	
	return 0;
}
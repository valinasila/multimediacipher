/*
=================================================================================================
Filename: mmc.c
Desciption:  mmc console
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/
#include <tchar.h>
#include "mmc.h"

mmcActionPtr actions = NULL;
BOOL bWaitForKb = FALSE;


MmCAPI api_mmc;
HMODULE hDll;
Encoder* encoderList = NULL;
unsigned int encoderListSize = 0;
Filter* filterList = NULL;
unsigned int filterListSize = 0;

int AddAction(MMCOption opt, void* target)
{
	mmcActionPtr node = actions;
	mmcActionPtr prev = NULL;
	while(node)
	{
		prev = node;
		node = node->m_pNext;
	}
	
	node = (mmcActionPtr) malloc( sizeof(mmcAction) );
	node->opt = opt;
	node->m_pTarget = target;
	node->m_pNext = NULL;	

	if(NULL == prev)
		actions = node;
	else
		prev->m_pNext = node;

	return 0;
}
int RemoveHeadAction()
{
	mmcActionPtr node = actions;
	if(NULL != actions)
	{		
		if(NULL != actions->m_pTarget)
			free(actions->m_pTarget);

		actions = actions->m_pNext;
		free(node);
	}
	return 0;
}



void ShowHelp()
{
	wprintf(L"MMC Help\n");
	wprintf(L"----------------------------------------------------------------------------\n");
	wprintf(L"mmc.exe -opt1 val1 val2 val3 -opt2 ......\n");
	wprintf(L"----------------------------------------------------------------------------\n\n");
	wprintf(L"Available options:\n");
	wprintf(L"-h	(alternate: ? )			  -> prints this message\n");
	wprintf(L"-kb		->on close wait for a key to be pressed\n");
	wprintf(L"-enum [val]\n");
	wprintf(L"      encoders (alternate: -ee) ->prints all available encoders\n");
	wprintf(L"      filters  (alternate: -ef) ->prints all available filters\n");
	wprintf(L"-get  [path]	 ->prints available encoder for specific file\n");
	wprintf(L"-enc [path] /src [paths] /dst [paths] /flt [filters] -> encodes data stored in first path with src paths using filters provided by their uid and saves content in dest paths\n");
	wprintf(L"-dec [paths] -> decodes files from paths\n");
}

void EnumEncoders()
{	
	unsigned int i;
	wprintf(L"----------------------------------------------------------------------------\n");
	wprintf(L"Encoders:\n");
	for(i = 0; i < encoderListSize; i++)
	{
		EncoderStructPtr tmpEnc = (EncoderStructPtr) *(encoderList + i);
		wprintf(L"%d. [%lld] %s %s  - %s\n",i+1,tmpEnc->m_ulUid,tmpEnc->m_szName,tmpEnc->m_szVersion,tmpEnc->m_szType);
	}
	wprintf(L"----------------------------------------------------------------------------\n\n");
}
void EnumFilters()
{	
	unsigned int i;
	wprintf(L"----------------------------------------------------------------------------\n");
	printf("Filters:\n");
	for(i = 0; i < filterListSize; i++)
	{
		FilterStructPtr tmpFil = (FilterStructPtr) *(filterList + i);		
		wprintf(L"%d. [%lld] %s %s  - %s\n",i+1,tmpFil->m_ulUid,tmpFil->m_szName,tmpFil->m_szVersion,tmpFil->m_szType);
	}
	wprintf(L"----------------------------------------------------------------------------\n\n");
}

void GetEncForFile(LPWSTR path)
{
	int ret;
	Encoder encoder = NULL;
	wprintf(L"----------------------------------------------------------------------------\n");
	ret =  api_mmc.m_lpfnGetEncoderForFile(path,&encoder);
	if(MMC_OK == ret)
	{
		EncoderStructPtr tmpEnc = (EncoderStructPtr) encoder;
		wprintf(L"\n\nFound encoder:\n[%lld] %s %s  - %s\n",tmpEnc->m_ulUid,tmpEnc->m_szName,tmpEnc->m_szVersion,tmpEnc->m_szType);		
	}
	else
	{
		wprintf(L"\n\nNo encoder found for that file\n");		
	}
	wprintf(L"----------------------------------------------------------------------------\n\n");
}
int Init()
{
	int ret;
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

	ret = api_mmc.m_lpfnInit();
	if(ret != MMC_OK)
	{
		wprintf(L"Error load mmclib.dll : %d",ret);
		api_mmc.m_lpfnUnInit();
		FreeLibrary(hDll);
		system("pause");
		return 1;
	}

	api_mmc.m_lpfnEnumerateEncoders(&encoderList,&encoderListSize);
	api_mmc.m_lpfnEnumerateFilters(&filterList,&filterListSize);

	return 0;
}



int _tmain(int argc, _TCHAR* argv[])
{
	int i;
	LPWSTR tmp = NULL;
	
	if( 0 != Init() )
		return 1;

	if(argc <= 1)
		AddAction(MMC_OPT_HELP,NULL);

	for( i = 1; i < argc; i++)
	{
		if( ( wcscmp(argv[i],L"-h") == 0 ) || ( wcscmp(argv[i],L"?") == 0) )
		{
			AddAction(MMC_OPT_HELP,NULL);
		}
		if( ( wcscmp(argv[i],L"-kb") == 0 ))
		{
			bWaitForKb = TRUE;
		}
		else if ( ( wcscmp(argv[i],L"-enum") == 0 ))
		{
			if( (i + 1) < argc )
			{
				i++;
				if ( ( wcscmp(argv[i],L"encoders") == 0 ))
				{
					AddAction(MMC_OPT_ENUM_ENCODER,NULL);
				}
				else if ( ( wcscmp(argv[i],L"filters") == 0 ))
				{
					AddAction(MMC_OPT_ENUM_FILTERS,NULL);
				}
			}
		}
		else if ( ( wcscmp(argv[i],L"-ee") == 0 ))
		{
			AddAction(MMC_OPT_ENUM_ENCODER,NULL);
		}
		else if ( ( wcscmp(argv[i],L"-ef") == 0 ))
		{
			AddAction(MMC_OPT_ENUM_FILTERS,NULL);
		}
		else if( ( wcscmp(argv[i],L"-get") == 0 ) )
		{
			if( (i + 1) < argc )
			{
				i++;
				tmp = (LPWSTR) malloc(sizeof(WCHAR) * MAX_PATH);
				*tmp = L'\0';
				wcscat_s(tmp,MAX_PATH - 1, argv[i] );
				AddAction(MMC_OPT_GET_ENCODER,tmp);
			}
		}
	}
	
	
	while(actions)
	{
		switch(actions->opt)
		{
		case MMC_OPT_HELP:	ShowHelp(); break;	
		case MMC_OPT_ENUM_ENCODER: EnumEncoders(); break;
		case MMC_OPT_ENUM_FILTERS: EnumFilters(); break;
		case MMC_OPT_GET_ENCODER: GetEncForFile(actions->m_pTarget);
		default: break;
		}
		RemoveHeadAction();
	}

	api_mmc.m_lpfnUnInit();

	if(bWaitForKb)
		system("pause");
	
	return 0;
}
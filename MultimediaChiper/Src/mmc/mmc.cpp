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
	encodeStructPtr tmpEncodeStruct = NULL;
	decodeStructPtr tmpDecodeStruct = NULL;
	unsigned int i = 0;
	if(NULL != actions)
	{		
		if(MMC_OPT_ENCODE == actions->opt)
		{	
			tmpEncodeStruct = (encodeStructPtr) actions->m_pTarget;

			if(NULL == tmpEncodeStruct)
			{
				actions = actions->m_pNext;
				free(node);
				return 0;
			}
			if(NULL != tmpEncodeStruct->srcPath)
				free(tmpEncodeStruct->srcPath);
			if(NULL != tmpEncodeStruct->filterList)
			{
				free(tmpEncodeStruct->filterList);
				tmpEncodeStruct->nFilterList = 0;
			}
			if(NULL != tmpEncodeStruct->mediaFiles)
			{
				for(i = 0 ; i < tmpEncodeStruct->nMediaFiles; i++)
					free( *(tmpEncodeStruct->mediaFiles + i) );
				free(tmpEncodeStruct->mediaFiles);
				tmpEncodeStruct->nMediaFiles = 0;
			}
			if(NULL != tmpEncodeStruct->outFiles)
			{
				for(i = 0; i < tmpEncodeStruct->nOutFiles; i++)
					free( *(tmpEncodeStruct->outFiles + i) );
				free(tmpEncodeStruct->outFiles);
				tmpEncodeStruct->nOutFiles = 0;
			}

			free(tmpEncodeStruct);
			
			
		}
		else
		{
			if(MMC_OPT_DECODE == actions->opt)
			{
				tmpDecodeStruct = (decodeStructPtr) actions->m_pTarget;

				if(NULL != tmpDecodeStruct->dstPath)
					free(tmpDecodeStruct->dstPath);
				
				for(i = 0; i < tmpDecodeStruct->nMediaFiles; i++)
					free(*(tmpDecodeStruct->mediaFiles + i));

				if(NULL != tmpDecodeStruct->mediaFiles)
					free(tmpDecodeStruct->mediaFiles);

			}
			else
			{
				if(NULL != actions->m_pTarget)
					free(actions->m_pTarget);
			}
		}

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
	wprintf(L"-enc  (alternate: -encode) [path] /src [paths] /dst [paths] /flt [filters] -> encodes data stored in first path with src paths using filters provided by their uid and saves content in dest paths\n");
	wprintf(L"-dec	(alternate: -decode) [paths] /dst [path]  -> decodes files from paths\n");
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




void Encode(encodeStructPtr p)
{
	wprintf(L"----------------------------------------------------------------------------\n");
	api_mmc.m_lpfnEncodeFile(p->srcPath,p->mediaFiles,(int) p->nMediaFiles,p->outFiles,(int)p->nOutFiles, p->filterList,(int) p->nFilterList);
	wprintf(L"----------------------------------------------------------------------------\n\n");
}

void Decode(decodeStructPtr p)
{
	wprintf(L"----------------------------------------------------------------------------\n");
	api_mmc.m_lpfnDecodeFiles(p->mediaFiles,p->nMediaFiles,p->dstPath);
	wprintf(L"----------------------------------------------------------------------------\n\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i;
	unsigned int j;
	LPWSTR tmp = NULL;
	encodeStructPtr tmpEncode = NULL;
	decodeStructPtr tmpDecode = NULL;
	
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
		else if ( ( wcscmp(argv[i],L"-enc") == 0 ) || ( wcscmp(argv[i],L"-encode") == 0 ))
		{
			tmpEncode = (encodeStructPtr) malloc( sizeof(encodeStruct) );
			tmpEncode->filterList = NULL;
			tmpEncode->mediaFiles = NULL;
			tmpEncode->srcPath = NULL;
			tmpEncode->outFiles = NULL;
			tmpEncode->nFilterList = 0;
			tmpEncode->nMediaFiles = 0;
			tmpEncode->nOutFiles = 0;
			if( (i + 1) < argc )
			{
				i++;
				tmp = (LPWSTR) malloc(sizeof(WCHAR) * MAX_PATH);
				*tmp = L'\0';
				wcscat_s(tmp,MAX_PATH - 1, argv[i] );
				tmpEncode->srcPath = tmp;
				if( (i + 1) < argc )
				{
					i++;
					if( ( wcscmp(argv[i],L"/src") == 0 ) )
					{
						i++;
						while( ( i < argc) && ( wcscmp(argv[i],L"/dst") != 0 ) )
						{
							tmp = (LPWSTR) malloc(sizeof(WCHAR) * MAX_PATH);
							*tmp = L'\0';
							wcscat_s(tmp,MAX_PATH - 1, argv[i] );
							tmpEncode->nMediaFiles++;					
							tmpEncode->mediaFiles = (LPWSTR*) realloc(tmpEncode->mediaFiles, sizeof(LPWSTR) * tmpEncode->nMediaFiles );
							*(tmpEncode->mediaFiles + (tmpEncode->nMediaFiles - 1) ) = tmp;  
							i++;
						}
						if( i >= argc)
						{							
							for(j = 0; j < tmpEncode->nMediaFiles; j++)
								free( *(tmpEncode->mediaFiles + j ) );

							free(tmpEncode->mediaFiles);
							free(tmpEncode->srcPath);
							free(tmpEncode);
							tmpEncode = NULL;
						}
						else
						{
							i++;
							while( ( i < argc) && ( wcscmp(argv[i],L"/flt") != 0 ) )
							{
								tmp = (LPWSTR) malloc(sizeof(WCHAR) * MAX_PATH);
								*tmp = L'\0';
								wcscat_s(tmp,MAX_PATH - 1, argv[i] );
								tmpEncode->nOutFiles++;					
								tmpEncode->outFiles = (LPWSTR*) realloc(tmpEncode->outFiles, sizeof(LPWSTR*) * tmpEncode->nOutFiles);
								*(tmpEncode->outFiles + (tmpEncode->nOutFiles - 1) ) = tmp;  
								i++;
							}
							if(tmpEncode->nOutFiles == 0)
							{
								for(j = 0; j < tmpEncode->nMediaFiles; j++)
									free( *(tmpEncode->mediaFiles + j ) );

								free(tmpEncode->mediaFiles);
								free(tmpEncode->srcPath);
								free(tmpEncode);
								tmpEncode = NULL;
							}
							else
							{
								if( (i + 1) < argc)
								{
									i++;
									while( ( i < argc) && ( argv[i][0] != L'-' ) )
									{								
										tmpEncode->nFilterList++;				
										for(j = 0; j <filterListSize; j++)
										{
											if( ((FilterStructPtr) *(filterList + j) )->m_ulUid == _wtoi64(argv[i])  )
											{
												tmpEncode->filterList = (Filter*) realloc(tmpEncode->filterList, sizeof(Filter) * tmpEncode->nFilterList);
												*(tmpEncode->filterList + (tmpEncode->nFilterList - 1) ) = *(filterList + j);  
												break;
											}
										}
										i++;
									}
								}
								AddAction(MMC_OPT_ENCODE,tmpEncode);
							}
						}
					}
					else
					{
						free(tmpEncode->srcPath);
						free(tmpEncode);
						tmpEncode = NULL;
					}
				}
				else
				{
					free(tmpEncode->srcPath);
					free(tmpEncode);
					tmpEncode = NULL;
				}
			}
			else
			{
				free(tmpEncode);
				tmpEncode = NULL;
			}
		}
		else if ( ( wcscmp(argv[i],L"-dec") == 0 ) || ( wcscmp(argv[i],L"-decode") == 0 ))
		{
			if( (i + i) <argc )
			{
				i++;
				while( ( i < argc) && ( wcscmp(argv[i],L"/dst") != 0 ) )
				{
					tmp = (LPWSTR) malloc(sizeof(WCHAR) * MAX_PATH);
					*tmp = L'\0';
					wcscat_s(tmp,MAX_PATH - 1, argv[i] );
					tmpDecode->nMediaFiles++;					
					tmpDecode->mediaFiles = (LPWSTR*) realloc(tmpDecode->mediaFiles, sizeof(LPWSTR) * tmpDecode->nMediaFiles );
					*(tmpDecode->mediaFiles + (tmpDecode->nMediaFiles - 1) ) = tmp;  
					i++;
				}
				if( (i + i) <argc )
				{
					i++;
					tmp = (LPWSTR) malloc(sizeof(WCHAR) * MAX_PATH);
					*tmp = L'\0';
					wcscat_s(tmp,MAX_PATH - 1, argv[i] );
					tmpDecode->dstPath = tmp;
					AddAction(MMC_OPT_DECODE,tmpDecode);
				}
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
		case MMC_OPT_ENCODE:	Encode(actions->m_pTarget);
		case MMC_OPT_DECODE:	Decode(actions->m_pTarget);
		default: break;
		}
		RemoveHeadAction();
	}

	api_mmc.m_lpfnUnInit();

	if(bWaitForKb)
		system("pause");
	
	return 0;
}
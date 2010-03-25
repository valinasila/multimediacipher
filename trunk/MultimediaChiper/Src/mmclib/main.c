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
				m_Encoders = (Encoder*) realloc(m_Encoders, sizeof(Encoder) * (EncodersSize + 1) );
				if(! m_Encoders)
				{
					free(dllPath);	
					free(dllDir);
					return MMC_MEMORY_ERROR;
				}
				*(m_Encoders + EncodersSize) = node->m_pEncoder;
				EncodersSize ++;
				node->m_pNext = encodersList;
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
	int bRunning = 1;	
	HANDLE hFile = NULL;	
	WIN32_FIND_DATA wfd;
	HMODULE* tmpDll;	
	LPWSTR dllPath = (LPWSTR) malloc(sizeof(WCHAR) * MAX_PATH);
	LPWSTR dllDir = (LPWSTR) malloc(sizeof(WCHAR) * ( wcslen(filtersDir) + 6 ) );

	memcpy(dllDir , filtersDir , sizeof(WCHAR) * wcslen(filtersDir) );
	memcpy(dllDir + wcslen(filtersDir),L"*.dll", sizeof(WCHAR) * 5);
	dllDir[wcslen(filtersDir) + 5] = L'\0';
	memcpy(dllPath , dllDir , sizeof(WCHAR) * wcslen(dllDir) );

	ReleaseFilters();
	
	do{
		if(hFile == NULL)
		{
			hFile = FindFirstFile(dllDir,&wfd);
			if(INVALID_HANDLE_VALUE == hFile)
			{
				free(dllPath);	
				free(dllDir);
				return MMC_WRONG_FILTERS_FOLDER;
			}
		}
		else
		{
			if( ! FindNextFile(hFile,&wfd) )
				break;
		}				
		memcpy(dllPath + wcslen(filtersDir) , wfd.cFileName , sizeof(WCHAR) * wcslen(wfd.cFileName)); 
		dllPath[wcslen(filtersDir) + wcslen(wfd.cFileName)] = L'\0';	
		tmpDll = (HMODULE*) malloc(sizeof(HMODULE));
		if( NULL ==(*tmpDll = LoadLibrary(dllPath)) )
		{
			free(tmpDll);
		}
		else
		{
			FiltersNodePtr node = (FiltersNodePtr) malloc(sizeof(FiltersNode));
			node->m_dllHandle = tmpDll;
			if( MMC_OK !=  LoadFiltersAPI(tmpDll,&node->m_API ) )
			{
				free(node);
			}
			else
			{
				node->m_API.m_lpfnInit();
				node->m_API.m_lpfnSetSaveTempBufferFn(SaveTempBuffer);
				node->m_API.m_lpfnSetGetTempBufferFn(GetTempBuffer);
				node->m_API.m_lpfnSetCloseTempBufferFn(CloseTempBuffer);
				node->m_pFilter = node->m_API.m_lpfnGetFilter();
				m_Filters = (Filter*) realloc(m_Filters, sizeof(Filter) * (FiltersSize + 1) );
				if(! m_Filters)
				{
					free(dllPath);	
					free(dllDir);
					return MMC_MEMORY_ERROR;
				}
				*(m_Filters + FiltersSize) = node->m_pFilter;
				FiltersSize ++;
				node->m_pNext = filtersList;
				filtersList = node;
			}
		}
	} while(1);

	free(dllPath);	
	free(dllDir);
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
	if(m_Encoders)
		free(m_Encoders);
	EncodersSize = 0;
	return MMC_OK;
}
int ReleaseFilters()
{
	FiltersNodePtr node = filtersList;
	while(node)
	{
		filtersList = node->m_pNext;
		node->m_API.m_lpfnUnInit();		
		FreeLibrary(*node->m_dllHandle);
		free(node->m_dllHandle);
		free(node);
		node = filtersList;
	}
	if(m_Filters)
		free(m_Filters);
	FiltersSize = 0;
	return MMC_OK;
}
int LoadEncodersAPI(HMODULE* dll, EncoderAPI* api)
{
#define CHECK_ENC_DLL(x) if(NULL == x) return MMC_WRONG_ENCODER_LIBRARY;
	api->m_lpfnIsEncoder			= (isEncoderFn) GetProcAddress(*dll,"IsEncoder");						CHECK_ENC_DLL(api->m_lpfnIsEncoder)
	api->m_lpfnInit					= (initEncoderFn) GetProcAddress(*dll,"Init");							CHECK_ENC_DLL(api->m_lpfnInit)
	api->m_lpfnUnInit				= (uninitEncoderFn) GetProcAddress(*dll,"UnInit");						CHECK_ENC_DLL(api->m_lpfnUnInit)
	api->m_lpfnGetEncoder			= (getEncoderFn) GetProcAddress(*dll,"GetEncoder");						CHECK_ENC_DLL(api->m_lpfnGetEncoder)
	api->m_lpfnGetEncoderSignature	= (getEncoderSignatureFn) GetProcAddress(*dll,"GetEncoderSignature");	CHECK_ENC_DLL(api->m_lpfnGetEncoderSignature)
	api->m_lpfnSetAction			= (setEncoderActionFn) GetProcAddress(*dll,"SetAction");						CHECK_ENC_DLL(api->m_lpfnSetAction)
	api->m_lpfnSetBuffer			= (setEncoderBufferFn) GetProcAddress(*dll,"SetBuffer");						CHECK_ENC_DLL(api->m_lpfnSetBuffer)	
	api->m_lpfnGetBuffer			= (getEncoderBufferFn) GetProcAddress(*dll,"GetBuffer");						CHECK_ENC_DLL(api->m_lpfnGetBuffer)	
	if(ENC_RET_IsEncoder  != api->m_lpfnIsEncoder() )
		return MMC_WRONG_ENCODER_LIBRARY;
	
	return MMC_OK;
}
int LoadFiltersAPI(HMODULE* dll, FilterAPI* api)
{
#define CHECK_FIL_DLL(x) if(NULL == x) return MMC_WRONG_FILTER_LIBRARY;
	api->m_lpfnIsFilter		= (isFilterFn) GetProcAddress(*dll,"IsFilter");		CHECK_FIL_DLL(api->m_lpfnIsFilter)
	api->m_lpfnInit			= (initFilterFn) GetProcAddress(*dll,"Init");		CHECK_FIL_DLL(api->m_lpfnInit)
	api->m_lpfnUnInit		= (uninitFilterFn) GetProcAddress(*dll,"UnInit");	CHECK_FIL_DLL(api->m_lpfnUnInit)
	api->m_lpfnGetFilter	= (getFilterFn) GetProcAddress(*dll,"GetFilter");	CHECK_FIL_DLL(api->m_lpfnGetFilter)
	api->m_lpfnSetSaveTempBufferFn	= (setSaveTempBufferFn) GetProcAddress(*dll,"SetSaveTempBufferFn");	CHECK_FIL_DLL(api->m_lpfnSetSaveTempBufferFn)
	api->m_lpfnSetGetTempBufferFn	= (setGetTempBufferFn) GetProcAddress(*dll,"SetGetTempBufferFn");	CHECK_FIL_DLL(api->m_lpfnSetGetTempBufferFn)
	api->m_lpfnSetCloseTempBufferFn	= (setCloseTempBufferFn) GetProcAddress(*dll,"SetCloseTempBufferFn");	CHECK_FIL_DLL(api->m_lpfnSetCloseTempBufferFn)
	api->m_lpfnSetAction	= (setFilterActionFn) GetProcAddress(*dll,"SetFilterAction");	CHECK_FIL_DLL(api->m_lpfnSetAction)
	api->m_lpfnSetBuffer	= (setFilterBufferFn) GetProcAddress(*dll,"SetFilterBuffer");	CHECK_FIL_DLL(api->m_lpfnSetBuffer)
	api->m_lpfnGetBuffer	= (getFilterBufferFn) GetProcAddress(*dll,"GetFilterBuffer");	CHECK_FIL_DLL(api->m_lpfnGetBuffer)
	api->m_lpfnSetInputBuffer = (setFilterInputBufferFn) GetProcAddress(*dll,"SetFilterInputBuffer");	CHECK_FIL_DLL(api->m_lpfnSetInputBuffer)
	if(FIL_RET_IsFilter  != api->m_lpfnIsFilter() )
		return MMC_WRONG_FILTER_LIBRARY;
	
	return MMC_OK;
}
int EnumerateEncoders(Encoder** encoders, unsigned int* size)
{
	*encoders = m_Encoders;
	*size = EncodersSize;
	return MMC_OK;
}
int EnumerateFilters(Filter** filters, unsigned int* size)
{
	*filters = m_Filters;
	*size = FiltersSize;
	return MMC_OK;
}
int EncodeFile(LPCWSTR sourceFile, LPCWSTR* destFiles, int nDestFiles, const Filter** useFilters, int nFilters)
{
	return MMC_OK;
}
int DecodeFiles(LPCWSTR* sourceFiles, int nSourceFiles)
{
	return MMC_OK;
}
int GetEncoderForFile(LPCWSTR filePath, Encoder* encoder)
{
	EncodersNodePtr node;
	Encoder	retEncoder = NULL;
	unsigned char* buffer = NULL;
	DWORD size = 0;
	unsigned long long max_size = 0;
	
	EncoderSignaturePtr signature;

	HANDLE hFile = CreateFile(filePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if( INVALID_HANDLE_VALUE == hFile)
	{
		return MMC_READ_FILE_ERROR;
	}
	
	node = encodersList;
	while(node)
	{
		signature = node->m_API.m_lpfnGetEncoderSignature();
		if(max_size > ( signature->m_ulSignatureStartPos + signature->m_ulSignatureSize ) )
			max_size = signature->m_ulSignatureStartPos + signature->m_ulSignatureSize;
		node = node->m_pNext;
	}
	
	buffer = (unsigned char*) malloc( sizeof(unsigned char) * ((size_t)(max_size+1)) );
	if(ReadFile(hFile,buffer,(DWORD)(max_size+1),&size,NULL) )
	{
		node = encodersList;
		while(node)
		{
			signature = node->m_API.m_lpfnGetEncoderSignature();
			if( 0 == memcmp(signature->m_Signature,buffer+signature->m_ulSignatureStartPos, (size_t) signature->m_ulSignatureSize) )
			{
				retEncoder = node->m_pEncoder;
				break;
			}
			node = node->m_pNext;
		}
	}
	free(buffer);
	if(NULL == retEncoder)
		return MMC_ENCODER_NOT_FOUND;
	*encoder = retEncoder;
	return MMC_OK;
}
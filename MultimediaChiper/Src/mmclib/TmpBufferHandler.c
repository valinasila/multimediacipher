/*
=================================================================================================
Filename: TmpBufferHandler.c
Desciption:  functions to handle temporary buffers sent by filters
Author: $Author: claudiu.guiman $
Last changed by:    $Author: claudiu.guiman $
Last changed date:    $Date: 2010-03-14 19:33:19 +0200 (D, 14 mar. 2010) $
ID:            $Id: mmc.h 14 2010-03-14 17:33:19Z claudiu.guiman $
=================================================================================================
*/
#include "TmpBufferhandler.h"

TmpFileStructPtr fileList = NULL;

int InitTempBufferHandler()
{
	fileList = NULL;
	return 0;
}
int SaveTempBuffer(Filter fltr,const unsigned char* buffer, unsigned int bufferSize)
{
	TmpFileStructPtr node = fileList;
	DWORD bw;

	while(node)
	{
		if(node->m_pFlt == fltr)
			break;
		node = node->m_pNext;
	}
	if(NULL == node) // create file
	{
		WCHAR path[MAX_PATH] = {0};
		HANDLE* hndl = (HANDLE*) malloc(sizeof(HANDLE));
		node = (TmpFileStructPtr) malloc(sizeof(TmpFileStruct));
		node->m_pFlt = fltr;
		node->bFirstRead = TRUE;
		fileList = node;
		CreateDirectory(L"temp",NULL);
		_snwprintf_s(path,MAX_PATH,MAX_PATH - 1,L"temp\\filter_%lld.tmp",((FilterStructPtr) fltr)->m_ulUid);		

		*hndl = CreateFile(path,GENERIC_READ | GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE,NULL);

		if(INVALID_HANDLE_VALUE == *hndl)
		{
			free(node);
			free(hndl);
			return -1;
		}
		node->m_pHFile = hndl;

		node->m_pNext = fileList;
	}
	bw = 0;
	WriteFile(*node->m_pHFile,buffer, bufferSize,&bw,NULL);
	if(bw == 0 && bufferSize > 0)
		return -2;
	return 0;
}
int GetTempBuffer(Filter fltr,unsigned char* buffer, unsigned int bufferSize, unsigned int* bytesWrote)
{
	TmpFileStructPtr node = fileList;	
	while(node)
	{
		if(node->m_pFlt == fltr)
			break;
		node = node->m_pNext;
	}
	if(NULL == node)
		return -1;

	if(TRUE == node->bFirstRead)
	{
		node->bFirstRead = FALSE;
		if(INVALID_SET_FILE_POINTER == SetFilePointer(*node->m_pHFile,0,NULL,FILE_BEGIN) )
			return -2;
	}	
	if(!ReadFile(*node->m_pHFile,buffer,bufferSize,bytesWrote,NULL) )
		return -3;
	return 0;
}
int CloseTempBuffer(Filter fltr)
{
	TmpFileStructPtr node;	
	TmpFileStructPtr prev = NULL;
	node = fileList;
	while(node)
	{		
		if( node->m_pFlt == fltr )
			break;
		prev = node;
		node = node->m_pNext;
	}
	if(node == NULL)
		return 0;

	CloseHandle( *node->m_pHFile );
	free(node->m_pHFile);
	if(NULL != prev)
		prev->m_pNext = node->m_pNext;	
	else
		fileList = fileList->m_pNext;
	
	free(node);
	return 0;
}
int UnInitTempBufferhandler()
{
	while(NULL != fileList)
	{
		CloseTempBuffer(fileList->m_pFlt);
	}
	return 0;
}
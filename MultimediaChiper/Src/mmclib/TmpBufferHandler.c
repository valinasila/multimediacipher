/*
=================================================================================================
Filename: TmpBufferHandler.c
Desciption:  functions to handle temporary buffers sent by filters
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/
#include "TmpBufferHandler.h"

int maxId = 0;
TmpFileStructPtr fileList = NULL;

int InitTempBufferHandler()
{
	fileList = NULL;
	return 0;
}
int GetNewTempHandle(TempHandle* hndl)
{
	TmpHandleStructPtr p = (TmpHandleStructPtr) malloc( sizeof(TmpHandleStruct) );
	maxId++;
	p->id = maxId;
	*hndl = p;
	return 0;

}
int DropTempHandle(TempHandle hndl)
{
	free(hndl);
	return 0;
}
int SaveTempBuffer(TempHandle hndl,const unsigned char* buffer, unsigned int bufferSize)
{
	TmpFileStructPtr node = fileList;
	DWORD bw;

	while(node)
	{
		if(node->hndl == hndl)
			break;
		node = node->m_pNext;
	}
	if(NULL == node) // create file
	{
		WCHAR path[MAX_PATH] = {0};
		HANDLE* hFile = (HANDLE*) malloc(sizeof(HANDLE));
		node = (TmpFileStructPtr) malloc(sizeof(TmpFileStruct));
		node->hndl = hndl;
		node->bFirstRead = TRUE;
		fileList = node;
		CreateDirectory(L"temp",NULL);
		_snwprintf_s(path,MAX_PATH,MAX_PATH - 1,L"temp\\tmp_%d.tmp",((TmpHandleStructPtr) hndl)->id);		

		*hFile = CreateFile(path,GENERIC_READ | GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE,NULL);

		if(INVALID_HANDLE_VALUE == *hFile)
		{
			free(node);
			free(hFile);
			return -1;
		}
		node->m_pHFile = hFile;

		node->m_pNext = fileList;
	}
	bw = 0;
	WriteFile(*node->m_pHFile,buffer, bufferSize,&bw,NULL);
	if(bw == 0 && bufferSize > 0)
		return -2;
	return 0;
}
int GetTempBuffer(TempHandle hndl,unsigned char* buffer, unsigned int bufferSize, unsigned int* bytesWrote)
{
	TmpFileStructPtr node = fileList;	
	while(node)
	{
		if(node->hndl == hndl)
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
int CloseTempBuffer(TempHandle hndl)
{
	TmpFileStructPtr node;	
	TmpFileStructPtr prev = NULL;
	node = fileList;
	while(node)
	{		
		if( node->hndl == hndl )
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
int UnInitTempBufferHandler()
{
	while(NULL != fileList)
	{
		CloseTempBuffer(fileList->hndl);		
	}
	return 0;
}
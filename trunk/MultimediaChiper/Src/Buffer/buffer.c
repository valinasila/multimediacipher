/*
=================================================================================================
Filename: buffer,c
Desciption: implementation of buffer
Author: $Author: claudiu.guiman $
Last changed by:    $Author: claudiu.guiman $
Last changed date:    $Date: 2010-03-12 10:09:18 +0200 (V, 12 mar. 2010) $
ID:            $Id: filter.h 6 2010-03-12 08:09:18Z claudiu.guiman $
=================================================================================================
*/

#include "Buffer.h"

int InitBuffer(BufferInstance* inst)
{
	BufferInstanceStructPtr ret = (BufferInstanceStructPtr) malloc ( sizeof(BufferInstanceStruct));
	ret->m_pFreeBuffers = NULL;
	ret->m_pEmptyNodes = NULL;
	ret->m_pThis = ret;
	*inst = ret;	
	return BUFF_RET_OK;
}
int GetBuffer(BufferInstance inst, Buffer* buffer)
{
	BufferInstanceStructPtr instance = (BufferInstanceStructPtr)*inst;
	BufferStructPtr retBuffer = NULL;
	BufferFreeNodePtr node;
	if( NULL == instance->m_pFreeBuffers)
	{
		retBuffer = (BufferStructPtr) malloc(sizeof(BufferStruct));		
		retBuffer->m_pThis = retBuffer;
		retBuffer->m_Len = 0;
		retBuffer->m_pData = (unsigned char*) malloc( 1024);
		retBuffer->m_Size = 1024;
	}
	else
	{
		node = instance->m_pFreeBuffers;
		retBuffer = (BufferStructPtr)node->m_pBuffer;
		instance->m_pFreeBuffers = node->m_pNext;
		node->m_pNext = instance->
	}
	return BUFF_RET_OK;
}
int SetBuffer(BufferInstance inst, Buffer* buffer)
{
	return BUFF_RET_OK;
}
int UnInitBuffer(BufferInstance* inst)
{
	BufferFreeNodePtr node;
	EmptyNodePtr	  node2;
	BufferInstanceStructPtr instance = (BufferInstanceStructPtr)*inst;
	BufferStructPtr buff;
	node = instance->m_pFreeBuffers;
	while(node)
	{
		instance->m_pFreeBuffers = node->m_pNext;
		buff = (BufferStructPtr) node->m_pBuffer;
		free(buff->m_pData);
		free(node->m_pBuffer);
		free(node);
		node = instance->m_pFreeBuffers;
	}
	node2 = instance->m_pEmptyNodes;
	while(node2)
	{
		instance->m_pEmptyNodes = node2->m_pNext;
		free(node2);
		node2 = instance->m_pEmptyNodes;
	}
	free(*inst);
	*inst = NULL;
	return BUFF_RET_OK;
}
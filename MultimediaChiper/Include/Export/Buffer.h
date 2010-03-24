/*
=================================================================================================
Filename: Buffer.h
Desciption: definitions for buffer
Author: $Author: claudiu.guiman $
Last changed by:    $Author: claudiu.guiman $
Last changed date:    $Date: 2010-03-12 10:09:18 +0200 (V, 12 mar. 2010) $
ID:            $Id: filter.h 6 2010-03-12 08:09:18Z claudiu.guiman $
=================================================================================================
*/
#ifndef _BUFFER_DEF
#define _BUFFER_DEF 1
#include <windows.h>

typedef void*	Buffer;
typedef struct _BufferStruct BufferStruct, *BufferStructPtr;
struct _BufferStruct{
	BufferStructPtr		m_pThis;
	unsigned long long	m_Len;
	unsigned long long	m_Size;
	unsigned char*		m_pData;
};
typedef struct _bufferFreeNode BufferFreeNode,*BufferFreeNodePtr;
struct _bufferFreeNode{
	Buffer				m_pBuffer;
	BufferFreeNodePtr	m_pNext;
};

typedef struct _emptyNode EmptyNode,*EmptyNodePtr;
struct _emptyNode{
	void*			m_pData;
	EmptyNodePtr	m_pNext;
};

typedef void*	BufferInstance;
typedef struct _bufferInstanceStruct BufferInstanceStruct, *BufferInstanceStructPtr;
struct _bufferInstanceStruct
{
	BufferInstanceStructPtr		m_pThis;
	BufferFreeNodePtr			m_pFreeBuffers;
	EmptyNodePtr				m_pEmptyNodes;
};

int InitBuffer(BufferInstance* inst);
int GetBuffer(BufferInstance inst, Buffer* buffer);
int SetBuffer(BufferInstance inst, Buffer* buffer);
int UnInitBuffer(BufferInstance* inst);

#define BUFF_RET_OK 0
#define BUFF_RET_MEMORY_ERROR 1
#endif
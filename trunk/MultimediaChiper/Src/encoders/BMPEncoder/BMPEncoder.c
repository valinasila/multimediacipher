/*
=================================================================================================
Filename: BMPEncoder.c
Desciption: 
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/
#include "BMPEncoder.h"


EncoderRet IsEncoder()
{
	return ENC_RET_IsEncoder;
}
EncoderRet Init()
{
	m_pInternalStruct = (EncoderStructPtr) malloc( sizeof(EncoderStruct) );
	m_pInternalStruct->m_pThis = m_pInternalStruct;
	m_pInternalStruct->m_ulUid = ENCODER_UID;
	m_pInternalStruct->m_szName = (LPWSTR) malloc( sizeof(WCHAR) * ( wcslen(ENCODER_NAME) + 1 ) );
	memcpy( m_pInternalStruct->m_szName , ENCODER_NAME , sizeof(WCHAR) * wcslen(ENCODER_NAME) );
	m_pInternalStruct->m_szName[wcslen(ENCODER_NAME)] = L'\0';
	m_pInternalStruct->m_szType = (LPWSTR) malloc( sizeof(WCHAR) * (wcslen(ENCODER_TYPE) + 1 ) );
	memcpy( m_pInternalStruct->m_szType , ENCODER_TYPE , sizeof(WCHAR) * wcslen(ENCODER_TYPE) );
	m_pInternalStruct->m_szType[wcslen(ENCODER_TYPE)] = L'\0';
	m_pInternalStruct->m_szVersion = (LPWSTR) malloc( sizeof(WCHAR) * (wcslen(ENCODER_VERSION) + 1 ) );	
	memcpy( m_pInternalStruct->m_szVersion , ENCODER_VERSION , sizeof(WCHAR) * wcslen(ENCODER_VERSION) );
	m_pInternalStruct->m_szVersion[wcslen(ENCODER_VERSION)] = L'\0';

	m_pSignature = (EncoderSignaturePtr) malloc(sizeof(EncoderSignature) );
	m_pSignature->m_Signature = (unsigned char*) malloc(sizeof(unsigned char) * 2);
	memcpy(m_pSignature->m_Signature,"BM",2);
	m_pSignature->m_ulSignatureSize = 2;
	m_pSignature->m_ulSignatureStartPos = 0;

	tempBuffer = (unsigned char*) malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
	if(NULL == tempBuffer)
		return ENC_RET_UnknownError;
	tempBufferSize = 0;

	sourceBuffer = (unsigned char*) malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
	if(NULL == sourceBuffer)
		return ENC_RET_UnknownError;
	sourceBufferSize = 0;
		
	m_bFirstBuffer = TRUE;
	offset = 0;

	return ENC_RET_OK;
}
Encoder GetEncoder()
{
	return (Encoder) m_pInternalStruct;
}
EncoderSignaturePtr GetEncoderSignature()
{
	return m_pSignature;
}
EncoderRet	UnInit()
{
	if(m_pInternalStruct)
	{
		free(m_pInternalStruct->m_szName);
		free(m_pInternalStruct->m_szType);
		free(m_pInternalStruct->m_szVersion);
		free(m_pInternalStruct);
	}
	if(m_pSignature)
	{
		if(m_pSignature->m_Signature)
			free(m_pSignature->m_Signature);
		free(m_pSignature);
	}
	if(NULL != tempBuffer)
		free(tempBuffer);

	tempBuffer = NULL;
	return ENC_RET_OK;
}
EncoderRet SetAction(int bEncode)
{
	m_bEncode = bEncode;
	return ENC_RET_OK;
}
EncoderRet SetSourceBuffer(const unsigned char* buffer, unsigned int bufferSize)
{
	if(NULL == sourceHandle)
		return ENC_RET_WrongArgument;

	pTmpApi->m_lpfnSaveTempBuffer(sourceHandle,buffer,bufferSize);
	m_sourceBufferSize += (unsigned long long) bufferSize;

	return ENC_RET_OK;
}
EncoderRet SetBuffer(const unsigned char* buffer, unsigned int bufferSize)
{
	int i;

	if(NULL == envelopeHandle)
		return ENC_RET_WrongArgument;
	if(NULL == sourceHandle)
		return ENC_RET_WrongArgument;

	if(m_bEncode)
	{
		// encode
		if(m_bFirstBuffer)
		{
			if(offset == 0)
			{
				fileStruct.bitsPerPixel = 0;
				fileStruct.header_size = 0;
				fileStruct.height = 0;
				fileStruct.imageSize = 0;
				fileStruct.offset = 0;
				fileStruct.size = 0;
				fileStruct.width = 0;
			}

			if( (bufferSize + offset) > DEFAULT_BUFFER_SIZE )
				tempBuffer = realloc(tempBuffer,bufferSize+offset);

			memcpy(tempBuffer+offset,buffer,bufferSize);
			tempBufferSize += bufferSize;			
			if(tempBufferSize > 2)
			{
				if( (tempBuffer[0] != 'B') || (tempBuffer[1] != 'M') )
				{
					return ENC_RET_Wrong_File_Format;
				}				
				offset = 2;
				if(tempBufferSize > 0x12) // buffer contains header size
				{
					fileStruct.header_size = *(unsigned int*)(tempBuffer + 0xE);
					fileStruct.offset	   = *(unsigned int*)(tempBuffer + 0xA);
					fileStruct.size		   = *(unsigned int*)(tempBuffer + 0x2);	
					if(tempBufferSize > (int)(0x12 + fileStruct.header_size))
					{ // entire header is in tempBuffer						
						pTmpApi->m_lpfnSaveTempBuffer(envelopeHandle,tempBuffer,0x12 + fileStruct.header_size);
						fileStruct.width  = *(int*)(tempBuffer + 0x12);
						fileStruct.height = *(int*)(tempBuffer + 0x16);
						fileStruct.bitsPerPixel = *(unsigned short*)(tempBuffer + 0x1C);
						if(fileStruct.bitsPerPixel != 24)
						{
							return ENC_RET_Not_Implemented;
						}
						for(i = fileStruct.offset; i < tempBufferSize; i++)
							*(tempBuffer + (i -  fileStruct.offset) ) = *(tempBuffer + i);
						offset = tempBufferSize;
						tempBufferSize -= fileStruct.offset;
						m_bFirstBuffer = FALSE;
						if(m_sourceBufferSize > (unsigned long long) (fileStruct.imageSize / 4))
							return ENC_RET_Insuficient_Size; 
					}
					
				}
			}					
			
		} // [if m_bFirstBuffer]
		else
		{
			
			if( (bufferSize + offset) > DEFAULT_BUFFER_SIZE )
				tempBuffer = realloc(tempBuffer,bufferSize+offset);
			memcpy(tempBuffer+offset,buffer,bufferSize);
			tempBufferSize +=bufferSize;
			offset = 0;
			for(i = 0; i < tempBufferSize; i++)
			{
				if( (sourceBufferSize == 0) ||  (sourceOffset > sourceBufferSize) )
				{
					pTmpApi->m_lpfnGetTempBuffer(sourceHandle,sourceBuffer,DEFAULT_BUFFER_SIZE,&sourceBufferSize);
					sourceOffset = 0;
					bitOffset = 0;
				}
				if(bitOffset == 0)
				{
					*(tempBuffer  + i) = (*(tempBuffer + i) & 0xfc) | ( *(sourceBuffer + sourceOffset) & 0x3 );
					bitOffset = 2;
				}
				else
				{
					if(bitOffset == 2)
					{
						*(tempBuffer  + i) = (*(tempBuffer + i) & 0xfc) | ( *(sourceBuffer + sourceOffset) & 0xC );
						bitOffset = 4;
					}
					else
					{
						if(bitOffset == 4)
						{
							*(tempBuffer  + i) = (*(tempBuffer + i) & 0xfc) | ( *(sourceBuffer + sourceOffset) & 0x30 );
							bitOffset = 6;
						}
						else
						{
							if(bitOffset == 6)
							{
								*(tempBuffer  + i) = (*(tempBuffer + i) & 0xfc) | ( *(sourceBuffer + sourceOffset) & 0xC0 );
								bitOffset = 0;
								sourceOffset ++;
							}
						}
					}
				}
			}	
			pTmpApi->m_lpfnSaveTempBuffer(envelopeHandle,tempBuffer,tempBufferSize);
			tempBufferSize = 0;
			
		}
	}
	return ENC_RET_OK;
}
EncoderRet GetBuffer(unsigned char* buffer, unsigned int bufferSize,unsigned int* bytesWrote)
{
	if(NULL == envelopeHandle)
		return ENC_RET_WrongArgument;
	pTmpApi->m_lpfnGetTempBuffer(envelopeHandle,buffer,bufferSize,bytesWrote);
	return ENC_RET_OK;
}
EncoderRet ReloadEncoder()
{
	pTmpApi->m_lpfnCloseTempHandle( sourceHandle);
	pTmpApi->m_lpfnCloseTempHandle( envelopeHandle);
	m_bFirstBuffer = TRUE;
	m_sourceBufferSize = 0;
	tempBufferSize = 0;
	sourceBufferSize = 0;
	sourceOffset = 0;
	bitOffset = 0;	
	offset = 0;
	return ENC_RET_OK;
}
EncoderRet SetTempFn(TempHandlerAPIPtr api)
{
	pTmpApi = api; 

	if(NULL == pTmpApi->m_lpfnGetTempHandle)
		return ENC_RET_WrongArgument;

	pTmpApi->m_lpfnGetTempHandle(&sourceHandle);
	pTmpApi->m_lpfnGetTempHandle(&envelopeHandle);
	return ENC_RET_OK;
}
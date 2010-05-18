/*
=================================================================================================
Filename: main.c
Desciption: 
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/
#include "genericEncoder.h"


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
	m_pSignature->m_Signature = NULL;
	m_pSignature->m_ulSignatureSize = 0;
	m_pSignature->m_ulSignatureStartPos = 0;
	
	
	m_bEncode = TRUE;
	m_bFirstBuffer = TRUE;
	m_bIsFirstSourceBuffer = TRUE;
	m_sourceBufferSize = 0;
	m_bIsFirstDecodeBuffer = TRUE;
	sourceHandle = NULL;
	envelopeHandle = NULL;
	return ENC_RET_OK;
}
EncoderRet ReloadEncoder()
{
	pTmpApi->m_lpfnCloseTempHandle( sourceHandle);
	pTmpApi->m_lpfnCloseTempHandle( envelopeHandle);
	m_bEncode = TRUE;
	m_bFirstBuffer = TRUE;
	m_bIsFirstSourceBuffer = TRUE;
	m_bIsFirstDecodeBuffer = TRUE;
	m_sourceBufferSize = 0;
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
	if(NULL != pTmpApi->m_lpfnDropTempHandle)
	{
		pTmpApi->m_lpfnDropTempHandle(sourceHandle);
		pTmpApi->m_lpfnDropTempHandle(envelopeHandle);

	}
	

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
	if(NULL == envelopeHandle)
			return ENC_RET_WrongArgument;

	if(m_bEncode)
	{
		// encode		
		pTmpApi->m_lpfnSaveTempBuffer(envelopeHandle,buffer,bufferSize);

	}
	else
	{
		//decode
		if(m_bIsFirstDecodeBuffer)
		{
			unsigned char* tmp = NULL;
			m_bIsFirstDecodeBuffer = FALSE;
			tmp = strstr(buffer,"MMC");
			if(NULL == tmp);
				return ENC_RET_UnknownError;
			tmp += 3;
			m_sourceBufferSize = *((unsigned long long*) tmp);
			tmp += sizeof(unsigned long long);
			if(m_sourceBufferSize > (unsigned long long)(bufferSize - 3 - sizeof(unsigned long long) ) )
			{
				pTmpApi->m_lpfnSaveTempBuffer(envelopeHandle,tmp,(unsigned int) m_sourceBufferSize );
				m_sourceBufferSize = 0;
			}
			else
			{
				pTmpApi->m_lpfnSaveTempBuffer(envelopeHandle,tmp,bufferSize - 3 - sizeof(unsigned long long) );
				m_sourceBufferSize -= (3 + sizeof(unsigned long long) );
			}
		}
		else
		{
			if(m_sourceBufferSize > 0)
			{
				if(m_sourceBufferSize > (unsigned long long)bufferSize  )
				{
					pTmpApi->m_lpfnSaveTempBuffer(envelopeHandle,buffer,(unsigned int) m_sourceBufferSize );
					m_sourceBufferSize = 0;
				}
				else
				{
					pTmpApi->m_lpfnSaveTempBuffer(envelopeHandle,buffer,bufferSize );
					m_sourceBufferSize -= bufferSize;
				}
			}
		}
	}
	return ENC_RET_OK;
}
EncoderRet GetBuffer(unsigned char* buffer, unsigned int bufferSize,unsigned int* bytesWrote)
{	
	if(NULL == envelopeHandle)
			return ENC_RET_WrongArgument;

	if(m_bEncode)
	{
		if(NULL == sourceHandle)
			return ENC_RET_WrongArgument;

		if(m_bIsFirstSourceBuffer)
		{		
			m_bIsFirstSourceBuffer = FALSE;
			pTmpApi->m_lpfnGetTempBuffer(sourceHandle,buffer,3,bytesWrote);
			if(*bytesWrote < 3)
				return ENC_RET_UnknownError;
			*((unsigned long long*)(buffer + 3)) = m_sourceBufferSize;
			*bytesWrote = sizeof(unsigned long long) + 3;
			return ENC_RET_OK;
		}
		else
		{
			pTmpApi->m_lpfnGetTempBuffer(sourceHandle,buffer,bufferSize,bytesWrote);
		}

		if(0 == *bytesWrote)
		{
			pTmpApi->m_lpfnGetTempBuffer(envelopeHandle,buffer,bufferSize,bytesWrote);
		}
	}
	else
	{
		pTmpApi->m_lpfnGetTempBuffer(envelopeHandle,buffer,bufferSize,bytesWrote);
	}
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
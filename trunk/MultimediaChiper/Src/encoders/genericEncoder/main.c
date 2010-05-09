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

	tempBuffer = (unsigned char*) malloc(sizeof(unsigned char) * 1024);
	if(NULL == tempBuffer)
		return ENC_RET_UnknownError;

	tempBufferSize = 1024;
	tempBufferUsedBytes = 0;
	m_bEncode = TRUE;
	m_bFirstBuffer = TRUE;
	return ENC_RET_OK;
}
EncoderRet ReloadEncoder()
{
	m_bEncode = TRUE;
	m_bFirstBuffer = TRUE;
	tempBufferUsedBytes = 0;
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
	if(tempBuffer)
		free(tempBuffer);
	return ENC_RET_OK;
}
EncoderRet SetAction(int bEncode)
{
	m_bEncode = bEncode;
	return ENC_RET_OK;
}
EncoderRet SetSourceBuffer(const unsigned char* buffer, unsigned int bufferSize)
{
	return ENC_RET_OK;
}
EncoderRet SetBuffer(const unsigned char* buffer, unsigned int bufferSize)
{	
	if(bufferSize > tempBufferSize)
	{
		if(tempBuffer)
			free(tempBuffer);
		tempBuffer = (unsigned char*) malloc( sizeof(unsigned char) * bufferSize);
		tempBufferSize = bufferSize;
	}
	memcpy(tempBuffer,buffer,bufferSize);
	tempBufferUsedBytes = bufferSize;
	
	return ENC_RET_OK;
}
EncoderRet GetBuffer(unsigned char* buffer, unsigned int bufferSize,unsigned int* bytesWrote)
{
	if(bufferSize > tempBufferSize)
	{
		*bytesWrote = tempBufferSize;
	}
	else
		*bytesWrote = bufferSize;
	memcpy(buffer,tempBuffer,*bytesWrote);
	return ENC_RET_OK;
}
/*
=================================================================================================
Filename: encoder.h
Desciption: The default API for an encoder
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/
#ifndef _ENCODER_DEF
#define _ENCODER_DEF 1

# ifdef __cplusplus
#  define API_EXPORT	extern "C"
# else
#  define API_EXPORT

# endif// __cplusplus

#include <windows.h>
#include "common.h"

typedef void* Encoder;
typedef struct _encoderStruct EncoderStruct, * EncoderStructPtr;
struct _encoderStruct {
	EncoderStructPtr	m_pThis;		/*!< pointer to self */
	LPWSTR				m_szName;		/*!< encoder name */
	LPWSTR				m_szVersion;	/*!< encoder version */
	LPWSTR				m_szType;		/*!< encoder type*/
	unsigned long long	m_ulUid;		/*!< encoder GUID */
};
typedef struct _encoderSignature EncoderSignature, *EncoderSignaturePtr;
struct _encoderSignature{
	unsigned long long	m_ulSignatureStartPos;
	unsigned char*		m_Signature;
	unsigned long long  m_ulSignatureSize;
};
typedef enum _encoderRet {
	ENC_RET_OK					= 0,
	ENC_RET_UnknownError		= -500,
	ENC_RET_WrongArgument		= -10,
	ENC_RET_IsEncoder           = 17564,
	ENC_RET_Wrong_File_Format   = -700,
	ENC_RET_Insuficient_Size	= -701
} EncoderRet;


/********************************************//**
       * Checks if the library is correctly loaded
       * @return ENC_RET_IsEncoder if everything is ok
 ***********************************************/
API_EXPORT EncoderRet IsEncoder();
/**	*@see IsEncoder	*/
typedef EncoderRet (*isEncoderFn)();


/********************************************//**
       * Init
       * @return ENC_RET_OK if everything is ok
 ***********************************************/
API_EXPORT EncoderRet Init();
/**	*@see Init	*/
typedef EncoderRet (*initEncoderFn)();


/********************************************//**
       * Get pointer to encoder struct      
	   * @return pointer to encoder. Must check if return value is NULL
 ***********************************************/
API_EXPORT Encoder GetEncoder();
/**	*@see GetEncoder	*/
typedef Encoder (*getEncoderFn) ();


/********************************************//**
       * UnInit       
	   * @return ENC_RET_OK if all is ok
 ***********************************************/
API_EXPORT EncoderRet UnInit();
/**	*@see UnInit */
typedef EncoderRet (*uninitEncoderFn) ();


/********************************************//**
       * Get encoder signature     
	   * @return pointer to encoder signature
 ***********************************************/
API_EXPORT EncoderSignaturePtr GetEncoderSignature();
/**	*@see GetEncoderSignature */
typedef EncoderSignaturePtr (*getEncoderSignatureFn)();


/********************************************//**
       * Set encoder action
       * @param [in] bEncode 	   
	   * @return ENC_RET_OK  if all is ok
 ***********************************************/ 
API_EXPORT EncoderRet SetAction(int bEncode);
/**	*@see SetAction */
typedef EncoderRet (*setEncoderActionFn)(int);


/********************************************//**
       * Set source buffer.
	   * mmclib will send the source to be encoded before sending the media source
	   * This source can be first filtered.
       * @param [in] buffer	   
	   * @param [in] bufferSize 
	   * @return ENC_RET_OK  if all is ok
 ***********************************************/ 
API_EXPORT EncoderRet SetSourceBuffer(const unsigned char* buffer, unsigned int bufferSize);
/**	*@see SetSourceBuffer */
typedef	EncoderRet (*setSourceBufferFn)(const unsigned char*,unsigned int);

/********************************************//**
       * Set media source.
	   * mmclib will alternate SetBuffer with GetBuffer
       * @param [in] buffer	   
	   * @param [in] bufferSize 
	   * @return ENC_RET_OK  if all is ok
 ***********************************************/ 
API_EXPORT EncoderRet SetBuffer(const unsigned char* buffer, unsigned int bufferSize);
/**	*@see SetBuffer */
typedef	EncoderRet (*setEncoderBufferFn)(const unsigned char*,unsigned int);


/********************************************//**
       * Get media source.
	   * mmclib will alternate SetBuffer with GetBuffer
       * @param [in] buffer	   
	   * @param [in] bufferSize 
	   * @return ENC_RET_OK  if all is ok
 ***********************************************/ 
API_EXPORT EncoderRet GetBuffer(unsigned char* buffer, unsigned int bufferSize,unsigned int* bytesWrote);
/**	*@see GetBuffer */
typedef	EncoderRet (*getEncoderBufferFn)(const unsigned char*,unsigned int,unsigned int*);


/********************************************//**
       * Resets the encoder
	   * On each new encoding action, mmclib will call this function       
	   * @return ENC_RET_OK  if all is ok
 ***********************************************/ 
API_EXPORT EncoderRet ReloadEncoder();
/**	*@see ReloadEncoder( */
typedef EncoderRet (*reloadEncoderFn) ();


/********************************************//**
       * Set functions to temporary buffer handler
       * @param [in] api   
	   * @return ENC_RET_OK if all is ok
 ***********************************************/
API_EXPORT EncoderRet SetTempFn(TempHandlerAPIPtr api);
/**	*@see SetTempFn	*/
typedef  EncoderRet (*setEncTempFn) (TempHandlerAPIPtr);


typedef struct _encoderAPI{
	isEncoderFn				m_lpfnIsEncoder	;	
	initEncoderFn			m_lpfnInit;
	getEncoderFn			m_lpfnGetEncoder;
	uninitEncoderFn			m_lpfnUnInit;
	getEncoderSignatureFn	m_lpfnGetEncoderSignature;
	setEncoderActionFn		m_lpfnSetAction;
	setSourceBufferFn		m_lpfnSetSourceBuffer;
	setEncoderBufferFn		m_lpfnSetBuffer;	
	getEncoderBufferFn		m_lpfnGetBuffer;
	reloadEncoderFn			m_lpfnReloadEncoder;
	setEncTempFn			m_lpfnSetTempFn;
} EncoderAPI;
#endif
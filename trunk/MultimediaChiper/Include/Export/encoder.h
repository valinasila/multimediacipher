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

typedef void* Encoder;
typedef struct _encoderStruct EncoderStruct, * EncoderStructPtr;
struct _encoderStruct {
	EncoderStructPtr	m_pThis;
	LPWSTR				m_szName;
	LPWSTR				m_szVersion;	
	LPWSTR				m_szType;	
	unsigned long long	m_ulUid;
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

API_EXPORT EncoderRet IsEncoder();
typedef EncoderRet (*isEncoderFn)();

API_EXPORT EncoderRet Init();
typedef EncoderRet (*initEncoderFn)();

API_EXPORT Encoder GetEncoder();
typedef Encoder (*getEncoderFn) ();

API_EXPORT EncoderRet UnInit();
typedef EncoderRet (*uninitEncoderFn) ();

API_EXPORT EncoderSignaturePtr GetEncoderSignature();
typedef EncoderSignaturePtr (*getEncoderSignatureFn)();

API_EXPORT EncoderRet SetAction(int bEncode);
typedef EncoderRet (*setEncoderActionFn)(int);

API_EXPORT EncoderRet SetSourceBuffer(const unsigned char* buffer, unsigned int bufferSize);
typedef	EncoderRet (*setSourceBufferFn)(const unsigned char*,unsigned int);

API_EXPORT EncoderRet SetBuffer(const unsigned char* buffer, unsigned int bufferSize);
typedef	EncoderRet (*setEncoderBufferFn)(const unsigned char*,unsigned int);

API_EXPORT EncoderRet GetBuffer(unsigned char* buffer, unsigned int bufferSize,unsigned int* bytesWrote);
typedef	EncoderRet (*getEncoderBufferFn)(const unsigned char*,unsigned int,unsigned int*);

API_EXPORT EncoderRet ReloadEncoder();
typedef EncoderRet (*reloadEncoderFn) ();

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
} EncoderAPI;
#endif
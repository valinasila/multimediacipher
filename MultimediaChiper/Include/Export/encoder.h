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
#define _ENCODER_DEF

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
typedef enum _encoderRet {
	ENC_RET_OK					= 0,
	ENC_RET_UnknownError		= -500,
	ENC_RET_WrongArgument		= -10,
	ENC_RET_IsEncoder           = 17564,
} EncoderRet;

API_EXPORT EncoderRet IsEncoder();
typedef EncoderRet (*isEncoderFn)();

API_EXPORT EncoderRet Init();
typedef EncoderRet (*initEncoderFn)();

API_EXPORT Encoder GetEncoder();
typedef Encoder (*getEncoderFn) ();

API_EXPORT EncoderRet UnInit();
typedef EncoderRet (*uninitEncoderFn) ();

typedef struct _encoderAPI{
	isEncoderFn				m_lpfnIsEncoder	;	
	initEncoderFn			m_lpfnInit;
	getEncoderFn			m_lpfnGetEncoder;
	uninitEncoderFn			m_lpfnUnInit;
} EncoderAPI;
#endif
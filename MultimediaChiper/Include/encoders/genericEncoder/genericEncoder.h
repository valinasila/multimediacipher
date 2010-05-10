/*
=================================================================================================
Filename: genericEncoder.h
Desciption: 
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/

#include "encoder.h"

#define ENCODER_VERSION L"1.0.0"
#define ENCODER_NAME	L"Generic Encoder"
#define ENCODER_UID		0x01
#define ENCODER_TYPE	L"generic"

EncoderStructPtr m_pInternalStruct = NULL;
EncoderSignaturePtr m_pSignature = NULL;
unsigned char* tempBuffer = NULL;
unsigned int tempBufferSize = 0;
unsigned int tempBufferUsedBytes = 0;
int m_bEncode = TRUE;
int m_bFirstBuffer = TRUE;
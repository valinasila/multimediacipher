/*
=================================================================================================
Filename: BMPEncoder.h
Desciption: 
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/

#include "encoder.h"

#define ENCODER_VERSION L"1.0.0"
#define ENCODER_NAME	L"BMP Encoder"
#define ENCODER_UID		0x02
#define ENCODER_TYPE	L"image encoder"

EncoderStructPtr m_pInternalStruct = NULL;
EncoderSignaturePtr m_pSignature = NULL;
int m_bEncode = 1;
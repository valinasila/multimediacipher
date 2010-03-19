/*
=================================================================================================
Filename: BMPEncoder.h
Desciption: 
Author: $Author: claudiu.guiman $
Last changed by:    $Author: claudiu.guiman $
Last changed date:    $Date: 2010-03-14 19:33:19 +0200 (D, 14 mar. 2010) $
ID:            $Id: genericEncoder.h 14 2010-03-14 17:33:19Z claudiu.guiman $
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
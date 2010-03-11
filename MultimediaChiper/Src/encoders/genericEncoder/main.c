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
	return ENC_RET_OK;
}
EncoderStructPtr GetEncoder()
{
	return m_pInternalStruct;
}
EncoderRet	UnInit()
{
	if(m_pInternalStruct)
		free(m_pInternalStruct);
	return ENC_RET_OK;
}
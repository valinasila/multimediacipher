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

TempHandlerAPIPtr pTmpApi = NULL;
TempHandle	sourceHandle = NULL;
TempHandle	envelopeHandle = NULL;
unsigned long long m_sourceBufferSize = 0;
int m_bFirstBuffer = FALSE;


typedef struct _bmpStruct bmpStruct,*bmpStructPtr;
struct _bmpStruct{
	unsigned int size;
	unsigned short offset;
	unsigned int   header_size;
	int		width;
	int		height;
	unsigned short bitsPerPixel;
	unsigned int imageSize;

};

bmpStruct fileStruct;

unsigned int offset = 0;
unsigned char* tempBuffer = NULL;
int			   tempBufferSize = 0;

unsigned int sourceOffset = 0;
unsigned char* sourceBuffer = NULL;
int				sourceBufferSize = 0;
unsigned char	bitOffset = 0;


#define DEFAULT_BUFFER_SIZE  2048
/*
************************************************
	BMP file format
************************************************
0000h  2 bytes		BM
0002h  4 bytes      size
0006h  2 bytes		reserved
0008h  2 bytes		reserved
000Ah  4 bytes		offset
Eh	4	the size of this header (40 bytes)
12h	4	the bitmap width in pixels (signed integer).
16h	4	the bitmap height in pixels (signed integer).
1Ah	2	the number of color planes being used. Must be set to 1.
1Ch	2	the number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32.
1Eh	4	the compression method being used. See the next table for a list of possible values.
22h	4	the image size. This is the size of the raw bitmap data (see below), and should not be confused with the file size.
26h	4	the horizontal resolution of the image. (pixel per meter, signed integer)
2Ah	4	the vertical resolution of the image. (pixel per meter, signed integer)
2Eh	4	the number of colors in the color palette, or 0 to default to 2n.
32h	4	the number of important colors used, or 0 when every color is important; generally ignored.
*/
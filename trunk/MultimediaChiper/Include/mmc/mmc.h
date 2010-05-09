/*
=================================================================================================
Filename: mmc.h
Desciption:  mmc console
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/
#ifndef __MMC_H_DEF
#define __MMC_H_DEF 1
#include <windows.h>
#include <stdio.h>

#include "mmclib.h"

#ifdef _DEBUG
#include "vld.h" // visual leak detector  http://www.codeproject.com/KB/applications/visualleakdetector.aspx
#endif

typedef enum _mmcOption {
	MMC_OPT_HELP,
	MMC_OPT_ENUM_ENCODER,
	MMC_OPT_ENUM_FILTERS,
	MMC_OPT_GET_ENCODER,
} MMCOption;


typedef struct _mmcAction mmcAction, *mmcActionPtr;
struct _mmcAction {
	MMCOption		opt;
	void*			m_pTarget;
	mmcActionPtr	m_pNext;
};
#endif

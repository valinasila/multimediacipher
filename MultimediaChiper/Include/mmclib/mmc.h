/*
=================================================================================================
Filename: mmc.h
Desciption: 
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/
#include "mmclib.h"
#define DEFAULT_ENCODERS_DIR L"encoders"
#define DEFAULT_FILTERS_DIR	 L"filters"
HMODULE* dllEncoders = NULL;
int dllEncodersSize;
HMODULE* dllFilters = NULL;
int dllFiltersSize;

LPWSTR encodersDir = NULL;
LPWSTR filtersDir = NULL;

int ScanEncoders();
int ScanFilters();
int ReleaseDllList(HMODULE** , int*);
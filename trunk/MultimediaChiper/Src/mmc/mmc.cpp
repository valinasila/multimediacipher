/*
=================================================================================================
Filename: mmc.c
Desciption:  mmc console
Author: $Author$
Last changed by:    $Author$
Last changed date:    $Date$
ID:            $Id$
=================================================================================================
*/
#include <tchar.h>
#include "mmc.h"

mmcActionPtr actions = NULL;

int AddAction(MMCOption opt, void* target)
{
	mmcActionPtr node = actions;
	while(node)
	{
		node = node->m_pNext;
	}

	if(NULL == node)
	{
		node = (mmcActionPtr) malloc( sizeof(mmcAction) );
		
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc <= 1)
		AddAction(MMC_OPT_HELP,NULL);

	//switch(
	return 0;
}
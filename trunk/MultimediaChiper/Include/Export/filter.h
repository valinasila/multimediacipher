#ifndef _FILTER_DEF
#define _FILTER_DEF
#include <windows.h>

# ifdef __cplusplus
#  define API_EXPORT	extern "C"
# else
#  define API_EXPORT
#endif

typedef struct _filterStruct{
	char* name;
	void* data;
} FilterStruct ;

#endif
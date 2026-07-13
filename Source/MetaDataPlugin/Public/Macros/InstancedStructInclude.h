#ifndef INCLUDE_INSTANCED_STRUCTS
#define INCLUDE_INSTANCED_STRUCTS
#include "Misc/EngineVersionComparison.h"

#if UE_VERSION_NEWER_THAN(5, 3, -1)
	// UE 5.3 and newer
	#include "StructUtils/InstancedStruct.h"
#else
	// UE 5.2 and older
	#include "InstancedStruct.h"
#endif

#endif


INCLUDE_INSTANCED_STRUCTS
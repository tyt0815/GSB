// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#ifdef WITH_EDITOR
#define WITH_EDITOR_MACRO(x) x
#else
#define WITH_EDITOR_MACRO(x) 
#endif

// String Log
#define SCREEN_DEBUG_MESSAGE(Key, TimeToDisplay, Color, DebugMessage) WITH_EDITOR_MACRO(\
	if(GEngine) \
	{\
		GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, Color, DebugMessage);\
	})

#define SCREEN_LOG(Key, DebugMessage)\
	switch (Key)\
	{ \
		case 0: SCREEN_DEBUG_MESSAGE(Key, 5, FColor::Red, DebugMessage); break;\
		case 1: SCREEN_DEBUG_MESSAGE(Key, 5, FColor::Green, DebugMessage); break;\
		case 2: SCREEN_DEBUG_MESSAGE(Key, 5, FColor::Blue, DebugMessage); break;\
		case 3: SCREEN_DEBUG_MESSAGE(Key, 5, FColor::Yellow, DebugMessage); break; \
		case 4: SCREEN_DEBUG_MESSAGE(Key, 5, FColor::Cyan, DebugMessage); break; \
		case 5: SCREEN_DEBUG_MESSAGE(Key, 5, FColor::Magenta, DebugMessage); break;\
		default: SCREEN_DEBUG_MESSAGE(Key, 5, FColor::Purple, DebugMessage); break;\
	}\


#define SCREEN_LOG_COLOR(Key, DisplayColor, DebugMessage) SCREEN_DEBUG_MESSAGE(Key, 5, DisplayColor, DebugMessage);
#define SCREEN_LOG_SINGLE_FRAME(DebugMessage) SCREEN_DEBUG_MESSAGE(INDEX_NONE, 0, FColor::Purple, DebugMessage);
#define SCREEN_LOG_NONE_KEY(DebugMessage) SCREEN_DEBUG_MESSAGE(INDEX_NONE, 5, FColor::Orange, DebugMessage);

#if defined(_MSC_VER)
#define FUNCTION_NAME_MACRO __FUNCSIG__
#elif defined(__clang__) || defined(__GNUC__)
#define FUNCTION_NAME_MACRO __PRETTY_FUNCTION__
#else
#define FUNCTION_NAME_MACRO __FUNCTION__ // fallback
#endif

#define TRACE_SCREEN_LOG(DebugMessage) SCREEN_DEBUG_MESSAGE(-1, 10, FColor::Cyan, FString(FUNCTION_NAME_MACRO) + TEXT(": ") + DebugMessage);



class FGSBDebugLibraryModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
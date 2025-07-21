// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#ifdef WITH_EDITOR
#define WITH_EDITOR_MACRO(x) x
#else
#define WITH_EDITOR_MACRO(x) 
#endif

// String Log
#define PRINT_SCREEN(Key, TimeToDisplay, Color, DebugMessage) WITH_EDITOR_MACRO(\
	if(GEngine) \
	{\
		GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, Color, DebugMessage);\
	})

#if defined(_MSC_VER)
#define FUNCTION_NAME_MACRO __FUNCSIG__
#elif defined(__clang__) || defined(__GNUC__)
#define FUNCTION_NAME_MACRO __PRETTY_FUNCTION__
#else
#define FUNCTION_NAME_MACRO __FUNCTION__ // fallback
#endif

#define TRACE_PRINT_SCREEN(DebugMessage) PRINT_SCREEN(-1, 10, FColor::Cyan, FString(FUNCTION_NAME_MACRO) + TEXT(": ") + DebugMessage);

inline void PrintLog_Internal(const FString & DebugMessage)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugMessage);
}

#define PRINT_LOG(DebugMessage) WITH_EDITOR_MACRO(PrintLog_Internal(DebugMessage))

#define TRACE_PRINT_LOG(DebugMessage) PRINT_LOG(FString(FUNCTION_NAME_MACRO) + TEXT(": ") + DebugMessage)

#define TRACE_PRINT_SCREEN_AND_LOG(DebugMessage) TRACE_PRINT_SCREEN(DebugMessage); TRACE_PRINT_LOG(DebugMessage);

class FGSBDebugLibraryModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
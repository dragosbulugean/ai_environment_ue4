#pragma once
#include "ModuleInterface.h"

class IKLiveStreaming : public IModuleInterface
{

public:

	static inline IKLiveStreaming& Get()
	{
		return FModuleManager::LoadModuleChecked<IKLiveStreaming>( "KLiveStreaming" );
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "KLiveStreaming" );
	}

};


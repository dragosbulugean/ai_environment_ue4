#include "kortex_environment.h"
#include "KLiveStreaming.h"
#include "ModuleManager.h"
#include "Runtime/Core/Public/Features/IModularFeatures.h"
#include "Runtime/Core/Public/Stats/Stats2.h"
//#include "ISettingsModule.h"

void KLiveStreaming::StartupModule()
{
	FMemory::Memzero( VideoBuffers );
	IModularFeatures::Get().RegisterModularFeature( TEXT( "LiveStreaming" ), this );
}

void KLiveStreaming::ShutdownModule()
{
	IModularFeatures::Get().UnregisterModularFeature( TEXT( "KLiveStreaming" ), this );

	// Clean up video buffers
	for (uint32 VideoBufferIndex = 0; VideoBufferIndex < BufferCount; ++VideoBufferIndex)
	{
		delete[] VideoBuffers[ VideoBufferIndex ];
		VideoBuffers[ VideoBufferIndex ] = nullptr;
	}
	AvailableVideoBuffers.Reset();
	
}

bool KLiveStreaming::IsModuleLoaded() {
	FName LiveStreamingFeatureName("LiveStreaming");
	bool isModuleLoaded = IModularFeatures::Get().IsModularFeatureAvailable(LiveStreamingFeatureName);
	return isModuleLoaded;
}

ILiveStreamingService::FOnStatusChanged& KLiveStreaming::OnStatusChanged()
{
	return OnStatusChangedEvent;
}

void KLiveStreaming::StartBroadcasting(const FBroadcastConfig& Config)
{

	bWantsToBroadcastNow = true;
	BroadcastConfig = Config;

	this->Tick( 0.0f );

	UE_LOG(LogTemp, Display, TEXT("Kortex broadcast configured for %i x %i resolution"), BroadcastConfig.VideoBufferWidth, BroadcastConfig.VideoBufferHeight);

	// Get our video buffers ready
	this->AvailableVideoBuffers.Reset();
	for (uint32 VideoBufferIndex = 0; VideoBufferIndex < this->BufferCount; ++VideoBufferIndex)
	{
		// Allocate buffer (width * height * bytes per pixel)
		this->VideoBuffers[VideoBufferIndex] = new uint8[this->BroadcastConfig.VideoBufferWidth * this->BroadcastConfig.VideoBufferHeight * 4];
		this->AvailableVideoBuffers.Add(this->VideoBuffers[VideoBufferIndex]);
	}
	
}

void KLiveStreaming::StopBroadcasting()
{
	if( IsBroadcasting() )
	{
		bWantsToBroadcastNow = false;

		this->Tick( 0.0f );

		// Clean up video buffers
		this->AvailableVideoBuffers.Reset();
		for (uint32 VideoBufferIndex = 0; VideoBufferIndex < this->BufferCount; ++VideoBufferIndex)
		{
			delete[] this->VideoBuffers[VideoBufferIndex];
			this->VideoBuffers[VideoBufferIndex] = nullptr;
		}
	}
}

bool KLiveStreaming::IsBroadcasting() const
{
	return bWantsToBroadcastNow;
}

bool KLiveStreaming::IsReadyForVideoFrames() const
{
	return true;
}

void KLiveStreaming::MakeValidVideoBufferResolution(int& VideoBufferWidth, int& VideoBufferHeight) const
{
	struct Local
	{
		/** Given a number, rounds the number of to the next specified multiple */
		static int RoundUpToNextMultipleOf( int Value, int Multiple )
		{ 
			int Result = Value;
			if( Multiple != 0 )
			{ 
				int Remainder = Value % Multiple;
				if( Remainder != 0 )
				{
					Result = Value + Multiple - Remainder;
				}
			}
			return Result;
		}
	};

	VideoBufferWidth = Local::RoundUpToNextMultipleOf( VideoBufferWidth, 32 );
	VideoBufferHeight = Local::RoundUpToNextMultipleOf( VideoBufferHeight, 16 );
}

void KLiveStreaming::QueryBroadcastConfig(FBroadcastConfig& OutBroadcastConfig) const
{
	if( ensure( IsBroadcasting() ) )
	{
		OutBroadcastConfig = BroadcastConfig;
	}
}
	
void KLiveStreaming::PushVideoFrame(const FColor* VideoFrameBuffer)
{
	UE_LOG(LogTemp, Warning, TEXT("GOT FRAME!"));

	/*const uint8_t* Buffer;
	this->AvailableVideoBuffers.Add(const_cast<uint8*>(Buffer));
	uint8* FrameBuffer = AvailableVideoBuffers.Pop();
	FMemory::Memcpy(FrameBuffer, VideoFrameBuffer, BroadcastConfig.VideoBufferWidth * BroadcastConfig.VideoBufferHeight * 4);*/

}

void KLiveStreaming::QueryLiveStreams(const FString& GameName, FQueryLiveStreamsCallback CompletionCallback)
{

}

void KLiveStreaming::Tick(float DeltaTime)
{

}

bool KLiveStreaming::IsTickable() const
{
	return true;
}

bool KLiveStreaming::IsTickableWhenPaused() const
{
	return true;
}

bool KLiveStreaming::IsTickableInEditor() const
{
	return true;
}

void KLiveStreaming::StartWebCam(const FWebCamConfig& Config)
{
	
}


void KLiveStreaming::StopWebCam()
{
	
}


bool KLiveStreaming::IsWebCamEnabled() const
{
	return false;
}


UTexture2D* KLiveStreaming::GetWebCamTexture(bool& bIsImageFlippedHorizontally, bool& bIsImageFlippedVertically)
{
	return nullptr;
}

ILiveStreamingService::FOnChatMessage& KLiveStreaming::OnChatMessage()
{
	return OnChatMessageEvent;
}

void KLiveStreaming::ConnectToChat()
{
	
}

void KLiveStreaming::DisconnectFromChat()
{
	
}

bool KLiveStreaming::IsChatEnabled() const
{
	return false;
}


void KLiveStreaming::SendChatMessage(const FString& ChatMessage)
{
	
}

TStatId KLiveStreaming::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(KLiveStreaming, STATGROUP_Tickables);
}


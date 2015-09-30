#include "kortex_environment.h"
#include "KLiveStreaming.h"
#include "ModuleManager.h"
#include "Runtime/Core/Public/Features/IModularFeatures.h"
#include "Runtime/Core/Public/Stats/Stats2.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "AllowWindowsPlatformTypes.h"
#include "zmq.hpp"
#include "HideWindowsPlatformTypes.h"

//#include "ISettingsModule.h"

void KLiveStreaming::StartupModule()
{
	IModularFeatures::Get().RegisterModularFeature( TEXT( "LiveStreaming" ), this );
}

void KLiveStreaming::ShutdownModule()
{
	IModularFeatures::Get().UnregisterModularFeature( TEXT( "LiveStreaming" ), this );
}

bool KLiveStreaming::IsModuleLoaded() 
{
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
	LastSendTime = FPlatformTime::Seconds();

	this->Tick( 0.0f );

	UE_LOG(LogTemp, Display, TEXT("Kortex started broadcast. (%i x %i)"), BroadcastConfig.VideoBufferWidth, BroadcastConfig.VideoBufferHeight);

	ZMQContext = zmq_init(1);
	if (ZMQContext == 0) {
		printf("Error zmq_init: '%s'\n", zmq_strerror(errno));
	}

	ZMQSocket = zmq_socket(ZMQContext, ZMQ_PUSH);
	if (ZMQSocket == 0) {
		printf("Error zmq_socket: '%s'\n", zmq_strerror(errno));
	}
	
	auto result = zmq_bind(ZMQSocket, "tcp://127.0.0.1:4440");
	if (result == 0) {
		printf("Error zmq_connect: '%s'\n", zmq_strerror(errno));
	}
}

void KLiveStreaming::StopBroadcasting()
{
	zmq_close(ZMQSocket);
	if (IsBroadcasting())
	{
		bWantsToBroadcastNow = false;
		this->Tick(0.0f);
	}
	UE_LOG(LogTemp, Display, TEXT("Kortex stopped broadcast. (%i x %i)"), BroadcastConfig.VideoBufferWidth, BroadcastConfig.VideoBufferHeight);
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

	int Width = BroadcastConfig.VideoBufferWidth,
		Height = BroadcastConfig.VideoBufferHeight,
		DataLength = Width * Height,
		BufferSize = DataLength * 4;
	
	double Now = FPlatformTime::Seconds();
	double ElapsedSinceLastSend = Now - LastSendTime;

	if (ElapsedSinceLastSend < SampleTimeInterval) {
		return;
	}

	if (Character->CurrentCameraIndex == 0) {
		ThreeFrameBuffer = (uint8*) malloc(3 * BufferSize);
		FMemory::Memcpy(ThreeFrameBuffer, VideoFrameBuffer, BufferSize);
		Character->CurrentCameraIndex = 1;
	}
	else if (Character->CurrentCameraIndex == 1){
		FMemory::Memcpy(ThreeFrameBuffer + BufferSize + 1, VideoFrameBuffer, BufferSize);
		Character->CurrentCameraIndex = 2;
	}
	else if (Character->CurrentCameraIndex == 2){
		FMemory::Memcpy(ThreeFrameBuffer + 2 * BufferSize + 1, VideoFrameBuffer, BufferSize);
		int rc = zmq_send(ZMQSocket, ThreeFrameBuffer, 3 * BufferSize, 0);

		if (rc == -1) {
			printf("Error zmq_connect: '%s'\n", zmq_strerror(errno));
		}

		free(ThreeFrameBuffer);

		LastSendTime = FPlatformTime::Seconds();
		Character->CurrentCameraIndex = 0;
	}
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

void my_free(void *data, void *hint) { free(data); }

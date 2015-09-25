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
	FMemory::Memzero( VideoBuffers );
	IModularFeatures::Get().RegisterModularFeature( TEXT( "LiveStreaming" ), this );
}

void KLiveStreaming::ShutdownModule()
{
	IModularFeatures::Get().UnregisterModularFeature( TEXT( "LiveStreaming" ), this );

	// Clean up video buffers
	for (uint32 VideoBufferIndex = 0; VideoBufferIndex < BufferCount; ++VideoBufferIndex)
	{
		delete[] VideoBuffers[ VideoBufferIndex ];
		VideoBuffers[ VideoBufferIndex ] = nullptr;
	}
	AvailableVideoBuffers.Reset();
	
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
	
	this->Tick( 0.0f );

	UE_LOG(LogTemp, Display, TEXT("Kortex started broadcast. (%i x %i)"), BroadcastConfig.VideoBufferWidth, BroadcastConfig.VideoBufferHeight);

	// Get our video buffers ready
	this->AvailableVideoBuffers.Reset();
	for (uint32 VideoBufferIndex = 0; VideoBufferIndex < this->BufferCount; ++VideoBufferIndex)
	{
		// Allocate buffer (width * height * bytes per pixel)
		this->VideoBuffers[VideoBufferIndex] = new uint8[this->BroadcastConfig.VideoBufferWidth * this->BroadcastConfig.VideoBufferHeight * 4];
		this->AvailableVideoBuffers.Add(this->VideoBuffers[VideoBufferIndex]);
	}

	ZMQContext = zmq_init(1);
	if (ZMQContext == 0) {
		printf("Error zmq_init: '%s'\n", zmq_strerror(errno));
	}

	ZMQSocket = zmq_socket(ZMQContext, ZMQ_PUSH);
	if (ZMQSocket == 0) {
		printf("Error zmq_socket: '%s'\n", zmq_strerror(errno));
	}
	
	auto result = zmq_connect(ZMQSocket, "tcp://127.0.0.1:4440");
	if (result == 0) {
		printf("Error zmq_connect: '%s'\n", zmq_strerror(errno));
	}
}

void KLiveStreaming::StopBroadcasting()
{

	//delete ZMQContext;
	zmq_close(ZMQSocket);

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


	int width = BroadcastConfig.VideoBufferWidth,
		height = BroadcastConfig.VideoBufferHeight,
		//DataLength = 10000, 
		DataLength = width * height,
		bufferSize = DataLength * 3;;

	int rc = zmq_send(ZMQSocket, VideoFrameBuffer, DataLength * sizeof(FColor), 0);

	if (rc == -1) {
		printf("Error zmq_connect: '%s'\n", zmq_strerror(errno));
	}
	
	/*uint8* frameBuffer = new uint8[bufferSize];
	int index = 0;

	for (auto i = 0; i < DataLength; i++)
	{
		frameBuffer[i * 3] = VideoFrameBuffer->R;
		frameBuffer[i * 3 + 1] = VideoFrameBuffer->G;
		frameBuffer[i * 3 + 2] = VideoFrameBuffer->B;
		VideoFrameBuffer++;
	}
	

	zmq_msg_t msg;
	void *hint = NULL;
	
	int rc = zmq_msg_init_data(&msg, frameBuffer, bufferSize, &my_free, hint);
	memcpy(zmq_msg_data(&msg), frameBuffer, DataLength * 3);
	memset(zmq_msg_data(&msg), frameBuffer, DataLength * 3);
	
	
	rc = zmq_sendmsg(ZMQSocket, &msg, 0);

	delete[] frameBuffer;*/

	/*bool successful = Socket->Send((uint8*)TCHAR_TO_UTF8(serializedChar), size, sent);
	if (successful) 
	{
		UE_LOG(LogTemp, Display, TEXT("Sent video frame"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Didn't send video frame"));
	}*/

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

void my_free(void *data, void *hint) { free(data); }

#pragma once

#include "IKLiveStreaming.h"
#include "Runtime/Engine/Public/Features/ILiveStreamingService.h"
#include "Runtime/Engine/Public/Tickable.h"
#include "KCharacter.h"
#include "AllowWindowsPlatformTypes.h"
#include "zmq.h"
#include "HideWindowsPlatformTypes.h"


/**
 * Kortex live streaming implementation
 */
class KLiveStreaming : public IKLiveStreaming, public ILiveStreamingService, public FTickableGameObject
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	bool IsModuleLoaded();


	/** ILiveStreamingService implementation */
	virtual FOnStatusChanged& OnStatusChanged() override;
	virtual void StartBroadcasting( const FBroadcastConfig& Config ) override;
	virtual void StopBroadcasting() override;
	virtual bool IsBroadcasting() const override;
	virtual bool IsReadyForVideoFrames() const override;
	virtual void MakeValidVideoBufferResolution( int& VideoBufferWidth, int& VideoBufferHeight ) const override;
	virtual void QueryBroadcastConfig( FBroadcastConfig& OutBroadcastConfig ) const override;
	virtual void PushVideoFrame( const FColor* VideoFrameBuffer ) override;
	virtual void StartWebCam( const FWebCamConfig& Config ) override;
	virtual void StopWebCam() override;
	virtual bool IsWebCamEnabled() const override;
	virtual UTexture2D* GetWebCamTexture( bool& bIsImageFlippedHorizontally, bool& bIsImageFlippedVertically ) override;
	virtual FOnChatMessage& OnChatMessage() override;
	virtual void ConnectToChat() override;
	virtual void DisconnectFromChat() override;
	virtual bool IsChatEnabled() const override;
	virtual void SendChatMessage( const FString& ChatMessage ) override;
	virtual void QueryLiveStreams( const FString& GameName, FQueryLiveStreamsCallback CompletionCallback ) override;

	AKCharacter* Character;
	TArray<FColor*> RenderBuffer;

	void *ZMQContext;
	void *ZMQSocket;
	
private:

	/** FTickableGameObject implementation */
	virtual void Tick( float DeltaTime ) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickableWhenPaused() const override;
	virtual bool IsTickableInEditor() const override;
	FOnChatMessage OnChatMessageEvent;
	

private:

	FOnStatusChanged OnStatusChangedEvent;
	bool bWantsToBroadcastNow;
	FBroadcastConfig BroadcastConfig;
	static const uint32 BufferCount = 3;
	uint8* VideoBuffers[BufferCount];
	TArray<uint8*> AvailableVideoBuffers;

};

void my_free(void *data, void *hint);

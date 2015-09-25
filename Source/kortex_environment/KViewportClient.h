#pragma once

#include "Engine/GameViewportClient.h"
#include "AllowWindowsPlatformTypes.h"
#include "zmq.h"
#include "HideWindowsPlatformTypes.h"
#include "KViewportClient.generated.h"

UCLASS()
class KORTEX_ENVIRONMENT_API UKViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

public:

	UKViewportClient(const FObjectInitializer& ObjectInitializer);

	bool toggleBroadcasting;

	virtual void Draw(FViewport* Viewport, FCanvas* SceneCanvas) override;

	void useFrames();

	void *ZMQContext;
	void *ZMQSocket;
};

void my_free_viewport(void *data, void *hint);


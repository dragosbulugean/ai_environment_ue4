#pragma once

#include "Engine/GameViewportClient.h"
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

};

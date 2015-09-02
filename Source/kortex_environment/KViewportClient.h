#pragma once

#include "Engine/GameViewportClient.h"
#include "KViewportClient.generated.h"

/**
*
*/
UCLASS()
class KORTEX_ENVIRONMENT_API UKViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

public:

	UKViewportClient(const class FPostConstructInitializeProperties& PCIP);
	////Controller
	//AVictoryGamePlayerController * VictoryPC;
	//Toggle Broadcasting
	bool toggleBroadcasting;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Draw(FViewport* Viewport, FCanvas* SceneCanvas) override;

	//Get frames from viewport 
	void useFrames();

};

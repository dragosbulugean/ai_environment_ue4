#pragma once

#include "GameFramework/GameMode.h"
#include "KGameMode.generated.h"

UCLASS()
class KORTEX_ENVIRONMENT_API AKGameMode : public AGameMode
{
	GENERATED_BODY()
    
    AKGameMode(const FObjectInitializer& ObjectInitializer);
	
	virtual void StartPlay() override;
};

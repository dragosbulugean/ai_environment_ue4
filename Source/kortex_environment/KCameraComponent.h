#pragma once

#include "Camera/CameraComponent.h"
#include "KCameraComponent.generated.h"

UCLASS()
class KORTEX_ENVIRONMENT_API UKCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
    
    UKCameraComponent(const FObjectInitializer& ObjectInitializer);
	
    void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView);
};

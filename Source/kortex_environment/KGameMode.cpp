#pragma once

#include "kortex_environment.h"
#include "KGameMode.h"
#include "KCharacter.h"
#include "KLiveStreaming.h"
#include "Runtime/Engine/Public/Features/ILiveStreamingService.h"
#include "Runtime/GameLiveStreaming/Public/IGameLiveStreaming.h"
#include "Editor/EditorLiveStreaming/Public/IEditorLiveStreaming.h"


AKGameMode::AKGameMode(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
    DefaultPawnClass = AKCharacter::StaticClass();}

void AKGameMode::StartPlay()
{

	if (GEngine)
	{
		UGameUserSettings* KSettings = GEngine->GetGameUserSettings();
		KSettings->SetScreenResolution(FIntPoint(2400, 800));
		KSettings->SetFullscreenMode(EWindowMode::Windowed);
		KSettings->SetVSyncEnabled(true);
		KSettings->ApplySettings(true);
	}

    Super::StartPlay();
    StartMatch();
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Kortex Environment"));
    }

	UWorld* currentWorld = GetWorld();

	UGameInstance* gameInstance = currentWorld->GetGameInstance();
	UGameViewportClient* gameViewport = currentWorld->GetGameViewport();
	FString error;
	ULocalPlayer* p2 = gameInstance->CreateLocalPlayer(2, error, true);

	AKCharacter* LeftEye = Cast<AKCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AKCharacter* RightEye = Cast<AKCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 1));

	UInputComponent* i1 = LeftEye->InputComponent;
	RightEye->SetupPlayerInputComponent(i1);
	
	FVector LeftEyeVector(2150, -7, 200);
	LeftEye->SetActorRelativeLocation(LeftEyeVector);
	FVector RightEyeVector(2240, -7, 200);
	RightEye->SetActorRelativeLocation(RightEyeVector);
	FRotator LeftEyeRotation(0.f, -20.f, 0.f);
	LeftEye->SetActorRotation(LeftEyeRotation);
	FRotator RightEyeRotation(0.f, 20.f, 0.f);
	RightEye->SetActorRotation(RightEyeRotation);


	KLiveStreaming* kss = new KLiveStreaming();
	kss->StartupModule();
	bool isLiveStreamingModuleLoaded = kss->IsModuleLoaded();
	if (isLiveStreamingModuleLoaded)
	{
		UE_LOG(LogTemp, Log, TEXT("Kortex streaming module loaded!"));
	}



}
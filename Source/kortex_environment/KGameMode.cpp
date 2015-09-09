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
	DefaultPawnClass = AKCharacter::StaticClass();
	//static ConstructorHelpers::FObjectFinder<UBlueprint> Blueprint(TEXT("Blueprint'/Game/Blueprints/PlayerCharacter.PlayerCharacter'"));
	//if (Blueprint.Object){
	//	DefaultPawnClass = (UClass*)Blueprint.Object->GeneratedClass;
	//}
}

void AKGameMode::StartPlay()
{

	Super::StartPlay();
	StartMatch();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Kortex Environment"));
	}

	if (GEngine)
	{
		UGameUserSettings* KSettings = GEngine->GetGameUserSettings();
		KSettings->SetFullscreenMode(EWindowMode::Windowed);
		KSettings->SetVSyncEnabled(true);
		KSettings->ApplySettings(true);
	}

	UWorld* currentWorld = GetWorld();
	UGameInstance* gameInstance = currentWorld->GetGameInstance();
	UGameViewportClient* gameViewport = currentWorld->GetGameViewport();

	KLiveStreaming* kss = new KLiveStreaming();
	AKCharacter* Character = Cast<AKCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	kss->Character = Character;
	kss->StartupModule();
	bool isLiveStreamingModuleLoaded = kss->IsModuleLoaded();
	if (isLiveStreamingModuleLoaded)
	{
		UE_LOG(LogTemp, Log, TEXT("Kortex streaming module loaded!"));
	}

}

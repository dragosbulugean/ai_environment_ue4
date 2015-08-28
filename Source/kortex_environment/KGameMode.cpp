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
	ULocalPlayer* p3 = gameInstance->CreateLocalPlayer(3, error, true);
	ULocalPlayer* p4 = gameInstance->CreateLocalPlayer(4, error, true);

	KLiveStreaming* kss = new KLiveStreaming();
	kss->StartupModule();
	bool isLiveStreamingModuleLoaded = kss->IsModuleLoaded();
	if (isLiveStreamingModuleLoaded)
	{
		UE_LOG(LogTemp, Log, TEXT("Kortex streaming module loaded!"));
	}

#if WITH_EDITOR
	IEditorLiveStreaming::Get().StartBroadcastingEditor();
#else
	FGameBroadcastConfig Config = FGameBroadcastConfig();
	IGameLiveStreaming::Get().StartBroadcastingGame(Config);
#endif

}
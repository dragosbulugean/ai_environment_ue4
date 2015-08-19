#include "kortex_environment.h"
#include "KGameMode.h"
#include "KCharacter.h"

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
}
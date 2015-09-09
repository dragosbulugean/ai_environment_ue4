#include "kortex_environment.h"
#include "KCharacter.h"
#include "Runtime/Engine/Public/Features/ILiveStreamingService.h"
#include "Runtime/GameLiveStreaming/Public/IGameLiveStreaming.h"
#include "Editor/EditorLiveStreaming/Public/IEditorLiveStreaming.h"
#include <sstream>
#include <string>
#include "Runtime/Engine/Public/Features/ILiveStreamingService.h"
#include "Runtime/GameLiveStreaming/Public/IGameLiveStreaming.h"
#include "Editor/EditorLiveStreaming/Public/IEditorLiveStreaming.h"

AKCharacter::AKCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

AKCharacter::AKCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

	MainCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("MainCamera"));
	FVector MainCameraLocation(0, 0, 210);
	MainCamera->SetRelativeLocation(MainCameraLocation);
	MainCamera->AttachParent = GetCapsuleComponent();
	MainCamera->bUsePawnControlRotation = true;

	TopCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("TopCamera"));
	FVector TopCameraLocation(0, 0, 210);
	TopCamera->SetRelativeLocation(TopCameraLocation);
	TopCamera->AttachParent = GetCapsuleComponent();
	TopCamera->bUsePawnControlRotation = true;

	LeftCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("LeftCamera"));
	FVector LeftCameraLocation(0, -110, 120);
	LeftCamera->SetRelativeLocation(LeftCameraLocation);
	LeftCamera->AttachParent = GetCapsuleComponent();
	LeftCamera->bUsePawnControlRotation = true;

	RightCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("RightCamera"));
	FVector RightCameraLocation(0, 110, 120);
	RightCamera->SetRelativeLocation(RightCameraLocation);
	RightCamera->AttachParent = GetCapsuleComponent();
	RightCamera->bUsePawnControlRotation = true;

}

void AKCharacter::BeginPlay()
{

    Super::BeginPlay();
	MainCamera->SetActive(true);
	CurrentCameraIndex = 0;
	IsSwitching = false;
}

void AKCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	if (IsSwitching == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current camera is: %d"), CurrentCameraIndex);
		switch (CurrentCameraIndex)
		{
		case 0:
			TopCamera->SetActive(true);
			LeftCamera->SetActive(false);
			RightCamera->SetActive(false);
			CurrentCameraIndex = 1;
			break;
		case 1:
			TopCamera->SetActive(false);
			LeftCamera->SetActive(true);
			RightCamera->SetActive(false);
			CurrentCameraIndex = 2;
			break;
		case 2:
			TopCamera->SetActive(false);
			LeftCamera->SetActive(false);
			RightCamera->SetActive(true);
			CurrentCameraIndex = 0;
			break;
		}
	}
}

void AKCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	this->Input = InputComponent;
    InputComponent->BindAxis("MoveForward", this, &AKCharacter::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AKCharacter::MoveRight);
    InputComponent->BindAxis("Turn", this, &AKCharacter::AddControllerYawInput);
    InputComponent->BindAxis("LookUp", this, &AKCharacter::AddControllerPitchInput);
	InputComponent->BindAction("Switch", IE_Pressed, this, &AKCharacter::ToggleIsSwitching);
	InputComponent->BindAction("Stream", IE_Pressed, this, &AKCharacter::ToggleStreaming);
}

void AKCharacter::MoveForward(float Value)
{

    if ( (Controller != NULL) && (Value != 0.0f) )
    {
        // find out which way is forward
        FRotator Rotation = Controller->GetControlRotation();
        // Limit pitch when walking or falling
        if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling() )
        {
            Rotation.Pitch = 0.0f;
        }
        // add movement in that direction
        const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AKCharacter::MoveRight(float Value)
{
    if ( (Controller != NULL) && (Value != 0.0f) )
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void AKCharacter::ToggleStreaming()
{

#if WITH_EDITOR
	if (!IEditorLiveStreaming::Get().IsBroadcastingEditor()) {
		IEditorLiveStreaming::Get().StartBroadcastingEditor();
	}
	else
	{
		IEditorLiveStreaming::Get().StopBroadcastingEditor();
	}
#else
	if (!IGameLiveStreaming::Get().IsBroadcastingGame()){
		FGameBroadcastConfig Config = FGameBroadcastConfig();
		IGameLiveStreaming::Get().StartBroadcastingGame(Config);
	}
	else
	{
		IGameLiveStreaming::Get().StopBroadcastingGame();
	}
#endif
}


void AKCharacter::ToggleIsSwitching() {
	MainCamera->SetActive(false);
	IsSwitching = IsSwitching == true ? false : true;
}
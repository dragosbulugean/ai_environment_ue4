#include "kortex_environment.h"
#include "KCharacter.h"
#include "Runtime/Engine/Public/Features/ILiveStreamingService.h"
#include "Runtime/GameLiveStreaming/Public/IGameLiveStreaming.h"
#include "Editor/EditorLiveStreaming/Public/IEditorLiveStreaming.h"
#include <sstream>
#include <string>

AKCharacter::AKCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

AKCharacter::AKCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

	UCameraComponent* LeftTopCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("LeftTopCamera"));
	LeftTopCamera->AttachParent = GetCapsuleComponent();
	LeftTopCamera->RelativeLocation = FVector(0, 0, 150.0f + BaseEyeHeight);
	LeftTopCamera->bUsePawnControlRotation = true;

	UCameraComponent* RightTopCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("RightTopCamera"));
	RightTopCamera->AttachParent = GetCapsuleComponent();
	RightTopCamera->RelativeLocation = FVector(0, 0, 150.0f + BaseEyeHeight);
	RightTopCamera->bUsePawnControlRotation = true;

	UCameraComponent* LeftBottomCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("LeftBottomCamera"));
	LeftBottomCamera->AttachParent = GetCapsuleComponent();
	LeftBottomCamera->RelativeLocation = FVector(0, 0, 150.0f + BaseEyeHeight);
	LeftBottomCamera->bUsePawnControlRotation = true;

	auto RightBottomCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("RightBottomCamera"));
	RightBottomCamera->AttachParent = GetCapsuleComponent();
	RightBottomCamera->RelativeLocation = FVector(0, 0, 150.0f + BaseEyeHeight);
	RightBottomCamera->bUsePawnControlRotation = true;

	Cameras.Add(LeftTopCamera);
	Cameras.Add(RightTopCamera);
	Cameras.Add(LeftBottomCamera);
	Cameras.Add(RightBottomCamera);

}

void AKCharacter::BeginPlay()
{
    Super::BeginPlay();

	PlayerController = Cast<APlayerController>(Controller);

	CurrentCameraIndex = 0;
	PlayerController->SetViewTarget(Cameras[CurrentCameraIndex]->GetOwner());

	UE_LOG(LogTemp, Warning, TEXT("Made 1 player!"));
}

void AKCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	CurrentCameraIndex++;
	if (CurrentCameraIndex == 4)
	{
		CurrentCameraIndex = 0;
	}
	PlayerController->SetViewTarget(Cameras[CurrentCameraIndex]->GetOwner());
}

void AKCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	this->Input = InputComponent;
    InputComponent->BindAxis("MoveForward", this, &AKCharacter::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AKCharacter::MoveRight);
    InputComponent->BindAxis("Turn", this, &AKCharacter::AddControllerYawInput);
    InputComponent->BindAxis("LookUp", this, &AKCharacter::AddControllerPitchInput);
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

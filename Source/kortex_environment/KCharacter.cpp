#include "kortex_environment.h"
#include "KCharacter.h"

AKCharacter::AKCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

AKCharacter::AKCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

    FirstPersonCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();
    FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 50.0f + BaseEyeHeight);
    FirstPersonCameraComponent->bUsePawnControlRotation = true;
    
    LSceneCaptureComponent2D = ObjectInitializer.CreateDefaultSubobject<USceneCaptureComponent2D>(this, TEXT("LScene"));
    static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> LMinimapTexObj(TEXT("TextureRenderTarget2D'/Game/TextureTargets/TextureLeft.TextureLeft'"));
    LTextureRenderTarget2D = LMinimapTexObj.Object;
    LSceneCaptureComponent2D->TextureTarget = LTextureRenderTarget2D;
    LCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("LCamera"));
    LSceneCaptureComponent2D->AttachParent = LCameraComponent;
    LCameraComponent->AttachParent = GetCapsuleComponent();
    LCameraComponent->RelativeLocation = FVector(0, 0, 50.0f + BaseEyeHeight);
    LCameraComponent->bUsePawnControlRotation = true;
    
    RSceneCaptureComponent2D = ObjectInitializer.CreateDefaultSubobject<USceneCaptureComponent2D>(this, TEXT("RScene"));
    static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RMinimapTexObj(TEXT("TextureRenderTarget2D'/Game/TextureTargets/TextureRight.TextureRight'"));
    RTextureRenderTarget2D = RMinimapTexObj.Object;
    RSceneCaptureComponent2D->TextureTarget = RTextureRenderTarget2D;
    RCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("RCamera"));
    RSceneCaptureComponent2D->AttachParent = RCameraComponent;
    RCameraComponent->AttachParent = GetCapsuleComponent();
    RCameraComponent->RelativeLocation = FVector(0, 0, 50.0f + BaseEyeHeight);
    RCameraComponent->bUsePawnControlRotation = true;
}

void AKCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Kortex AI"));
    }
}

void AKCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AKCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
    InputComponent->BindAxis("MoveForward", this, &AKCharacter::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AKCharacter::MoveRight);
    InputComponent->BindAxis("Turn", this, &AKCharacter::AddControllerYawInput);
    InputComponent->BindAxis("LookUp", this, &AKCharacter::AddControllerPitchInput);
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


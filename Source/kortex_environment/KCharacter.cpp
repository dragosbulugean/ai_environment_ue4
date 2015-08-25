//#include <vector>
#include "kortex_environment.h"
#include "KCharacter.h"

#include <sstream>
#include <string>

#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>




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
    
    /* 
        Left Camera
    */
    LSceneCaptureComponent2D = ObjectInitializer.CreateDefaultSubobject<USceneCaptureComponent2D>(this, TEXT("LScene"));
    LTextureRenderTarget2D = ObjectInitializer.CreateDefaultSubobject<UTextureRenderTarget2D>(this, TEXT("LTexture"));
    LTextureRenderTarget2D->InitAutoFormat(512, 512);
    LTextureRenderTarget2D->AddressX = TA_Wrap;
    LTextureRenderTarget2D->AddressY = TA_Wrap;

    LSceneCaptureComponent2D->TextureTarget = LTextureRenderTarget2D;
    LCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("LCamera"));
    LSceneCaptureComponent2D->AttachParent = LCameraComponent;
    LCameraComponent->AttachParent = GetCapsuleComponent();
    LCameraComponent->RelativeLocation = FVector(0, 0, 50.0f + BaseEyeHeight);
    LCameraComponent->bUsePawnControlRotation = true;
    
    
    
    /*
        Right Camera
     */
    RSceneCaptureComponent2D = ObjectInitializer.CreateDefaultSubobject<USceneCaptureComponent2D>(this, TEXT("RScene"));
    RTextureRenderTarget2D = ObjectInitializer.CreateDefaultSubobject<UTextureRenderTarget2D>(this, TEXT("RTexture"));
    RTextureRenderTarget2D->InitAutoFormat(512, 512);
    RTextureRenderTarget2D->AddressX = TA_Wrap;
    RTextureRenderTarget2D->AddressY = TA_Wrap;
    
    RSceneCaptureComponent2D->TextureTarget = RTextureRenderTarget2D;
    RCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("RCamera"));
    RSceneCaptureComponent2D->AttachParent = RCameraComponent;
    RCameraComponent->AttachParent = GetCapsuleComponent();
    RCameraComponent->RelativeLocation = FVector(0, 0, 50.0f + BaseEyeHeight);
    RCameraComponent->bUsePawnControlRotation = true;
    
    index = 0;
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
    
    if (AKCharacter::LTextureRenderTarget2D->TextureReference.IsInitialized())
    {
        int width = LTextureRenderTarget2D->SizeX;
        int height = LTextureRenderTarget2D->SizeY;
        TArray<FColor>* pix = new TArray<FColor>();
        LTextureRenderTarget2D->GameThread_GetRenderTargetResource()->ReadPixels(*pix);
        
        FColor* data = pix->GetData();
        
        FColor* matrix[height];
        for (int i = 0; i < height; i++){
            matrix[i] = new FColor[width];
            for(int j=0; j<width; j++){
                matrix[i][j] = data[i*width+j];
            }
        }

        using namespace boost::gil;
        rgb8_image_t img(height, width);
        
        for (int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                FColor currentPx = data[i*width+j];
                rgba8_pixel_t px (currentPx.R, currentPx.G, currentPx.B, currentPx.A);
                view(img)(j,i) = px;
            }
        }
        
        // Uncomment this to capture every frame :)
        //        std::stringstream ss;
        //        ss << "~/Desktop/frames/frame" << index << ".png";
        //        std::string src = ss.str();
        
        std::string src = "/Users/andrei.leica/Desktop/frame.png";
        png_write_view(src, const_view(img));
        index++;
    }
    
    //GetLeftPixelMatrix();
    //GetRightPixelMatrix();
    
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

void AKCharacter::GetPixelMatrixFromTexture(UTextureRenderTarget2D* TextureRenderTarget2D) {
    
    UTexture2D* Texture = TextureRenderTarget2D->ConstructTexture2D(this, "AlphaTex", EObjectFlags::RF_NoFlags, CTF_DeferCompression);
    FTexture2DMipMap* MyMipMap = &Texture->PlatformData->Mips[0];
    FByteBulkData* RawImageData = &MyMipMap->BulkData;
    FColor* FormatedImageData = static_cast<FColor*>( RawImageData->Lock( LOCK_READ_ONLY ) );
    uint32 TextureWidth = MyMipMap->SizeX, TextureHeight = MyMipMap->SizeY;
    FColor PixelColor;
    
    for(int i=0; i<TextureWidth; i++)
    {
        for(int j=0; j<TextureHeight; j++)
        {
            PixelColor = FormatedImageData[i * TextureWidth + j];
        }
    }
    
    RawImageData->Unlock();

}

void AKCharacter::GetLeftPixelMatrix() {
    return GetPixelMatrixFromTexture(LTextureRenderTarget2D);
}

void AKCharacter::GetRightPixelMatrix() {
    return GetPixelMatrixFromTexture(RTextureRenderTarget2D);
}




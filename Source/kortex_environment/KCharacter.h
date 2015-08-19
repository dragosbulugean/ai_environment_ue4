#pragma once

#include "GameFramework/Character.h"
#include "KCharacter.generated.h"

UCLASS()
class KORTEX_ENVIRONMENT_API AKCharacter : public ACharacter
{
	GENERATED_BODY()

public:
    
    AKCharacter();
    AKCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
    
    UFUNCTION()
    void MoveForward(float Val);
    UFUNCTION()
    void MoveRight(float Val);
    
    /** First person camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    UCameraComponent* FirstPersonCameraComponent;
    
    /** Left eye camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    UCameraComponent* LCameraComponent;
    
    /** Left eye scenecapture */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    USceneCaptureComponent2D* LSceneCaptureComponent2D;
    
    /** Left eye texture render target */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    UTextureRenderTarget2D* LTextureRenderTarget2D;
    
    /** Right eye camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    UCameraComponent* RCameraComponent;
    
    /** Right eye scenecapture */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    USceneCaptureComponent2D* RSceneCaptureComponent2D;
    
    /** Right eye texture render target */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    UTextureRenderTarget2D* RTextureRenderTarget2D;

};

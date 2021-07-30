// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "TestProject/Items/ItemStruct.h"
#include "PlayerCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

private:
	/** FPS mesh */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;
	
	// /** Soft light around player for dark areas*/
	// UPROPERTY(VisibleDefaultsOnly, Category=Lights)
	// class UPointLightComponent* PointLight;

	/** FPS camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Displays inventory item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HeldItemMeshComponent;

	/** Item player is currently looking at */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	class AItem* ViewedItem;
	
	/** Held item struct */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	FItemDef HeldItemDef;

	/** How far away can player pickup items */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	float ItemReach = 200.f;

	/** How far away dropped items will land */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	float ItemDropDistance = 100.f;

	UPROPERTY()
	class ATestProjectPlayerController* PlayerController;

public:
	APlayerCharacter();
	
	void TraceView();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	bool bHasItem;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay();
	
	void MoveForward(float Val);
	void MoveRight(float Val);

	UFUNCTION()
	void PickupItem();

	UFUNCTION()
	void ThrowItem();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
};


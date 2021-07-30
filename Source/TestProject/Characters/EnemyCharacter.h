// Roman Galikov, 2021

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum EChaseMode
{
	Hunt	UMETA(DisplayName = "Hunt"),
	Evade	UMETA(DisplayName = "Evade")
};

UCLASS()
class TESTPROJECT_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_BODY()

private:
	AEnemyCharacter();

	/** Changes mesh and components if chasemode changed */
	UFUNCTION()
	void ChangeMode(TEnumAsByte<EChaseMode> NewChaseMode);

	/** Checks blackboard if player has item */
	UFUNCTION()
	void CheckMode();

	UPROPERTY(VisibleDefaultsOnly, Category=Lights)
	class UPointLightComponent* PointLight;

	/** Head light */
	UPROPERTY(VisibleDefaultsOnly, Category=Lights)
	class USpotLightComponent* SpotLight;

	/** Plays when entering hunt mode */
	UPROPERTY(EditAnywhere, Category=Sounds)
	UAudioComponent* HuntSound;

	/** Plays when entering evade mode */
	UPROPERTY(EditAnywhere, Category=Sounds)
	UAudioComponent* EvadeSound;

	virtual void PostInitializeComponents() override;
	
public:
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mode")
	TEnumAsByte<EChaseMode> CurrentChaseMode;

	/** Visor material in hunt mode */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Mode")
	UMaterialInstance* HuntVisorMaterial;

	/** Visor material in evade mode */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Mode")
	UMaterialInstance* EvadeVisorMaterial;

};

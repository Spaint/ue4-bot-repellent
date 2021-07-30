// Roman Galikov, 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemStruct.h"
#include "Item.generated.h"

UCLASS()
class TESTPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
private:
	/** Actual collision */
	UPROPERTY()
	class USphereComponent* CollisionComponent;
	
	/** Displayed mesh */
	UPROPERTY(VisibleAnywhere, Category="Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	virtual void PostInitializeComponents() override;

	virtual void PostRegisterAllComponents() override;

	/** Looks through DT to find itemdef and tries to asyncload mesh*/
	UFUNCTION()
	void FindMesh();

	/** Asyncload callback */
	UFUNCTION()
	void SetupMesh(TSoftObjectPtr<UStaticMesh> MeshPtr);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
public:	

	AItem();
	
	virtual void Tick(float DeltaTime) override;
	
	/** Handles pickup by player */
	UFUNCTION()
	void Pickup();

	/** Found item struct */
	UPROPERTY()
	FItemDef ItemDef;

	/** Type of item for editor setup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TEnumAsByte<EItemType> ItemType;

protected:

	virtual void BeginPlay() override;
};

// Roman Galikov, 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestProjectPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API ATestProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

private:

	ATestProjectPlayerController();
	

public:

	/** Handles HUD updates */
	UFUNCTION()
	void UpdateHUD();
	
protected:
	
	virtual void OnPossess(class APawn* InPawn) override;

	TSubclassOf<class UUserWidget> WidgetGameClass;
	
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UWidgetBase* WidgetGame;
};

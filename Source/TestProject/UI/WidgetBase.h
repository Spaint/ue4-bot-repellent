// Roman Galikov, 2021

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UWidgetBase : public UUserWidget
{
	GENERATED_BODY()

private:
	
public:
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Update();
	
protected:

};

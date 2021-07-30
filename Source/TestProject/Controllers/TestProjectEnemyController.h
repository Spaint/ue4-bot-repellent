// Roman Galikov, 2021

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TestProjectEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API ATestProjectEnemyController : public AAIController
{
	GENERATED_BODY()
	
private:

	UPROPERTY(transient)
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorComp;

public:
	ATestProjectEnemyController();

protected:
	virtual void OnPossess(class APawn* InPawn) override;
};

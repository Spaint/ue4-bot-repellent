// Roman Galikov, 2021

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_FindPlayer.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UBTS_FindPlayer : public UBTService
{
	GENERATED_BODY()

private:

public:
	UBTS_FindPlayer();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = Player)
	struct FBlackboardKeySelector PlayerKey;

	UPROPERTY(EditAnywhere, Category = Player)
	struct FBlackboardKeySelector PlayerItemKey;
	
protected:

	
};

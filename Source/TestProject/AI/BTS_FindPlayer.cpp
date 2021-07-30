// Roman Galikov, 2021


#include "BTS_FindPlayer.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "TestProject/Characters/PlayerCharacter.h"

UBTS_FindPlayer::UBTS_FindPlayer()
{
	NodeName = "Find Player";
 
	bNotifyTick = true;
	bTickIntervals = true;
	bNotifyBecomeRelevant = false;
	bNotifyCeaseRelevant = false;
	
	PlayerKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_FindPlayer, PlayerKey), AActor::StaticClass());
	
	PlayerKey.SelectedKeyName = FName("Player");
	PlayerKey.SelectedKeyType = UBlackboardKeyType_Object::StaticClass();

	PlayerItemKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_FindPlayer, PlayerItemKey));
	
	PlayerItemKey.SelectedKeyName = FName("PlayerHasItem");
	PlayerItemKey.SelectedKeyType = UBlackboardKeyType_Bool::StaticClass();
 
	Interval = 0.2f;
	RandomDeviation = 0.f;
}

void UBTS_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if(!UGameplayStatics::GetPlayerPawn(this, 0)){return;}
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if(!PlayerCharacter){return;}
	
	Blackboard->SetValueAsObject(PlayerKey.SelectedKeyName, PlayerCharacter);
	Blackboard->SetValueAsBool(PlayerItemKey.SelectedKeyName, PlayerCharacter->bHasItem);

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
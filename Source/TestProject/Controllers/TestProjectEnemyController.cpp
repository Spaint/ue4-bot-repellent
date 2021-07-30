// Roman Galikov, 2021


#include "TestProjectEnemyController.h"
#include "TestProject/Characters/EnemyCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

ATestProjectEnemyController::ATestProjectEnemyController()
{
 	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));
 	
	BrainComponent = BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}

void ATestProjectEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemyCharacter* PossessedCharacter = Cast<AEnemyCharacter>(InPawn);
	if (!PossessedCharacter){return;}
	
	UBehaviorTree* BehaviorTree = LoadObject<UBehaviorTree>(nullptr, TEXT("BehaviorTree'/Game/TestProject/Blueprints/AI/BT_EnemyAI.BT_EnemyAI'"));
	if(!BehaviorTree || !BehaviorTree->BlackboardAsset){return;}
	
	BlackboardComp->InitializeBlackboard(*(BehaviorTree->BlackboardAsset));
	BehaviorComp->StartTree(*(BehaviorTree));
}
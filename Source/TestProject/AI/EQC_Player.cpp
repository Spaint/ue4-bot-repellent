// Roman Galikov, 2021


#include "EQC_Player.h"
#include "GameFramework/Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/GameplayStatics.h"

void UEQC_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	if(!UGameplayStatics::GetPlayerPawn(this, 0)){return;}
	
	AActor* Player = Cast<AActor>(UGameplayStatics::GetPlayerPawn(this, 0));

	if(!Player){return;}
	
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, Player);
}

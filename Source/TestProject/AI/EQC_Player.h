// Roman Galikov, 2021

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQC_Player.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UEQC_Player : public UEnvQueryContext
{
	GENERATED_BODY()
	
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

};

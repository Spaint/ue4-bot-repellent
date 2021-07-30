#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemStruct.generated.h"

UENUM(BlueprintType)
enum EItemType
{
	Rock	UMETA(DisplayName = "Rock"),
	Paper	UMETA(DisplayName = "Paper"),
	Bottle	UMETA(DisplayName = "Bottle")
};

USTRUCT(BlueprintType)
struct FItemDef : public FTableRowBase
{
	GENERATED_BODY()

public:
	
	FItemDef():Type(EItemType::Rock), Name(TEXT("")),Mesh(nullptr){}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Item")
	TEnumAsByte<EItemType> Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Item")
	FString Name;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Item")
	TSoftObjectPtr<UStaticMesh> Mesh;
};

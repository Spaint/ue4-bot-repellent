// Roman Galikov, 2021

#include "Item.h"
#include "Components/SphereComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	//Using sphere collision instead of actual mesh for ease of pickup
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionProfileName("OverlapAllDynamic");
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(CollisionComponent);
	MeshComponent->SetCollisionProfileName("NoCollision");
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Simple animation effect
	AddActorLocalRotation(FRotator(0,3,0));
}

void AItem::Pickup()
{
	//Just destroy it for now
	Destroy();
}



void AItem::FindMesh()
{
	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/TestProject/Data/DT_Items.DT_Items'"));
	
	if (DataTable)
	{
		TArray<FItemDef*> AllRows;
		DataTable->GetAllRows<FItemDef>("Item mesh search", AllRows);
		for(FItemDef* Row : AllRows)
		{
			if(Row->Type==ItemType)
			{
				ItemDef = *Row;
				if (!Row->Mesh.IsNull())
				{
					FStreamableManager& AssetLoad = UAssetManager::GetStreamableManager();
					AssetLoad.RequestAsyncLoad(TArray<FSoftObjectPath>{Row->Mesh.ToSoftObjectPath()}, FStreamableDelegate::CreateUObject(this, &AItem::SetupMesh, Row->Mesh));
				}
				break;
			}
		}
	}
}

void AItem::SetupMesh(TSoftObjectPtr<UStaticMesh> MeshPtr)
{
	UStaticMesh* LoadedMesh = MeshPtr.Get();
	if(LoadedMesh)
	{
		MeshComponent->SetStaticMesh(LoadedMesh);
	}
}

void AItem::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
	FindMesh();
}

void AItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	FindMesh();
}

#if WITH_EDITOR
void AItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//Update item mesh after changing its type
	if(PropertyChangedEvent.Property)
	{
		FString PropertyName = PropertyChangedEvent.Property->GetNameCPP();
		if (PropertyName == TEXT("ItemType"))
		{
			FindMesh();
		}
	}
}
#endif

// Roman Galikov, 2021

#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PointLightComponent.h"
#include "TestProject/Controllers/TestProjectPlayerController.h"
#include "TestProject/Items/Item.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	HeldItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeldItemMesh"));
	HeldItemMeshComponent->SetupAttachment(Mesh1P, "GripPoint");
	HeldItemMeshComponent->bCastDynamicShadow = false;
	HeldItemMeshComponent->CastShadow = false;
	HeldItemMeshComponent->SetRelativeScale3D(FVector(0.3, 0.3, 0.3));

	// PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	// PointLight->bUseInverseSquaredFalloff = true;
	// PointLight->AttenuationRadius = 500.f;
	// PointLight->Intensity = 100.f;
	// PointLight->SourceRadius = 100.f;
	// PointLight->SetLightColor(FLinearColor(255,255,100));
	// PointLight->SetupAttachment(Mesh1P, "spine_01");

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMeshFinder(TEXT("/Game/TestProject/Assets/Character/Mesh/SK_Mannequin_Arms"));
	if(PlayerMeshFinder.Succeeded())
	{
		Mesh1P->SetSkeletalMesh(PlayerMeshFinder.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> PlayerAnimFinder(TEXT("/Game/TestProject/Assets/Animations/FirstPerson_AnimBP"));
	if(PlayerAnimFinder.Succeeded())
	{
		Mesh1P->SetAnimInstanceClass(PlayerAnimFinder.Class);
	}
}

void APlayerCharacter::TraceView()
{
	if(!PlayerController){return;}
	
	FHitResult Hit;
	FVector CameraLocation;
	FRotator CameraRotation;
	
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
	const FVector TraceEnd = CameraLocation + CameraRotation.Vector()*ItemReach;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, TraceEnd, ObjectTypes);

	//Weird structure with pawn telling controller to update HUD, but will do for now
	if(Hit.bBlockingHit&&Hit.GetActor())
	{
		ViewedItem = Cast<AItem>(Hit.GetActor());
		if(ViewedItem)
		{
			PlayerController->UpdateHUD();
		}
	}
	else if (ViewedItem)
	{
		ViewedItem = nullptr;
		PlayerController->UpdateHUD();
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TraceView();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<ATestProjectPlayerController>(GetController());
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &APlayerCharacter::PickupItem);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &APlayerCharacter::ThrowItem);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerCharacter::PickupItem()
{
	if(bHasItem||!ViewedItem){return;}

	HeldItemDef = ViewedItem->ItemDef;
	
	//Item mesh should still be referenced
	HeldItemMeshComponent->SetStaticMesh(HeldItemDef.Mesh.Get());
	
	ViewedItem->Pickup();
	ViewedItem=nullptr;
	bHasItem = true;

	if(PlayerController)
	{
		//Weird structure with pawn telling controller to update HUD, but will do for now
		PlayerController->UpdateHUD();
	}
}

void APlayerCharacter::ThrowItem()
{
	if(!bHasItem){return;}

	const FTransform SpawnTransform(GetActorRotation(), GetActorLocation()+GetActorRotation().Vector()*ItemDropDistance);
	
	AItem* SpawningItem = Cast<AItem>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, AItem::StaticClass(), SpawnTransform));
	if (SpawningItem)
	{
		SpawningItem->ItemType = HeldItemDef.Type;
		UGameplayStatics::FinishSpawningActor(SpawningItem, SpawnTransform);
	}

	HeldItemDef = FItemDef();
	bHasItem = false;
	HeldItemMeshComponent->SetStaticMesh(nullptr);
	
	//Weird structure with pawn telling controller to update HUD, but will do for now
	if(PlayerController)
	{
		PlayerController->UpdateHUD();
	}
}

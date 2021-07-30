// Roman Galikov, 2021


#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/AudioComponent.h"
#include "TestProject/Controllers/TestProjectEnemyController.h"

AEnemyCharacter::AEnemyCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMeshFinder(TEXT("SkeletalMesh'/Game/SciFi_Robot/MESHES/SCIFI_ROBOT_IK_SK.SCIFI_ROBOT_IK_SK'"));
	if(EnemyMeshFinder.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(EnemyMeshFinder.Object);
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -80.f));
		GetMesh()->CastShadow = false;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> EnemyAnimFinder(TEXT("/Game/SciFi_Robot/Animations/ThirdPerson_AnimBP"));
	if(EnemyAnimFinder.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(EnemyAnimFinder.Class);
	}
	
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	PointLight->bUseInverseSquaredFalloff = false;
	PointLight->AttenuationRadius = 200.f;
	PointLight->Intensity = 20.f;
	PointLight->SourceRadius = 100.f;
	PointLight->SetLightColor(FLinearColor(255,0,0));

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spotlight"));
	SpotLight->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	SpotLight->AttenuationRadius = 2000.f;
	SpotLight->Intensity = 20000.f;
	SpotLight->OuterConeAngle = 25.f;
	SpotLight->SetLightColor(FLinearColor(255,0,0));

	if(GetMesh()->SkeletalMesh)
	{
		PointLight->SetupAttachment(GetMesh(), "spine_01");
		SpotLight->SetupAttachment(GetMesh(), "head");
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> HuntVisorFinder(TEXT("MaterialInstanceConstant'/Game/SciFi_Robot/MATERIALS/VISOR/MI_Visor_Red.MI_Visor_Red'"));
	if(HuntVisorFinder.Succeeded())
	{
		HuntVisorMaterial = HuntVisorFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> EvadeVisorFinder(TEXT("MaterialInstanceConstant'/Game/SciFi_Robot/MATERIALS/VISOR/MI_Visor_Green.MI_Visor_Green'"));
	if(EvadeVisorFinder.Succeeded())
	{
		EvadeVisorMaterial = EvadeVisorFinder.Object;
	}

	HuntSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Hunt Sound"));
	HuntSound->SetupAttachment(RootComponent);
	HuntSound->SetAutoActivate(false);
	
	static ConstructorHelpers::FObjectFinder<USoundBase> HuntSoundFinder(TEXT("SoundCue'/Game/TestProject/Assets/Audio/A_Hunt_Cue.A_Hunt_Cue'"));
	if(HuntSoundFinder.Succeeded())
	{
		HuntSound->SetSound(HuntSoundFinder.Object);
	}

	
	EvadeSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Evade Sound"));
	EvadeSound->SetupAttachment(RootComponent);
	EvadeSound->SetAutoActivate(false);
	
	static ConstructorHelpers::FObjectFinder<USoundBase> EvadeSoundFinder(TEXT("SoundCue'/Game/TestProject/Assets/Audio/A_Evade_Cue.A_Evade_Cue'"));
	if(EvadeSoundFinder.Succeeded())
	{
		EvadeSound->SetSound(EvadeSoundFinder.Object);
	}


	AIControllerClass = ATestProjectEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//Limits checkmode
	SetActorTickInterval(0.2f);

	//Smooth turns
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

void AEnemyCharacter::ChangeMode(TEnumAsByte<EChaseMode> NewChaseMode)
{
	if(CurrentChaseMode==NewChaseMode){return;}

	CurrentChaseMode = NewChaseMode;

	switch (CurrentChaseMode)
	{
	case Hunt:
		GetMesh()->SetMaterialByName("VISOR_MAT", HuntVisorMaterial);
		PointLight->SetLightColor(FLinearColor(255,0,0));
		SpotLight->SetLightColor(FLinearColor(255,0,0));
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
		if(!HuntSound->IsPlaying())
		{
			HuntSound->Play();
		}
		break;
	case Evade:
		GetMesh()->SetMaterialByName("VISOR_MAT", EvadeVisorMaterial);
		PointLight->SetLightColor(FLinearColor(0,255,0));
		SpotLight->SetLightColor(FLinearColor(0,255,0));
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		if(!EvadeSound->IsPlaying())
		{
			EvadeSound->Play();
		}
		break;
	default:
		break;
	}
}

void AEnemyCharacter::CheckMode()
{
	ATestProjectEnemyController* AIController = Cast<ATestProjectEnemyController>(GetController());
	if(!AIController){return;}
	
	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if(!Blackboard){return;}
	
	ChangeMode(Blackboard->GetValueAsBool("PlayerHasItem")? Evade : Hunt);
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckMode();
}

void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
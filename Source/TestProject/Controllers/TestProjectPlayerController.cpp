// Roman Galikov, 2021


#include "TestProjectPlayerController.h"
#include "TestProject/UI/WidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "TestProject/Characters/PlayerCharacter.h"

ATestProjectPlayerController::ATestProjectPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetFinder(TEXT("/Game/TestProject/Blueprints/UI/WBP_HUD"));
	if(WidgetFinder.Succeeded())
	{
		WidgetGameClass = WidgetFinder.Class;
	}
}

void ATestProjectPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	APlayerCharacter* PossessedCharacter = Cast<APlayerCharacter>(InPawn);
	if (!PossessedCharacter){return;}

	WidgetGame = CreateWidget<UWidgetBase>(this, WidgetGameClass);
	if (!WidgetGame) return;

	WidgetGame->AddToViewport();
}

void ATestProjectPlayerController::UpdateHUD()
{
	if(!WidgetGame){return;}
	WidgetGame->Update();
}

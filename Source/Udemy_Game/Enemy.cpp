#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainChr.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600.0f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(90.0f);

	bOverlappingCombatSphere = false;

	//Stats
	Health = 75.0f;
	MaxHealth = 100.0f;
	Damage = 10.0f;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());

	//Bind Overlap events to overlap component
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		AMainChr* Main = Cast<AMainChr>(OtherActor);
		if (Main)
		{
			MoveToTarget(Main);
		}
	}
}

void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{ 
	if (OtherActor != nullptr)
	{
		AMainChr* Main = Cast<AMainChr>(OtherActor);
		if (Main)
		{
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			if (AIController != nullptr)
			{
				AIController->StopMovement();
			}
		}
	}
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		AMainChr* Main = Cast<AMainChr>(OtherActor);
		if (Main)
		{
			bOverlappingCombatSphere = true;

			//Set target reference
			CombatTarget = Main;

			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
	}
}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr)
	{
		AMainChr* Main = Cast<AMainChr>(OtherActor);
		if (Main)
		{
			bOverlappingCombatSphere = false; 

			if (EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking)
			{
				//Unset target reference
				CombatTarget = nullptr;

				MoveToTarget(Main);
			}
		}
	}
}

void AEnemy::MoveToTarget(AMainChr* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController)
	{
		//Create 2 data types needed for the MoveTo function
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(25.0f);

		FNavPathSharedPtr NavPath;

		//Movement
		AIController->MoveTo(MoveRequest, &NavPath);
		
		/*TArray<FNavPathPoint> PathPoints = NavPath->GetPathPoints(); 
		for (auto point : PathPoints) 
			UKismetSystemLibrary::DrawDebugSphere(this, point.Location, 25.0f, 24, FLinearColor::Red, 10.0f, 1.5f);*/
	}
}


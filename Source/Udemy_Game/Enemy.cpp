#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainChr.h"
#include "Components/BoxComponent.h"
#include "MainChr.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600.0f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(90.0f);

	//Attack box collision to claw socket 
	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Enemy_Socket"));

	bOverlappingCombatSphere = false;
	bAttacking = false;

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
	  
	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatBoxOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatBoxOnOverlapEnd);

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); //Activate collisions while attacking 
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //First ignore everything and then set collision to pawn
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); //Player is a pawn, Overlap with it
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

			Attack();
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

void AEnemy::CombatBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		AMainChr* main = Cast<AMainChr>(OtherActor);
		if (main != nullptr)
		{
			if (main->HitParticles != nullptr)
			{
				const USkeletalMeshSocket* tipSocket = GetMesh()->GetSocketByName("Tip_Socket");
				if (tipSocket != nullptr)
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), main->HitParticles, tipSocket->GetSocketLocation(GetMesh()), FRotator::ZeroRotator, true);
				else
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), main->HitParticles, main->GetTargetLocation(), FRotator::ZeroRotator, true);
			}
			if (main->HitSound != nullptr) //Play hit sound
				UGameplayStatics::PlaySound2D(this, main->HitSound);
		}
	}
}

void AEnemy::CombatBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEnemy::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //Not use physics, Only generate overlap events
}

void AEnemy::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Attack()
{
	if (AIController != nullptr)
	{
		//Stop
		AIController->StopMovement(); 
		//Set Attack State
		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
	}
	if (!bAttacking)
	{
		bAttacking = true;
		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (animInstance != nullptr)
		{
			animInstance->Montage_Play(CombatMontage, 1.35f);
			animInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
		} 
	}
}

void AEnemy::AttackEnd()
{
	bAttacking = false;

	//Attack again if still overlapping with the player 
	if (bOverlappingCombatSphere)
		Attack();
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


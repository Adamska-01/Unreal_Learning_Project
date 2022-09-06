#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "MainChr.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


AWeapon::AWeapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	bWeaponParticle = false;
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor != nullptr)
	{
		AMainChr* main = Cast<AMainChr>(OtherActor);
		if (main != nullptr)
		{
			main->SetActiveOverlappingItem(this);
		}
	}
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor != nullptr)
	{
		AMainChr* main = Cast<AMainChr>(OtherActor);
		if (main != nullptr)
		{
			main->SetActiveOverlappingItem(nullptr);
		}
	}
}

void AWeapon::Equip(AMainChr* Chr)
{
	if (Chr != nullptr)
	{
		//Makes sure that the camera does not zoom in if the sword gets 
		//between the character and the camera. Also ignore collisions
		//with the pawn itself
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		//Stop simulating physics
		SkeletalMesh->SetSimulatePhysics(false);

		//Attach the sword to the character's hand
		const USkeletalMeshSocket* rightHandSocket = Chr->GetMesh()->GetSocketByName("RightHandSocket");
		if (rightHandSocket != nullptr)
		{
			//Attach
			rightHandSocket->AttachActor(this, Chr->GetMesh());

			//Reset position (preview is based on object pos and rot zeroed out
			SkeletalMesh->SetRelativeLocation(FVector(0.0f));
			SkeletalMesh->SetRelativeRotation(FRotator(0.0f));
			
			//Turn off rotation functionality (while still an pickuppable item)
			bRotate = false;

			Chr->SetEquippedWeapon(this);
		}

		if (OnEquipSound != nullptr) 
			UGameplayStatics::PlaySound2D(this, OnEquipSound);
		
		if (!bWeaponParticle)
			IdleParticlesComponent->Deactivate();
	}
}

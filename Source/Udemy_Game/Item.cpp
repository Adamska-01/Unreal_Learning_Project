#include "Item.h"
#include "Components/SphereComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	SetRootComponent(CollisionVolume);

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{ 
	UE_LOG(LogTemp, Warning, TEXT("Super::OnOverlapBegin()"));
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{ 
	UE_LOG(LogTemp, Warning, TEXT("Super::OnOverlapEnd()"));
}


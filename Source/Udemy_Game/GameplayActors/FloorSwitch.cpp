#include "FloorSwitch.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

AFloorSwitch::AFloorSwitch()
{ 
	PrimaryActorTick.bCanEverTick = true;

	//Trigger Box
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	 
	//Trigger Box Collision's settings
	TriggerBox->SetGenerateOverlapEvents(true); //Ensures events are sent
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //Basically same as trigger in Unity
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic); 
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //Set response to all channels
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); //Set response to a particular channel
	//Box Size
	TriggerBox->SetBoxExtent(FVector(62.0f, 62.0f, 32.0f));

	//Floor Switch
	FloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSwitch"));
	FloorSwitch->SetupAttachment(GetRootComponent());

	//Door
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(GetRootComponent());

	//Timer
	SwitchTime = 2.0f; 
	bCharacterOnSwitch = false;
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	//Set collision functions
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapEnd);

	InitialDoorLocation = Door->GetComponentLocation();
	InitialSwitchLocation = FloorSwitch->GetComponentLocation();
}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
	 
	bCharacterOnSwitch = true;

	RaiseDoor();
	LowerFloorSwitch(); 
}

void AFloorSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap End"));
	 
	bCharacterOnSwitch = false;

	//Set Delay for function call (CloseDoor)
	GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitch::CloseDoor, SwitchTime); 
}

void AFloorSwitch::UpdateDoorLocation(float Z)
{
	FVector newLocation = InitialDoorLocation;
	newLocation.Z += Z;
	 
	Door->SetWorldLocation(newLocation);
}

void AFloorSwitch::UpdateFloorSwitchLocation(float Z)
{
	FVector newLocation = InitialSwitchLocation;
	newLocation.Z += Z;

	FloorSwitch->SetWorldLocation(newLocation);
}

void AFloorSwitch::CloseDoor()
{
	if (!bCharacterOnSwitch)
	{
		LowerDoor();
		RaiseFloorSwitch(); 
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameActor_GravityPlanet.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MiniGame/Character/MiniGamePlayerCharacter.h"
#include "MiniGame/Logging/MiniGameLogChannels.h"


// Sets default values
AMiniGameActor_GravityPlanet::AMiniGameActor_GravityPlanet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	check(CapsuleComponent);
}

// Called when the game starts or when spawned
void AMiniGameActor_GravityPlanet::BeginPlay()
{
	Super::BeginPlay();

	if (false == ensure(CapsuleComponent))
	{
		UE_LOG(LogMiniGame, Warning, TEXT("GravityPlanet [%s] CapsuleComponent is not valid!"), *GetName());
	}
	else
	{
		CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AMiniGameActor_GravityPlanet::OnCapsuleBeginOverlap);
		CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AMiniGameActor_GravityPlanet::OnCapsuleEndOverlap);
	}
}

// Called every frame
void AMiniGameActor_GravityPlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePlayerGravityDirectionByPlanet();
}

void AMiniGameActor_GravityPlanet::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AMiniGamePlayerCharacter::StaticClass()))
	{
		PlayerCharacter = Cast<AMiniGamePlayerCharacter>(OtherActor);
	}

	UpdatePlayerGravityDirectionByPlanet();
}

void AMiniGameActor_GravityPlanet::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == PlayerCharacter)
	{
		PlayerCharacter->ChangeGravityDirectionByNewDirection(FVector(0, 0, -1));
		PlayerCharacter = nullptr;
	}
}

void AMiniGameActor_GravityPlanet::UpdatePlayerGravityDirectionByPlanet() const
{
	if (false == IsValid(PlayerCharacter))
	{
		return;
	}

	PlayerCharacter->ChangeGravityDirectionByNewDirection(GetUnitGravity());
}

FVector AMiniGameActor_GravityPlanet::GetUnitGravity() const
{
	if (false == IsValid(PlayerCharacter))
	{
		return FVector(0, 0, 0);
	}

	return UKismetMathLibrary::GetDirectionUnitVector(PlayerCharacter->GetActorLocation(), GetActorLocation());
}





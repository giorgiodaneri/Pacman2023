// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// lui l'ha messa a false ma non so se farlo anche qui
	PrimaryActorTick.bCanEverTick = true;
	// devo avere oggetto della StaticMeshCOmponent, ovvero rappr grafica dell'attore
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StaticMeshComponent->SetRelativeScale3D(FVector(0.0f, 0.0f, 0.0f));
	Status = ETileStatus::EMPTY;
	// inizializzo acc a -1
	Accessible = -1;
	// inizializzo tile non assegnata
	PlayerOwner = -1;
	// costruttore della tile
	TileGridPosition = FVector2D(0, 0);

	bReplicates = true;
}

int32 ATile::GetOwner()
{
	return PlayerOwner;
}

int32 ATile::GetAccessible()
{
	return Accessible;
}

FVector2D ATile::GetGridPosition()
{
	return TileGridPosition;
}

void ATile::SetAccessibility(const int32 IsAcc)
{
	Accessible = IsAcc;
}

void ATile::SetGridPosition(const double InX, const double InY)
{
	TileGridPosition.Set(InX, InY);
}

void ATile::SetTileStatus(const int32 TileOwner, const TEnumAsByte<ETileStatus> TileStatus)
{
	PlayerOwner = TileOwner;
	Status = TileStatus;
}

// Called when the game starts or when spawned

void ATile::BeginPlay()
{
	Super::BeginPlay();

}


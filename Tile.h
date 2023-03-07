// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "string"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UENUM()
enum ETileStatus
{
	EMPTY UMETA(DispalyName = "Empty"),
	OCCUPIED UMETA(DispalyName = "Occupied"),
};

UCLASS()
class PACMANGRID_API ATile : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATile();
	// devo codificare gli stati della mattonella
	// e specificare quale attore la occupa 
	int32 GetAccessible();
	int32 GetOwner();
	FVector2D GetGridPosition();
	void SetAccessibility(const int32 Acc);
	// funzione che, date due coordinate x e y, setta la griglia
	void SetGridPosition(const double InX, const double InY);
	void SetTileStatus(const int32 TileOwner, const TEnumAsByte<ETileStatus> TileStatus);
	void BeginPlay();


protected:
	// Called when the game starts or when spawned
	// costruttore dell'attore nel gioco
	//virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		TEnumAsByte<ETileStatus> Status;

	// acc sta per accessible, mi serve a distinguere se tile può essere acceduta o meno dagli
	// actors. Se è a 0 non è accessibile, se è a 1 sì
	// 0 ==> TileBlack
	// 1 ==> TileBlue
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 Accessible;

	UPROPERTY(EditAnywhere)
		int32 PlayerOwner;


	// data coordinata spaziale assoluta mi serve funzione
	// che ritorna coordinata spaziale relativa
	UPROPERTY(EditAnywhere)
		FVector2D TileGridPosition;

	// per creare attori 
	// usare puntatori e non oggetti per rendere leggero il codice 
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;


public:
	// Called every frame
	// tutto per default funziona attraverso la tick
	// è segnale che viene inviato in broadcast a tutti gli 
	// attori che hanno quel booleano a true (ovviamente consumano risorse)

	// virtual void Tick(float DeltaTime) override;

};

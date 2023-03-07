// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "GameFramework/Actor.h"
#include "GameField.generated.h"

UCLASS()
class PACMANGRID_API AGameField : public AActor
{
	GENERATED_BODY()
	
public:
	// keeps track of tiles
	TArray<ATile*> TileArray;

	// given a position returns a tile
	TMap<FVector2D, ATile*> TileMap;

	float NormalizedCellPadding;

	const int32 NOT_ASSIGNED = -1;

	// contiene tile che deve essere spawnata, attinge al BP corrispondente, blue o black
	TSubclassOf<ATile> BlueprintVar;	
	
	// remove all signs from the field
	// verrà fatto binding con un bottone per resettare il gioco
	UFUNCTION(BlueprintCallable)
		void ResetField();

	// generate an empty game field
	UFUNCTION()
		void GenerateField();


	// BlueprintAssignable Usable with Multicast Delegates only. Exposes the property for assigning in Blueprints.
	// declare a variable of type FOnReset (delegate)
	//UPROPERTY(BlueprintAssignable)
	//	FOnReset OnResetEvent;
	// il delegate, una volta che accade evento particolare, trasmesso in broadcast, lo intercetta 
	// tutte le classi registrate ==> verrà eseguita funzione che sta dall'altra parte
	// ovvero la destroy. Trasmissione broadcast funziona come TRIGGER, ha scopo di
	// sincronizzare in risposta ad eventi asincroni

protected:
	// size of field
	// PacMan grid is 28x36
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 SizeX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 SizeY;


	// TSubclassOf template class that provides UClass type safety
	// devo avere riferimento alla tile, devo fare in modo che classe gameField
	// possa usarla. 
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ATile> TileClass;

	// tile padding dimension
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CellPadding;

	// tile size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float TileSize;

	// Sets default values for this actor's properties
	AGameField();

	// Called when an instance of this class is placed (in editor) or spawned
	// a runtime posiziona le celle nel livello ==> IMPORTANTE
	//virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* SpawnVolume;

	// return a (x,y) position given a hit (click) on a field tile
	// questo serve a posizionare croce in TTT da player, NON dovrebbe servire 
	//
	//
	FVector2D GetPosition(const FHitResult& Hit);

	// return the array of tile pointers
	TArray<ATile*>& GetTileArray();

	// return a relative position given (x,y) position
	// ritorna la posizione all'interno del livello dato array di tot dimensioni
	// direi che dimensione corrisponde a grandezza della griglia 
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY);

	// return (x,y) position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location);


	// check if a position is a win position
	// non serve
	//
	//
	//bool IsWinPosition(const FVector2D Position) const;

	// check if is a win line
	// non serve
	//
	//
	//inline bool IsWinLine(const FVector2D Begin, const FVector2D End) const;

	// checking if is a valid field position
	//inline bool IsValidPosition(const FVector2D Position) const;

	// get a line given a begin and end positions
	// non serve
	//
	//
	//TArray<int32> GetLine(const FVector2D Begin, const FVector2D End) const;

	// check if a line contains all equal elements
	// non serve
	//
	//
	//bool AllEqual(const TArray<int32>& Array) const;



};

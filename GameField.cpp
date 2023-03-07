// Fill out your copyright notice in the Description page of Project Settings.

#include "GameField.h"
#include "Tile.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameMode.h"


// Sets default values
AGameField::AGameField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// size of the field (28x36)
	SizeX = 28;
	SizeY = 36;
	// tile dimension
	TileSize = 120;
	// tile padding dimension
	CellPadding = 20;
	//normalized tilepadding, fa in modo di avere padding proporzionale a dimensione della tile
	NormalizedCellPadding = round(((TileSize + CellPadding) / TileSize) * 100) / 100;

	// aggiunta posticcia
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SpawnVolume->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

/*
void AGameField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}
*/

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	GenerateField();
}

void AGameField::ResetField()
{

	for (ATile* Obj : TileArray)
	{
		Obj->SetTileStatus(NOT_ASSIGNED, EMPTY);
		Obj->SetAccessibility(-1);
	}

	// send broadcast event to registered objects
	//OnResetEvent.Broadcast();

	//AMyGameMode* GameMode = (AMyGameMode*)(GetWorld()->GetAuthGameMode());
	//GameMode->IsGameOver = false;
	// counter serve per verificare se la partita è patta, una volta che arriva a 9
	//GameMode->MoveCounter = 0;
	//GameMode->ChoosePlayerAndStartGame();
}


void AGameField::GenerateField()
{	
	// serve per posizionare le celle nel gamefield
	// 
	// ricordare che la dimensione deve essere essere aumentata di 1
	// poiche array sono 0-basati
	// pertanto griglia sarà 36x29
	// qui sotto non considero le righe esterne alla griglia vera e propria 
	// ergo la griglia sarà di dimensione 31x29

	const int32 WinSizeX = 31;
	const int32 WinSizeY = 29;

	// costruttore che permette di spawnare la tile
	auto PlaceBlackTile = ConstructorHelpers::FObjectFinder<UBlueprint>(TEXT("Blueprint'/Game/PacMan/Blueprints/BP_TileBlack.BP_TileBlack'"));
	auto PlaceBlueTile = ConstructorHelpers::FObjectFinder<UBlueprint>(TEXT("Blueprint'/Game/PacMan/Blueprints/BP_TileBlue.BP_TileBlue'"));

	// prendo riferimento al mondo
	UWorld* const World = GetWorld();

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// quindi creo vettore di interi che emula la struttura della griglia
	static char Grid[WinSizeX][WinSizeY] = {
		"XXXXXXXXXXXXXXXXXXXXXXXXXXXX",
		"XP           XX            X",
		"X XXXX XXXXX XX XXXXX XXXX X",
		"X XXXX XXXXX XX XXXXX XXXX X",
		"X XXXX XXXXX XX XXXXX XXXX X",
		"X                          X",
		"X XXXX XX XXXXXXXX XX XXXX X",
		"X XXXX XX XXXXXXXX XX XXXX X",
		"X      XX    XX    XX      X",
		"XXXXXX XXXXX XX XXXXX XXXXXX",
		"XXXXXX XXXXX XX XXXXX XXXXXX",
		"XXXXXX XX          XX XXXXXX",
		"XXXXXX XX XXXXXXXX XX XXXXXX",
		"XXXXXX XX X   G  X XX XXXXXX",
		"          X G    X          ",
		"XXXXXX XX X   G  X XX XXXXXX",
		"XXXXXX XX XXXXXXXX XX XXXXXX",
		"XXXXXX XX          XX XXXXXX",
		"XXXXXX XX XXXXXXXX XX XXXXXX",
		"XXXXXX XX XXXXXXXX XX XXXXXX",
		"X            XX            X",
		"X XXXX XXXXX XX XXXXX XXXX X",
		"X XXXX XXXXX XX XXXXX XXXX X",
		"X   XX       G        XX   X",
		"XXX XX XX XXXXXXXX XX XX XXX",
		"XXX XX XX XXXXXXXX XX XX XXX",
		"X      XX    XX    XX      X",
		"X XXXXXXXXXX XX XXXXXXXXXX X",
		"X XXXXXXXXXX XX XXXXXXXXXX X",
		"X                          X",
		"XXXXXXXXXXXXXXXXXXXXXXXXXXXX" };

	// ora devo fare in modo di posizionare la tile corretta 
	// mentre scorro l'array di cui sopra
	// Faccio questo seguendo le regole:
	// "X" => TileBlack dove non è lecito camminare
	// " " => TileBlue dove è lecito camminare
	// Per ora assimilo P e G a X
	// poi implementerò anche le successive funzionalità

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	GetWorld()->SpawnActor<ATile>(SpawnLocation, SpawnRotation);



	for (int32 x = 0; x < WinSizeX; x++)
	{
		for (int32 y = 0; y < WinSizeY; y++)
		{
			FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y);
			FRotator Rotation(0.0f, 0.0f, 0.0f);
			// getworld è tutto quello che vedo all'interno del viewport
			ATile* Obj = Cast<ATile>(GetWorld()->SpawnActor(TileClass, &Location));
			float TileScale = TileSize / 100;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			Obj->SetGridPosition(x, y);
			Obj->SetAccessibility(-1);	
			
			// se leggo X allora tile non è calpestabile
			if (Grid[x][y] == 'X') {
				// Obj->SetAccessibility(0);
				BlueprintVar = (UClass*)PlaceBlackTile.Object->GeneratedClass;
				auto Tile = World->SpawnActor<ATile>(BlueprintVar, Location, Rotation, SpawnInfo);
				Tile->SetAccessibility(0);
				TileArray.Add(Tile);
				TileMap.Add(FVector2D(x, y), Tile);
			}
			else
			{
				BlueprintVar = (UClass*)PlaceBlueTile.Object->GeneratedClass;
				auto Tile = World->SpawnActor<ATile>(BlueprintVar, Location, Rotation, SpawnInfo);
				Tile->SetAccessibility(1);
				TileArray.Add(Tile);
				TileMap.Add(FVector2D(x, y), Tile);
			}
			// TileArray.Add(Obj);
			// TileMap.Add(FVector2D(x, y), Obj);
		}
	}
}

FVector2D AGameField::GetPosition(const FHitResult& Hit)
{
	// utilizzo funzione cast di Unreal, evocata solo nel momento in cui
	// clicco effettivamenet all'interno del template, se clicclo fuori fallisce
	// senza dare problemi
	return Cast<ATile>(Hit.GetActor())->GetGridPosition();
}

TArray<ATile*>& AGameField::GetTileArray()
{
	return TileArray;
}

FVector AGameField::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY)
{
	// calcolo posizione relativa in base alla conoscenza della conformazione della tabella
	// quindi delle dimensioni di celle e padding che le separa, tengo conto della normalizzazione
	// tutte le distanze che calcolo di riferiscono al sistemi di riferimento assoluto
	return  TileSize * NormalizedCellPadding * FVector(InX, InY, 0);
}

FVector2D AGameField::GetXYPositionByRelativeLocation(const FVector& Location)
{
	double x = Location[0] / (TileSize * NormalizedCellPadding);
	double y = Location[1] / (TileSize * NormalizedCellPadding);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), x, y));
	return FVector2D(x, y);
}



/*
bool AGameField::IsWinPosition(const FVector2D Position) const
{
	int32 offset = WinSize - 1;
	// vertical lines
	for (int32 i = Position[0] - offset; i <= Position[0]; i++)
	{
		if (IsWinLine(FVector2D(i, Position[1]), FVector2D(i + offset, Position[1])))
		{
			return true;
		}
	}

	// horizontal lines
	for (int32 i = Position[1] - offset; i <= Position[1]; i++)
	{
		if (IsWinLine(FVector2D(Position[0], i), FVector2D(Position[0], i + offset)))
		{
			return true;
		}
	}

	// diagonal lines
	for (int32 i = -offset; i <= 0; i++)
	{
		if (IsWinLine(FVector2D(Position[0] + i, Position[1] + i), FVector2D(Position[0] + offset + i, Position[1] + offset + i)))
			return true;
		if (IsWinLine(FVector2D(Position[0] + i, Position[1] - i), FVector2D(Position[0] + offset + i, Position[1] - offset - i)))
			return true;
	}

	return false;
}


inline bool AGameField::IsWinLine(const FVector2D Begin, const FVector2D End) const
{
	return IsValidPosition(Begin) && IsValidPosition(End) && AllEqual(GetLine(Begin, End));
}

inline bool AGameField::IsValidPosition(const FVector2D Position) const
{
	return 0 <= Position[0] && Position[0] < Size && 0 <= Position[1] && Position[1] < Size;
}

TArray<int32> AGameField::GetLine(const FVector2D Begin, const FVector2D End) const
{
	int32 xSign;
	if (Begin[0] == End[0])
		xSign = 0;
	else
		xSign = Begin[0] < End[0] ? 1 : -1;

	int32 ySign;
	if (Begin[1] == End[1])
		ySign = 0;
	else
		ySign = Begin[1] < End[1] ? 1 : -1;

	TArray<int32> line;
	int32 x = Begin[0] - xSign;
	int32 y = Begin[1] - ySign;
	do {
		x += xSign;
		y += ySign;
		line.Add((TileMap[FVector2D(x, y)])->GetOwner());
	} while (x != End[0] || y != End[1]);

	return line;
}

bool AGameField::AllEqual(const TArray<int32>& Array) const
{
	if (Array.Num() == 0)
	{
		return false;
	}
	int32 Value = Array[0];

	if (Value == NOT_ASSIGNED)
	{
		return false;
	}

	for (int32 i = 1; i < Array.Num(); i++)
	{
		if (Value != Array[i])
			return false;
	}

	return true;
};

*/


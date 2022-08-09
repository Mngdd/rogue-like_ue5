// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include <stdlib.h>  // useless
// #include <time.h>  // for random, useless maybe
// #include <algorithm>  // used for searching, useless maybe

#include "DelaunayHelper.h"  // delaunay generator from world gen plugin
#include "Math/RandomStream.h"  // stream for easy level generation
#include "Containers/Array.h"  // dynamic array, python-like array
#include "Math/UnrealMathUtility.h"  // FVector for coords and RandRange
#include "Components/InstancedStaticMeshComponent.h"  // models of floor, walls and etc.
#include "DrawDebugHelpers.h"  //draw debug lines and boxes sdsj
//#include "PathFinder.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Generator.generated.h"

USTRUCT(BlueprintType)
struct FRoomConnectedARRAY  // contains data about connections with other rooms (FVector2D)
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FVector2D> connections;
	FORCEINLINE FRoomConnectedARRAY();
	explicit FORCEINLINE FRoomConnectedARRAY(TArray<FVector2D> Arr);

	bool operator==(const FRoomConnectedARRAY& Other) const
	{
		return connections == Other.connections;
	}
};

//USTRUCT(BlueprintType)
//struct FRoomData // contains data about room(center, coords of floor tiles)
//{
//	GENERATED_BODY()
//
//	UPROPERTY(BlueprintReadWrite)
//	FVector2D Center;
//	TArray<FVector2D> FloorTiles;
//
//	FORCEINLINE FRoomData();
//	explicit FORCEINLINE FRoomData(FVector2D Center_, TArray<FVector2D> FloorTiles_);
//
//	bool operator==(const FRoomData& Other) const
//	{
//		return Center == Other.Center;
//	}
//};

//FRoomConnectedARRAY struct constructors & get hash func
FORCEINLINE FRoomConnectedARRAY::FRoomConnectedARRAY()
{
}
FORCEINLINE FRoomConnectedARRAY::FRoomConnectedARRAY(TArray<FVector2D> Arr) : connections(Arr)
{
	connections = Arr;
}
FORCEINLINE uint32 GetTypeHash(const FRoomConnectedARRAY& idk)
{
	uint32 Hash = FCrc::MemCrc32(&idk, sizeof(FRoomConnectedARRAY));
	return Hash;
}

//FRoomData struct constructors & get hash func
//FORCEINLINE FRoomData::FRoomData()
//{
//}
//FORCEINLINE FRoomData::FRoomData(FVector2D Center_, TArray<FVector2D> FloorTiles_) : Center(Center_), FloorTiles(FloorTiles_)
//{
//	Center = Center_;
//	FloorTiles = FloorTiles_;
//}
//FORCEINLINE uint32 GetTypeHash(const FRoomData& idk)
//{
//	uint32 Hash = FCrc::MemCrc32(&idk, sizeof(FRoomData));
//	return Hash;
//}

UCLASS()
class SOMETHING_API ACPP_Generator : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACPP_Generator();
	//int MakeRandInt(int min, int max);  // generates int witch follows condition: min <= int <= max. 
	// пока что генерирует только пол
	UFUNCTION(BlueprintCallable, Category = "Dungeon generator")
	TArray<FVector2D> CreateRooms(int RoomCount, int RoomSizeMin, int RoomSizeMax, int FloorTileSize, int MinDistance = -1, int MaxDistance = -1);
	UFUNCTION(BlueprintCallable, Category = "Dungeon generator")
	TArray<FRoomConnectedARRAY> MST(TArray<FVector2D> RoomConnections, int BranchingPrecent, int max_loops);
	//UFUNCTION(BlueprintCallable, Category = "Dungeon generator")
	//bool FRoomTArrayCheck(FRoomConnectedARRAY One, TArray<FRoomConnectedARRAY> Two);
	//UFUNCTION(BlueprintCallable, Category = "Dungeon generator")
	//TArray<FVector> FindPath(TArray<FVector> FloorTiles);

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dungeon generator")
	//TArray<FRoomData> RoomDataArray;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dungeon generator")
	FRandomStream GeneratorStream;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dungeon generator")
	bool DEBUG_ENABLE = false;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dungeon generator")
	class UInstancedStaticMeshComponent* InstancedStaticMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dungeon generator")
	int MaxLoops = 25;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};

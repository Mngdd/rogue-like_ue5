// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Generator.h"

#include "Engine/StaticMeshSocket.h"

// Sets default values
ACPP_Generator::ACPP_Generator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>("floor");
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	
	InstancedStaticMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACPP_Generator::BeginPlay()
{
	Super::BeginPlay();
	
}

 //floor tile coords generator(for each room)
TArray<FVector2D> ACPP_Generator::CreateRooms(int RoomCount, int RoomSizeMin, int RoomSizeMax, int FloorTileSize, int MinDistance, int MaxDistance)
{

	return TArray<FVector2D>();

	UE_LOG(LogTemp, Warning, TEXT("USING %d SEED"), GeneratorStream.GetCurrentSeed());

	if (MinDistance == -1)  // if the minDistance is not set, set by default
	{
		MinDistance = RoomSizeMin;// * FloorTileSize + FloorTileSize;
	}
	if (MaxDistance == -1)  // if the maxDistance is not set, set by default
	{
		MaxDistance = RoomSizeMax;// * FloorTileSize + FloorTileSize;
	}

	bool IsIncorrectCords = true;  // bool for while-checking cycle
	bool neighbors = false;  // neighbors cells of TMP_cords
	TArray<FVector2D> vCords; // ue array which will be returned for floor tiles
	TArray<FVector2D> TMP_Cords;  // temporary ue array for room floor tiles
	FVector2D RoomStarterPoint;
	int kill_loop;
	int len_x;
	int len_y;
	int RoomSizeX = -1;
	int RoomSizeY = -1;
	//RoomDataArray.Empty();

	for (int RoomIndex = 0; RoomIndex < RoomCount; RoomIndex++)  // generate rooms coord
	{
		kill_loop = 0;
		IsIncorrectCords = true;
		len_x = 0;
		len_y = 0;
	
		while (IsIncorrectCords && kill_loop < MaxLoops)  // GENERATES CORRECT ROOM COORDS
		{
	 		IsIncorrectCords = false;  // if overlap checking fails, the value doesnt change
			// coord of the left bottom of the room in ue units
			RoomStarterPoint.X = GeneratorStream.RandRange((MinDistance), (MaxDistance + RoomSizeMax));
			RoomStarterPoint.Y = GeneratorStream.RandRange((MinDistance), (MaxDistance + RoomSizeMax));
	
			RoomSizeX = GeneratorStream.RandRange(RoomSizeMin, RoomSizeMax);// * FloorTileSize;  // create room size in ue units
			RoomSizeY = GeneratorStream.RandRange(RoomSizeMin, RoomSizeMax);// * FloorTileSize;
			while (RoomSizeY % 2 != 0)
			{
				RoomSizeY = GeneratorStream.RandRange(RoomSizeMin, RoomSizeMax);
			}
	
			for (int y = RoomStarterPoint.Y; y < RoomStarterPoint.Y + RoomSizeY ; y++, len_y++)//y += FloorTileSize)  // room size and cords are generated, making floor tiles
			{
				for (int x = RoomStarterPoint.X; x < RoomStarterPoint.X + RoomSizeX; x++, len_x++)//x += FloorTileSize)
				{
					TMP_Cords.Add(FVector2D(x, y));  // add 2 coords to the TEMPORARY vector of floor tiles
				}
			}
	
			for (int i = 0; i < TMP_Cords.Num(); i++)  // coords overlap checking
			{
				neighbors = false;
				for (int x = 0; x < 8 && neighbors == false; x++) // neighbor cells list
				{
					switch (x)
					{
					case 0:
						neighbors = vCords.Contains(FVector2D(0, 1));
						break;
					case 1:
						neighbors = vCords.Contains(FVector2D(1, 1));
						break;
					case 2:
						neighbors = vCords.Contains(FVector2D(1, 0));
						break;
					case 3:
						neighbors = vCords.Contains(FVector2D(1, -1));
						break;
					case 4:
						neighbors = vCords.Contains(FVector2D(0, -1));
						break;
					case 5:
						neighbors = vCords.Contains(FVector2D(-1, -1));
						break;
					case 6:
						neighbors = vCords.Contains(FVector2D(-1, 0));
						break;
					case 7:
						neighbors = vCords.Contains(FVector2D(-1, 1));
						break;
					}
					
				}
				if (vCords.Contains(TMP_Cords[i]) || neighbors)
				{
					IsIncorrectCords = true;  // overlap detected
					TMP_Cords.Empty();
					break;
				}
			}
			kill_loop ++;
		}
		if (kill_loop >= MaxLoops)
		{
			UE_LOG(LogTemp, Warning, TEXT("ROOM %d GENERATION FAILURE"), RoomIndex);
		}
		else
		{
			vCords.Append(TMP_Cords);
			//RoomDataArray.Add(FRoomData(FVector2D(RoomStarterPoint.X + RoomSizeX / 2, RoomStarterPoint.Y + RoomSizeY / 2), TMP_Cords));

		}
		TMP_Cords.Empty();
	}
	UE_LOG(LogTemp, Warning, TEXT("====ROOMS BUILDING IS FINISHED===="));
	return vCords;
}

//returns mst with branched lines if branching precent(of remaining rooms) 0 <= X <= 100
TArray<FRoomConnectedARRAY> ACPP_Generator::MST(TArray<FVector2D> RoomConnections, int BranchingPrecent, int max_loops)
{


	return TArray<FRoomConnectedARRAY>();



	TMap<FVector2D, FRoomConnectedARRAY> connections_map;  // contains ALL connections
	TMap < FRoomConnectedARRAY, float > distance_map;  // contains distance between every 2 points
	TArray<FRoomConnectedARRAY> final_connections;  // will contain final connections with mst and branched points
	TArray<FVector2D> LocalTriangle;
	for (int arrId = 0; arrId < RoomConnections.Num(); arrId +=3)  // creating more comfortable list of connected points
	{
		for (int tmp = 0; tmp < 3; tmp++)
		{
			LocalTriangle.Add(RoomConnections[tmp + arrId]);
		}
		for (int key = 0; key < 3;key++)
		{
			if (connections_map.Find(LocalTriangle[key]) == nullptr)
			{
				connections_map.Add(LocalTriangle[key], FRoomConnectedARRAY());
			}
			FRoomConnectedARRAY *CurrentPtr = connections_map.Find(LocalTriangle[key]);
			for (int val = 0; val < 3; val++)
			{
				if (val == key)
				{
					continue;
				}
				if (CurrentPtr != nullptr)
				{
					CurrentPtr->connections.AddUnique(LocalTriangle[val]);  
				}
			
				// doing distance stuff below
				if (distance_map.Find(FRoomConnectedARRAY ({ LocalTriangle[key], LocalTriangle[val] })) == nullptr &&
					distance_map.Find(FRoomConnectedARRAY ({ LocalTriangle[val], LocalTriangle[key] })) == nullptr)
				{
					if(DEBUG_ENABLE)
					{
						DrawDebugBox(GetWorld(), FVector(LocalTriangle[key].X, LocalTriangle[key].Y, AActor::GetActorLocation().Z),
							FVector(50), FColor(252, 198, 0), true);
					}
					distance_map.Add(FRoomConnectedARRAY({ LocalTriangle[key], LocalTriangle[val] }),
						FVector2D::Distance(LocalTriangle[key], LocalTriangle[val]));
				}
			}
		}
		LocalTriangle.Empty();
	}


	// MST searching starts here
	FRoomConnectedARRAY success_key1;
	FRoomConnectedARRAY possible1;
	TArray<int> UsedPoints = {0};  // contains index of used points
	TArray<FRoomConnectedARRAY> connected_array;
	TArray<FVector2D> AllPoints;  // list of connections map keys
	int PointsAmount = connections_map.GetKeys(AllPoints);  // connections map keys amount
	
	float min_distance = -1.0;
	float distance;
	float* result1;

	//looping until we reach loop limit or we connects everything
	for (int kill_loop = 0; kill_loop < max_loops && UsedPoints.Num() < AllPoints.Num(); kill_loop++)
	{
		//looping for each available keyID(indexes of keys from UsedPoints)
		for (int keyIndex = 0; keyIndex < UsedPoints.Num(); keyIndex++)
		{
			// key >> AllPoints[UsedPoints[curr_point]]
			// value array from key >> connections_map.Find(AllPoints[UsedPoints[curr_point]])->connections
			// point from value array >> current_val_arr[neighbor_point]

			TArray<FVector2D> val_array = connections_map.Find(AllPoints[UsedPoints[keyIndex]])->connections;

			for (int valIndex = 0; valIndex < val_array.Num(); valIndex++)
			{
				possible1.connections.Append({ AllPoints[UsedPoints[keyIndex]], val_array[valIndex] });
							
				//should find k0k1 or k1k0
				result1 = distance_map.Find(possible1);
				
				if (result1 == nullptr)
				{
					possible1.connections.Empty();
					possible1.connections.Append({ val_array[valIndex], AllPoints[UsedPoints[keyIndex]] });
					result1 = distance_map.Find(possible1);
				}
				
				if (result1 != nullptr)
				{
					distance = *result1;
					connected_array.AddUnique(possible1);
				}
				else
				{
					distance = -1.0;
				}
				if ((distance >= 0.0 && (min_distance > distance || min_distance == -1)) 
					&& (UsedPoints.Find(AllPoints.Find(possible1.connections[0])) == -1 
						|| UsedPoints.Find(AllPoints.Find(possible1.connections[1])) == -1))
				{
					min_distance = distance;
					success_key1 = possible1;
				}
				possible1.connections.Empty();
			}
		}

		if (min_distance == -1.0)
		{
			UE_LOG(LogTemp, Warning, TEXT("MST WARNING! MIN_DISTANCE == -1!"));
			continue;
		}

		if (DEBUG_ENABLE)
		{
			DrawDebugLine(GetWorld(), FVector(success_key1.connections[0].X, success_key1.connections[0].Y, AActor::GetActorLocation().Z),
				FVector(success_key1.connections[1].X, success_key1.connections[1].Y, AActor::GetActorLocation().Z),
				FColor(252, 93, 0), true, -1, 0, 10);
		}

		min_distance = -1.0;
		final_connections.Add(success_key1);
		UsedPoints.AddUnique(AllPoints.Find(success_key1.connections[0]));
		UsedPoints.AddUnique(AllPoints.Find(success_key1.connections[1]));
	}
	UE_LOG(LogTemp, Warning, TEXT("===MST FINISHED WITH %d LINES, FROM %d POINTS==="), final_connections.Num(), UsedPoints.Num());
	

	//making random branches here
	//FRoomConnectedARRAY add_me;
	////creating an array of non-used lines
	//int arr_len = connected_array.Num(); //cuz using connected_array.Num() directly in for loop is useless(.Num() updates every loop lololol)
	//
	//for (int i = arr_len - 1; i >= 0; i--)
	//{
	//	if (FRoomTArrayCheck(connected_array[i], final_connections))
	//	{
	//		connected_array.Remove(connected_array[i]);
	//	}
	//}
	////branching
	//for (int i = 0; i < connected_array.Num() / 100 * BranchingPrecent; i++)
	//{
	//	add_me = connected_array[GeneratorStream.RandRange(0, connected_array.Num() - 1)];
	//
	//	for (int tmp = 0; tmp < 100 && FRoomTArrayCheck(add_me, final_connections); tmp++)
	//	{
	//		add_me = connected_array[GeneratorStream.RandRange(0, connected_array.Num() - 1)];
	//	}
	//	final_connections.Add(add_me);
	//}
	
	return final_connections;
}


/*.find analog for nested conatiners.
too lazy to get round of impossibility of .find in nested containers lol*/
//bool ACPP_Generator::FRoomTArrayCheck(FRoomConnectedARRAY One, TArray<FRoomConnectedARRAY> Two)
//{
//	return false;
//
//
//	//bool item_found = false;  
//	//for (int a = 0; a < Two.Num(); a++)
//	//{
//	//	if (Two[a].connections[0].XY == One.connections[0].XY &&
//	//		Two[a].connections[1].XY == One.connections[1].XY)
//	//	{
//	//		item_found = true;
//	//	}
//	//}
//	//return item_found;
//}

//TArray<FVector> ACPP_Generator::FindPath(TArray<FVector> FloorTiles)
//{
	//
	//
	//return TArray<FVector>();
	//
	//
	//if (FloorTiles.Num() < 2)
	//{
	//	return TArray<FVector>();
	//}
	//
	//TArray<UPathingPoint*> CachedPathingPoints = APathFinder::VectorToPathingPoint(FloorTiles);
	////Generate your pathing points
	//
	//UPathingPoint* StartPathingPoint = CachedPathingPoints[0];
	//UPathingPoint* EndPathingPoint = CachedPathingPoints[FloorTiles.Num() - 1];
	//
	//TArray<UPathingPoint*> OutPathingPoints;
	//
	//if (APathFinder::Solve_AStar(CachedPathingPoints, StartPathingPoint, EndPathingPoint, OutPathingPoints))
	//{
	//	TArray<FVector> RoutePathingVectors = APathFinder::PathingPointsToVector(OutPathingPoints);
	//	//return RoutePathingVectors;
	//}
	//return TArray<FVector>();
//}

// Called every frame
void ACPP_Generator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


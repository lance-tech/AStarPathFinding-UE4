// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoardIterTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Board Iteration Trigger"));
	//BoardIterTrigger->SetSimulatePhysics(true);
	BoardIterTrigger->SetNotifyRigidBodyCollision(true);
	BoardIterTrigger->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	BoardIterTrigger->OnComponentHit.AddDynamic(this, &ABoard::OnCompHit);
}

ABoard::~ABoard()
{
	for (int x = 0; x < XNodeCount; x++)
	{
		for (int y = 0; y < YNodeCount; y++)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Delete Node Data] : %d, %d"), x, y);
			delete MyGrid[x][y];
		}
		delete[] MyGrid[x];
	}
	delete[] MyGrid;
	UE_LOG(LogTemp, Warning, TEXT("[Delete Node Data Success]"));
}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();

	if (BottomLeft && TopRight)
	{
		CreateGrid();
	}
}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoard::CreateGrid()
{
	NodeWidth = 100;
	NodeHeight = 100;

	StartX = BottomLeft->GetActorLocation().X;
	StartY = BottomLeft->GetActorLocation().Y;
	EndX = TopRight->GetActorLocation().X;
	EndY = TopRight->GetActorLocation().Y;

	UE_LOG(LogTemp, Warning, TEXT("[Start Position] : %f, %f"), StartX, StartY);
	UE_LOG(LogTemp, Warning, TEXT("[End Position] : %f, %f"), EndX, EndY);

	XNodeCount = FMath::RoundToInt( ( (EndX - StartX) / NodeHeight) ) + 1;
	YNodeCount = FMath::RoundToInt( ( (EndY - StartY) / NodeWidth) ) + 1;

	UE_LOG(LogTemp, Warning, TEXT("[XNodeCount, YNodeCount] : %d, %d"), XNodeCount, YNodeCount);

	MyGrid = new Node**[XNodeCount];
	for (int i = 0; i < XNodeCount; i++)
	{
		MyGrid[i] = new Node*[YNodeCount];
	}

	SetGrid();
}

void ABoard::SetGrid()
{
	for (int x = 0; x < XNodeCount; x++)
	{
		for (int y = 0; y < YNodeCount; y++)
		{
			float worldY = StartY + (NodeWidth * y);
			float worldX = StartX + (NodeHeight * x);
			float worldZ = GetActorLocation().Z;

			FVector worldPosition = FVector(worldX, worldY, worldZ);
			// Check Collision for Obstacle.
			
			CheckObstacle(worldPosition);

			UE_LOG(LogTemp, Warning, TEXT("Checker[%d : %d] [%f, %f], [Is Collision ? %d]"), x, y, worldX, worldY, checkWall);

			// Create Node
			MyGrid[x][y] = new Node(x, y, worldPosition, checkWall);
		}
	}
}


Node* ABoard::WorldToNode(FVector Position)
{
	int nodeY = FMath::RoundToInt((Position.Y - StartY) / NodeWidth);
	int nodeX = FMath::RoundToInt((Position.X - StartX) / NodeHeight);

	return MyGrid[nodeX][nodeY];
}

void ABoard::CheckObstacle(FVector worldPosition)
{
	checkWall = false;
	BoardIterTrigger->SetRelativeLocation(worldPosition);
	BoardIterTrigger->SetRelativeLocation(FVector(StartX, StartY, 0.f));
}

TArray<Node*> ABoard::GetNeighbors(Node* current)
{
	TArray<Node*> neighborNodes;

	if (current->ArrayY > 0)
	{
		// Left position node
		neighborNodes.Add(MyGrid[current->ArrayX][current->ArrayY - 1]);
	}

	if (current->ArrayY < YNodeCount - 1)
	{
		// Right position node
		neighborNodes.Add(MyGrid[current->ArrayX][current->ArrayY + 1]);
	}

	if (current->ArrayX > 0)
	{
		// Bottom position node
		neighborNodes.Add(MyGrid[current->ArrayX - 1][current->ArrayY]);
	}

	if (current->ArrayX < XNodeCount - 1)
	{
		// Top position node
		neighborNodes.Add(MyGrid[current->ArrayX + 1][current->ArrayY]);
	}

	return neighborNodes;
}

void ABoard::OnCompHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
		}
	}
}


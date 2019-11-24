// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinding.h"

// Sets default values
APathFinding::APathFinding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Enemy");
}

// Called when the game starts or when spawned
void APathFinding::BeginPlay()
{
	Super::BeginPlay();

	if (Target != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Start Path Finding : %s"), *Target->GetName());
	}
}

// Called every frame
void APathFinding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target != nullptr)
	{
		AStarPathFinding();

		// If final path list is not empty
		// Move to follow to path
		if (FinalNodeList.Num() > 0)
		{
			// 1. Set to height target node Z maintain same position
			FVector destPos = FinalNodeList[0]->Position;
			FVector currentPos = GetActorLocation();
			destPos.Z = currentPos.Z;

			// 2. Turn to rotate to target smoothly
			FVector dir = destPos - currentPos;

			FQuat dirQuat(dir.Rotation());
			FQuat rot = FQuat::Slerp(GetActorQuat(), dirQuat, 5 * DeltaTime);

			SetActorRotation(rot);

			// 3. Move to target node
			SetActorLocation(GetActorLocation() + (dir.GetSafeNormal() * 3.f));

			// 4. The distance had lower than 0.001 with target node,
			//	  start again path finding.
			if (FVector::Distance(GetActorLocation(), destPos) < 0.001f)
			{
				//Target
				AStarPathFinding();
			}
		}
		else
		{
			// no has found out nodes, run path finding
		}
	}
}

void APathFinding::FinishFinding(Node* startNode, Node* goalNode)
{
	// Clear final path list / Temp
	TArray<Node*> tempPath;

	// 1. Set to final target node at pointing now.
	Node *indicator = goalNode;

	// 2. if current pointing node is not start node...
	while (indicator != startNode)
	{
		// 3. add node of pointing to fianl path list
		tempPath.Add(indicator);

		// 4. change to previous node to current pointing node
		indicator = indicator->ParentNode;
	}

	// 5. reverse to entire list( start node -> end node )
	Algo::Reverse(tempPath);

	// 6. set final path list
	FinalNodeList = tempPath;
}

int APathFinding::ManhattanDistance(const Node& current, const Node& neighbor)
{
	int distance = FMath::Abs((current.ArrayX - neighbor.ArrayX) * MyBoard->NodeHeight) +
					FMath::Abs((current.ArrayY - neighbor.ArrayY) * MyBoard->NodeWidth);
	return distance;
}

void APathFinding::AStarPathFinding()
{
	//UE_LOG(LogTemp, Log, TEXT("Start Path Finding : %s"), *Target->GetName());

		// Get start node with target node
	Node* startNode = MyBoard->WorldToNode(GetActorLocation());
	Node* goalNode = MyBoard->WorldToNode(Target->GetActorLocation());

	UE_LOG(LogTemp, Warning, TEXT("Start Node [%d, %d]"), startNode->ArrayX, startNode->ArrayY);
	UE_LOG(LogTemp, Warning, TEXT("Goal Node [%d, %d]"), goalNode->ArrayX, goalNode->ArrayY);

	// Clear list of open and close list
	// Lance : ( Why clear list every time, consider more good way)
	OpenList.Empty();
	CloseList.Empty();

	// Add start node in open list(first node)
	OpenList.Add(startNode);

	// Is not empty to open list, start to find
	while (OpenList.Num() > 0)
	{
		// Set current node at the first of open list
		Node* currentNode = OpenList[0];

		// Compare with fcost current node with open list elements
		for (int i = 1; i < OpenList.Num(); i++)
		{
			// 1. The neighbor node fcost lower than current node fcost
			// 2. Same fcost neighbor node with current node, 
			//	  and neighbor node hcost lower than current node hcost
			if (OpenList[i]->GetFCost() < currentNode->GetFCost() ||
				OpenList[i]->GetGCost() == currentNode->GetFCost() &&
				OpenList[i]->GetHCost() < currentNode->GetHCost())
			{
				// 3. Set change the neighbor node to current node.
				currentNode = OpenList[i];
			}
		}

		// Remove current node in open list, and add closed list
		OpenList.Remove(currentNode);
		CloseList.Add(currentNode);

		// Finish loop if new current node same with tartget (Final path finding)
		if (currentNode == goalNode)
		{
			// Finish the finding.
			UE_LOG(LogTemp, Warning, TEXT("Found out Node [%d, %d]"), currentNode->ArrayX, currentNode->ArrayY);
			FinishFinding(startNode, goalNode);

			for (auto& node : FinalNodeList)
			{
				UE_LOG(LogTemp, Warning, TEXT("The Path is [%d, %d]"), node->ArrayX, node->ArrayY);
			}

			return;
		}

		// If not goal node
		// and all neighbor node add in open list,
		// and re calcutate to gcost with hcost
		for (auto& neighborNode : MyBoard->GetNeighbors(currentNode))
		{
			// neighbor node is not obstacle and not contains close list
			// set to g/h cost value
			if (!neighborNode->IsObstacle && !CloseList.Contains(neighborNode))
			{
				// Set gcost
				int gCost = currentNode->GetGCost() + ManhattanDistance(*currentNode, *neighborNode);

				// 1. not contains neighbor in open list
				if (!OpenList.Contains(neighborNode))
				{
					// 2. overwrite current node gcost
					neighborNode->SetGCost(gCost);

					// 3. hcost
					neighborNode->SetHCost(ManhattanDistance(*neighborNode, *goalNode));

					// 4. Set parent ( set previous node)
					neighborNode->ParentNode = currentNode;

					// 5. this neighbor node not contains in open list,
					//    then this neighbor node add to open list
					if (!OpenList.Contains(neighborNode))
					{
						OpenList.Add(neighborNode);
					}
				}

			}
		}
	}
}
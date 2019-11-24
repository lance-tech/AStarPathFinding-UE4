// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Board.h"
#include "Node.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathFinding.generated.h"

UCLASS()
class ASTARPATHFINDING_API APathFinding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathFinding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere) UStaticMeshComponent* EnemyMesh;
	UPROPERTY(EditAnywhere) ABoard* MyBoard = nullptr;
	UPROPERTY(EditAnywhere) AActor* Target = nullptr;

private:
	TArray<Node*> OpenList;
	TArray<Node*> CloseList;
	TArray<Node*> FinalNodeList;

	void AStarPathFinding();
	void FinishFinding(Node* startNode, Node* goalNode);

	int ManhattanDistance(const Node& current, const Node& neighbor);
};
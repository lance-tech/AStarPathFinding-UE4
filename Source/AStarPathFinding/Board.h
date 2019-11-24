// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Node.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Board.generated.h"

UCLASS()
class ASTARPATHFINDING_API ABoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoard();
	~ABoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere) AActor* BottomLeft = nullptr;
	UPROPERTY(EditAnywhere) AActor* TopRight = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Box") class UBoxComponent* BoardIterTrigger;

	Node*** MyGrid;

	int NodeWidth;
	int NodeHeight;

	bool checkWall;
	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	Node* WorldToNode(FVector Position);
	TArray<Node*> GetNeighbors(Node* current);

private:
	// Bottom Left Gizmo World Position
	float StartY, StartX;

	// Top Right Gizmo World Position
	float EndY, EndX;

	int YNodeCount;
	int XNodeCount;

	void CreateGrid();
	void SetGrid();

	void CheckObstacle(FVector worldPosition);
	


};

// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleMesh.h"
#include "Components/BoxComponent.h"

//ConstructorHelpers::FObjectFinder<UStaticMesh> AObstacleMesh::MyAsset;

// Sets default values
AObstacleMesh::AObstacleMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>("Obstacle Mesh");
}

// Called when the game starts or when spawned
void AObstacleMesh::BeginPlay()
{
	Super::BeginPlay();
	
	UBoxComponent* Component = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	if (Component)
	{
		UE_LOG(LogTemp, Warning, TEXT("Component Name : %s"), *Component->GetName());

		Component->OnComponentBeginOverlap.AddDynamic(this, &AObstacleMesh::OnOverlapBegin);
		Component->OnComponentEndOverlap.AddDynamic(this, &AObstacleMesh::OnOverlapEnd);
	}	
}

// Called every frame
void AObstacleMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObstacleMesh::OnOverlapBegin(
	class UPrimitiveComponent* OverlappedComp, 
	class AActor* OtherActor, 
	class UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	// check if Actors do not equal nullptr
	if (OtherActor && (OtherActor != this))
	{
		FVector hitPosition = OtherActor->GetActorLocation();

		ABoard* _board = dynamic_cast<ABoard*>(OtherActor);
		_board->checkWall = true;

		UE_LOG(LogTemp, Warning, TEXT("Obstacle Besin Collision : %d"), _board->checkWall);
		UE_LOG(LogTemp, Warning, TEXT("%s : (%f, %f)"), *OverlappedComp->GetName(), hitPosition.X, hitPosition.Y);
	}
}

void AObstacleMesh::OnOverlapEnd(
	class UPrimitiveComponent* OverlappedComp, 
	class AActor* OtherActor, 
	class UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		//UE_LOG(LogTemp, Warning, TEXT("We Ended : %s"), *GetName());
	}
}
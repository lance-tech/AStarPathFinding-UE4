// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ASTARPATHFINDING_API Node
{
public:
	Node();
	Node(int arrayX, int arrayY, FVector worldPosition, bool isWall);
	~Node();

	// Array index position
	int ArrayX;		// Vertical Axis
	int ArrayY;		// Horizontal Axis

	// Flag to judge for whether wall or not.
	bool IsObstacle;
	Node* ParentNode;
	FVector Position;
	
	int GetGCost();
	void SetGCost(int value);
	int GetHCost();
	void SetHCost(int value);
	int GetFCost();

private:
	int gCost;
	int hCost;
};

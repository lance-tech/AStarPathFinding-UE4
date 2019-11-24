// Fill out your copyright notice in the Description page of Project Settings.


#include "Node.h"

Node::Node() :
	ArrayX(0), ArrayY(0),
	IsObstacle(false),
	ParentNode(nullptr),
	gCost(0), hCost(0)
{
}

Node::Node(int arrayX, int arrayY, FVector worldPosition, bool isWall) : Node()
{
	ArrayX = arrayX;
	ArrayY = arrayY;
	Position = worldPosition;
	IsObstacle = isWall;
}

Node::~Node()
{
}

int Node::GetFCost()
{
	return gCost + hCost;
}

int Node::GetGCost()
{
	return gCost;
}

void Node::SetGCost(int value)
{
	gCost = value;
}

int Node::GetHCost()
{
	return hCost;
}

void Node::SetHCost(int value)
{
	hCost = value;
}
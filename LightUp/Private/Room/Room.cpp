// Fill out your copyright notice in the Description page of Project Settings.

#include "LightUp.h"
#include "Room.h"

ARoom::ARoom()
{
	// Disable ticking.
	PrimaryActorTick.bCanEverTick = false;
	
	RoomType = ERoomType::RTE_Room1X1;
	RoomCenterLocation = FVector(0, 0, 0);
	bRotateRoom = false;
	bIsHidden = false;
}

ARoom::ARoom(const ERoomType & InRoomType)
{
	// Disable ticking.
	PrimaryActorTick.bCanEverTick = false;
	
	RoomType = InRoomType;
	RoomCenterLocation = FVector(0, 0, 0);
	bRotateRoom = false;
	bIsHidden = false;
}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
	
}



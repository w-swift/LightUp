// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Room.generated.h"

/**
 * Room type divide by the size of room.
 * Corridor1X1 Corridor1X2 are pontes between two rooms.
 * Room1X1 and Room1X2 contains most of the rooms in game. Boss room always appear in Room2X2.
 * SpecialRoom1X1 always appear in hidden path with special events and rewards.
 */
UENUM(BlueprintType)
enum class ERoomType : uint8
{
	RTE_Corridor1X1 = 0		UMETA(DisplayName = "Corridor1X1"),
	RTE_Corridor1X2			UMETA(DisplayName = "Corridor1X2"),
	RTE_Room1X1				UMETA(DisplayName = "Room1X1"),
	RTE_Room2X2				UMETA(DisplayName = "Room2X2"),
	RTE_SpecialRoom1X1		UMETA(DisplayName = "SpecialRoom1X1")
};

//////////////////////////////////////////Use Room buff???
/**
 * Room buff will update to character when entered a room.
 */
// UENUM(BlueprintType)
// enum class ERoomBuff : uint8
// {
// 	RBE_Accelerate = 0		UMETA(DisplayName = "Accelerate"),
// 	RBE_Decelerate			UMETA(DisplayName = "Decelerate"),
// 	RBE_Regeneration		UMETA(DisplayName = "Regeneration"),
// 	RBE_Poison				UMETA(DisplayName = "Poison"),
// 	RBE_PhyAttackEnhance
// }

/**
 * The basic class of room. Declare all the properties a room should have.
 */
UCLASS()
class LIGHTUP_API ARoom : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoom();

	/** Constructor with given room type. */
	ARoom(const ERoomType & InRoomType);

	/** Change light state when character is entering or leaving the room. */
	void SwitchLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	/** Every room has a room type to tell the size and category. */
	ERoomType RoomType;

	/** The center location of the room. */
	FVector RoomCenterLocation;

	/** True if the room should be rotated 90 degree cw. Useful when the room type is Corridor1X2. */
	uint8 bRotateRoom : 1;

	/** Whether the room is in hidden path. */
	uint8 bIsHidden : 1;
};

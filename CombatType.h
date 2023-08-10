#pragma once

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Idle UMETA(DisplayName="Idle"),
	ECS_Attacking UMETA(DisplayName="Attacking"),
	ECS_Moving UMETA(DisplayName="Moving"),
	ECS_Running UMETA(DisplayName="Running"),
	ECS_Dead UMETA(DisplayName="Dead"),

	ECS_MAX UMETA(DisplayName="DefaultMAX")
};
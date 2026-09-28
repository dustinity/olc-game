#include "AI/OLCFormationManager.h"

#include "World/OLCGroundUnit.h"
#include "World/OLCUnitBase.h"

TArray<FVector> UOLCFormationManager::CalculateFormationPositions(const TArray<AOLCUnitBase*>& Units, const FVector& AnchorLocation, EOLCFormationType FormationType, float Spacing, bool bSnapToGrid) const
{
	TArray<FVector> Positions;
	for (int32 Index = 0; Index < Units.Num(); ++Index)
	{
		FVector Position = AnchorLocation + GetFormationOffset(Index, Units.Num(), FormationType, Spacing);
		Positions.Add(bSnapToGrid ? Snap(Position, 180.0f) : Position);
	}
	return Positions;
}

FVector UOLCFormationManager::GetFormationOffset(int32 Index, int32 UnitCount, EOLCFormationType FormationType, float Spacing) const
{
	switch (FormationType)
	{
		case EOLCFormationType::Line:
			return FVector(0.0f, (Index - (UnitCount - 1) * 0.5f) * Spacing, 0.0f);
		case EOLCFormationType::Column:
			return FVector(-Index * Spacing, 0.0f, 0.0f);
		case EOLCFormationType::Diamond:
		{
			if (Index == 0) return FVector::ZeroVector;
			const int32 Ring = (Index + 3) / 4;
			const int32 Side = (Index - 1) % 4;
			const float Radius = Ring * Spacing;
			const FVector Offsets[4] = { FVector(Radius, 0, 0), FVector(0, Radius, 0), FVector(-Radius, 0, 0), FVector(0, -Radius, 0) };
			return Offsets[Side];
		}
		case EOLCFormationType::VShape:
		{
			if (Index == 0) return FVector::ZeroVector;
			const int32 Row = (Index + 1) / 2;
			const float Side = (Index % 2 == 0) ? 1.0f : -1.0f;
			return FVector(-Row * Spacing, Side * Row * Spacing, 0.0f);
		}
		default:
			return FVector::ZeroVector;
	}
}

void UOLCFormationManager::ApplyFormation(const TArray<AOLCUnitBase*>& Units, const FVector& AnchorLocation, EOLCFormationType FormationType)
{
	const TArray<FVector> Positions = CalculateFormationPositions(Units, AnchorLocation, FormationType);
	for (int32 Index = 0; Index < Units.Num(); ++Index)
	{
		if (AOLCGroundUnit* Ground = Cast<AOLCGroundUnit>(Units[Index]))
		{
			Ground->MoveTo(Positions[Index]);
		}
		else if (Units[Index])
		{
			Units[Index]->SetActorLocation(Positions[Index]);
		}
	}
}

void UOLCFormationManager::RegisterFormation(const FOLCFormationConfig& Config)
{
	RegisteredFormations.Add(Config);
}

FVector UOLCFormationManager::Snap(const FVector& Location, float GridSize) const
{
	return FVector(FMath::GridSnap(Location.X, GridSize), FMath::GridSnap(Location.Y, GridSize), Location.Z);
}

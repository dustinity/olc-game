#include "OLCPlanetTerrainGenerator.h"

#include "Core/OLCPlanetTerrainProfile.h"

void UOLCPlanetTerrainGenerator::GenerateTerrain(const FOLCTerrainGenerationSettings& Settings, const UOLCPlanetTerrainProfile* Profile, TArray<FOLCTerrainTile>& OutTiles)
{
	OutTiles.Reset(Settings.MapWidth * Settings.MapHeight);

	const float WaterLevel = Profile ? Profile->WaterLevel : 0.24f;
	const float BlockerDensity = Profile ? Profile->BlockerDensity : 0.12f;
	const float ResourceDensity = Profile ? Profile->ResourceDensity : 0.018f;
	const float DecorationDensity = Profile ? Profile->DecorationDensity : 0.25f;

	for (int32 Y = 0; Y < Settings.MapHeight; ++Y)
	{
		for (int32 X = 0; X < Settings.MapWidth; ++X)
		{
			FOLCTerrainTile Tile;
			Tile.Coord = FIntPoint(X, Y);
			Tile.Biome = Settings.Biome;

			const float BaseNoise = SmoothNoise(Settings, X, Y, 11, 3);
			const float DetailNoise = SmoothNoise(Settings, X, Y, 23, 1);
			Tile.Height = FMath::Clamp(BaseNoise * 0.72f + DetailNoise * 0.28f, 0.0f, 1.0f);
			Tile.DecorationDensity = TileNoise(Settings, X, Y, 71) * DecorationDensity;

			if (!IsInsidePlayableArea(Settings, X, Y))
			{
				Tile.Role = EOLCTerrainTileRole::Outside;
			}
			else if (IsInsideLandingArea(Settings, X, Y))
			{
				Tile.Role = EOLCTerrainTileRole::Landing;
				Tile.bDiscovered = true;
			}
			else if ((Settings.Biome == EOLCBiomeType::Water || Settings.Biome == EOLCBiomeType::Swamp) && Tile.Height < WaterLevel)
			{
				Tile.Role = EOLCTerrainTileRole::Water;
			}
			else if (Tile.Height > 0.78f || SmoothNoise(Settings, X, Y, 31, 2) < (0.34f + BlockerDensity * 0.5f))
			{
				Tile.Role = EOLCTerrainTileRole::Blocked;
			}
			else if (TileNoise(Settings, X, Y, 43) < ResourceDensity)
			{
				Tile.Role = EOLCTerrainTileRole::Resource;
				Tile.ResourceRichness = FMath::Lerp(0.35f, 1.0f, TileNoise(Settings, X, Y, 47));
				Tile.ResourceType = TileNoise(Settings, X, Y, 53) > 0.72f ? EOLCResourceType::Fuel : EOLCResourceType::Minerals;
			}
			else
			{
				const int32 CenterX = Settings.MapWidth / 2;
				const int32 CenterY = Settings.MapHeight / 2;
				const int32 DistanceSquared = FMath::Square(X - CenterX) + FMath::Square(Y - CenterY);
				const bool bFarFromLanding = DistanceSquared >= FMath::Square(15);
				if (bFarFromLanding && TileNoise(Settings, X, Y, 59) > 0.985f)
				{
					Tile.Role = EOLCTerrainTileRole::Dungeon;
					}
					else if (TileNoise(Settings, X, Y, 67) > 0.965f)
					{
						Tile.Role = EOLCTerrainTileRole::Restricted;
					}
					else
					{
						Tile.Role = EOLCTerrainTileRole::Buildable;
					}
			}

			OutTiles.Add(Tile);
		}
	}
}

float UOLCPlanetTerrainGenerator::TileNoise(const FOLCTerrainGenerationSettings& Settings, int32 X, int32 Y, int32 Salt)
{
	FRandomStream Stream(Settings.Seed + X * 928371 + Y * 689287 + Salt * 31337);
	return Stream.GetFraction();
}

float UOLCPlanetTerrainGenerator::SmoothNoise(const FOLCTerrainGenerationSettings& Settings, int32 X, int32 Y, int32 Salt, int32 Radius)
{
	float Sum = 0.0f;
	int32 Count = 0;

	for (int32 OffsetY = -Radius; OffsetY <= Radius; ++OffsetY)
	{
		for (int32 OffsetX = -Radius; OffsetX <= Radius; ++OffsetX)
		{
			Sum += TileNoise(Settings, X + OffsetX, Y + OffsetY, Salt);
			++Count;
		}
	}

	return Count > 0 ? Sum / static_cast<float>(Count) : 0.0f;
}

bool UOLCPlanetTerrainGenerator::IsInsidePlayableArea(const FOLCTerrainGenerationSettings& Settings, int32 X, int32 Y)
{
	const float CenterX = (Settings.MapWidth - 1) * 0.5f;
	const float CenterY = (Settings.MapHeight - 1) * 0.5f;
	const float NormalizedX = (X - CenterX) / (Settings.MapWidth * 0.48f);
	const float NormalizedY = (Y - CenterY) / (Settings.MapHeight * 0.42f);
	const float Distortion = (TileNoise(Settings, X, Y, 101) - 0.5f) * 0.10f;
	return (NormalizedX * NormalizedX + NormalizedY * NormalizedY + Distortion) < 1.0f;
}

bool UOLCPlanetTerrainGenerator::IsInsideLandingArea(const FOLCTerrainGenerationSettings& Settings, int32 X, int32 Y)
{
	const int32 CenterX = Settings.MapWidth / 2;
	const int32 CenterY = Settings.MapHeight / 2;
	return FMath::Abs(X - CenterX) <= 4 && FMath::Abs(Y - CenterY) <= 4;
}

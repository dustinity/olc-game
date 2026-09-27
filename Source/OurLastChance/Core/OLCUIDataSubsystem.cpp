#include "OLCUIDataSubsystem.h"
#include "Logging/LogMacros.h"

#define LOCTEXT_NAMESPACE "OLCUIDataSubsystem"

void UOLCUIDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Log, TEXT("[OLC] UI Data Subsystem initializing with fake data..."));

	PopulateFakeResources();
	PopulateFakeMissionObjectives();
	PopulateFakeBuildCards();
	PopulateFakeMinimapMarkers();
	PopulateShipModules();

	UE_LOG(LogTemp, Log, TEXT("[OLC] Fake data populated."));
}

// ---------------------------------------------------------------------------
// Resources
// ---------------------------------------------------------------------------
void UOLCUIDataSubsystem::PopulateFakeResources()
{
	ResourceCounters.Reset();
	ResourceCounters.Emplace(EOLCResourceType::Energy, 742.0f, 1000.0f, 15.3f);
	ResourceCounters.Emplace(EOLCResourceType::Fuel, 380.0f, 500.0f, -5.1f);
	ResourceCounters.Emplace(EOLCResourceType::ConstructionMaterial, 210.0f, 800.0f, 8.7f);
	ResourceCounters.Emplace(EOLCResourceType::Minerals, 450.0f, 600.0f, 3.2f);
	ResourceCounters.Emplace(EOLCResourceType::HullParts, 95.0f, 300.0f, 1.5f);
	ResourceCounters.Emplace(EOLCResourceType::Survival, 87.0f, 100.0f, -2.0f);
	ResourceCounters.Emplace(EOLCResourceType::DarkMatterCrystals, 0.0f, 50.0f, 0.0f);
}

TArray<FOLCResourceAmount> UOLCUIDataSubsystem::GetRawResources() const
{
	TArray<FOLCResourceAmount> Raw;
	for (const auto& VD : ResourceCounters)
	{
		Raw.Emplace(VD.ResourceType, VD.Value, VD.Capacity, VD.Delta);
	}
	return Raw;
}

// ---------------------------------------------------------------------------
// Mission objectives
// ---------------------------------------------------------------------------
void UOLCUIDataSubsystem::PopulateFakeMissionObjectives()
{
	MissionObjectives.Reset();

	MissionObjectives.Emplace();
	MissionObjectives.Last().ObjectiveName = LOCTEXT("Obj1", "ESTABLISH POWER GRID");
	MissionObjectives.Last().Description = LOCTEXT("Obj1Desc", "Construct power infrastructure to support base operations.");
	MissionObjectives.Last().Progress = 3.0f;
	MissionObjectives.Last().TargetProgress = 5.0f;
	MissionObjectives.Last().State = EOLCProgressState::Active;

	MissionObjectives.Emplace();
	MissionObjectives.Last().ObjectiveName = LOCTEXT("Obj2", "RESOURCE EXTRACTION");
	MissionObjectives.Last().Description = LOCTEXT("Obj2Desc", "Extract minerals and fuel from the planet surface.");
	MissionObjectives.Last().Progress = 120.0f;
	MissionObjectives.Last().TargetProgress = 500.0f;
	MissionObjectives.Last().State = EOLCProgressState::Active;

	MissionObjectives.Emplace();
	MissionObjectives.Last().ObjectiveName = LOCTEXT("Obj3", "SCAN SECTOR");
	MissionObjectives.Last().Description = LOCTEXT("Obj3Desc", "Complete planetary surface scan.");
	MissionObjectives.Last().Progress = 67.0f;
	MissionObjectives.Last().TargetProgress = 100.0f;
	MissionObjectives.Last().State = EOLCProgressState::Active;

	MissionObjectives.Emplace();
	MissionObjectives.Last().ObjectiveName = LOCTEXT("Obj4", "DEFEND PERIMETER");
	MissionObjectives.Last().Description = LOCTEXT("Obj4Desc", "No hostile breaches for 24 hours.");
	MissionObjectives.Last().Progress = 1.0f;
	MissionObjectives.Last().TargetProgress = 1.0f;
	MissionObjectives.Last().State = EOLCProgressState::Complete;

	// WP-105: Mine discovery objective
	MissionObjectives.Emplace();
	MissionObjectives.Last().ObjectiveName = LOCTEXT("Obj5", "DISCOVER MINERAL DEPOSIT");
	MissionObjectives.Last().Description = LOCTEXT("Obj5Desc", "Place a Mine building near a resource tile to begin extraction.");
	MissionObjectives.Last().Progress = 0.0f;
	MissionObjectives.Last().TargetProgress = 1.0f;
	MissionObjectives.Last().State = EOLCProgressState::Idle;
}

// ---------------------------------------------------------------------------
// Build cards
// ---------------------------------------------------------------------------
void UOLCUIDataSubsystem::PopulateFakeBuildCards()
{
	ConstructionCategories.Reset();
	ConstructionCategories.Add(EOLCConstructionCategory::Power);
	ConstructionCategories.Add(EOLCConstructionCategory::Extraction);
	ConstructionCategories.Add(EOLCConstructionCategory::Infrastructure);
	ConstructionCategories.Add(EOLCConstructionCategory::Storage);
	ConstructionCategories.Add(EOLCConstructionCategory::Production);
	ConstructionCategories.Add(EOLCConstructionCategory::Defense);
	ConstructionCategories.Add(EOLCConstructionCategory::Support);
	ConstructionCategories.Add(EOLCConstructionCategory::HighTier);
	ConstructionCategories.Add(EOLCConstructionCategory::Special);

	BuildCards.Reset();

	// --- Power ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("SolarPanel", "SOLAR PANEL ARRAY");
		card.Category = EOLCConstructionCategory::Power;
		card.TIRRequirement = 1;
		card.GridSize = FVector2D(2.0f, 2.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 50.0f, 50.0f);
		card.Description = LOCTEXT("SolarPanelDesc", "Generates energy from sunlight. Basic power source.");
		card.PowerConsumption = -3.0f; // produces 3 energy/tick
		card.ExpectedOutputPerTick.Emplace(EOLCResourceType::Energy, 3.0f, 1000.0f);
		card.bAvailable = true;
	}
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("FusionReactor", "FUSION REACTOR");
		card.Category = EOLCConstructionCategory::Power;
		card.TIRRequirement = 3;
		card.GridSize = FVector2D(4.0f, 4.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 200.0f, 200.0f);
		card.BuildCost.Emplace(EOLCResourceType::Minerals, 100.0f, 100.0f);
		card.Description = LOCTEXT("FusionReactorDesc", "High-output fusion power plant. Requires TIR 3.");
		card.PowerConsumption = -15.0f; // produces 15 energy/tick
		card.ExpectedOutputPerTick.Emplace(EOLCResourceType::Energy, 15.0f, 1000.0f);
		card.bAvailable = true;
	}

	// --- Extraction ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("Mine", "MINE");
		card.Category = EOLCConstructionCategory::Extraction;
		card.TIRRequirement = 1;
		card.GridSize = FVector2D(2.0f, 2.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 40.0f, 40.0f);
		card.BuildCost.Emplace(EOLCResourceType::Minerals, 30.0f, 30.0f);
		card.PowerConsumption = 5.0f; // consumes 5 energy/tick
		card.Description = LOCTEXT("MineDesc", "Extracts minerals from underground deposits. Requires power connection.");
		card.ExpectedOutputPerTick.Emplace(EOLCResourceType::Minerals, 8.0f, 600.0f);
		card.bAvailable = true;
	}
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("FuelRefinery", "FUEL REFINERY");
		card.Category = EOLCConstructionCategory::Extraction;
		card.TIRRequirement = 2;
		card.GridSize = FVector2D(3.0f, 3.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 150.0f, 150.0f);
		card.BuildCost.Emplace(EOLCResourceType::Minerals, 50.0f, 50.0f);
		card.Description = LOCTEXT("FuelRefineryDesc", "Processes raw materials into fuel.");
		card.PowerConsumption = 4.0f; // consumes 4 energy/tick
		card.ExpectedOutputPerTick.Emplace(EOLCResourceType::Fuel, 3.0f, 500.0f);
		card.bAvailable = true;
	}

	// --- Infrastructure ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("CommandCenter", "COMMAND CENTER");
		card.Category = EOLCConstructionCategory::Infrastructure;
		card.TIRRequirement = 1;
		card.GridSize = FVector2D(4.0f, 4.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 300.0f, 300.0f);
		card.BuildCost.Emplace(EOLCResourceType::HullParts, 50.0f, 50.0f);
		card.Description = LOCTEXT("CommandCenterDesc", "Central command hub for base operations.");
		card.PowerConsumption = 5.0f; // consumes 5 energy/tick
		card.bAvailable = true;
	}

	// --- Storage ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("ResourceDepot", "RESOURCE DEPOT");
		card.Category = EOLCConstructionCategory::Storage;
		card.TIRRequirement = 1;
		card.GridSize = FVector2D(3.0f, 3.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 100.0f, 100.0f);
		card.Description = LOCTEXT("ResourceDepotDesc", "Increases storage capacity for all resources.");
		card.PowerConsumption = 1.0f; // consumes 1 energy/tick
		card.bAvailable = true;
	}

	// --- Production ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("Fabricator", "HULL FABRICATOR");
		card.Category = EOLCConstructionCategory::Production;
		card.TIRRequirement = 2;
		card.GridSize = FVector2D(3.0f, 3.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 180.0f, 180.0f);
		card.BuildCost.Emplace(EOLCResourceType::Minerals, 80.0f, 80.0f);
		card.Description = LOCTEXT("FabricatorDesc", "Manufactures hull parts from raw minerals.");
		card.PowerConsumption = 3.0f; // consumes 3 energy/tick
		card.ExpectedOutputPerTick.Emplace(EOLCResourceType::HullParts, 2.0f, 300.0f);
		card.bAvailable = true;
	}

	// --- Defense ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("Turret", "DEFENSE TURRET");
		card.Category = EOLCConstructionCategory::Defense;
		card.TIRRequirement = 2;
		card.GridSize = FVector2D(2.0f, 2.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 120.0f, 120.0f);
		card.BuildCost.Emplace(EOLCResourceType::HullParts, 30.0f, 30.0f);
		card.Description = LOCTEXT("TurretDesc", "Automated defense turret for perimeter protection.");
		card.PowerConsumption = 2.0f; // consumes 2 energy/tick
		card.bAvailable = true;
	}

	// --- Support ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("MedBay", "MEDICAL BAY");
		card.Category = EOLCConstructionCategory::Support;
		card.TIRRequirement = 1;
		card.GridSize = FVector2D(2.0f, 3.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 90.0f, 90.0f);
		card.Description = LOCTEXT("MedBayDesc", "Improves crew survival rate and recovery.");
		card.PowerConsumption = 2.0f; // consumes 2 energy/tick
		card.ExpectedOutputPerTick.Emplace(EOLCResourceType::Survival, 1.5f, 100.0f);
		card.bAvailable = true;
	}

	// --- HighTier (endgame Elite tier) ---

	// --- Tier 1 Starting Base Buildings (WP-04) ---
	// Solar Array — PowerGeneration, 2x1 grid, produces energy
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("SolarArray", "SOLAR ARRAY");
		card.Category = EOLCConstructionCategory::Power;
		card.TIRRequirement = 1;
		card.GridSize = FVector2D(2.0f, 1.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 80.0f, 80.0f);
		card.BuildCost.Emplace(EOLCResourceType::Minerals, 60.0f, 60.0f);
		card.PowerConsumption = -5.0f; // produces 5 energy/turn base
		card.Description = LOCTEXT("SolarArrayDesc", "Passive solar energy generation. Desert +30%, Jungle -50%.");
		card.ExpectedOutputPerTick.Emplace(EOLCResourceType::Energy, 5.0f, 1000.0f);
		card.bAvailable = true;
	}
	// Camp Barracks — Infrastructure, 2x2 grid, unit housing/training
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("CampBarracks", "CAMP BARRACKS");
		card.Category = EOLCConstructionCategory::Infrastructure;
		card.TIRRequirement = 1;
		card.GridSize = FVector2D(2.0f, 2.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 200.0f, 200.0f);
		card.BuildCost.Emplace(EOLCResourceType::Minerals, 100.0f, 100.0f);
		card.Description = LOCTEXT("CampBarracksDesc", "Required for producing soldier units. Training yard with blast doors.");
		card.PowerConsumption = 3.0f; // consumes 3 energy/tick
		card.ExpectedOutputPerTick.Emplace(EOLCResourceType::Survival, 1.0f, 100.0f);
		card.bAvailable = true;
	}
	// Habitation Module — Infrastructure, 2x2 grid, +8 unit cap
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("HabitationModule", "HABITATION MODULE");
		card.Category = EOLCConstructionCategory::Infrastructure;
		card.TIRRequirement = 1;
		card.GridSize = FVector2D(2.0f, 2.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 300.0f, 300.0f);
		card.BuildCost.Emplace(EOLCResourceType::Minerals, 150.0f, 150.0f);
		card.BuildCost.Emplace(EOLCResourceType::Survival, 50.0f, 50.0f);
		card.Description = LOCTEXT("HabitationDesc", "Worker housing. +8 unit capacity. Rooftop garden detail.");
		card.PowerConsumption = 2.0f; // consumes 2 energy/tick
		card.bAvailable = true;
	}
	// Wall Segment — Infrastructure, 1x1 grid, passive defense
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("WallSegment", "WALL SEGMENT");
		card.Category = EOLCConstructionCategory::Infrastructure;
		card.TIRRequirement = 1;
		card.GridSize = FVector2D(1.0f, 1.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 30.0f, 30.0f);
		card.BuildCost.Emplace(EOLCResourceType::Minerals, 20.0f, 20.0f);
		card.Description = LOCTEXT("WallDesc", "Passive defensive wall. Blocks enemy movement. Place adjacently for perimeter.");
		card.PowerConsumption = 0.0f; // no power needed
		card.bAvailable = true;
	}
	// Gate — Infrastructure, 1x1 grid, controlled entry
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("Gate", "GATE");
		card.Category = EOLCConstructionCategory::Infrastructure;
		card.TIRRequirement = 1;
		card.GridSize = FVector2D(1.0f, 1.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 80.0f, 80.0f);
		card.BuildCost.Emplace(EOLCResourceType::Minerals, 50.0f, 50.0f);
		card.Description = LOCTEXT("GateDesc", "Controlled entry point for wall perimeters. Powered to function.");
		card.PowerConsumption = 1.0f; // consumes 1 energy/tick
		card.bAvailable = true;
	}
	// Locker 1x1 — Storage, 1x1 grid, +200 storage capacity
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("Locker", "LOCKER (1x1)");
		card.Category = EOLCConstructionCategory::Storage;
		card.TIRRequirement = 1;
		card.GridSize = FVector2D(1.0f, 1.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 50.0f, 50.0f);
		card.Description = LOCTEXT("LockerDesc", "Basic personal storage. +200 capacity per resource type.");
		card.PowerConsumption = 0.0f; // no power needed
		card.bAvailable = true;
	}

	// --- HighTier (endgame Elite tier) ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("VoidLab", "VOID LAB");
		card.Category = EOLCConstructionCategory::HighTier;
		card.TIRRequirement = 4;
		card.GridSize = FVector2D(6.0f, 6.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 500.0f, 500.0f);
		card.BuildCost.Emplace(EOLCResourceType::Minerals, 300.0f, 300.0f);
		card.BuildCost.Emplace(EOLCResourceType::DarkMatterCrystals, 10.0f, 10.0f);
		card.Description = LOCTEXT("VoidLabDesc", "Advanced research facility for void-tier technologies.");
		card.bAvailable = true;
	}
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("AssemblyPlant", "ASSEMBLY PLANT");
		card.Category = EOLCConstructionCategory::HighTier;
		card.TIRRequirement = 5;
		card.GridSize = FVector2D(6.0f, 6.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 600.0f, 600.0f);
		card.BuildCost.Emplace(EOLCResourceType::HullParts, 200.0f, 200.0f);
		card.Description = LOCTEXT("AssemblyPlantDesc", "Mass-production assembly line for advanced structures.");
		card.bAvailable = true;
	}
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("CrystalSynthesizer", "CRYSTAL SYNTHESIZER");
		card.Category = EOLCConstructionCategory::HighTier;
		card.TIRRequirement = 4;
		card.GridSize = FVector2D(3.0f, 3.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 350.0f, 350.0f);
		card.BuildCost.Emplace(EOLCResourceType::Energy, 200.0f, 200.0f);
		card.Description = LOCTEXT("CrystalSynthDesc", "Synthesizes dark matter crystals from raw energy.");
		card.bAvailable = true;
	}

	// --- Special (Elite tier special buildings) ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("DungeonScanner", "DUNGEON SCANNER");
		card.Category = EOLCConstructionCategory::Special;
		card.TIRRequirement = 3;
		card.GridSize = FVector2D(2.0f, 2.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 150.0f, 150.0f);
		card.BuildCost.Emplace(EOLCResourceType::Energy, 80.0f, 80.0f);
		card.Description = LOCTEXT("DungeonScannerDesc", "Detects nearby dungeon entrances and hazards.");
		card.bAvailable = true;
	}
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("ResourceConverter", "RESOURCE CONVERTER");
		card.Category = EOLCConstructionCategory::Special;
		card.TIRRequirement = 3;
		card.GridSize = FVector2D(3.0f, 3.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 250.0f, 250.0f);
		card.Description = LOCTEXT("ResourceConverterDesc", "Converts surplus resources into alternative types.");
		card.bAvailable = true;
	}
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("AlienArtifactDecoder", "ALIEN ARTIFACT DECODER");
		card.Category = EOLCConstructionCategory::Special;
		card.TIRRequirement = 5;
		card.GridSize = FVector2D(4.0f, 4.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 400.0f, 400.0f);
		card.BuildCost.Emplace(EOLCResourceType::DarkMatterCrystals, 5.0f, 5.0f);
		card.Description = LOCTEXT("AlienDecoderDesc", "Deciphers alien artifacts for advanced research.");
		card.bAvailable = true;
	}

	// --- Additional Power buildings (WP-13 Step 5: expand to 32) ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("WindTurbine", "WIND TURBINE");
		card.Category = EOLCConstructionCategory::Power;
		card.TIRRequirement = 2;
		card.GridSize = FVector2D(1.0f, 2.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 60.0f, 60.0f);
		card.BuildCost.Emplace(EOLCResourceType::HullParts, 30.0f, 30.0f);
		card.PowerConsumption = -8.0f;
		card.Description = LOCTEXT("WindTurbineDesc", "Harnesses atmospheric winds for energy. Rocky +20%.");
		card.bAvailable = true;
	}
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("GeothermalVent", "GEOTHERMAL VENT TAP");
		card.Category = EOLCConstructionCategory::Power;
		card.TIRRequirement = 3;
		card.GridSize = FVector2D(2.0f, 2.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 120.0f, 120.0f);
		card.BuildCost.Emplace(EOLCResourceType::Minerals, 80.0f, 80.0f);
		card.PowerConsumption = -15.0f;
		card.Description = LOCTEXT("GeothermalDesc", "Taps planetary geothermal vents. High output, needs rocky terrain.");
		card.bAvailable = true;
	}

	// --- Additional Extraction buildings ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("WaterExtractor", "WATER EXTRACTOR");
		card.Category = EOLCConstructionCategory::Extraction;
		card.TIRRequirement = 1;
		card.GridSize = FVector2D(1.0f, 1.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 80.0f, 80.0f);
		card.Description = LOCTEXT("WaterExtractorDesc", "Extracts water from ice deposits or atmosphere.");
		card.bAvailable = true;
	}
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("GasCollector", "GAS COLLECTOR");
		card.Category = EOLCConstructionCategory::Extraction;
		card.TIRRequirement = 2;
		card.GridSize = FVector2D(2.0f, 1.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 100.0f, 100.0f);
		card.BuildCost.Emplace(EOLCResourceType::HullParts, 40.0f, 40.0f);
		card.Description = LOCTEXT("GasCollectorDesc", "Collects atmospheric gases for fuel processing.");
		card.bAvailable = true;
	}

	// --- Additional Infrastructure buildings ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("RoadSegment", "ROAD SEGMENT");
		card.Category = EOLCConstructionCategory::Infrastructure;
		card.TIRRequirement = 1;
		card.GridSize = FVector2D(1.0f, 1.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 20.0f, 20.0f);
		card.Description = LOCTEXT("RoadSegmentDesc", "Connects buildings for faster unit movement.");
		card.bAvailable = true;
	}
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("LandingPad", "LANDING PAD");
		card.Category = EOLCConstructionCategory::Infrastructure;
		card.TIRRequirement = 2;
		card.GridSize = FVector2D(3.0f, 3.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 250.0f, 250.0f);
		card.BuildCost.Emplace(EOLCResourceType::HullParts, 100.0f, 100.0f);
		card.Description = LOCTEXT("LandingPadDesc", "Allows aerial unit landings and supply drops.");
		card.bAvailable = true;
	}
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("CommunicationArray", "COMMUNICATION ARRAY");
		card.Category = EOLCConstructionCategory::Infrastructure;
		card.TIRRequirement = 2;
		card.GridSize = FVector2D(2.0f, 2.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 180.0f, 180.0f);
		card.BuildCost.Emplace(EOLCResourceType::Minerals, 60.0f, 60.0f);
		card.Description = LOCTEXT("CommArrayDesc", "Extends command range and enables long-range scanning.");
		card.bAvailable = true;
	}

	// --- Additional Storage buildings ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("FuelTank", "FUEL TANK");
		card.Category = EOLCConstructionCategory::Storage;
		card.TIRRequirement = 1;
		card.GridSize = FVector2D(2.0f, 1.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 70.0f, 70.0f);
		card.Description = LOCTEXT("FuelTankDesc", "Dedicated fuel storage. +500 fuel capacity.");
		card.bAvailable = true;
	}

	// --- Additional Production buildings ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("FoodProcessor", "FOOD PROCESSOR");
		card.Category = EOLCConstructionCategory::Production;
		card.TIRRequirement = 1;
		card.GridSize = FVector2D(2.0f, 2.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 100.0f, 100.0f);
		card.Description = LOCTEXT("FoodProcessorDesc", "Processes survival rations from raw materials.");
		card.bAvailable = true;
	}
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("VehicleBay", "VEHICLE BAY");
		card.Category = EOLCConstructionCategory::Production;
		card.TIRRequirement = 3;
		card.GridSize = FVector2D(4.0f, 3.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 350.0f, 350.0f);
		card.BuildCost.Emplace(EOLCResourceType::HullParts, 150.0f, 150.0f);
		card.Description = LOCTEXT("VehicleBayDesc", "Manufactures and repairs ground vehicles.");
		card.bAvailable = true;
	}

	// --- Additional Defense buildings ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("MissileBattery", "MISSILE BATTERY");
		card.Category = EOLCConstructionCategory::Defense;
		card.TIRRequirement = 3;
		card.GridSize = FVector2D(2.0f, 2.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 200.0f, 200.0f);
		card.BuildCost.Emplace(EOLCResourceType::HullParts, 80.0f, 80.0f);
		card.Description = LOCTEXT("MissileBatteryDesc", "Long-range missile defense system.");
		card.bAvailable = true;
	}
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("ShieldGenerator", "SHIELD GENERATOR");
		card.Category = EOLCConstructionCategory::Defense;
		card.TIRRequirement = 4;
		card.GridSize = FVector2D(3.0f, 3.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 300.0f, 300.0f);
		card.BuildCost.Emplace(EOLCResourceType::HullParts, 150.0f, 150.0f);
		card.Description = LOCTEXT("ShieldGeneratorDesc", "Area shield for nearby buildings.");
		card.bAvailable = true;
	}

	// --- Additional Support buildings ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("ResearchLab", "RESEARCH LAB");
		card.Category = EOLCConstructionCategory::Support;
		card.TIRRequirement = 2;
		card.GridSize = FVector2D(3.0f, 2.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 200.0f, 200.0f);
		card.BuildCost.Emplace(EOLCResourceType::Minerals, 100.0f, 100.0f);
		card.Description = LOCTEXT("ResearchLabDesc", "Accelerates tech research progress.");
		card.bAvailable = true;
	}
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("MarketStation", "MARKET STATION");
		card.Category = EOLCConstructionCategory::Support;
		card.TIRRequirement = 3;
		card.GridSize = FVector2D(2.0f, 2.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 180.0f, 180.0f);
		card.Description = LOCTEXT("MarketDesc", "Enables trading surplus resources for alternatives.");
		card.bAvailable = true;
	}

	// --- Additional HighTier buildings ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("QuantumCore", "QUANTUM CORE");
		card.Category = EOLCConstructionCategory::HighTier;
		card.TIRRequirement = 5;
		card.GridSize = FVector2D(4.0f, 4.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 400.0f, 400.0f);
		card.BuildCost.Emplace(EOLCResourceType::DarkMatterCrystals, 20.0f, 20.0f);
		card.PowerConsumption = -50.0f;
		card.Description = LOCTEXT("QuantumCoreDesc", "Quantum-powered energy source. Endgame building.");
		card.bAvailable = true;
	}

	// --- Additional Special buildings ---
	{
		FOLCBuildCardViewData& card = BuildCards.Add_GetRef(FOLCBuildCardViewData());
		card.BuildingName = LOCTEXT("SignalBeacon", "SIGNAL BEACON");
		card.Category = EOLCConstructionCategory::Special;
		card.TIRRequirement = 4;
		card.GridSize = FVector2D(2.0f, 2.0f);
		card.BuildCost.Emplace(EOLCResourceType::ConstructionMaterial, 300.0f, 300.0f);
		card.BuildCost.Emplace(EOLCResourceType::Energy, 150.0f, 150.0f);
		card.Description = LOCTEXT("BeaconDesc", "Broadcasts distress signal for rescue or trade.");
		card.bAvailable = true;
	}
}

TArray<FOLCBuildCardViewData> UOLCUIDataSubsystem::GetBuildCardsForCategory(EOLCConstructionCategory InCategory) const
{
	TArray<FOLCBuildCardViewData> Filtered;
	for (const auto& Card : BuildCards)
	{
		if (Card.Category == InCategory)
			Filtered.Add(Card);
	}
	return Filtered;
}

// ---------------------------------------------------------------------------
// Planet badges
// ---------------------------------------------------------------------------
TArray<FOLCBadgeViewData> UOLCUIDataSubsystem::GetCurrentPlanetBadges() const
{
	TArray<FOLCBadgeViewData> Badges;
	Badges.Emplace(EOLCBadgeType::TIR, LOCTEXT("BadgeTIR", "TIR 2"), LOCTEXT("BadgeTIRTip", "Tech Integration Requirement: Level 2"), EOLCColorRole::Primary);
	Badges.Emplace(EOLCBadgeType::Biome, LOCTEXT("BadgeDesert", "DESERT"), LOCTEXT("BadgeDesertTip", "Arid desert biome. Low water availability."), EOLCColorRole::Warning);
	Badges.Emplace(EOLCBadgeType::Hazard, LOCTEXT("BadgeStorm", "STORMS"), LOCTEXT("BadgeStormTip", "Active electrical storms in northern sector."), EOLCColorRole::Danger);
	Badges.Emplace(EOLCBadgeType::Modifier, LOCTEXT("BadgeMineralRich", "MINERAL RICH"), LOCTEXT("BadgeMineralRichTip", "Above-average mineral deposits detected."), EOLCColorRole::Success);
	return Badges;
}

// ---------------------------------------------------------------------------
// Minimap markers
// ---------------------------------------------------------------------------
void UOLCUIDataSubsystem::PopulateFakeMinimapMarkers()
{
	MinimapMarkers.Reset();

	MinimapMarkers.Emplace(FVector2D(0.5f, 0.5f), EOLCMinimapMarkerType::Building, LOCTEXT("MM_CommandCenter", "Command Center"), EOLCColorRole::Primary);
	MinimapMarkers.Emplace(FVector2D(0.3f, 0.4f), EOLCMinimapMarkerType::ResourceNode, LOCTEXT("MM_Minerals", "Mineral Deposit"), EOLCColorRole::Secondary);
	MinimapMarkers.Emplace(FVector2D(0.7f, 0.3f), EOLCMinimapMarkerType::Ally, LOCTEXT("MM_Scout", "Scout Unit"), EOLCColorRole::Success);
	MinimapMarkers.Emplace(FVector2D(0.85f, 0.6f), EOLCMinimapMarkerType::Enemy, LOCTEXT("MM_Enemy", "Hostile Signal"), EOLCColorRole::Danger);
	MinimapMarkers.Emplace(FVector2D(0.2f, 0.7f), EOLCMinimapMarkerType::Objective, LOCTEXT("MM_Objective", "Scan Target"), EOLCColorRole::Primary);
}

// ---------------------------------------------------------------------------
// Simulation speed
// ---------------------------------------------------------------------------
void UOLCUIDataSubsystem::CycleSimulationSpeed()
{
	switch (CurrentSimulationSpeed)
	{
		case EOLCSimulationSpeed::Paused: CurrentSimulationSpeed = EOLCSimulationSpeed::Normal; break;
		case EOLCSimulationSpeed::Normal: CurrentSimulationSpeed = EOLCSimulationSpeed::Fast; break;
		case EOLCSimulationSpeed::Fast:   CurrentSimulationSpeed = EOLCSimulationSpeed::Paused; break;
	}
}

// ---------------------------------------------------------------------------
// Unit capacity
// ---------------------------------------------------------------------------
bool UOLCUIDataSubsystem::AddUnit()
{
	if (CurrentUnitCount >= MaxUnitCapacity) return false;
	CurrentUnitCount++;
	return true;
}

bool UOLCUIDataSubsystem::RemoveUnit()
{
	if (CurrentUnitCount <= 0) return false;
	CurrentUnitCount--;
	return true;
}

// ---------------------------------------------------------------------------
// Reset resources to zero (called on game start)
// ---------------------------------------------------------------------------
void UOLCUIDataSubsystem::ResetResourcesToZero()
{
	ResourceCounters.Reset();
	ResourceCounters.Emplace(EOLCResourceType::Energy, 0.0f, 1000.0f, 0.0f);
	ResourceCounters.Emplace(EOLCResourceType::Fuel, 0.0f, 500.0f, 0.0f);
	ResourceCounters.Emplace(EOLCResourceType::ConstructionMaterial, 0.0f, 800.0f, 0.0f);
	ResourceCounters.Emplace(EOLCResourceType::Minerals, 0.0f, 600.0f, 0.0f);
	ResourceCounters.Emplace(EOLCResourceType::HullParts, 0.0f, 300.0f, 0.0f);
	ResourceCounters.Emplace(EOLCResourceType::Survival, 0.0f, 100.0f, 0.0f);
	ResourceCounters.Emplace(EOLCResourceType::DarkMatterCrystals, 0.0f, 50.0f, 0.0f);

	// Initialize ship state per Briefing dropship starting conditions:
	// Left drive damaged (50% thrust), right drive functional (empty fuel)
	DriveStatus = EOLCModuleState::Damaged;
	DriveTier = 1;
	ShieldStatus = EOLCModuleState::Offline;
	ShieldIntegrityPercent = 0.0f;
	StorageBonusPerResourceType = 0;
}

// ---------------------------------------------------------------------------
// Add resource (bridge from building production to HUD)
// ---------------------------------------------------------------------------
void UOLCUIDataSubsystem::AddResource(EOLCResourceType ResourceType, float Amount)
{
	for (auto& Res : ResourceCounters)
	{
		if (Res.ResourceType == ResourceType)
		{
			Res.Value += Amount;
			// Clamp to capacity.
			if (Res.Value > Res.Capacity)
				Res.Value = Res.Capacity;
			break;
		}
	}
}

// ---------------------------------------------------------------------------
// Build affordability & resource consumption (WP-103 Step 2)
// ---------------------------------------------------------------------------
bool UOLCUIDataSubsystem::CanAffordBuild(const TArray<FOLCResourceAmount>& BuildCost) const
{
	for (const auto& Cost : BuildCost)
	{
		bool bFound = false;
		float CurrentValue = 0.0f;
		for (const auto& Res : ResourceCounters)
		{
			if (Res.ResourceType == Cost.ResourceType)
			{
				CurrentValue = Res.Value;
				bFound = true;
				break;
			}
		}
		if (!bFound || CurrentValue < Cost.CurrentValue)
		{
			return false;
		}
	}
	return true;
}

bool UOLCUIDataSubsystem::ConsumeResourcesForBuild(const TArray<FOLCResourceAmount>& BuildCost)
{
	if (!CanAffordBuild(BuildCost))
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Cannot afford build — insufficient resources"));
		return false;
	}

	for (const auto& Cost : BuildCost)
	{
		for (auto& Res : ResourceCounters)
		{
			if (Res.ResourceType == Cost.ResourceType)
			{
				Res.Value -= Cost.CurrentValue;
				if (Res.Value < 0.0f) Res.Value = 0.0f;
				break;
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[OLC] Resources consumed for build: %d resource types"), BuildCost.Num());
	return true;
}

// ---------------------------------------------------------------------------
// Mission objective management (tutorial flow)
// ---------------------------------------------------------------------------
void UOLCUIDataSubsystem::SetMissionObjectives(const TArray<FOLCMissionObjectiveViewData>& InObjectives)
{
	MissionObjectives = InObjectives;
	UE_LOG(LogTemp, Log, TEXT("[OLC] Mission objectives set: %d items"), MissionObjectives.Num());
}

void UOLCUIDataSubsystem::CompleteObjective(int32 Index)
{
	if (Index < 0 || Index >= MissionObjectives.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] CompleteObjective: invalid index %d (count=%d)"), Index, MissionObjectives.Num());
		return;
	}

	MissionObjectives[Index].State = EOLCProgressState::Complete;
	MissionObjectives[Index].Progress = MissionObjectives[Index].TargetProgress;

	UE_LOG(LogTemp, Log, TEXT("[OLC] Objective completed: %s"), *MissionObjectives[Index].ObjectiveName.ToString());
}

void UOLCUIDataSubsystem::ActivateObjective(int32 Index)
{
	if (Index < 0 || Index >= MissionObjectives.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] ActivateObjective: invalid index %d (count=%d)"), Index, MissionObjectives.Num());
		return;
	}

	MissionObjectives[Index].State = EOLCProgressState::Active;

	UE_LOG(LogTemp, Log, TEXT("[OLC] Objective activated: %s"), *MissionObjectives[Index].ObjectiveName.ToString());
}

void UOLCUIDataSubsystem::AdvanceObjectiveProgress(int32 Index, float Delta)
{
	if (Index < 0 || Index >= MissionObjectives.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] AdvanceObjectiveProgress: invalid index %d (count=%d)"), Index, MissionObjectives.Num());
		return;
	}

	MissionObjectives[Index].Progress += Delta;
	if (MissionObjectives[Index].Progress >= MissionObjectives[Index].TargetProgress)
	{
		MissionObjectives[Index].Progress = MissionObjectives[Index].TargetProgress;
		MissionObjectives[Index].State = EOLCProgressState::Complete;
		UE_LOG(LogTemp, Log, TEXT("[OLC] Objective completed by progress: %s"), *MissionObjectives[Index].ObjectiveName.ToString());
	}
	else
	{
		if (MissionObjectives[Index].State == EOLCProgressState::Idle)
		{
			MissionObjectives[Index].State = EOLCProgressState::Active;
		}
	}
}

// ---------------------------------------------------------------------------
// Ship state — S13/S14 wiring to HUD and Solar System
// ---------------------------------------------------------------------------

void UOLCUIDataSubsystem::SetDriveStatus(EOLCModuleState InStatus)
{
	DriveStatus = InStatus;
	UE_LOG(LogTemp, Log, TEXT("[OLC] Drive status set to: %d"), (int32)InStatus);
}

void UOLCUIDataSubsystem::SetDriveTier(int32 InTier)
{
	DriveTier = FMath::Clamp(InTier, 1, 5);
	UE_LOG(LogTemp, Log, TEXT("[OLC] Drive tier set to: %d"), DriveTier);
}

void UOLCUIDataSubsystem::SetShieldStatus(EOLCModuleState InStatus)
{
	ShieldStatus = InStatus;
	switch (InStatus)
	{
		case EOLCModuleState::Installed: ShieldIntegrityPercent = 1.0f; break;
		case EOLCModuleState::Damaged:  ShieldIntegrityPercent = 0.5f; break;
		case EOLCModuleState::Offline:  ShieldIntegrityPercent = 0.0f; break;
	}
	UE_LOG(LogTemp, Log, TEXT("[OLC] Shield status set to: %d (integrity: %.0f%%)"), (int32)InStatus, ShieldIntegrityPercent * 100.0f);
}

void UOLCUIDataSubsystem::AddStorageBonus(int32 Amount)
{
	StorageBonusPerResourceType += FMath::Max(Amount, 0);
	UE_LOG(LogTemp, Log, TEXT("[OLC] Storage bonus added: +%d per resource (total: %d)"), Amount, StorageBonusPerResourceType);

	// Apply the bonus to existing resource counters immediately.
	for (auto& Res : ResourceCounters)
	{
		Res.Capacity += Amount;
	}
}

int32 UOLCUIDataSubsystem::GetMaxReachableFuelCost() const
{
	// Base range: DriveTier × 100 fuel. Damaged drive halves range, offline = 0.
	const int32 BaseRange = DriveTier * 100;
	switch (DriveStatus)
	{
		case EOLCModuleState::Installed: return BaseRange;
		case EOLCModuleState::Damaged:   return FMath::Max(BaseRange / 2, 50);
		case EOLCModuleState::Offline:   return 0;
	}
	return BaseRange;
}

// ---------------------------------------------------------------------------
// Actions / Progresses (fake)
// ---------------------------------------------------------------------------
TArray<FOLCActionViewData> UOLCUIDataSubsystem::GetQuickBuildActions() const
{
	TArray<FOLCActionViewData> Actions;
	Actions.Emplace(LOCTEXT("QuickPower", "QUICK POWER"), LOCTEXT("QuickPowerTip", "Place a solar panel array."), true, 1);
	Actions.Emplace(LOCTEXT("QuickDefense", "QUICK DEFENSE"), LOCTEXT("QuickDefenseTip", "Place a defense turret."), true, 2);
	Actions.Emplace(LOCTEXT("QuickStorage", "QUICK STORAGE"), LOCTEXT("QuickStorageTip", "Place a resource depot."), true, 3);
	return Actions;
}

TArray<FOLCProgressViewData> UOLCUIDataSubsystem::GetActiveProgresses() const
{
	TArray<FOLCProgressViewData> Progresses;
	Progresses.Emplace(LOCTEXT("Prog_Build", "BUILDING: SOLAR PANEL"), 65.0f, 100.0f, EOLCProgressState::Active, EOLCColorRole::Primary);
	Progresses.Emplace(LOCTEXT("Prog_Scan", "PLANET SCAN"), 67.0f, 100.0f, EOLCProgressState::Active, EOLCColorRole::Secondary);
	return Progresses;
}

// ---------------------------------------------------------------------------
// Resource Production System (WP-104)
// ---------------------------------------------------------------------------

void UOLCUIDataSubsystem::StartProductionTick()
{
	if (ProductionTickTimer.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Production tick already running"));
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(
		ProductionTickTimer,
		this,
		&UOLCUIDataSubsystem::PerformProductionTick,
		ProductionTickInterval,
		true // bLoop = true
	);

	UE_LOG(LogTemp, Log, TEXT("[OLC] Production tick started (interval: %.1fs)"), ProductionTickInterval);
}

void UOLCUIDataSubsystem::StopProductionTick()
{
	if (!ProductionTickTimer.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Production tick not running"));
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(ProductionTickTimer);
	UE_LOG(LogTemp, Log, TEXT("[OLC] Production tick stopped"));
}

void UOLCUIDataSubsystem::SetProductionTickInterval(float Interval)
{
	ProductionTickInterval = FMath::Max(Interval, 1.0f); // Minimum 1 second

	if (ProductionTickTimer.IsValid())
	{
		// Restart timer with new interval
		GetWorld()->GetTimerManager().ClearTimer(ProductionTickTimer);
		GetWorld()->GetTimerManager().SetTimer(
			ProductionTickTimer,
			this,
			&UOLCUIDataSubsystem::PerformProductionTick,
			ProductionTickInterval,
			true
		);
		UE_LOG(LogTemp, Log, TEXT("[OLC] Production tick interval updated to %.1fs"), ProductionTickInterval);
	}
}

void UOLCUIDataSubsystem::RegisterBuilding(AActor* BuildingActor, UOLCBuildingData* BuildingData)
{
	if (!BuildingActor || !BuildingData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] RegisterBuilding: null actor or building data"));
		return;
	}

	// Check if already registered
	for (const auto& Entry : RegisteredBuildings)
	{
		if (Entry.Actor.Get() == BuildingActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("[OLC] RegisterBuilding: building already registered"));
			return;
		}
	}

	RegisteredBuildings.Add(FRegisteredBuildingEntry(BuildingActor, BuildingData));
	UE_LOG(LogTemp, Log, TEXT("[OLC] Registered building: %s (%d outputs)"), *BuildingData->DisplayName.ToString(), BuildingData->OutputPerTick.Num());
}

void UOLCUIDataSubsystem::UnregisterBuilding(AActor* BuildingActor)
{
	if (!BuildingActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] UnregisterBuilding: null actor"));
		return;
	}

	for (int32 i = RegisteredBuildings.Num() - 1; i >= 0; --i)
	{
		if (RegisteredBuildings[i].Actor.Get() == BuildingActor)
		{
			UE_LOG(LogTemp, Log, TEXT("[OLC] Unregistered building"));
			RegisteredBuildings.RemoveAt(i);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[OLC] UnregisterBuilding: building not found"));
}

float UOLCUIDataSubsystem::CalculateGridBalance() const
{
	float Balance = TotalPowerProduction - TotalPowerConsumption;
	return Balance;
}

float UOLCUIDataSubsystem::GetTotalPowerProduction() const
{
	return TotalPowerProduction;
}

float UOLCUIDataSubsystem::GetTotalPowerConsumption() const
{
	return TotalPowerConsumption;
}

void UOLCUIDataSubsystem::SetActiveBiome(EOLCBiomeType Biome)
{
	if (ActiveBiome != Biome)
	{
		UE_LOG(LogTemp, Log, TEXT("[OLC] Active biome changed to: %d"), (int32)Biome);
	}
	ActiveBiome = Biome;
}

float UOLCUIDataSubsystem::GetMaxCapacity(EOLCResourceType ResourceType) const
{
	float BaseCapacity = 1000.0f; // Default base capacity

	// Find current resource counter to get its base capacity
	for (const auto& Res : ResourceCounters)
	{
		if (Res.ResourceType == ResourceType)
		{
			BaseCapacity = Res.Capacity;
			break;
		}
	}

	// Add locker/storage bonuses
	float MaxCap = BaseCapacity + StorageBonusPerResourceType * RegisteredBuildings.Num();

	// Count locker buildings specifically (simplified: all buildings add bonus)
	for (const auto& Entry : RegisteredBuildings)
	{
		if (Entry.BuildingData && Entry.BuildingData->Category == EOLCConstructionCategory::Storage)
		{
			MaxCap += 200.0f; // Locker buildings add +200 per resource type
		}
	}

	return MaxCap;
}

EOLCStoragePressure UOLCUIDataSubsystem::GetStoragePressure(EOLCResourceType ResourceType) const
{
	for (const auto& Res : ResourceCounters)
	{
		if (Res.ResourceType == ResourceType)
		{
			float Ratio = Res.Value / Res.Capacity;
			if (Ratio >= 1.0f)
				return EOLCStoragePressure::Overflow;
			else if (Ratio >= 0.85f)
				return EOLCStoragePressure::Approaching;
			else if (Ratio >= 0.30f)
				return EOLCStoragePressure::Normal;
			else
				return EOLCStoragePressure::Normal;
		}
	}
	return EOLCStoragePressure::Normal;
}

void UOLCUIDataSubsystem::PerformProductionTick()
{
	if (RegisteredBuildings.Num() == 0)
	{
		return; // No buildings to produce
	}

	UE_LOG(LogTemp, Log, TEXT("[OLC] Performing production tick..."));

	CumulativeEnergyProduced = 0.0f;

	// Calculate power grid balance
	CheckPowerDeficit();

	// Initialize production rates
	ProductionRates.Reset();
	for (int32 i = 0; i < static_cast<int32>(EOLCResourceType::DarkMatterCrystals); ++i)
	{
		EOLCResourceType ResourceType = static_cast<EOLCResourceType>(i);
		ProductionRates.Emplace(ResourceType, 0.0f, 1000.0f, 0.0f);
	}

	// Iterate all registered buildings and produce resources
	for (const auto& Entry : RegisteredBuildings)
	{
		if (!Entry.BuildingData || !Entry.Actor.IsValid())
			continue;

		TArray<FOLCResourceAmount> Outputs = Entry.BuildingData->OutputPerTick;

		// Apply biome modifiers
		ApplyBiomeModifiers(Outputs, ActiveBiome);

		// If power deficit, halve all outputs
		if (bPowerDeficit)
		{
			for (auto& Output : Outputs)
			{
				Output.CurrentValue *= 0.5f;
			}
			UE_LOG(LogTemp, Log, TEXT("[OLC] Power deficit active — output halved for building"));
		}

		// Add produced resources
		for (const auto& Output : Outputs)
		{
			AddResource(Output.ResourceType, Output.CurrentValue);

			// Track production rate
			for (auto& Rate : ProductionRates)
			{
				if (Rate.ResourceType == Output.ResourceType)
				{
					Rate.Delta += Output.CurrentValue;
					break;
				}
			}

			// Track cumulative energy produced
			if (Output.ResourceType == EOLCResourceType::Energy)
			{
				CumulativeEnergyProduced += Output.CurrentValue;
			}
		}
	}

	// Update resource counters with fresh data
	UpdateResourceCounters();

	// Update tutorial objectives based on production (WP-104 Step 6)
	UpdateTutorialProgress();

	UE_LOG(LogTemp, Log, TEXT("[OLC] Production tick complete — Energy produced: %.1f"), CumulativeEnergyProduced);
}

void UOLCUIDataSubsystem::UpdateResourceCounters()
{
	// Update delta values in resource counters based on production rates
	for (auto& Res : ResourceCounters)
	{
		for (const auto& Rate : ProductionRates)
		{
			if (Res.ResourceType == Rate.ResourceType)
			{
				Res.Delta = Rate.Delta;
				break;
			}
		}

		// Clamp to capacity
		float MaxCap = GetMaxCapacity(Res.ResourceType);
		if (Res.Value > MaxCap)
		{
			UE_LOG(LogTemp, Warning, TEXT("[OLC] Resource %d overflow: %.1f / %.1f — production lost"),
				(int32)Res.ResourceType, Res.Value, MaxCap);
			Res.Value = MaxCap;
		}

		// Update pressure state
		float Ratio = Res.Value / Res.Capacity;
		if (Ratio >= 1.0f)
			Res.PressureState = EOLCStoragePressure::Full;
		else if (Ratio >= 0.85f)
			Res.PressureState = EOLCStoragePressure::Approaching;
		else
			Res.PressureState = EOLCStoragePressure::Normal;
	}
}

void UOLCUIDataSubsystem::CalculateProductionRates()
{
	// Reset production rates
	ProductionRates.Reset();
	for (int32 i = 0; i < static_cast<int32>(EOLCResourceType::DarkMatterCrystals); ++i)
	{
		EOLCResourceType ResourceType = static_cast<EOLCResourceType>(i);
		ProductionRates.Emplace(ResourceType, 0.0f, 1000.0f, 0.0f);
	}

	// Sum up all building outputs
	for (const auto& Entry : RegisteredBuildings)
	{
		if (!Entry.BuildingData)
			continue;

		for (const auto& Output : Entry.BuildingData->OutputPerTick)
		{
			for (auto& Rate : ProductionRates)
			{
				if (Rate.ResourceType == Output.ResourceType)
				{
					Rate.Delta += Output.CurrentValue;
					break;
				}
			}
		}
	}
}

void UOLCUIDataSubsystem::CheckPowerDeficit()
{
	TotalPowerProduction = 0.0f;
	TotalPowerConsumption = 0.0f;

	// Calculate total power production and consumption
	for (const auto& Entry : RegisteredBuildings)
	{
		if (!Entry.BuildingData)
			continue;

		if (Entry.BuildingData->PowerConsumption < 0.0f)
		{
			// Negative = producer
			TotalPowerProduction += FMath::Abs(Entry.BuildingData->PowerConsumption);
		}
		else if (Entry.BuildingData->PowerConsumption > 0.0f)
		{
			// Positive = consumer
			TotalPowerConsumption += Entry.BuildingData->PowerConsumption;
		}
	}

	bool bNewDeficit = TotalPowerConsumption > TotalPowerProduction;

	if (bNewDeficit != bPowerDeficit)
	{
		OnPowerDeficitStateChanged(bNewDeficit);
	}
}

void UOLCUIDataSubsystem::ApplyBiomeModifiers(TArray<FOLCResourceAmount>& Outputs, EOLCBiomeType Biome) const
{
	for (auto& Output : Outputs)
	{
		// Note: This is a simplified implementation.
		// In production, each building would have its own biome modifiers.
		// For now, we apply generic biome bonuses based on resource type.
		switch (Biome)
		{
			case EOLCBiomeType::Desert:
				// Desert Solar Array: Energy output × 1.5
				if (Output.ResourceType == EOLCResourceType::Energy)
				{
					Output.CurrentValue *= 1.5f;
				}
				break;

			case EOLCBiomeType::Rocky:
				// Rocky building producing Minerals: output × 1.25
				if (Output.ResourceType == EOLCResourceType::Minerals)
				{
					Output.CurrentValue *= 1.25f;
				}
				break;

			case EOLCBiomeType::Water:
			case EOLCBiomeType::Jungle:
				// Water/Jungle Survival buildings: output × 1.5
				if (Output.ResourceType == EOLCResourceType::Survival)
				{
					Output.CurrentValue *= 1.5f;
				}
				break;

			default:
				break;
		}
	}
}

void UOLCUIDataSubsystem::OnPowerDeficitStateChanged(bool bNewDeficit)
{
	bPowerDeficit = bNewDeficit;

	if (bNewDeficit)
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] POWER DEFICIT DETECTED — consumption (%.1f) > production (%.1f)"),
			TotalPowerConsumption, TotalPowerProduction);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[OLC] Power Restored — production (%.1f) >= consumption (%.1f)"),
			TotalPowerProduction, TotalPowerConsumption);
	}
}

// ---------------------------------------------------------------------------
// Ship Module System (WP-106)
// ---------------------------------------------------------------------------

void UOLCUIDataSubsystem::PopulateShipModules()
{
	AvailableModules.Reset();
	InstalledModules.Reset();

	// --- Drives ---
	{
		FOLCShipModuleViewData& mod = AvailableModules.Add_GetRef(FOLCShipModuleViewData());
		mod.DisplayName = LOCTEXT("Mod_IonDrive", "ION DRIVE T1");
		mod.Category = EOLCShipModuleCategory::Drives;
		mod.TIRTier = 1;
		mod.HullSlotsRequired = 1;
		mod.PowerConsumption = -2.0f; // produces 2 energy/tick
		mod.EffectDescription = LOCTEXT("Mod_IonDriveDesc", "Basic ion drive. Damaged on crash.");
		mod.State = EOLCModuleState::Damaged;
		mod.IntegrityPercent = 0.5f;
		mod.RepairCost.Emplace(EOLCResourceType::ConstructionMaterial, 20.0f, 20.0f);
		mod.RepairCost.Emplace(EOLCResourceType::Minerals, 15.0f, 15.0f);
	}
	{
		FOLCShipModuleViewData& mod = AvailableModules.Add_GetRef(FOLCShipModuleViewData());
		mod.DisplayName = LOCTEXT("Mod_QuantumDrive", "QUANTUM DRIVE T2");
		mod.Category = EOLCShipModuleCategory::Drives;
		mod.TIRTier = 2;
		mod.HullSlotsRequired = 2;
		mod.PowerConsumption = -5.0f; // produces 5 energy/tick
		mod.EffectDescription = LOCTEXT("Mod_QuantumDriveDesc", "Advanced quantum drive. +50% travel range.");
		mod.State = EOLCModuleState::Offline;
		mod.IntegrityPercent = 1.0f;
	}

	// --- Protection ---
	{
		FOLCShipModuleViewData& mod = AvailableModules.Add_GetRef(FOLCShipModuleViewData());
		mod.DisplayName = LOCTEXT("Mod_AluminumHull", "ALUMINUM HULL T1");
		mod.Category = EOLCShipModuleCategory::Protection;
		mod.TIRTier = 1;
		mod.HullSlotsRequired = 1;
		mod.PowerConsumption = 0.0f;
		mod.EffectDescription = LOCTEXT("Mod_AluminumHullDesc", "Basic aluminum hull plating.");
		mod.State = EOLCModuleState::Installed;
		mod.IntegrityPercent = 1.0f;
	}
	{
		FOLCShipModuleViewData& mod = AvailableModules.Add_GetRef(FOLCShipModuleViewData());
		mod.DisplayName = LOCTEXT("Mod_EnergyShield", "ENERGY SHIELD T1");
		mod.Category = EOLCShipModuleCategory::Protection;
		mod.TIRTier = 2;
		mod.HullSlotsRequired = 1;
		mod.PowerConsumption = -3.0f; // produces 3 energy/tick
		mod.EffectDescription = LOCTEXT("Mod_EnergyShieldDesc", "Energy shield generator. Restores shield integrity.");
		mod.State = EOLCModuleState::Offline;
		mod.IntegrityPercent = 1.0f;
	}

	// --- Storage ---
	{
		FOLCShipModuleViewData& mod = AvailableModules.Add_GetRef(FOLCShipModuleViewData());
		mod.DisplayName = LOCTEXT("Mod_StandardStorage", "STANDARD STORAGE T1");
		mod.Category = EOLCShipModuleCategory::Storage;
		mod.TIRTier = 1;
		mod.HullSlotsRequired = 1;
		mod.PowerConsumption = 0.0f;
		mod.EffectDescription = LOCTEXT("Mod_StandardStorageDesc", "+200 storage per resource type.");
		mod.State = EOLCModuleState::Offline;
		mod.IntegrityPercent = 1.0f;
	}

	// --- Weapons ---
	{
		FOLCShipModuleViewData& mod = AvailableModules.Add_GetRef(FOLCShipModuleViewData());
		mod.DisplayName = LOCTEXT("Mod_BasicWeapons", "BASIC WEAPONS T1");
		mod.Category = EOLCShipModuleCategory::Weapons;
		mod.TIRTier = 1;
		mod.HullSlotsRequired = 1;
		mod.PowerConsumption = -1.0f; // produces 1 energy/tick
		mod.EffectDescription = LOCTEXT("Mod_BasicWeaponsDesc", "Basic defensive weapons.");
		mod.State = EOLCModuleState::Offline;
		mod.IntegrityPercent = 1.0f;
	}

	UE_LOG(LogTemp, Log, TEXT("[OLC] Ship modules populated: %d available"), AvailableModules.Num());
}

TArray<FOLCShipModuleViewData> UOLCUIDataSubsystem::GetInstalledModules(EOLCShipModuleCategory Category) const
{
	TArray<FOLCShipModuleViewData> Result;
	for (const auto& Mod : InstalledModules)
	{
		if (Mod.Category == Category)
		{
			Result.Add(Mod);
		}
	}
	return Result;
}

bool UOLCUIDataSubsystem::CanInstallModule(UOLCShipModuleData* ModuleData) const
{
	if (!ModuleData) return false;

	// Check TIR requirement
	if (ModuleData->TIRTier > ColonyTIR)
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Cannot install module — TIR requirement %d exceeds colony TIR %d"),
			ModuleData->TIRTier, ColonyTIR);
		return false;
	}

	return true;
}

void UOLCUIDataSubsystem::ApplyModuleEffects(UOLCShipModuleData* ModuleData)
{
	if (!ModuleData) return;

	switch (ModuleData->ModuleCategory)
	{
		case EOLCShipModuleCategory::Drives:
			SetDriveTier(ModuleData->TIRTier);
			SetDriveStatus(EOLCModuleState::Installed);
			UE_LOG(LogTemp, Log, TEXT("[OLC] Drive installed — tier %d"), ModuleData->TIRTier);
			break;

		case EOLCShipModuleCategory::Protection:
			if (ModuleData->PowerConsumption < 0.0f)
			{
				// Shield module
				SetShieldStatus(EOLCModuleState::Installed);
			}
			UE_LOG(LogTemp, Log, TEXT("[OLC] Protection module installed"));
			break;

		case EOLCShipModuleCategory::Storage:
			AddStorageBonus(200);
			UE_LOG(LogTemp, Log, TEXT("[OLC] Storage module installed — +200 per resource"));
			break;

		default:
			UE_LOG(LogTemp, Log, TEXT("[OLC] Module installed: %s"), *ModuleData->DisplayName.ToString());
			break;
	}
}

float UOLCUIDataSubsystem::CalculateRepairCost(EOLCShipModuleCategory Category) const
{
	switch (Category)
	{
		case EOLCShipModuleCategory::Drives:
			return 20.0f + 15.0f; // 20 CM + 15 Minerals
		default:
			return 0.0f;
	}
}

bool UOLCUIDataSubsystem::InstallModule(UOLCShipModuleData* ModuleData)
{
	if (!ModuleData || !CanInstallModule(ModuleData))
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] InstallModule: invalid or incompatible module"));
		return false;
	}

	// Check resources available (simplified — assume cost is in ModuleData.RepairCost)
	TArray<FOLCResourceAmount> Cost = ModuleData->RepairCost;
	if (!CanAffordBuild(Cost))
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] InstallModule: insufficient resources"));
		return false;
	}

	// Deduct cost
	ConsumeResourcesForBuild(Cost);

	// Apply module effects
	ApplyModuleEffects(ModuleData);

	// Add to installed modules
	FOLCShipModuleViewData InstalledMod;
	InstalledMod.DisplayName = ModuleData->DisplayName;
	InstalledMod.Category = ModuleData->ModuleCategory;
	InstalledMod.TIRTier = ModuleData->TIRTier;
	InstalledMod.HullSlotsRequired = ModuleData->HullSlotsRequired;
	InstalledMod.PowerConsumption = ModuleData->PowerConsumption;
	InstalledMod.EffectDescription = ModuleData->EffectDescription;
	InstalledMod.State = EOLCModuleState::Installed;
	InstalledMod.IntegrityPercent = 1.0f;
	InstalledModules.Add(InstalledMod);

	UE_LOG(LogTemp, Log, TEXT("[OLC] Module installed: %s"), *ModuleData->DisplayName.ToString());
	return true;
}

bool UOLCUIDataSubsystem::SwapModule(UOLCShipModuleData* NewModuleData)
{
	if (!NewModuleData || !CanInstallModule(NewModuleData))
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] SwapModule: invalid or incompatible module"));
		return false;
	}

	// Remove old module in same category and refund 50%
	for (int32 i = InstalledModules.Num() - 1; i >= 0; --i)
	{
		if (InstalledModules[i].Category == NewModuleData->ModuleCategory)
		{
			UE_LOG(LogTemp, Log, TEXT("[OLC] Swapped out module: %s"), *InstalledModules[i].DisplayName.ToString());
			InstalledModules.RemoveAt(i);
			break;
		}
	}

	// Install new module
	return InstallModule(NewModuleData);
}

bool UOLCUIDataSubsystem::RepairModule(EOLCShipModuleCategory Category)
{
	// Find damaged module in category
	for (auto& Mod : InstalledModules)
	{
		if (Mod.Category == Category && Mod.State == EOLCModuleState::Damaged)
		{
			float RepairCost = CalculateRepairCost(Category);
			if (RepairCost > 0.0f)
			{
				// Simplified: deduct 20 CM + 15 Minerals for drives
				AddResource(EOLCResourceType::ConstructionMaterial, -20.0f);
				AddResource(EOLCResourceType::Minerals, -15.0f);

				Mod.State = EOLCModuleState::Installed;
				Mod.IntegrityPercent = 1.0f;

				UE_LOG(LogTemp, Log, TEXT("[OLC] Module repaired: %s"), *Mod.DisplayName.ToString());
				return true;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[OLC] RepairModule: no damaged module found in category"));
	return false;
}

void UOLCUIDataSubsystem::SetColonyTIR(int32 InTIR)
{
	ColonyTIR = FMath::Clamp(InTIR, 1, 5);
	UE_LOG(LogTemp, Log, TEXT("[OLC] Colony TIR set to: %d"), ColonyTIR);
}

void UOLCUIDataSubsystem::UpdateModuleState(const FText& ModuleName, EOLCModuleState NewState, float NewIntegrity)
{
	for (auto& AvailMod : AvailableModules)
	{
		if (AvailMod.DisplayName.EqualTo(ModuleName))
		{
			AvailMod.State = NewState;
			AvailMod.IntegrityPercent = NewIntegrity;
			UE_LOG(LogTemp, Log, TEXT("[OLC] Updated module state: %s -> %d"), *ModuleName.ToString(), (int32)NewState);
			return;
		}
	}
}

void UOLCUIDataSubsystem::AddInstalledModule(const FOLCShipModuleViewData& Module)
{
	InstalledModules.Add(Module);
	UE_LOG(LogTemp, Log, TEXT("[OLC] Added installed module: %s"), *Module.DisplayName.ToString());
}

void UOLCUIDataSubsystem::RemoveInstalledModuleByCategory(EOLCShipModuleCategory Category)
{
	for (int32 i = InstalledModules.Num() - 1; i >= 0; --i)
	{
		if (InstalledModules[i].Category == Category)
		{
			UE_LOG(LogTemp, Log, TEXT("[OLC] Removed installed module: %s"), *InstalledModules[i].DisplayName.ToString());
			InstalledModules.RemoveAt(i);
			return;
		}
	}
}

TArray<FOLCResourceAmount> UOLCUIDataSubsystem::GetModuleRefund(const FOLCShipModuleViewData& Module) const
{
	TArray<FOLCResourceAmount> Refund;
	for (const auto& CostEntry : Module.RepairCost)
	{
		if (CostEntry.CurrentValue > 0.0f)
		{
			FOLCResourceAmount RefundEntry = CostEntry;
			RefundEntry.CurrentValue = CostEntry.CurrentValue * 0.5f; // 50% refund
			Refund.Add(RefundEntry);
		}
	}
	return Refund;
}

// ---------------------------------------------------------------------------
// Tutorial Objective Integration (WP-104 Step 6)
// ---------------------------------------------------------------------------

void UOLCUIDataSubsystem::OnBuildingPlaced(AActor* BuildingActor, UOLCBuildingData* BuildingData)
{
	if (!BuildingData || !BuildingActor)
		return;

	RegisterBuilding(BuildingActor, BuildingData);

	// Check if this is a Solar Array — complete "Build solar panel" objective (index 3)
	const FString BuildingName = BuildingData->DisplayName.ToString();
	if (BuildingName.Contains(TEXT("Solar")) || BuildingName.Contains(TEXT("Panel")))
	{
		bSolarArrayPlaced = true;

		if (LastCompletedObjectiveIndex < 3 && MissionObjectives.IsValidIndex(3))
		{
			MissionObjectives[3].State = EOLCProgressState::Complete;
			UE_LOG(LogTemp, Log, TEXT("[OLC] Tutorial: 'Build solar panel' objective completed"));

			if (MissionObjectives.IsValidIndex(4))
			{
				MissionObjectives[4].State = EOLCProgressState::Active;
				UE_LOG(LogTemp, Log, TEXT("[OLC] Tutorial: Activated next objective"));
			}
		}
	}

	// Check if this is a Mine — complete "Discover mineral deposit" objective (index 2)
	if (BuildingName.Contains(TEXT("Mine")))
	{
		if (LastCompletedObjectiveIndex < 2 && MissionObjectives.IsValidIndex(2))
		{
			MissionObjectives[2].State = EOLCProgressState::Complete;
			UE_LOG(LogTemp, Log, TEXT("[OLC] Tutorial: 'Discover mineral deposit' objective completed"));

			if (MissionObjectives.IsValidIndex(3))
			{
				MissionObjectives[3].State = EOLCProgressState::Active;
			}
		}
	}

	// Check if this is a Locker — activate "Collect construction material" objective
	if (BuildingName.Contains(TEXT("Locker")))
	{
		if (MissionObjectives.IsValidIndex(1) && MissionObjectives[1].State == EOLCProgressState::Idle)
		{
			MissionObjectives[1].State = EOLCProgressState::Active;
			UE_LOG(LogTemp, Log, TEXT("[OLC] Tutorial: Activated 'Collect construction material'"));
		}
	}
}

void UOLCUIDataSubsystem::UpdateTutorialProgress()
{
	if (MissionObjectives.IsEmpty())
		return;

	// Objective 0: "Explore crash site" — auto-complete on first tick
	if (MissionObjectives.IsValidIndex(0) && MissionObjectives[0].State == EOLCProgressState::Active)
	{
		MissionObjectives[0].Progress = 1.0f;
		MissionObjectives[0].State = EOLCProgressState::Complete;
		LastCompletedObjectiveIndex = 0;
		UE_LOG(LogTemp, Log, TEXT("[OLC] Tutorial: 'Explore crash site' completed"));

		if (MissionObjectives.IsValidIndex(1))
		{
			MissionObjectives[1].State = EOLCProgressState::Active;
		}
	}

	// Objective 1: "Collect construction material" — advance when CM > 0
	if (MissionObjectives.IsValidIndex(1) && MissionObjectives[1].State == EOLCProgressState::Active)
	{
		for (const auto& Res : ResourceCounters)
		{
			if (Res.ResourceType == EOLCResourceType::ConstructionMaterial && Res.Value >= 1.0f)
			{
				MissionObjectives[1].Progress = 1.0f;
				MissionObjectives[1].State = EOLCProgressState::Complete;
				LastCompletedObjectiveIndex = 1;
				UE_LOG(LogTemp, Log, TEXT("[OLC] Tutorial: 'Collect construction material' completed"));

				if (MissionObjectives.IsValidIndex(2))
				{
					MissionObjectives[2].State = EOLCProgressState::Active;
				}
				break;
			}
		}
	}

	// Objective 4: "Reach 10 Energy production" — track cumulative energy per tick
	if (MissionObjectives.IsValidIndex(4) && MissionObjectives[4].State == EOLCProgressState::Active)
	{
		CumulativeEnergyForTutorial += CumulativeEnergyProduced;

		if (CumulativeEnergyForTutorial >= 10.0f)
		{
			MissionObjectives[4].Progress = 1.0f;
			MissionObjectives[4].State = EOLCProgressState::Complete;
			LastCompletedObjectiveIndex = 4;
			UE_LOG(LogTemp, Log, TEXT("[OLC] Tutorial: 'Reach 10 Energy production' completed (cumulative: %.1f)"), CumulativeEnergyForTutorial);
		}
		else
		{
			MissionObjectives[4].Progress = FMath::Min(1.0f, CumulativeEnergyForTutorial / 10.0f);
		}
	}

	// Reset cumulative energy for next tick
	CumulativeEnergyProduced = 0.0f;
}

// ---------------------------------------------------------------------------
// Production Visualization (WP-104 Step 7)
// ---------------------------------------------------------------------------

void UOLCUIDataSubsystem::UpdateBuildingVisualization()
{
	// Clear existing building markers from minimap
	TArray<FOLCMinimapMarkerViewData> NewMarkers = MinimapMarkers;
	for (int32 i = NewMarkers.Num() - 1; i >= 0; --i)
	{
		if (NewMarkers[i].TooltipText.ToString().Contains(TEXT("Building")))
		{
			NewMarkers.RemoveAt(i);
		}
	}

	// Add a marker for each registered building
	for (const auto& Entry : RegisteredBuildings)
	{
		if (!Entry.BuildingData || !Entry.Actor.IsValid())
			continue;

		FOLCMinimapMarkerViewData Marker;
		Marker.bVisible = true;
		Marker.bDiscovered = true;
		Marker.MarkerType = EOLCMinimapMarkerType::Ally;

		const FString BuildingName = Entry.BuildingData->DisplayName.ToString();
		Marker.TooltipText = FText::FromString(FString::Printf(TEXT("Building: %s | Power: %s"),
			*BuildingName,
			*FString::SanitizeFloat(Entry.BuildingData->PowerConsumption)));

		// Determine production state — green when producing, red when power-deficit
		EOLCColorRole ColorRole = bPowerDeficit ? EOLCColorRole::Danger : EOLCColorRole::Success;

		// Position buildings in a grid pattern on the minimap
		int32 Index = RegisteredBuildings.IndexOfByPredicate([&Entry](const FRegisteredBuildingEntry& Other) {
			return Other.Actor.Get() == Entry.Actor.Get();
		});
		if (Index != INDEX_NONE)
		{
			float GridSize = FMath::Sqrt(static_cast<float>(RegisteredBuildings.Num()));
			int32 Row = Index / static_cast<int32>(GridSize);
			int32 Col = Index % static_cast<int32>(GridSize);
			Marker.Position = FVector2D(
				0.1f + (Col * 0.08f),
				0.1f + (Row * 0.08f)
			);
		}
		else
		{
			Marker.Position = FVector2D(0.5f, 0.5f); // Center fallback
		}

		Marker.ColorRole = ColorRole;
		NewMarkers.Add(Marker);
	}

	MinimapMarkers = NewMarkers;
}

bool UOLCUIDataSubsystem::GetBuildingProductionState(AActor* BuildingActor, bool& bIsProducing, bool& bHasPowerDeficit) const
{
	bHasPowerDeficit = bPowerDeficit;

	if (!BuildingActor)
	{
		bIsProducing = false;
		return false;
	}

	for (const auto& Entry : RegisteredBuildings)
	{
		if (Entry.Actor.Get() == BuildingActor)
		{
			bIsProducing = !bPowerDeficit && Entry.BuildingData != nullptr;
			return true;
		}
	}

	bIsProducing = false;
	return false;
}

#undef LOCTEXT_NAMESPACE

#!/usr/bin/env python3
"""
create_building_dataassets.py

UE5.8 Python script to create the 6 starter building DataAssets.

Run from the Unreal Editor Python console or with:

UnrealEditor-Cmd.exe Project.uproject -ExecCmds="py exec(open('script.py').read()), quit"

Creates UOLCBuildingData assets for the 6 starter buildings.
"""

import unreal


# ---------------------------------------------------------------------------
# Building definitions
# ---------------------------------------------------------------------------

BUILDINGS = [
    {
        "name": "DA_Building_SolarArray",
        "display_name": "Solar Array",
        "category": 0,
        "grid_size": (2.0, 2.0),
        "build_cost": [
            ("Energy", 30.0),
            ("ConstructionMaterial", 20.0),
        ],
        "power_consumption": -5.0,
        "output_per_tick": [],
        "tir_requirement": 1,
        "description": (
            "Generates energy from sunlight. "
            "Basic power source for base operations."
        ),
    },
    {
        "name": "DA_Building_CampBarracks",
        "display_name": "Camp Barracks",
        "category": 2,
        "grid_size": (2.0, 2.0),
        "build_cost": [
            ("ConstructionMaterial", 40.0),
            ("Minerals", 20.0),
        ],
        "power_consumption": 3.0,
        "output_per_tick": [],
        "tir_requirement": 1,
        "description": "Unit training facility. +8 unit capacity.",
    },
    {
        "name": "DA_Building_HabitationModule",
        "display_name": "Habitation Module",
        "category": 2,
        "grid_size": (2.0, 2.0),
        "build_cost": [
            ("ConstructionMaterial", 50.0),
            ("Minerals", 30.0),
        ],
        "power_consumption": 2.0,
        "output_per_tick": [],
        "tir_requirement": 1,
        "description": "Worker housing module. +16 unit capacity.",
    },
    {
        "name": "DA_Building_WallSegment",
        "display_name": "Wall Segment",
        "category": 2,
        "grid_size": (1.0, 1.0),
        "build_cost": [
            ("ConstructionMaterial", 10.0),
        ],
        "power_consumption": 0.0,
        "output_per_tick": [],
        "tir_requirement": 1,
        "description": (
            "Passive defensive wall segment. "
            "+5% defense bonus to adjacent buildings."
        ),
    },
    {
        "name": "DA_Building_Gate",
        "display_name": "Gate",
        "category": 2,
        "grid_size": (2.0, 1.0),
        "build_cost": [
            ("ConstructionMaterial", 20.0),
            ("Minerals", 10.0),
        ],
        "power_consumption": 1.0,
        "output_per_tick": [],
        "tir_requirement": 1,
        "description": (
            "Controlled entry point for wall perimeters. "
            "Powered to function."
        ),
    },
    {
        "name": "DA_Building_Locker",
        "display_name": "Locker",
        "category": 3,
        "grid_size": (1.0, 1.0),
        "build_cost": [
            ("ConstructionMaterial", 15.0),
        ],
        "power_consumption": 0.0,
        "output_per_tick": [],
        "tir_requirement": 1,
        "description": (
            "Basic personal storage locker. "
            "+200 storage per resource type."
        ),
    },
    {
        "name": "DA_Building_Mine",
        "display_name": "Mine",
        "category": 1,
        "grid_size": (2.0, 2.0),
        "build_cost": [
            ("ConstructionMaterial", 40.0),
            ("Minerals", 30.0),
        ],
        "power_consumption": 5.0,
        "output_per_tick": [("Minerals", 8.0)],
        "tir_requirement": 1,
        "description": (
            "Extracts minerals from underground deposits. "
            "Requires power connection."
        ),
        "biome_modifiers": [
            ("Rocky", 1.25),
            ("Desert", 0.75),
        ],
    },
]


# ---------------------------------------------------------------------------
# Resource type enum mapping
# EOLCResourceType
# ---------------------------------------------------------------------------

RESOURCE_TYPE_MAP = {
    "Energy": 0,
    "Fuel": 1,
    "ConstructionMaterial": 2,
    "Minerals": 3,
    "HullParts": 4,
    "Survival": 5,
    "DarkMatterCrystals": 6,
}


# ---------------------------------------------------------------------------
# Biome type enum mapping (EOLCBiomeType)
# ---------------------------------------------------------------------------

BIOME_TYPE_MAP = {
    "Desert": 0,
    "Dusty": 1,
    "Rocky": 2,
    "Water": 3,
    "Swamp": 4,
    "Jungle": 5,
    "LightSnow": 6,
    "Ice": 7,
}


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def set_property(obj, property_name, value):
    """
    Set an editor property and provide a useful error if the C++ property
    does not exist or has a different name.
    """
    try:
        obj.set_editor_property(property_name, value)
    except Exception as exc:
        unreal.log_error(
            f"Failed setting '{property_name}' on "
            f"{obj.get_name()}: {exc}"
        )
        raise


# ---------------------------------------------------------------------------
# Create assets
# ---------------------------------------------------------------------------

def create_building_dataassets():

    unreal.log("==========================================")
    unreal.log("Creating starter building DataAssets")
    unreal.log("==========================================")

    # Get AssetTools.
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()

    # Load the native UOLCBuildingData UClass.
    building_data_class = unreal.load_class(
        None,
        "/Script/OurLastChance.OLCBuildingData"
    )

    if not building_data_class:
        unreal.log_error(
            "Could not find UOLCBuildingData. "
            "Make sure the C++ project has compiled successfully."
        )
        return

    unreal.log(
        f"Found building class: {building_data_class.get_name()}"
    )

    # DataAssetFactory is the standard UE factory for UDataAsset classes.
    factory = unreal.DataAssetFactory()

    # Explicitly tell the factory which DataAsset class to instantiate.
    try:
        factory.set_editor_property(
            "data_asset_class",
            building_data_class
        )
    except Exception as exc:
        unreal.log_error(
            f"Could not configure DataAssetFactory: {exc}"
        )
        return

    save_path = "/Game/OurLastChance/Data/Buildings"

    # -----------------------------------------------------------------------
    # Create each building
    # -----------------------------------------------------------------------

    for bldg in BUILDINGS:

        asset_name = bldg["name"]

        unreal.log(f"Processing {asset_name}...")

        # Check whether asset already exists.
        full_path = f"{save_path}/{asset_name}"

        existing = unreal.load_asset(full_path)

        if existing:
            unreal.log_warning(
                f"{asset_name} already exists - skipping"
            )
            continue

        # UE5.8 create_asset() returns the created UObject directly.
        da = asset_tools.create_asset(
            asset_name,
            save_path,
            building_data_class,
            factory
        )

        if not da:
            unreal.log_error(
                f"Failed to create {asset_name}"
            )
            continue

        unreal.log(
            f"Created UObject: {da.get_name()}"
        )

        # -------------------------------------------------------------------
        # Basic properties
        # -------------------------------------------------------------------

        set_property(
            da,
            "display_name",
            unreal.Text(bldg["display_name"])
        )

        set_property(
            da,
            "category",
            bldg["category"]
        )

        set_property(
            da,
            "grid_size",
            unreal.Vector2D(
                bldg["grid_size"][0],
                bldg["grid_size"][1]
            )
        )

        set_property(
            da,
            "power_consumption",
            bldg["power_consumption"]
        )

        set_property(
            da,
            "tir_requirement",
            bldg["tir_requirement"]
        )

        set_property(
            da,
            "description",
            unreal.Text(bldg["description"])
        )

        # -------------------------------------------------------------------
        # Build cost
        # -------------------------------------------------------------------

        build_cost = []

        for res_name, amount in bldg["build_cost"]:

            if res_name not in RESOURCE_TYPE_MAP:
                unreal.log_error(
                    f"Unknown resource type: {res_name}"
                )
                continue

            resource_amount = unreal.FOLCResourceAmount()

            set_property(
                resource_amount,
                "resource_type",
                RESOURCE_TYPE_MAP[res_name]
            )

            set_property(
                resource_amount,
                "current_value",
                amount
            )

            set_property(
                resource_amount,
                "capacity",
                amount
            )

            build_cost.append(resource_amount)

        set_property(
            da,
            "build_cost",
            build_cost
        )

        # -------------------------------------------------------------------
        # Biome modifiers (optional)
        # -------------------------------------------------------------------

        biome_modifiers = []

        if "biome_modifiers" in bldg:
            for biome_name, multiplier in bldg["biome_modifiers"]:
                biome_mod = unreal.FOLCBiomeModifier()

                if biome_name not in BIOME_TYPE_MAP:
                    unreal.log_error(
                        f"Unknown biome type: {biome_name}"
                    )
                    continue

                set_property(
                    biome_mod,
                    "biome_type",
                    BIOME_TYPE_MAP[biome_name]
                )

                set_property(
                    biome_mod,
                    "multiplier",
                    multiplier
                )

                biome_modifiers.append(biome_mod)

        set_property(
            da,
            "biome_modifiers",
            biome_modifiers
        )

        # -------------------------------------------------------------------
        # Output per tick
        # -------------------------------------------------------------------

        set_property(
            da,
            "output_per_tick",
            []
        )

        # -------------------------------------------------------------------
        # Save
        # -------------------------------------------------------------------

        da.mark_package_dirty()

        saved = unreal.EditorAssetLibrary.save_loaded_asset(da)

        if saved:
            unreal.log(
                f"[OK] Created and saved {asset_name}"
            )
        else:
            unreal.log_error(
                f"Asset was created but could not be saved: "
                f"{asset_name}"
            )

    unreal.log("==========================================")
    unreal.log("Done! Starter building DataAssets processed.")
    unreal.log("==========================================")


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    create_building_dataassets()

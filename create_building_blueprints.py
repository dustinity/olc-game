#!/usr/bin/env python3
"""
create_building_blueprints.py

UE5.8 Python script to create building Blueprint assets.

Creates:
    BP_BuildingBase
    BP_SolarArray
    BP_CampBarracks
    BP_HabitationModule
    BP_WallSegment
    BP_Gate
    BP_Locker

Run from the Unreal Editor Python console:

    exec(open(r"C:/Path/To/create_building_blueprints.py").read())

Or with UnrealEditor-Cmd:

    UnrealEditor-Cmd.exe Project.uproject ^
        -ExecutePythonScript="C:/Path/To/create_building_blueprints.py"
"""

import unreal


# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

SAVE_PATH = "/Game/OurLastChance/Buildings"

BUILDING_DATA_PATH = "/Game/OurLastChance/Data/Buildings"


BASE_BLUEPRINTS = [
    {
        "name": "BP_BuildingBase",
        "parent_class": "/Script/OurLastChance.OLCBuildingBase",
    },
]


BUILDING_BLUEPRINTS = [
    {
        "name": "BP_SolarArray",
        "parent_class": "/Script/OurLastChance.OLCPowerGenerator",
        "data_asset": (
            f"{BUILDING_DATA_PATH}/"
            "DA_Building_SolarArray.DA_Building_SolarArray"
        ),
        "mesh_type": "box_cylinder",
    },
    {
        "name": "BP_CampBarracks",
        "parent_class": "/Script/OurLastChance.OLCInfrastructure",
        "data_asset": (
            f"{BUILDING_DATA_PATH}/"
            "DA_Building_CampBarracks.DA_Building_CampBarracks"
        ),
        "mesh_type": "box",
    },
    {
        "name": "BP_HabitationModule",
        "parent_class": "/Script/OurLastChance.OLCInfrastructure",
        "data_asset": (
            f"{BUILDING_DATA_PATH}/"
            "DA_Building_HabitationModule.DA_Building_HabitationModule"
        ),
        "mesh_type": "box_large",
    },
    {
        "name": "BP_WallSegment",
        "parent_class": "/Script/OurLastChance.OLCDefenseStructure",
        "data_asset": (
            f"{BUILDING_DATA_PATH}/"
            "DA_Building_WallSegment.DA_Building_WallSegment"
        ),
        "mesh_type": "box_flat",
    },
    {
        "name": "BP_Gate",
        "parent_class": "/Script/OurLastChance.OLCInfrastructure",
        "data_asset": (
            f"{BUILDING_DATA_PATH}/"
            "DA_Building_Gate.DA_Building_Gate"
        ),
        "mesh_type": "box_gate",
    },
    {
        "name": "BP_Locker",
        "parent_class": "/Script/OurLastChance.OLCBuildingBase",
        "data_asset": (
            f"{BUILDING_DATA_PATH}/"
            "DA_Building_Locker.DA_Building_Locker"
        ),
        "mesh_type": "box_small",
    },
    {
        "name": "BP_Mine",
        "parent_class": "/Script/OurLastChance.OLCResourceExtractor",
        "data_asset": (
            f"{BUILDING_DATA_PATH}/"
            "DA_Building_Mine.DA_Building_Mine"
        ),
        "mesh_type": "cylinder",
    },
]


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def load_class(class_path):
    """Load a native Unreal class."""

    cls = unreal.load_class(None, class_path)

    if not cls:
        unreal.log_error(
            f"Could not load class: {class_path}"
        )

    return cls


def asset_exists(asset_path):
    """Return True if an asset exists at the supplied package path."""

    return unreal.EditorAssetLibrary.does_asset_exist(asset_path)


def create_blueprint(asset_tools, name, parent_class):
    """
    Create a Blueprint asset using the UE5.8 BlueprintFactory API.
    """

    factory = unreal.BlueprintFactory()

    factory.set_editor_property(
        "parent_class",
        parent_class
    )

    bp = asset_tools.create_asset(
        name,
        SAVE_PATH,
        unreal.Blueprint,
        factory
    )

    return bp


def get_generated_class(bp):
    """
    Get the generated UObject class from a UBlueprint.
    """

    generated_class = None

    try:
        generated_class = unreal.BlueprintEditorLibrary.generated_class(bp)
    except Exception:
        pass

    if not generated_class:
        unreal.log_error(
            f"Could not get generated class for {bp.get_name()}"
        )

    return generated_class


def get_default_object(bp):
    """
    Get the Blueprint Class Default Object.

    This is where Blueprint-exposed properties such as BuildingData
    can be assigned.
    """

    generated_class = get_generated_class(bp)

    if not generated_class:
        return None

    cdo = unreal.get_default_object(generated_class)

    if not cdo:
        unreal.log_error(
            f"Could not get CDO for {bp.get_name()}"
        )

    return cdo


def set_property_if_exists(obj, property_name, value):
    """
    Safely assign an editor property.

    Returns True if successful.
    """

    try:
        obj.set_editor_property(
            property_name,
            value
        )

        return True

    except Exception as exc:

        unreal.log_warning(
            f"Could not set '{property_name}' on "
            f"{obj.get_name()}: {exc}"
        )

        return False


# ---------------------------------------------------------------------------
# Create one Blueprint
# ---------------------------------------------------------------------------

def create_building_blueprint(
    asset_tools,
    bp_def,
    assign_data_asset=False
):

    name = bp_def["name"]

    asset_path = f"{SAVE_PATH}/{name}"

    # ---------------------------------------------------------------
    # Existing asset
    # ---------------------------------------------------------------

    if asset_exists(asset_path):

        unreal.log_warning(
            f"{name} already exists - skipping"
        )

        return None

    # ---------------------------------------------------------------
    # Parent class
    # ---------------------------------------------------------------

    parent_class = load_class(
        bp_def["parent_class"]
    )

    if not parent_class:
        unreal.log_error(
            f"Cannot create {name}: "
            f"parent class could not be loaded."
        )

        return None

    unreal.log(
        f"Creating {name} "
        f"(parent: {parent_class.get_name()})"
    )

    # ---------------------------------------------------------------
    # Blueprint factory
    # ---------------------------------------------------------------

    bp = create_blueprint(
        asset_tools,
        name,
        parent_class
    )

    if not bp:

        unreal.log_error(
            f"Failed to create Blueprint: {name}"
        )

        return None

    # ---------------------------------------------------------------
    # Assign DataAsset
    # ---------------------------------------------------------------

    if assign_data_asset:

        data_asset_path = bp_def.get("data_asset")

        if data_asset_path:

            data_asset = unreal.load_asset(
                data_asset_path
            )

            if not data_asset:

                unreal.log_error(
                    f"Could not load DataAsset: "
                    f"{data_asset_path}"
                )

            else:

                cdo = get_default_object(bp)

                if cdo:

                    success = set_property_if_exists(
                        cdo,
                        "building_data",
                        data_asset
                    )

                    if success:

                        unreal.log(
                            f"Assigned BuildingData to {name}"
                        )

    # ---------------------------------------------------------------
    # Compile Blueprint
    # ---------------------------------------------------------------

    try:

        unreal.BlueprintEditorLibrary.compile_blueprint(
            bp
        )

    except Exception as exc:

        unreal.log_warning(
            f"Blueprint compilation warning for "
            f"{name}: {exc}"
        )

    # ---------------------------------------------------------------
    # Save
    # ---------------------------------------------------------------

    saved = unreal.EditorAssetLibrary.save_asset(
        asset_path
    )

    if saved:

        unreal.log(
            f"[OK] Created {name}"
        )

    else:

        unreal.log_error(
            f"Created {name}, but failed to save it."
        )

    return bp


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def create_building_blueprints():

    unreal.log("==========================================")
    unreal.log("Creating building Blueprints")
    unreal.log("==========================================")

    # ---------------------------------------------------------------
    # AssetTools
    # ---------------------------------------------------------------

    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()

    if not asset_tools:

        unreal.log_error(
            "Could not obtain AssetTools."
        )

        return

    # ---------------------------------------------------------------
    # Create base Blueprint
    # ---------------------------------------------------------------

    for bp_def in BASE_BLUEPRINTS:

        create_building_blueprint(
            asset_tools,
            bp_def,
            assign_data_asset=False
        )

    # ---------------------------------------------------------------
    # Create building Blueprints
    # ---------------------------------------------------------------

    for bp_def in BUILDING_BLUEPRINTS:

        create_building_blueprint(
            asset_tools,
            bp_def,
            assign_data_asset=False
        )

    unreal.log("==========================================")
    unreal.log("Done!")
    unreal.log("==========================================")


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    create_building_blueprints()

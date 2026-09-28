#!/usr/bin/env python3
"""
create_dataset.py

UE5.8 Python script to create building DataAssets and Blueprint assets.

Creates:
    7 UOLCBuildingData assets (Solar Array, Camp Barracks, Habitation Module,
    Wall Segment, Gate, Locker, Mine)
    8 Blueprints (BP_BuildingBase + 7 building-specific blueprints)

Run from the Unreal Editor Python console:

    exec(open(r"C:/Path/To/create_dataset.py").read())

Or with UnrealEditor-Cmd:

    UnrealEditor-Cmd.exe Project.uproject ^
        -ExecutePythonScript="C:/Path/To/create_dataset.py"

Data files live in Scripts/Building/data/.
"""

import json
import os

import unreal


# ---------------------------------------------------------------------------
# Paths
# ---------------------------------------------------------------------------

_SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
_DATA_DIR = os.path.join(_SCRIPT_DIR, "data")

SAVE_PATH = "/Game/OurLastChance/Buildings"
DATA_SAVE_PATH = "/Game/OurLastChance/Data/Buildings"

BUILDINGS_JSON = os.path.join(_DATA_DIR, "buildings.json")
BLUEPRINTS_JSON = os.path.join(_DATA_DIR, "blueprints.json")


# ---------------------------------------------------------------------------
# Enum maps  (sync with C++ EOLCResourceType / EOLCBiomeType)
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

def _load_json(path):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


def _set_prop(obj, name, value):
    """Set an editor property; log a warning on failure instead of crashing."""
    try:
        obj.set_editor_property(name, value)
        return True
    except Exception as exc:
        unreal.log_warning(
            f"Could not set '{name}' on {obj.get_name()}: {exc}"
        )
        return False


def _load_class(class_path):
    cls = unreal.load_class(None, class_path)
    if not cls:
        unreal.log_error(f"Could not load class: {class_path}")
    return cls


def _get_cdo(bp):
    """Return the CDO of a Blueprint asset."""
    try:
        generated = unreal.BlueprintEditorLibrary.generated_class(bp)
    except Exception:
        generated = None

    if not generated:
        unreal.log_error(f"Could not get generated class for {bp.get_name()}")
        return None

    cdo = unreal.get_default_object(generated)
    if not cdo:
        unreal.log_error(f"Could not get CDO for {bp.get_name()}")
    return cdo


def _resolve_resource(name):
    idx = RESOURCE_TYPE_MAP.get(name)
    if idx is None:
        unreal.log_error(f"Unknown resource type: {name}")
        return None
    return idx


def _resolve_biome(name):
    idx = BIOME_TYPE_MAP.get(name)
    if idx is None:
        unreal.log_error(f"Unknown biome type: {name}")
        return None
    return idx


# ---------------------------------------------------------------------------
# DataAsset creation
# ---------------------------------------------------------------------------

def create_data_assets(buildings, asset_tools, building_data_class):
    """Create UOLCBuildingData assets from the buildings JSON list."""

    factory = unreal.DataAssetFactory()
    factory.set_editor_property("data_asset_class", building_data_class)

    for bldg in buildings:
        name = bldg["name"]
        path = f"{DATA_SAVE_PATH}/{name}"

        if unreal.EditorAssetLibrary.does_asset_exist(path):
            unreal.log_warning(f"{name} already exists — skipping")
            continue

        unreal.log(f"Creating DataAsset {name} ...")

        da = asset_tools.create_asset(
            name, DATA_SAVE_PATH, building_data_class, factory
        )
        if not da:
            unreal.log_error(f"Failed to create {name}")
            continue

        # Basic scalar / text properties
        _set_prop(da, "display_name", unreal.Text(bldg["display_name"]))
        _set_prop(da, "category", bldg["category"])
        _set_prop(
            da, "grid_size",
            unreal.Vector2D(*bldg["grid_size"])
        )
        _set_prop(da, "power_consumption", bldg["power_consumption"])
        _set_prop(da, "tir_requirement", bldg["tir_requirement"])
        _set_prop(da, "description", unreal.Text(bldg["description"]))

        # Build cost → array of FOLCResourceAmount structs
        cost_items = []
        for item in bldg.get("build_cost", []):
            res_idx = _resolve_resource(item["resource_type"])
            if res_idx is None:
                continue

            ra = unreal.FOLCResourceAmount()
            _set_prop(ra, "resource_type", res_idx)
            _set_prop(ra, "current_value", item["amount"])
            _set_prop(ra, "capacity", item["amount"])
            cost_items.append(ra)

        _set_prop(da, "build_cost", cost_items)

        # Biome modifiers (optional)
        biome_items = []
        for entry in bldg.get("biome_modifiers", []):
            b_idx = _resolve_biome(entry["biome_type"])
            if b_idx is None:
                continue

            bm = unreal.FOLCBiomeModifier()
            _set_prop(bm, "biome_type", b_idx)
            _set_prop(bm, "multiplier", entry["multiplier"])
            biome_items.append(bm)

        _set_prop(da, "biome_modifiers", biome_items)

        # Output per tick (empty array for now; populated later if needed)
        _set_prop(da, "output_per_tick", [])

        da.mark_package_dirty()
        saved = unreal.EditorAssetLibrary.save_loaded_asset(da)

        if saved:
            unreal.log(f"[OK] DataAsset {name}")
        else:
            unreal.log_error(f"Failed to save DataAsset {name}")


# ---------------------------------------------------------------------------
# Blueprint creation
# ---------------------------------------------------------------------------

def create_blueprints(blueprints, buildings_map, asset_tools):
    """Create Blueprint assets and wire in their BuildingData references."""

    for bp_def in blueprints:
        name = bp_def["name"]
        path = f"{SAVE_PATH}/{name}"

        if unreal.EditorAssetLibrary.does_asset_exist(path):
            unreal.log_warning(f"{name} already exists — skipping")
            continue

        parent_class = _load_class(bp_def["parent_class"])
        if not parent_class:
            unreal.log_error(f"Cannot create {name}: parent class missing")
            continue

        unreal.log(f"Creating Blueprint {name} ...")

        factory = unreal.BlueprintFactory()
        factory.set_editor_property("parent_class", parent_class)

        bp = asset_tools.create_asset(
            name, SAVE_PATH, unreal.Blueprint, factory
        )
        if not bp:
            unreal.log_error(f"Failed to create Blueprint {name}")
            continue

        # If this blueprint references a DataAsset, wire it up on the CDO.
        data_asset_ref = bp_def.get("data_asset")
        if data_asset_ref and data_asset_ref in buildings_map:
            cdo = _get_cdo(bp)
            if cdo:
                da_path = f"{DATA_SAVE_PATH}/{data_asset_ref}"
                da = unreal.load_asset(da_path)
                if da:
                    _set_prop(cdo, "building_data", da)
                    unreal.log(f"  Assigned BuildingData ← {data_asset_ref}")
                else:
                    unreal.log_error(
                        f"DataAsset not found at {da_path}"
                    )

        # Compile & save
        try:
            unreal.BlueprintEditorLibrary.compile_blueprint(bp)
        except Exception as exc:
            unreal.log_warning(f"Compile warning for {name}: {exc}")

        saved = unreal.EditorAssetLibrary.save_asset(path)
        if saved:
            unreal.log(f"[OK] Blueprint {name}")
        else:
            unreal.log_error(f"Failed to save Blueprint {name}")


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    unreal.log("=" * 50)
    unreal.log("Creating building DataAssets and Blueprints")
    unreal.log("=" * 50)

    # Asset tools
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    if not asset_tools:
        unreal.log_error("Could not obtain AssetTools.")
        return

    # Load C++ class
    building_data_class = _load_class(
        "/Script/OurLastChance.OLCBuildingData"
    )
    if not building_data_class:
        unreal.log_error(
            "Could not find UOLCBuildingData. "
            "Compile the C++ project first."
        )
        return

    # Load JSON data
    buildings = _load_json(BUILDINGS_JSON)
    blueprints = _load_json(BLUEPRINTS_JSON)

    # Build a lookup: name → raw definition (for Blueprint wiring)
    buildings_map = {b["name"]: b for b in buildings}

    # Step 1 — create DataAssets
    create_data_assets(buildings, asset_tools, building_data_class)

    # Step 2 — create Blueprints and wire BuildingData references
    create_blueprints(blueprints, buildings_map, asset_tools)

    unreal.log("=" * 50)
    unreal.log("Done!")
    unreal.log("=" * 50)


if __name__ == "__main__":
    main()

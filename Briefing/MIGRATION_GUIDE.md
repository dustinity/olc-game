# Briefing Migration Guide

> **Do NOT delete existing files.** The current flat-file structure in `Briefing/README.md` is the master navigation and stays as-is. New per-entity folders are ADDED alongside it.

## Relationship to Briefing/README.md

`Briefing/README.md` is the canonical index linking to all game systems. It remains unchanged. The new folder-per-entity structure supplements it with UE5 integration tracking.

## New Structure (Added Alongside Existing)

### Buildings: `Briefing/buildings/BUILDING_ID/`
- BUILDING_ID.md (extracted from `buildings/planet_buildings.md`)
- UE5.md (integration status + checklist)
- *.png (reference images)

Example: `Briefing/buildings/PB-EX-01-Mine/`

### Biomes: `Briefing/Planets/Biomes/BIOME_NAME/`
- BIOME_NAME.md (extracted from `planet/planet_types.md`)
- UE5.md (integration status + checklist)

Example: `Briefing/Planets/Biomes/Desert/`

## Existing Files (Keep as Master Reference)
All files linked from [Briefing/README.md](./README.md) remain the authoritative game design source:
- `planet/planet_types.md` - All biomes + bosses + TIR progression
- `buildings/planet_buildings.md` - All buildings by tier
- `resources/types.md` - Energy, Fuel, Construction, Minerals, Hull, Survival
- `factions/factions.md`, `champions.md`, `races.md`
- `units/units.md`, `upgrade_system.md`
- `weapons/weapon_types.md`, `tir_system.md`
- `tech_tree/overview.md`, `research_categories.md`
- `spaceship/dropship.md`, `progression.md`
- `gameplay/core_loop.md`, `dungeons.md`, `space_travel.md`, `attraction_system.md`

## Migration Steps
1. Extract entry from old flat file → new BUILDING_ID.md
2. Create UE5.md (template in Knowledge/HOWTO_ADD_BUILDING.md)
3. Add reference images from Assets/
4. Update Knowledge/INTEGRATION_STATUS.md

## Building Doc Template (Preserve ALL Fields!)

When extracting a building, include EVERY field:

```markdown
# Name (ID)

## Overview
One-line description and unlock method.

## Specs
- **Grid Size:** ...
- **Cost:** ...
- **Power:** ...
- **Output:** ...
- **Biome Compatibility:** ...

## Asset Files
- [filename.png](path) — description
- **Asset Format:** dimensions, format details

## Visual Design
Full visual description from source.

## Variants
List all variants.

## Notes
All gameplay notes, mechanics, interactions.

## Source
Extracted from `buildings/planet_buildings.md`
```

**Do NOT skip:** Asset Files, Asset Format, Visual Design, Notes — critical for UE5 implementation.

## Building Doc Template (Preserve ALL Fields!)

When extracting a building, include EVERY field:

```markdown
# Name (ID)

## Overview
One-line description and unlock method.

## Specs
- **Grid Size:** ...
- **Cost:** ...
- **Power:** ...
- **Output:** ...
- **Biome Compatibility:** ...

## Asset Files
- [filename.png](path) — description
- **Asset Format:** dimensions, format details

## Visual Design
Full visual description from source.

## Variants
List all variants.

## Notes
All gameplay notes, mechanics, interactions.

## Source
Extracted from `buildings/planet_buildings.md`
```

**Do NOT skip:** Asset Files, Asset Format, Visual Design, Notes — critical for UE5 implementation.

## UE5 Workflow
```
Briefing/ID/ID.md (WHAT)
  → Knowledge/HOWTO_XXXX.md (HOW)
  → Implement C++ + DataAssets + Blueprints
  → Briefing/ID/UE5.md (status)
  → INTEGRATION_STATUS.md
  → Play mode test → DONE
```

## Status: NOT_STARTED → IN_PROGRESS → TESTING → DONE | FAILED

</parameter</parameter>
<task_progress>
- [x] Analyze current project structure
- [x] Review existing Briefing/ and UE5 structure
- [x] Design new documentation framework
- [x] Create Knowledge/ folder (all files)
- [x] Create INTEGRATION_STATUS.md
- [x] Create example building folder (PB-EX-01-Mine)
- [x] Create example biome folder (Desert)
- [x] Create Briefing migration README
</task_progress>
</write_to_file><write_to_file>
<path>Briefing/MIGRATION_GUIDE.md</path>
<content>
# Briefing Migration Guide

## New Structure

### Buildings: `Briefing/buildings/BUILDING_ID/`
- BUILDING_ID.md (game design)
- UE5.md (integration status + checklist)
- *.png (reference images)

Example: `Briefing/buildings/PB-EX-01-Mine/`

### Biomes: `Briefing/Planets/Biomes/BIOME_NAME/`
- BIOME_NAME.md (game design)
- UE5.md (integration status + checklist)

Example: `Briefing/Planets/Biomes/Desert/`

## Old Structure (Legacy - DO NOT EDIT)
- `buildings/planet_buildings.md` - all buildings in one file
- `planet/planet_types.md` - all biomes in one file

## Migration Steps
1. Extract entry from old flat file → new BUILDING_ID.md
2. Create UE5.md (template in Knowledge/HOWTO_ADD_BUILDING.md)
3. Add reference images from Assets/
4. Update Knowledge/INTEGRATION_STATUS.md

## UE5 Workflow
```
Briefing/ID/ID.md (WHAT)
  → Knowledge/HOWTO_XXXX.md (HOW)
  → Implement C++ + DataAssets + Blueprints
  → Briefing/ID/UE5.md (status)
  → INTEGRATION_STATUS.md
  → Play mode test → DONE
```

## Status: NOT_STARTED → IN_PROGRESS → TESTING → DONE | FAILED

</parameter
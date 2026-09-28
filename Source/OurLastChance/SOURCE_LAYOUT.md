# Our Last Chance Source Layout

This module follows a small feature-oriented structure inspired by Epic's asset prefix guidance and the Allar/Gamemakin style guide principle that project structure should be predictable and consistent.

## Folders

- `Core/` - shared data types, subsystems, base widgets, low-level project utilities.
- `UI/` - Slate/UMG widgets and UI-only presentation code.
- `Player/` - player controllers, input routing, view switching.
- `GameModes/` - game mode classes and bootstrapping for menu/test/prototype flows.
- `World/` - world actors, generated terrain, placement surfaces, gameplay scene actors.

## Rules

- Keep the `OLC` C++ prefix for project classes.
- Include cross-folder project headers with their folder prefix, for example `Core/OLCResourceTypes.h`.
- Keep `.generated.h` includes as the last include in each reflected header.
- Do not add new gameplay code to the module root; only `OurLastChance.*` and build files should live there.
- Use Unreal asset prefixes for Content Browser assets: `BP_`, `WBP_`, `SM_`, `SK_`, `M_`, `MI_`, `T_`, `DT_`, `DA_`, etc.
- Prefer one project-owned Content root for game assets when we move imported UI/source art into Unreal assets.

# Research Tech Tree — Implementation Guide

Build after completing `UE5/Assets/UI/Base.md`.

## Widgets

- `WBP_Research_Root`
- `WBP_ResearchCategoryFilter`
- `WBP_ResearchNode`
- `WBP_ResearchConnectionLine`
- `WBP_ResearchGateNode`
- `WBP_ResearchDetailPanel`
- `WBP_ResearchQueue`

## Data Required

- research node id, title, category, TIR ring
- prerequisites and branch/intersection gates
- required building: Forge, Energy Lab, Dark Matter Lab, Void Lab
- cost using canonical resources plus documented special resources
- research time/progress
- unlock effects

## Implementation Steps

1. Create radial/branch hybrid layout with TIR rings.
2. Add category filters: Weapons, Armor, Drives, Energy, Vision, Storage, Buildings, Units.
3. Render nodes with states: locked, available, researching, complete.
4. Render prerequisite connection lines and gate intersections.
5. Bind selected node to right detail panel.
6. Add research queue and active progress.

## In-Game Test

1. Press `F7` in the UI test map.
2. Select locked, available, researching, and complete nodes.
3. Start fake research.
4. Verify queue/progress updates.
5. Toggle category filters.

## Acceptance

- TIR rings are clear.
- Gate requirements are readable.
- Costs do not introduce undocumented resource names.


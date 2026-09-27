# Faction Selection UE5 Implementation Guide

This guide is for implementing the `WBP_FactionSelect` screen for **Our Last Chance**.

Source mockup:

`Assets/Style/Stil-1/View/Mockups/13-Faction-Selection-View.png`

Source assets:

`UE5/Assets/UI/FactionSelection`

Import destination:

`/Game/UI/FactionSelection`

## Visual Target

Match the welcome screen style:

- Dark realistic hard sci-fi UI.
- Chamfered gunmetal panels and heavy mechanical borders.
- Orange primary action and selected faction states.
- Gray inactive faction states with small blue interface accents.
- Bold uppercase condensed typography.

Use the global palette from `Assets/Style/Stil-1/UI_THEME.md`.

## Provided Assets

| Source File | UE Texture Name | Use |
|---|---|---|
| `Button_Faction_Active.png` | `T_FS_Button_Faction_Active` | Selected faction button |
| `Button_Faction_Inactive.png` | `T_FS_Button_Faction_Inactive` | Normal faction button |
| `Button_Faction_Hover.png` | `T_FS_Button_Faction_Hover` | Hover / keyboard focus faction button |
| `Button_Faction_Pressed.png` | `T_FS_Button_Faction_Pressed` | Pressed faction button |
| `Button_Faction_Disabled.png` | `T_FS_Button_Faction_Disabled` | Locked / unavailable faction button |
| `Panel_Faction_Dossier.png` | `T_FS_Panel_Dossier` | Large selected faction detail panel |
| `Panel_Faction_Stats.png` | `T_FS_Panel_Stats` | Stat comparison block |
| `Panel_HUD_BottomStrip.png` | `T_FS_HUD_BottomStrip` | Bottom navigation/status strip |
| `Panel_HUD_SideDock.png` | `T_FS_HUD_SideDock` | Optional left/right dock panel |
| `Panel_HUD_TopStatus.png` | `T_FS_HUD_TopStatus` | Optional top status strip |
| `Bar_Stat_Green.png` | `T_FS_Bar_Stat_Green` | High/positive stat bar |
| `Bar_Stat_Orange.png` | `T_FS_Bar_Stat_Orange` | Primary faction stat bar |
| `Bar_Stat_Blue.png` | `T_FS_Bar_Stat_Blue` | Mobility/info stat bar |
| `Bar_Stat_Red.png` | `T_FS_Bar_Stat_Red` | Low/danger stat bar |
| `Faction_Icon_Atlas_4x1.png` | `T_FS_Faction_Icon_Atlas` | Four faction icons, 128 px cells |

Recommended texture settings:

- Texture group: `UserInterface2D`.
- Compression: UI/default color compression.
- sRGB: enabled.
- Mipmaps: disabled for crisp UMG rendering.
- Preserve alpha on all transparent UI slices.

## Widget Blueprints

Create these widgets under `/Game/UI/FactionSelection/Widgets`:

| Widget | Responsibility |
|---|---|
| `WBP_FactionSelect` | Full faction selection screen |
| `WBP_FactionButton` | Reusable faction choice button with state textures |
| `WBP_FactionDossier` | Selected faction image, description, and stat rows |
| `WBP_FactionStatRow` | Label + segmented stat bar |
| `WBP_FactionConfirmBar` | Bottom status strip and confirm/back actions |

## Layout

Baseline design resolution is `1920 x 1080`.

Recommended hierarchy:

```text
CanvasPanel Root
  Image BackgroundPlate
  WBP_WS_Frame OuterFrame
  VerticalBox LeftColumn
    TextBlock SELECT FACTION
    WBP_FactionButton NeonPunk
    WBP_FactionButton DarkRealistic
    WBP_FactionButton CartoonSciFi
    WBP_FactionButton BrightRealistic
  WBP_FactionDossier DossierPanel
  WBP_FactionConfirmBar BottomBar
```

Suggested placement:

- Reuse the welcome screen background/frame where practical.
- Left column: `X=145`, `Y=145`, width about `560`.
- Faction buttons: `512 x 96`, stacked with `16 px` spacing.
- Dossier panel: right side, `X=820`, `Y=185`, size about `900 x 640`.
- Bottom strip: anchored bottom, centered, max height `120`.

## Faction Data

Back the screen with a data asset or table, not hardcoded widget text.

Recommended fields:

```text
FactionId
DisplayName
ThemeDescription
MiningRating
DefenseRating
MobilityRating
SurvivalRating
PreferredRaceBonus
SignatureBuilding
AvailableChampions
PreviewTexture
```

Initial faction rows:

| FactionId | DisplayName | Mining | Defense | Mobility | Survival |
|---|---|---:|---:|---:|---:|
| `NeonPunk` | `NEON PUNK` | 5 | 2 | 2 | 2 |
| `DarkRealistic` | `DARK REALISTIC` | 2 | 5 | 3 | 3 |
| `CartoonSciFi` | `CARTOON SCIFI` | 2 | 3 | 5 | 3 |
| `BrightRealistic` | `BRIGHT REALISTIC` | 3 | 3 | 3 | 5 |

## Interaction

`WBP_FactionButton` should expose:

- `FactionId`
- `DisplayName`
- `bSelected`
- `bDisabled`
- `OnFactionSelected`

State mapping:

| State | Texture |
|---|---|
| Normal | `T_FS_Button_Faction_Inactive` |
| Hovered / focused | `T_FS_Button_Faction_Hover` |
| Pressed | `T_FS_Button_Faction_Pressed` |
| Selected | `T_FS_Button_Faction_Active` |
| Disabled | `T_FS_Button_Faction_Disabled` |

Confirm flow:

1. Player selects a faction.
2. Dossier panel updates instantly.
3. Confirm button becomes active.
4. On confirm, write selected `FactionId` into the new campaign state.
5. Continue to champion selection, character setup, or the first playable map depending on campaign flow.

Back flow:

- Return to the welcome/new campaign menu without losing global settings.

## Acceptance Checklist

- View appears after New Campaign.
- Four faction buttons are navigable by mouse, keyboard, and gamepad.
- Hover/focus/selected states are visually distinct.
- Selected faction updates stats and dossier data.
- Confirm is disabled until a faction is selected.
- Confirm persists the selected faction into campaign setup state.
- The layout remains readable at `1920x1080`, `2560x1440`, `3440x1440`, and Steam Deck resolution.
- No raw PNG paths are referenced from gameplay code after import.

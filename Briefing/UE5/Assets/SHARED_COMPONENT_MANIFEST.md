# Shared UI Component Atlas Manifest

Atlas:

`SHR-UI-Atlas-Components-4x4.png`

## Tile Order

| Tile | Asset ID | Component | Use |
|---|---|---|---|
| Row 1, Col 1 | `SHR-PNL-01` | Angular Panel Small | Small stat/action panel. |
| Row 1, Col 2 | `SHR-PNL-02` | Angular Panel Wide | Header/resource strip panel. |
| Row 1, Col 3 | `SHR-PNL-03` | Right Detail Panel Shell | Entity/planet/module detail panel. |
| Row 1, Col 4 | `SHR-PNL-04` | Modal Panel Shell | Settings/dialog/confirmation modal. |
| Row 2, Col 1 | `SHR-BTN-01` | Primary Orange Button | Primary action. |
| Row 2, Col 2 | `SHR-BTN-02` | Secondary Blue Button | Scan/navigation/secondary action. |
| Row 2, Col 3 | `SHR-BTN-03` | Danger Red Button | Remove/cancel/danger action. |
| Row 2, Col 4 | `SHR-BTN-04` | Disabled Gray Button | Disabled action state. |
| Row 3, Col 1 | `SHR-TAB-01` | Tab Normal | Inactive tab. |
| Row 3, Col 2 | `SHR-TAB-02` | Tab Active | Active tab. |
| Row 3, Col 3 | `SHR-BTN-05` | Icon Button Frame | Square icon button shell. |
| Row 3, Col 4 | `SHR-RSC-FRM-01` | Resource Counter Frame | One resource counter shell. |
| Row 4, Col 1 | `SHR-PRG-01` | Progress Bar Empty | Progress/health/research base. |
| Row 4, Col 2 | `SHR-PRG-02` | Progress Bar Fill Segment | Fill material/brush. |
| Row 4, Col 3 | `SHR-TIP-01` | Tooltip Panel | Hover/click tooltip. |
| Row 4, Col 4 | `SHR-MAP-01` | Minimap Frame | Radar/minimap frame. |

## UMG Notes

- Treat these as first-pass sliced brush sources.
- For production, rebuild scalable 9-slice materials where needed instead of stretching bitmap corners.
- Keep text separate in UMG; these components intentionally contain no labels.


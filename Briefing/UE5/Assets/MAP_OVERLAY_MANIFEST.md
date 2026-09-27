# Map And Radar Overlay Atlas Manifest

Atlas:

`SHR-MAP-Atlas-Overlays-4x4.png`

## Tile Order

| Tile | Asset ID | Overlay | Use |
|---|---|---|---|
| Row 1, Col 1 | `SHR-MAP-01` | Minimap Border Square | Minimap/radar frame. |
| Row 1, Col 2 | `SHR-MAP-02` | Radar Sweep Wedge | Animated radar sweep. |
| Row 1, Col 3 | `SHR-SEL-01` | Selection Box Outline | Drag selection box. |
| Row 1, Col 4 | `SHR-SEL-02` | Unit Selection Ring | Selected unit marker. |
| Row 2, Col 1 | `SHR-SEL-03` | Building Selection Ring | Selected building marker. |
| Row 2, Col 2 | `SHR-MRK-02` | Enemy Triangle Marker | World/minimap enemy marker. |
| Row 2, Col 3 | `SHR-MRK-03` | Ally Dot Marker | Ally/minimap marker. |
| Row 2, Col 4 | `SHR-MRK-04` | Resource Node Marker | Resource marker. |
| Row 3, Col 1 | `SHR-MRK-05` | Objective Marker | Objective marker. |
| Row 3, Col 2 | `SHR-MRK-06` | Ping Marker | Player ping/attention marker. |
| Row 3, Col 3 | `SHR-MAP-03` | Fog Of War Soft Patch | Fog overlay placeholder. |
| Row 3, Col 4 | `SHR-MAP-04` | Scan Pulse Ring | Scan pulse animation source. |
| Row 4, Col 1 | `SHR-RTE-01` | Route Dashed Segment | Route/path line segment. |
| Row 4, Col 2 | `SHR-BLD-01` | Valid Area Grid | Valid placement/area overlay. |
| Row 4, Col 3 | `SHR-BLD-02` | Invalid Area Grid | Invalid placement/area overlay. |
| Row 4, Col 4 | `SHR-CAM-01` | Camera Bounds Frame | Camera/map boundary marker. |

## Notes

- These are overlay sprites; prefer translucent/additive UI materials in Unreal.
- Use red triangle markers only for enemy/threat indicators.


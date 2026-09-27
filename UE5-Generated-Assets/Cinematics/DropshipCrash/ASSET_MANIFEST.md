# Dropship Crash Cinematic Asset Manifest

Generated replacement plates for the campaign-opening dropship crash sequence.

Import destination:

`/Game/Cinematics/DropshipCrash`

| File | Beat | Use |
|---|---|---|
| `Crash-01-Night-Approach-Wing-Shear.png` | Night approach | Dropship descends over desert, engine failure and wing damage begins |
| `Crash-02-Night-Camera-Rock-Strike.png` | Camera-side rock strike | Dropship clips a huge foreground rock while continuing into open terrain |
| `Crash-03-Predawn-Hard-Ground-Crash.png` | Hard ground crash | Dropship slams into open desert, wings gone, trench begins |

## Sequence Timing

```text
00.0s - 03.0s  Crash-01: low night approach, wing starts tearing away
03.0s - 06.0s  Crash-02: camera-side rock strike, wing/hull scrape, debris and sparks
06.0s - 09.0s  Crash-03: hard ground impact in open desert, trench, smoke, predawn horizon
09.0s - 13.0s  Blend from predawn crash plate into playable day crash-site level
```

Use camera shake, layered smoke, spark particles, and sound design over these plates until the full in-engine cinematic exists.

## Daybreak Blend

The final pre-dawn plate should blend into the existing daytime desert crash-site asset/level:

1. Crossfade the plate into the level camera from the same side angle.
2. Animate exposure and color temperature from blue night to warm sunrise.
3. Fade smoke from baked image into Niagara smoke near the actual ship.
4. Spawn the selected champion near the dropship ramp.
5. Hand control to the RTS/gameplay camera once the hero is visible.

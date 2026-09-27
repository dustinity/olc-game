# Planet Navigation

Solar system map, TIR progression, scanning mechanics, and landing procedures.

---

## Solar System Structure

### Overview

Each solar system contains:
- **1 Central Star** (sun) — visible at map center
- **5-10 Planets** — orbiting the star (displayed as colored orbital lines)
- **Optional asteroid belts** between certain planets
- **Optional space stations** — abandoned, trade posts, or alien structures

### Map Display

- **View:** Top-down 2D map, zoomable
- **Planet movement:** Shown as colored orbital lines around the sun (planets do NOT physically move to avoid tracking confusion)
- **Visited planets:** Marked with colony icon 🏠
- **Unvisited planets:** Show as unknown sphere — details revealed only after scanning
- **Center galaxy planet:** Visible as large glowing target at galaxy center

### Solar System Count

- **Total solar systems:** 3,000+ scattered across the galaxy
- **Reachable systems:** Start with 1-2 systems accessible
- **Progressive unlock:** Each hop with normal rocket reaches 1-2 new systems
- **Alien warp drive:** Unlocks direct wormhole access to systems within range (5+ systems per jump at max tier)

---

## TIR (Technology/Threat Intensity Rating) System

### Galaxy Ring Structure

Planets are classified by their ring position from galaxy center:

| Ring Position | Distance from Center | TIR Range | Difficulty |
|---------------|---------------------|-----------|------------|
| Outer Ring (15-20 systems out) | Farthest | TIR 1 | Low |
| Mid-Outer (10-15 systems out) | Distant | TIR 1-2 | Low-Medium |
| Mid Galaxy (5-10 systems out) | Moderate | TIR 2-3 | Medium |
| Mid-Inner (2-5 systems out) | Close | TIR 3-4 | High |
| Inner Ring (1-2 systems out) | Very close | TIR 4-5 | Very High |
| Galaxy Center | Zero | TIR 5+ / Alien | Extreme |

### TIR Impacts

TIR affects multiple game systems:

| System | TIR Influence |
|--------|---------------|
| Planet biome variety | Higher TIR = more extreme biomes (ice, volcanic) |
| Enemy race tier | TIR 1 planets = basic sub-races only; TIR 5 = elite variants |
| Building unlocks | Each building has minimum TIR requirement |
| Weapon availability | Weapons scale TIR 1-5 + Alien tier |
| Dungeon loot quality | Higher TIR dungeons yield higher tier blueprints |
| Resource variety | Higher TIR planets have rarer minerals (titanium, dark matter) |

### Cross-Galaxy Progression

After reaching galaxy center and completing the final boss:
1. **Galaxy shatters** into pieces — abandoned ships everywhere to salvage
2. **Central structure** discovered (requires side-view exploration to access)
3. **Alien tech obtained** — better drive system to reach alien galaxy
4. **Alien galaxy** mirrors original structure but with energy fields, abandoned stations, warfields
5. **Addon potential:** Another thread from yet another galaxy

---

## Scanning Mechanics

### Pre-Landing Planet Scan

Scanning reveals planet information before landing:

| Scan Cost (Energy) | Information Revealed |
|--------------------|---------------------|
| 0 energy (basic flyby) | Planet name, general color (biome hint), TIR range |
| 50 energy | Biome type, basic resource categories (fuel/minerals/construction) |
| 100 energy | Full biome details, specific mineral types, hostile area location |
| 200+ energy | Dungeon locations, camp positions, optimal landing zones |

### Scan Range Formula

**Base scan range = 100 energy → reveals area equal to 2x fuel range**

Scan range can be increased by:
- **Radar Tower building** (+50% range per TIR tier) [See planet buildings](../../buildings/README.md)
- **Ship scanning suite upgrades** [See ship modules](../../ShipModules/Scanning/README.md)
- **Tech tree research** (radar efficiency improvements) [See tech tree](../../Tech-Tree/Research-Categories/README.md)

### Ship-Based Scanning Suite

| Module | Function | Range | Energy Cost |
|--------|----------|-------|-------------|
| Radar | Basic radial scan, detects structures | Short-Medium | 100 energy per use |
| Deep Sonar | Detects underground/hidden structures | Medium-Long | 150 energy per use |
| Laser Targeting | Precise location of specific resource types | Short (targeted) | 50 energy per pulse |
| Satellite Deployment | Long-range search for specific conditions over time | Planet-wide | 200 energy + satellite maintenance |

### Satellite Scanning

Satellites can be deployed to search for specific conditions:
- **Search duration:** 1-10 minutes (real-time or accelerated)
- **Target types:** Fuel deposits, mineral-rich areas, dungeon signatures, life signs
- **Data delivery:** Results transmitted when search complete, shows location on map
- **Multiple satellites:** Can deploy up to 3 simultaneously (requires multiple satellite bays on ship)

---

## Landing Procedures

### Required Before Landing

1. **Fuel sufficient** for descent (1-2x additional fuel beyond hop cost for selected landing) [See resources/types.md](../../resources/README.md)
2. **Orbit scan complete** (recommended but not required)
3. **Landing zone clear** of hostile units (auto-detected if radar active)

### Landing Types

| Type | Fuel Cost | Accuracy | Notes |
|------|-----------|----------|-------|
| Random landing | 1x base fuel | Low — random point within biome | Fast, risky near hostile areas |
| Selected landing | 1-2x additional fuel | High — specific coordinates | Requires scan data or visual identification |
| Emergency landing | Variable | Very low — crash lands | When damaged in atmosphere, takes structural damage |

### Landing Animation Sequence

1. **Side-view:** Dropship descending through atmosphere
2. **Smoke trails** from heat friction (hull integrity affects burn intensity)
3. **Crash impact** — dust/debris explosion based on landing biome
4. **Transition** to 2.5D top-down RTS view as pilot exits

### Post-Landing

- **Hull damage assessed** — if below 20%, further landings risk catastrophic failure
- **Resources deposited** — crash landing deposits scattered resources (10-30% of carried amount)
- **Local threats identified** — nearby enemies alerted if landing was noisy (explosion/smoke)
- **Base building enabled** — can begin constructing buildings immediately

---

## Interplanetary Travel

### Fuel-Based Movement

Travel is entirely fuel-dependent:

| Journey Type | Fuel Cost | Prerequisites |
|--------------|-----------|---------------|
| Planet to planet (same system) | 1x base | Clear orbital path |
| Leave solar system | 5x base fuel | Engineered ship, TIR 1+ |
| Between solar systems | 5x base fuel minimum | TIR 2+ recommended |
| To galaxy center | 200+ hops (normal drive) | Progressive exploration |
| Wormhole transit (alien drive) | Variable | Alien drive installed, wormhole located |

### Storage and Range

Storage capacity directly affects travel range:

| Storage Configuration | Effective Range |
|----------------------|-----------------|
| Base storage (1x fuel tank) | 1 hop per tank |
| 2x storage expansion | 2 hops per tank equivalent |
| 5x storage (full haul + expansions) | 5 hops per tank equivalent |
| Alien black hole storage | Effectively unlimited (energy-scaled) |

### Space Events During Travel

Between jumps, random events may occur:

| Event | Trigger | Options | Reward | Risk |
|-------|---------|---------|--------|------|
| Pirate encounter | Random in empty space | Fight or flee | Loot from pirates | Lose fuel/ships if lose fight |
| Derelict ship | Scanned signal | Board or skip | Resources, blueprints | Time cost, possible ambush |
| Space anomaly | Near wormhole or black hole | Investigate or bypass | Rare minerals | Random effect (good/bad) |
| Merchant convoy | Near trade routes | Trade or raid | Needed resources | Moderate |
| Meteor shower | In asteroid belt areas | Dodge minigame | None | Hull damage if hit |

### Wormhole Transit

Alien wormholes enable instant travel:
- **Opening time:** 10 seconds (ship holds position)
- **Transit time:** 5 seconds through wormhole
- **Animation:** Side-view of ship entering glowing portal
- **Requirements:** Alien warp drive on mothership
- **Range:** Limited by drive TIR tier (TIR 3+ = access more wormholes)

---

## Colony Markers

### Visited Planet Tracking

- **Visited planets** display colony icon on map
- **Owned planets** (with player buildings) show flag icon
- **Abandoned planets** (player left but buildings remain) show faded flag [See core loop](../../Gameplay/Core-Loop/README.md)

### Colony Information Display

Clicking a planet marker shows:
- Planet name and biome type
- TIR rating
- Resources currently stored in buildings
- Active enemy presence (if any)
- Time since last visit
- Building list and production status

---

## See Also

- [Planet Types and Biomes](../Planet-Types/README.md)
- [Planet Events and Dungeons](../Planet-Events/README.md)
- [Resource Fuel Mechanics](../../resources/README.md)
- [Ship Scanning Modules](../../ShipModules/Scanning/README.md)
- [Tech Tree Radar Research](../../Tech-Tree/Research-Categories/README.md)
- [Space Travel Events](../../Gameplay/Space-Travel/README.md)

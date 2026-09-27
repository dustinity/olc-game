# Resource Types — Core Game Resources, Biome Modifiers, and Generation Methods

Six core resource types that drive all gameplay systems: Energy, Fuel, Construction Material, Minerals, Hull, and Survival (Food + Oxygen combined).

---

## Resource Overview

| Resource | Primary Use | Generation Methods | Storage Type |
|----------|-------------|-------------------|--------------|
| **Energy** | Power buildings, ships, weapons | Solar panels, wind turbines, coal reactors, water turbines, atomic reactors, geothermal vents | Battery storage [See Ship Modules](../ShipModules/README.md) |
| **Fuel** | Interplanetary travel, vehicle operation | Oil pumps, gas extractors, biofuel processors, dark matter synthesizers | Fuel tanks [See Dropship](../Spaceship/Dropship/README.md) |
| **Construction Material** | Build structures, repair ships | Mines (stone, concrete), factories (plastic, carbon), scavenging | Storage containers [See Storage System](../buildings/Reference/Storage-System.md) |
| **Minerals** | Craft items, upgrade equipment | Mines (glass, crystal, sulfur, uranium), refinery output | Storage containers [See Storage System](../buildings/Reference/Storage-System.md) |
| **Hull** | Ship armor, heat resistance | Mines (titanium, advanced alloys), refinery processing | Storage containers [See Storage System](../buildings/Reference/Storage-System.md) |
| **Survival** | Unit sustenance (food + oxygen combined) | Farms, atmospheric extractors, hunting, fishing | Perishable storage [See Dropship](../Spaceship/Dropship/README.md) |

---

## Resource Detail Pages

Each resource has a dedicated page with full tables for generation, consumption, costs, and biome modifiers:

| # | Resource | Detail Page |
|---|----------|-------------|
| 1 | **Energy** | [Energy](Energy/README.md) — Power for buildings, ships, weapons; six generation methods with biome ratings |
| 2 | **Fuel** | [Fuel](Fuel/README.md) — Propellant for interplanetary travel; six fuel types with efficiency ratings |
| 3 | **Construction Material** | [Construction Material](Construction-Material/README.md) — Building material for structures and ship modules |
| 4 | **Minerals** | [Minerals](Minerals/README.md) — Advanced crafting materials; six mineral types with color indicators |
| 5 | **Hull** | [Hull](Hull/README.md) — Ship armor plating and heat resistance; four hull material tiers |
| 6 | **Survival** | [Survival](Survival/README.md) — Combined food + oxygen for unit sustenance |

---

## Resource Conversion Rates

### Cross-Resource Conversion (at Resource Converter building) [See Planet Buildings](../buildings/README.md)

| From | To | Yield Rate | Energy Cost per Cycle | Time per Cycle |
|------|-----|-----------|---------------------|----------------|
| Construction Material | Minerals | 100 → 70 (70%) | 50 energy | 3 minutes |
| Minerals | Construction Material | 100 → 70 (70%) | 50 energy | 3 minutes |
| Fuel | Energy | 100 → 70 (70%) | None (reverse conversion) | Instant |
| Hull | Construction Material | 100 → 50 (50%) | 30 energy | 5 minutes |
| Survival | Energy | N/A (perishable, cannot convert) | — | — |

### Tier-Based Conversion Improvements [See Tech Tree](../Tech-Tree/Research-Categories/README.md)

| Research Level | Conversion Yield Improvement | Unlock Requirement |
|---------------|---------------------------|-------------------|
| Resource Conversion I | 70% yield (baseline) | Ring 1, Storage Organization II complete |
| Resource Conversion II | 75% yield (+5% improvement) | Ring 3, Haul Storage Construction complete |

---

## Biome Resource Modifier Table

### Resource Generation Multiplier by Planet Biome

| Biome | Energy Mod | Fuel Mod | Construction Mod | Minerals Mod | Hull Mod | Survival Mod |
|-------|:----------:|:--------:|:----------------:|:------------:|:--------:|:------------:|
| Desert | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐ | ⭐⭐ | ⭐⭐ |
| Dusty | ⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐ | ⭐⭐ |
| Rocky | ⭐⭐⭐ | ⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐ |
| Water | ⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐ | ⭐⭐ | ⭐⭐ | ⭐⭐⭐⭐⭐ |
| Swamp | ⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐ | ⭐⭐⭐⭐ |
| Jungle | ⭐ | ⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| Light Snow | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐ |
| Ice | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐ |

**Legend:** ⭐ = Low (0.5x), ⭐⭐ = Below Average (0.75x), ⭐⭐⭐ = Average (1.0x), ⭐⭐⭐⭐ = Above Average (1.25x), ⭐⭐⭐⭐⭐ = High (1.5x)

---

## Resource Progression Timeline

### Early Game (TIR 1-2) — Survival Phase
**Primary Resources:** Construction Material, Basic Minerals, Chemical Fuel
**Key Activities:**
- Manual collection from crash site and nearby area [See Dropship](../Spaceship/Dropship/README.md)
- Establish basic mine for construction material and minerals
- Build solar panels or wind turbines for energy [See Planet Buildings](../buildings/README.md)
- Refuel dropship drive using scavenged or processed fuel

### Mid Game (TIR 2-3) — Expansion Phase
**Primary Resources:** All six types, focus on Fuel and Hull accumulation
**Key Activities:**
- Build oil pump or biofuel processor for reliable fuel supply [See Planet Buildings](../buildings/README.md)
- Establish hydroponic farm or atmospheric extractor for Survival
- Construct atomic reactor for stable energy production
- Begin hull reinforcement on dropship [See Dropship](../Spaceship/Dropship/README.md)

### Late Game (TIR 4-5) — Dominance Phase
**Primary Resources:** Dark Matter Crystals, Advanced Alloys, Frozen Methane Fuel
**Key Activities:**
- Crystal Synthesizer for dark matter fuel and crystals [See Planet Buildings](../buildings/README.md)
- Geothermal vent harnessing for near-infinite energy
- Zero-Point Energy Extraction for unlimited power
- Alien composite shield integration on dropship [See Ship Modules](../ShipModules/README.md)

---

## See Also

- [Resource generation buildings](../buildings/README.md)
- [Fuel consumption by travel type](../Planets/Navigation/README.md)
- [Storage capacity progression](../buildings/Reference/Storage-System.md)
- [Research material costs](../Tech-Tree/Research-Categories/README.md)
- [Unit upgrade mineral requirements](../units/Upgrade-System/README.md)

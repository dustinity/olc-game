# Planet Types & Biomes

Planet classification system based on biome, environmental modifiers, hostile areas, and boss mechanics.

---

## Biome Classification

Planets are classified by their dominant biome type. Each planet receives a random combination of biome factors at generation, creating unique planetary profiles.

### Primary Biome Types

| Biome | Visual Signature | Temperature | Water Level | Accessible Early Game |
|-------|------------------|-------------|-------------|----------------------|
| Desert | Golden sands, dune formations | Extreme Hot | Dusty | ✅ Yes |
| Dusty | Brown-tan, particulate atmosphere | Hot | Low | ✅ Yes |
| Rocky | Gray/brown terrain, exposed rock | Variable | None | ✅ Yes |
| Water | Blue with cloud cover | Mild | High (oceans) | ⚠️ Partial |
| Swamp | Brown-green, murky waterways | Warm | Very High | ⚠️ Partial |
| Jungle | Dense green canopy | Hot/Humid | High | ⚠️ Partial |
| Light Snow | White-pale gray, light snowfall | Cold | Frozen | ✅ Yes |
| Ice | Bright white, ice sheet coverage | Extreme Cold | Frozen solid | ❌ No (TIR 3+) |

### Secondary Modifiers (Random at Generation)

Each planet receives 1-3 secondary modifiers that affect resource yields and gameplay:

| Modifier | Effect | Example |
|----------|--------|---------|
| Mineral Rich | +50% mineral deposits | Rocky desert with abundant crystal veins |
| Fuel Rich | +50% fuel/oil deposits | Swamp planet with vast oil reserves |
| High Gravity | -20% movement speed, +50% building HP | Dense jungle world |
| Low Gravity | +30% movement speed, -20% building HP | Light snow moon-like planet |
| Storm Active | Periodic energy damage to unprotected structures | Dusty storm planet |
| Geologically Active | Volcanic events, lava flows | Rocky volcano world |
| Abundant Flora | +100% food yield, more animal spawns | Jungle with dense vegetation |
| Crystal Caves | Underground crystal network, dungeon-rich | Ice planet with crystal caverns |

---

## Biome-Specific Resource Modifiers

Detailed per-biome resource modifiers are documented in each biome's subfolder:

| Biome | Design Doc | Key Characteristics |
|-------|-----------|-------------------|
| Desert | [Biomes/Desert](../Biomes/Desert/) | Solar +30%, stone high yield, dust storms reduce radar 40% |
| Dusty | [Biomes/Dusty](../Biomes/Dusty/) | Wind turbines +20%, particulate shield bonus -10% damage |
| Rocky | [Biomes/Rocky](../Biomes/Rocky/) | Geothermal available, titanium increased, natural rock formations +15% wall HP |
| Water | [Biomes/Water](../Biomes/Water/) | Water turbines high output, fishing/hunting food, landing requires flat coast |
| Swamp | [Biomes/Swamp](../Biomes/Swamp/) | Biofuel unique resource, toxic spore events, uranium in swamp water |
| Jungle | [Biomes/Jungle](../Biomes/Jungle/) | Food +80%, movement -15% vegetation, natural cover +20% defense |
| Light Snow | [Biomes/Light Snow](../Biomes/Light%20Snow/) | Wind turbines +25%, frost events -25% efficiency, frozen methane TIR 2+ |
| Ice | [Biomes/Ice](../Biomes/Ice/) | Crystal very abundant, dark matter crystals near poles, ice sheet cracking risk |

---

## Hostile Areas & Boss Mechanics

Every planet contains at least one **unique hostile area** — a high-difficulty zone with escalating encounters culminating in a boss fight.

### Hostile Area Types by Biome

| Biome | Hostile Area Type | Boss Theme | Reward Blueprint Category |
|-------|-------------------|------------|--------------------------|
| Desert | Sun-baked ruin complex with sand-infested tunnels | Sand Worm Titan | Hull armor, heat-resistant materials |
| Dusty | Abandoned mining colony with dust storm arena | Dust Storm Elemental | Wind energy tech, filtration systems |
| Rocky | Volcanic crater fortress | Magma Golem Lord | Geothermal tech, titanium processing |
| Water | Sunken underwater facility | Abyssal Leviathan | Submersible drives, pressure hulls |
| Swamp | Overgrown alien research station | Spore Queen Mother | Biofuel refining, toxin resistance |
| Jungle | Ancient temple complex in canopy | Jungle Predator Alpha | Camouflage tech, bio-weapons |
| Light Snow | Frozen military outpost | Frost Warden | Insulated armor, cold-weather units |
| Ice | Polar alien monolith | Ice Colossus | Crystal energy tech, dark matter |

### Boss Fight Mechanics

**General Structure:**
1. **Entry Phase:** Navigate through increasingly difficult encounters (3-5 waves)
2. **Arena Entry:** Final chamber/area triggers boss spawn
3. **Boss Phase:** Multi-stage fight with unique mechanics per boss
4. **Loot Window:** 60 seconds post-victory to collect blueprint and rare resources

**Boss Mechanic Examples:**

| Boss | Unique Mechanic | Counter Strategy |
|------|-----------------|------------------|
| Sand Worm Titan | Burrows underground, surfaces randomly | Seismic sensors reveal location, area damage |
| Dust Storm Elemental | Reduces visibility to near-zero | Flash charges or thermal targeting |
| Magma Golem Lord | Melts nearby buildings, creates lava pools | Coolant bombs, attack from elevated positions |
| Abyssal Leviathan | Pulls units toward water, drowning risk | Tethers, net launchers, stay on dry ground |
| Spore Queen Mother | Deploys toxic spore clouds that spawn mini-swarms | Air filtration, fire-based attacks |
| Jungle Predator Alpha | Camouflages, ambushes from canopy | Motion sensors, area denial weapons |
| Frost Warden | Freezes units solid for 10 seconds on contact | Heating cores, rapid attack to break freeze |
| Ice Colossus | Splits into two smaller versions at 50% HP | Focus fire before split, or destroy ice anchors |

### Blueprint Rewards from Bosses

Boss blueprints are **optional but high-value** — they unlock powerful tech paths:

- **Fuel-efficient drives** (reduces travel cost by 15%)
- **Heat-resistant hull plating** (+50% environmental resistance)
- **Biome-specific weapons** (+30% damage in matching biome)
- **Advanced scanning modules** (reveals dungeon types on map)
- **Passive permanent improvements** to faction attributes

---

## TIR and Planet Progression

Planet difficulty scales by ring position within the galaxy:

| Ring | TIR Range | Planet Difficulty | Hostile Area Strength | Boss Level |
|------|-----------|-------------------|----------------------|------------|
| Outer Ring (15-20) | TIR 1 | Low | Basic encounters, 1-2 waves | Tier 1 boss |
| Mid-Outer (10-15) | TIR 1-2 | Low-Medium | Standard encounters, 2-3 waves | Tier 1-2 boss |
| Mid (5-10) | TIR 2-3 | Medium | Mixed encounters, 3-4 waves | Tier 2-3 boss |
| Mid-Inner (2-5) | TIR 3-4 | High | Advanced encounters, 4-5 waves | Tier 3-4 boss |
| Inner Ring (1-2) | TIR 4-5 | Very High | Elite encounters, 5+ waves | Tier 4-5 boss |
| Center | TIR 5+ | Extreme | All enemy types, 6+ waves | Final boss + alien variant |

---

## See Also

- [Planet Navigation Mechanics](../Navigation/README.md)
- [Planet Events and Dungeon Types](../Planet-Events/README.md)
- [Resource Types and Biome Modifiers](../../resources/README.md)
- [Race Families and Preferred Biomes](../../factions/Races/README.md)
- [Building Placement by Biome](../../buildings/Biome-Compatibility.md)

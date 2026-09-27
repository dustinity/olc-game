# Factions

Four playable factions, each with unique bonuses, playstyles, and thematic identities.

## Faction Selection View

The faction selection screen is registered as a Stil-1 UI view and should match the welcome screen's dark realistic frame language.

| View | Mockup | UE5 Source Assets | Implementation Notes |
|------|--------|-------------------|----------------------|
| Faction Selection | [13-Faction-Selection-View.png](../../Assets/Style/Stil-1/View/Mockups/13-Faction-Selection-View.png) | [UE5/Assets/UI/FactionSelection](../../UE5/Assets/UI/FactionSelection) | [IMPLEMENTATION_GUIDE.md](../../UE5/Assets/UI/FactionSelection/IMPLEMENTATION_GUIDE.md) |
| Champion Selection | [14-Champion-Selection-View.png](../../Assets/Style/Stil-1/View/Mockups/14-Champion-Selection-View.png) | [UE5/Assets/UI/ChampionSelection](../../UE5/Assets/UI/ChampionSelection) | [IMPLEMENTATION_GUIDE.md](../../UE5/Assets/UI/ChampionSelection/IMPLEMENTATION_GUIDE.md) |

Use faction selection after New Campaign. After the player confirms a faction and reviews its benefits, open champion selection filtered to that faction. After the player confirms a champion and reviews champion benefits, begin the dropship crash flight and start the first playable desert crash-site scene with the selected hero.

## Faction Overview

| Faction | Theme | Mining/Refining | Weapons/Base Defense | Drives/weapons | Survivability/Crafting |
|---------|-------|:---------------:|:---------------------:|:--------------:|:----------------------:|
| [Neon Punk](./Neon-Punk/README.md) | Mech-inspired, cyberpunk aesthetic | ⭐⭐⭐⭐⭐ | ⭐⭐ | ⭐⭐ | ⭐⭐ |
| [Dark Realistic](./Dark-Realistic/README.md) | Military sci-fi, starship troopers | ⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐ |
| [Cartoon SciFi](./Cartoon-SciFi/README.md) | Borderlands-style, exaggerated tech | ⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ |
| [Bright Realistic](./Bright-Realistic/README.md) | Civilian/realistic, practical design | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ |

---

## Faction Comparison Matrix

### Early Game (TIR 1-2)

| Aspect | Neon Punk | Dark Realistic | Cartoon SciFi | Bright Realistic |
|--------|-----------|----------------|---------------|------------------|
| Resource gathering | Best | Good | Good | Very Good |
| Base defense | Good | Best | Fair | Very Good |
| Exploration speed | Good | Fair | Best | Very Good |
| Unit sustainability | Good | Very Good | Good | Best |

### Mid Game (TIR 2-3)

| Aspect | Neon Punk | Dark Realistic | Cartoon SciFi | Bright Realistic |
|--------|-----------|----------------|---------------|------------------|
| Resource gathering | Best | Very Good | Very Good | Excellent |
| Base defense | Very Good | Excellent | Good | Very Good |
| Exploration speed | Very Good | Good | Excellent | Very Good |
| Unit sustainability | Good | Very Good | Good | Excellent |

### Late Game (TIR 4-5)

| Aspect | Neon Punk | Dark Realistic | Cartoon SciFi | Bright Realistic |
|--------|-----------|----------------|---------------|------------------|
| Resource gathering | Excellent | Very Good | Very Good | Excellent |
| Base defense | Very Good | Excellent | Good | Very Good |
| Exploration speed | Very Good | Good | Excellent | Very Good |
| Unit sustainability | Very Good | Very Good | Very Good | Excellent |

---

## Faction Army Composition — Combined Arms by Phase

Each faction excels at different army compositions. These are recommended ratios, not hard rules. The equipment system ([Equipment System](../units/Equipment-System/README.md)) allows any unit to fill unexpected roles through gear swaps.

### Early Game (TIR 1-2)

| Faction | Infantry | Light Vehicles | Support | Aerial | Focus |
|---------|----------|----------------|---------|--------|-------|
| Neon Punk | 40% Mech Workers + Scouts | 30% Scout Cars | 15% Engineers | 15% Scout Drones | Resource extraction, fast scouting |
| Dark Realistic | 60% Soldiers + Shock Troopers | 20% Light APCs | 15% Heavy Gunners | 5% Scout Drones | Defensive perimeter, firepower |
| Cartoon SciFi | 40% Blaster Rangers + Jump Troopers | 35% Jump Bikes | 10% Engineers | 15% Scout Drones | Hit-and-run, rapid deployment |
| Bright Realistic | 40% Soldiers + Survival Scouts | 20% Light APCs | 30% Medics + Engineers | 10% Scout Drones | Sustainable operations, healing |

### Mid Game (TIR 2-3)

| Faction | Infantry | Heavy Vehicles | Support | Aerial | Focus |
|---------|----------|----------------|---------|--------|-------|
| Neon Punk | 30% Scouts + Circuit Breakers | 35% Tanks | 15% Engineers | 20% Fighter Jets | Electric damage vs mechanical enemies |
| Dark Realistic | 40% Shock Troopers + Heavy Gunners | 30% Tanks (reinforced) | 15% Medics | 15% Dropships | Overwhelming firepower, fortified defense |
| Cartoon SciFi | 25% Jump Troopers + Blaster Rangers | 25% Walkers | 15% Engineers | 35% Fighter Jets + Drones | Air superiority, mobile warfare |
| Bright Realistic | 30% Soldiers + Survival Scouts | 25% Heavy Transports | 30% Medics + Engineers | 15% Dropships | Logistics, sustained campaigns |

### Late Game (TIR 4-5) — Combined Arms Peak

| Faction | Infantry | Heavy Vehicles | Support | Aerial | Focus |
|---------|----------|----------------|---------|--------|-------|
| Neon Punk | 20% elite scouts with legendary gear | 35% mechs + tanks | 15% engineers | 30% fighter wings | Precision strikes, tech disruption |
| Dark Realistic | 30% shock troopers with fortress armor | 35% heavy mechs | 15% medics | 20% orbital-capable dropships | Unstoppable frontline, area denial |
| Cartoon SciFi | 20% jump trooper elites | 25% walkers | 15% chaos specialists | 40% air dominance | Total mobility, unpredictable combat |
| Bright Realistic | 25% survival scouts with legendary gear | 25% heavy transports + mechs | 30% medics + engineers | 20% support dropships | Endless sustainability, field repair |

---

## Faction Interactions with Other Systems

### Champion Availability [See champions](./Champions/README.md)

Each faction has 3 exclusive champions plus shared champions available to all factions. Total: 12 champions across all factions + custom character editor option.

### Race Preferences [See races](./Races/README.md)

Certain factions have bonuses against specific race families:
- **Dark Realistic:** +20% damage vs Insectoid (organized swarm tactics match military style)
- **Neon Punk:** +15% loot from Mechanical encounters (understands tech)
- **Cartoon SciFi:** +25% speed vs Reptilian (agility counters brute force)
- **Bright Realistic:** +30% survival resource yield vs Molluskoid (biome adaptation research)

### Building Synergies [See planet buildings](../buildings/README.md)

Faction signature buildings integrate with standard building ecosystem:
- Neon Punk Quantum Mine feeds excess resources to refinery network
- Dark Realistic Fortress Wall forms core of defensive perimeter
- Cartoon SciFi Ion Thruster Pad enables rapid unit deployment
- Bright Realistic Hydroponics Bay sustains long-term occupation

---

## See Also

- [Champion roster and character editor](./Champions/README.md)
- [Race families and faction bonuses](./Races/README.md)
- [Building placement and synergies](../buildings/README.md)
- [Core gameplay loop](../Gameplay/Core-Loop/Core-Loop.md)
- [Equipment system — faction-exclusive gear categories](../units/Equipment-System/README.md)

# Storage System

Grid-based storage mechanics for planet buildings and mothership modules, including alien black hole storage.

---

## Storage Fundamentals

### Grid-Based Placement

All storage units occupy grid cells on either:
- **Planet surface:** Buildings placed on terrain grid [See building design sheets](../)
- **Mothership:** Modules placed on ship hull grid [See Ship Modules](../../ShipModules/)

Storage capacity is per-resource-type. Each of the 6 resources [See resources/types.md](../../resources/README.md) has independent storage limits.

### Storage Types Overview

| Type | Grid Size | Capacity Per Resource | Tier | Availability |
|------|-----------|----------------------|------|--------------|
| Locker | 1x1 | 200 | TIR 1 | Starting |
| Container (wheel-shaped) | 2x2 or 4x1 | 2,000 | TIR 1 | Starting |
| Haul Storage | 4x4 | 10,000 | TIR 2 | Forge research |
| Reinforced Vault | 4x4 | 25,000 | TIR 3 | Factory production |
| Alien Black Hole Storage | Variable | Unlimited* (energy-scaled) | Alien | Center galaxy reward |

---

## Planet Storage Buildings

### Locker (1x1)

- **Grid Size:** 1x1
- **Cost:** 50 construction material
- **Capacity:** 200 resources per type
- **Total Capacity:** 1,200 resources (6 types x 200)
- **Power:** No power required (passive storage)
- **Notes:** 
  - First storage available at game start
  - Personal-sized locker in starting dropship
  - Limited utility — fills quickly in mid-game
  - Can be placed anywhere on planet surface

### Container (2x2 or 4x1 Wheel-Shaped)

- **Grid Size:** 2x2 OR 4x1 (rotatable with wheel interface)
- **Cost:** 300 construction material, 100 minerals
- **Capacity:** 2,000 resources per type
- **Total Capacity:** 12,000 resources (6 types x 2,000)
- **Power:** No power required (passive storage)
- **Notes:**
  - Wheel-shaped rotation interface allows shape change
  - Must maintain same total grid slot count (4 slots either way)
  - First meaningful storage expansion
  - Shape flexibility: compact 2x2 for tight spaces, elongated 4x1 for linear placement

### Haul Storage (4x4)

- **Grid Size:** 4x4
- **Cost:** 1,500 construction material, 500 minerals
- **Capacity:** 10,000 resources per type
- **Total Capacity:** 60,000 resources (6 types x 10,000)
- **Power:** No power required (passive storage)
- **Notes:**
  - Large-scale permanent storage
  - Required for mid-game resource management
  - Unlocked via forge research or dungeon blueprint
  - Can store excess production from mines and factories

### Reinforced Vault (4x4)

- **Grid Size:** 4x4
- **Cost:** 3,000 construction material, 2,000 minerals
- **Capacity:** 25,000 resources per type
- **Total Capacity:** 150,000 resources (6 types x 25,000)
- **Power:** Requires 10 energy/turn (active field)
- **Special Protection:** Contents protected against dungeon crash loss
- **Notes:**
  - If ship is damaged on landing (hull below 20%), regular storage loses 10-30% of contents randomly
  - Reinforced Vault prevents this loss entirely
  - Essential for late-game resource accumulation
  - Hull material construction provides additional protection

---

## Mothership Storage Modules

### Ship Locker (Starting)

- **Grid Size:** 1x1
- **Capacity:** 200 resources per type
- **Notes:** Starting storage on dropship. Very limited — upgrade immediately for exploration

### Ship Container

- **Grid Size:** 2x2 or 4x1
- **Capacity:** 2,000 resources per type
- **Notes:** First expansion. Same wheel-shaped rotation as planet version

### Ship Haul Storage

- **Grid Size:** 4x4
- **Capacity:** 10,000 resources per type
- **Notes:** Required for interplanetary travel with full resource loads

### Ship Reinforced Vault

- **Grid Size:** 4x4
- **Capacity:** 25,000 resources per type
- **Special:** Protected against atmospheric entry damage and space events
- **Notes:** Essential for long-duration missions through dangerous systems

---

## Alien Black Hole Storage

### Overview

- **Grid Size:** 3x3 (physical emitter)
- **TIR:** Alien
- **Capacity:** Scales with energy consumption — effectively unlimited while powered
- **Cost:** 5,000 construction material, 3,000 dark matter crystals, 2,000 energy (startup)
- **Energy Cost:** 50 energy/turn to maintain active field
- **Visual:** Swirling miniaturized event horizon at storage center

### Mechanics

| Energy Status | Effective Capacity | Behavior |
|---------------|-------------------|----------|
| Powered (50+/turn) | Unlimited | All resources stored without limit |
| Underpowered (< 50/turn) | Scales with deficit | Reduced capacity based on energy shortfall |
| Unpowered (0/turn) | Ejects contents | Random resource distribution on next landing |

### Special Properties

- **Compressed State:** All resources stored in quantum-compressed form
- **Instant Access:** No retrieval time — any resource available immediately
- **Multi-Planet Sync:** Storage contents accessible from any planet where emitter is placed
- **Crash Protection:** Immune to all crash, explosion, and environmental damage

### Risks

- **Power Dependency:** If energy supply fails completely, all stored resources are ejected randomly across nearby terrain on next landing
- **Energy Competition:** High energy draw may starve other ship systems during low-energy situations
- **Alien Radiation:** Units standing adjacent to emitter for extended periods gain +10% radiation damage but -5% accuracy

---

## Storage Management Mechanics

### Resource Transfer

| Method | Speed | Cost | Notes |
|--------|-------|------|-------|
| Manual walk-up | Instant | Free | Champion/unit must physically carry resources |
| Vehicle transport | Instant (when adjacent) | Fuel cost | Vehicles have personal storage capacity |
| Quantum Gate | Instant | 50 energy per transfer | Requires linked gates [See building design sheets](../) |
| Resource Converter | 1 minute | None | Converts between resource types at 70% yield [See building design sheets](../) |

### Storage Overflow Behavior

When storage is full:
1. **Mines/Pumps** stop producing (no place to put resources)
2. **New arrivals** from space travel deposit overflow randomly on landing terrain (10-30% loss chance)
3. **Crafting** fails if required materials exceed available storage

### Storage Optimization Tips

| Strategy | Benefit | Tier Required |
|----------|---------|---------------|
| Prioritize Haul Storage early | 5x capacity increase over lockers | TIR 2 |
| Use Reinforced Vault for valuable resources | Crash protection for endgame materials | TIR 3 |
| Maintain Black Hole Storage powered | Effectively unlimited late-game capacity | Alien |
| Place storage near production buildings | Reduces transport time | Any |

---

## Storage by Resource Type Priority

| Resource | Early Game | Mid Game | Late Game | Recommended Storage |
|----------|-----------|----------|-----------|---------------------|
| Energy | 200 (locker) | 2,000 (container) | 10,000+ (haul/black hole) | Scale with power needs |
| Fuel | 200 (locker) | 2,000 (container) | 10,000+ (haul/black hole) | Critical for travel |
| Construction | 200 (locker) | 5,000 (container/haul) | 25,000+ (vault/black hole) | High volume usage |
| Minerals | 200 (locker) | 2,000 (container) | 10,000+ (haul/black hole) | Varied subtypes matter |
| Hull Materials | 200 (locker) | 2,000 (container) | 10,000+ (vault) | Less frequent use |
| Survival | 200 (locker) | 1,000 (container) | 5,000+ (container) | Lower priority |

---

## Storage Visual Indicators

### Fill Level Colors

All storage UI displays fill level with color coding:
- **Green:** 0-33% full — Normal operation
- **Yellow:** 34-66% full — Monitor closely
- **Orange:** 67-90% full — Approaching capacity
- **Red:** 91-100% full — Full, production will stop when reached
- **Flashing Red:** 100% — Overflow active, resources being lost

### Proximity Bonuses

No proximity bonuses exist between storage and buildings (independent system). However:
- Storage near Command Center shows unified inventory HUD [See building design sheets](../)
- Storage within base walls protected from environmental damage
- Outdoor storage subject to biome effects (flood, fire, etc.)

---

## See Also

- [Resource Types](../../resources/README.md) — Resource types and capacities
- [Grid Placement](./Grid-Placement.md) — Building grid placement rules
- [Building Tiers](./Building-Tiers.md) — Storage buildings by tier
- [Ship Modules](../../ShipModules/) — Mothership storage modules
- [Dropship](../../Spaceship/Dropship/README.md) — Starting dropship storage
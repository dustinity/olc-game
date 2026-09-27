# Resource 1: Energy

### Purpose
Powers all buildings, ships, and weapon systems. Without energy, structures cease operation and ship systems shut down.

[Back to Resources Overview](../README.md)

---

### Generation Methods by Biome

| Biome | Solar Panels | Wind Turbines | Coal Reactors | Water Turbines | Geothermal Vents | Atomic Reactors |
|-------|:------------:|:-------------:|:-------------:|:--------------:|:----------------:|:---------------:|
| Desert | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | — | ⭐⭐ | ⭐⭐⭐⭐⭐ |
| Dusty | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | — | ⭐⭐ | ⭐⭐⭐⭐ |
| Rocky | ⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ | — | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| Water | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐ | ⭐⭐⭐⭐⭐ | — | ⭐⭐⭐⭐ |
| Swamp | ⭐⭐ | ⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐ | ⭐⭐⭐ |
| Jungle | ⭐ | ⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐ |
| Light Snow | ⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | — | ⭐⭐⭐ | ⭐⭐⭐⭐ |
| Ice | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | — | ⭐⭐ | ⭐⭐⭐⭐⭐ |

### Energy Output Values (per generation unit)

| Generation Type | Output (energy/turn) | Maintenance Cost | Notes |
|----------------|---------------------|------------------|-------|
| Solar Panel | 20 energy/turn | None | -50% in Jungle, +20% in Desert/Ice |
| Wind Turbine | 15-30 energy/turn | None | Scales with wind speed biome modifier |
| Coal Reactor | 40 energy/turn | 5 construction material/turn | Requires coal mineral input |
| Water Turbine | 35 energy/turn | None | Requires Water or Swamp biome |
| Geothermal Vent | 100 energy/turn | 10 construction material/turn | Rocky, Desert, Light Snow biomes only |
| Atomic Reactor | 60 energy/turn | 2 minerals/turn | Stable in all biomes |

### Energy Consumption by System

| System | Energy Cost/Turn | Notes |
|--------|-----------------|-------|
| Life Support (Dropship) | 10 energy/turn | Powers air, temperature, pressure |
| Ship Systems & Instruments | 5 energy/turn | Navigation, displays, computers |
| Cryo-Stasis Pods | 8 energy/turn | Per 10 soldiers in stasis |
| Lights & HVAC | 7 energy/turn | Interior lighting and climate |
| Ballistic Turret (active) | 5 energy/turn | Standby mode draws 2 energy/turn |
| Plasma Cannon (active) | 15 energy/turn | Overheat after 45 seconds continuous fire |
| Radar Array (planet base) | 10 energy/turn | Short-medium range scan |
| Deep Sonar Array | 25 energy/turn | Underground structure detection |

---

## See Also

- [Fuel](../Fuel/README.md) — Fuel can be converted to Energy at 70% yield
- [Resource generation buildings](../../buildings/README.md)
- [Battery storage (Ship Modules)](../../ShipModules/README.md)

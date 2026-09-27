# Resource 2: Fuel

### Purpose
Propellant for interplanetary travel and vehicle operation. Each full fuel tank (1x1 = 1 hop) enables one jump between planets within the same solar system.

[Back to Resources Overview](../README.md)

---

### Fuel Types and Efficiency

| Fuel Type | Efficiency Rating | Source Biome | Processing Required | Output per Cycle |
|-----------|------------------|--------------|-------------------|-----------------|
| Chemical Propellant (Basic) | 1x baseline | All biomes (scavenged) | None | 30 fuel units |
| Crude Oil Refined | 1.5x baseline | Water, Swamp, Desert | Oil Pump required | 50 fuel units |
| Gas Extract | 1.2x baseline | Dusty, Rocky | Gas Extractor required | 30 fuel units |
| Biofuel | 2x baseline | Jungle, Water, Swamp | Biofuel Processor required | 80 fuel units |
| Frozen Methane | 2.5x baseline | Ice, Light Snow | Methane Compressor required | 120 fuel units |
| Dark Matter Fuel | 3x baseline | Crystal Caves (dungeon loot) | Crystal Synthesizer required | 200 fuel units |

### Fuel Consumption by Travel Type

| Travel Type | Fuel Required | Notes |
|-------------|--------------|-------|
| Intra-system hop (1 planet to another) | 100 units (baseline) | -40% with Atomic Drive, -60% with Ionic Drive |
| Inter-system hop (leave solar system) | 500 units (baseline) | Same drive modifiers apply |
| Selected planet landing (precise) | 500-750 units | 1-2x more depending on distance from orbital path |
| Wormhole transit (Void Warp Drive) | 10 units | Within 5-system radius [See Navigation](../../Planets/Navigation/README.md) |
| Alien warp transit | 0.01 units | Effectively free, unlimited range [See Navigation](../../Planets/Navigation/README.md) |

### Fuel Generation Buildings [See Planet Buildings](../../buildings/README.md)

| Building | Grid Size | TIR Required | Output | Energy Cost/Turn |
|----------|-----------|--------------|--------|-----------------|
| Oil Pump | 2x2 | TIR 1 | 50 fuel/cycle from crude oil | None |
| Gas Extractor | 2x2 | TIR 1 | 30 fuel/cycle from gas deposits | None |
| Biofuel Processor | 3x3 | TIR 2 | 80 fuel/cycle from organic matter | 20 energy/turn |
| Methane Compressor | 3x3 | TIR 2 | 120 fuel/cycle from ice deposits | 30 energy/turn |
| Crystal Synthesizer | 3x3 | TIR 4 | Converts raw energy to dark matter crystals | 50 energy/turn input |

---

## See Also

- [Energy](../Energy/README.md) — Fuel converts to Energy at 70% yield (instant, no cost)
- [Navigation and travel costs](../../Planets/Navigation/README.md)
- [Fuel tanks (Dropship)](../../Spaceship/Dropship/README.md)

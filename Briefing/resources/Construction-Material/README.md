# Resource 3: Construction Material

### Purpose
Primary building material for structures on planets and ship modules. Required for all construction, repair, and expansion activities.

[Back to Resources Overview](../README.md)

---

### Construction Material Types and Sources

| Type | Source Method | Biome Modifier | Processing Required |
|------|--------------|----------------|-------------------|
| Stone | Basic mine | +20% in Rocky, Desert | None (raw output) |
| Concrete | Mine output + processing | Standard | Cement mixer at base |
| Plastic | Factory production | Standard | 50 construction → 40 plastic (80% yield) |
| Carbon | Factory production or mine | +15% in Swamp, Jungle | Carbon extractor or factory synthesis |

### Construction Material Output Values

| Source | Output per Cycle | Time per Cycle | Energy Cost |
|--------|-----------------|----------------|-------------|
| Manual collection (hand mining) | 5 material/cycle | 2 minutes | None |
| Basic mine (stone deposits) | 10 material/cycle | 5 minutes | None |
| Concrete mixer | 8 material/cycle | 3 minutes | 10 energy/turn |
| Factory production line | 20 material/cycle | 10 minutes | 30 energy/turn |
| Assembly Plant (3x multiplier) | 60 material/cycle | 10 minutes | 90 energy/turn |

### Construction Material Costs by Activity [See Planet Buildings](../../buildings/README.md)

| Activity | Material Cost | Notes |
|----------|--------------|-------|
| Wall segment (2m) | 80 construction material | 200 HP, blocks infantry/light vehicles |
| Gate (opening/closing) | 150 construction material | 100 HP, control panel required |
| Basic mine placement | 100 construction material | 2x2 grid slot |
| Solar array installation | 80 construction material per panel | 2x1 grid slot per panel |
| Oil pump construction | 200 construction material | 2x2 grid slot, TIR 1 required |
| Factory construction | 800 construction material | 4x4 grid slot, TIR 2 required |
| Container storage (2x2) | 300 construction material | Wheel-shaped rotation [See Storage System](../../buildings/Reference/Storage-System.md) |

---

## See Also

- [Minerals](../Minerals/README.md) — Converts to/from Construction Material at 70% yield
- [Planet Buildings](../../buildings/README.md)
- [Storage System](../../buildings/Reference/Storage-System.md)

# Hull Plating Assets

Hull plating is the mothership builder grid base material. Players choose a hull tier first, then place ship modules on top of that hull surface.

## Planned Hull Tiers

| Module ID | Name | TIR | Role | File | Status |
|---|---|---:|---|---|---|
| SM-HL-01 | Used Rusty Hull | 1 | Starting damaged/rusty grid base | `SM-HL-01-Used-Rusty-Hull.png` | Review |
| SM-HL-02 | Standard Steel Hull | 2 | More solid standard grid base | `SM-HL-02-Standard-Steel-Hull.png` | Current image |
| SM-HL-03 | Heat-Resistant Alloy Hull | 3 | Hot-entry resistant grid base | `SM-HL-03-Heat-Resistant-Alloy-Hull.png` | Generated |
| SM-HL-04 | Titanium Alloy Hull | 4 | High-strength clean armor grid base | `SM-HL-04-Titanium-Alloy-Hull.png` | Generated |
| SM-HL-05 | Reinforced Composite Hull | 5 | Advanced carbon-fiber composite grid base with restrained light cyan neon seams | `SM-HL-05-Reinforced-Composite-Hull.png` | Replaced with BP-15 carbon/cyan direction |

## Blueprint Hull Variants

These are optional hull ground materials found through dungeon blueprints, faction/race encounters, or alien artifact decoding. Most should be sidegrades or specialist skins, not replacements for the main TIR ladder. The Void Alien Hull is the exception: it is a TIR 6 high-end center-galaxy reward.

| Module ID | Name | TIR | Source | Visual Direction | File | Status |
|---|---|---:|---|---|---|---|
| SM-HL-BP-01 | Void Alien Hull | 6 | Center galaxy / Alien Artifact Decoder high-end reward | Dark gunmetal with deep purple void seams, black ceramic plates, faint starlight flecks, non-human geometry kept flat and grid-readable | SM-HL-BP-01-Void-Alien-Hull.png | Generated |
| SM-HL-BP-02 | Crystalloid Prism Hull | Blueprint | Crystalloid blueprint | Gunmetal plates fractured by crystalline veins, prism facets, pale cyan/violet glow restrained to seams | TBD | Planned |
| SM-HL-BP-03 | Insectoid Chitin Hull | Blueprint | Insectoid blueprint | Segmented chitin armor over steel backing, overlapping shell plates, amber resin seams, organic pattern but hard-surface readable | TBD | Planned |
| SM-HL-BP-04 | Molluskoid Shell Hull | Blueprint | Molluskoid blueprint | Shell/clam-inspired layered armor, nacre dark pearl panels, acid-resistant gasket channels, subtle bioluminescent dots | TBD | Planned |
| SM-HL-BP-05 | Reptilian Scale Hull | Blueprint | Reptilian blueprint | Interlocking scale-like metal plates, heat/cold terrain adaptation markings, dark green-gray/bronze undertones | TBD | Planned |
| SM-HL-BP-06 | Bloody Rust Hull | 2 | Rustborn Clan blueprint | Worn gunmetal steel, rusty welded patch plates, scratches, dents, dried dark red stains, brutal outlaw repairs kept flat and grid-readable | SM-HL-BP-06-Bloody-Rust-Hull.png | Generated |
| SM-HL-BP-07 | Mech Industrial Hull | 2 | Machine / mech blueprint | Thick interlocking titanium armor, reinforced ribs, recessed bolts, heat vents, magnetic seams, sensor modules, white markings, and cyan conduits under narrow gaps | SM-HL-BP-07-Mech-Industrial-Hull.png | Replaced with BP-12 mech faction direction |
| SM-HL-BP-08 | Civilian Utility Hull | 2 | Civilian / colony blueprint | Practical maintained gunmetal and muted blue-gray panels, access hatches, clean service seams, cargo-ship modular construction | SM-HL-BP-08-Civilian-Utility-Hull.png | Generated |
| SM-HL-BP-09 | Corporate Security Hull | 2 | Corporate security / military police blueprint | Corporate security symmetry mixed with military-police authority: white ceramic armor blocks, dark gunmetal frame, blue center stripe, shield badge, corner security markings, blue LEDs | SM-HL-BP-09-Corporate-Security-Hull.png | Replaced with BP-13 hybrid direction |
| SM-HL-BP-10 | Scrapper Clan Hull | 2 | Scavenger / scrapper blueprint | Mismatched salvaged plates, reused panels, uneven bolts, patched seams, faded paint and warning-color fragments without readable text | SM-HL-BP-10-Scrapper-Clan-Hull.png | Generated |
| SM-HL-BP-11 | Military Police Hull | 2 | Military Outposts / police blueprint | Matte titanium-gray alloy with white ceramic armor sections, dark gunmetal framing, blue horizontal stripe, shield insignia, maintained patrol wear, blue LEDs and sensor nodes | SM-HL-BP-11-Military-Police-Hull.png | Generated |
| SM-HL-BP-12 | SciFi Mech Faction Armor Plate | 2 | Machine / mech blueprint | Source comparison image used to replace SM-HL-BP-07 | SM-HL-BP-12-SciFi-Mech-Faction-Armor-Plate.png | Superseded |
| SM-HL-BP-13 | Corporate Military Police Hull | 2 | Corporate security / military police blueprint | Source comparison image used to replace SM-HL-BP-09 | SM-HL-BP-13-Corporate-Military-Police-Hull.png | Superseded |
| SM-HL-BP-14 | Scrapper Clan Hull Plate | 2 | Scavenger / scrapper blueprint | More rugged comparison variant with salvaged starship plates, rough welds, oxidation, scrap mesh, embedded chain segments, faded hazard paint, abstract clan marks, and orange maintenance lights | SM-HL-BP-14-Scrapper-Clan-Hull-Plate.png | Generated |
| SM-HL-BP-15 | Carbon Cyan Composite Hull | 5 | Reinforced Composite comparison | Source comparison image used to replace SM-HL-05 | SM-HL-BP-15-Carbon-Cyan-Composite-Hull.png | Superseded |

## Usage Notes

- Hull tiles are not normal draggable ship modules.
- Empty valid hull cells render with the selected hull material.
- Functional modules sit above the hull material.
- Damaged cells can tint, crack, scorch, or expose the hull tile.
- Keep hull tiles mostly flat so placed modules remain readable.
- Blueprint variants may change resistances or biome bonuses, but should keep the same gameplay footprint as hull base tiles.

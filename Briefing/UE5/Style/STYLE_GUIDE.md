# Stil-1 Style Guide

## Dark Realistic Hard Sci-Fi RTS Asset Style

**Version:** 1.0  
**Last Updated:** 2026-06-30  
**Category:** Dark Military-Industrial Aesthetic

---

# Table of Contents

1. [Style Overview](#style-overview)
2. [Camera Specifications](#camera-specifications)
3. [Visual Language](#visual-language)
4. [Color Palette](#color-palette)
5. [Materials & Textures](#materials--textures)
6. [Lighting Requirements](#lighting-requirements)
7. [Architecture & Design Principles](#architecture--design-principles)
8. [AI Image Generation Guide](#ai-image-generation-guide)
9. [Asset Naming Convention](#asset-naming-convention)
10. [Technical Specifications](#technical-specifications)
11. [Asset Catalog](#asset-catalog)
12. [Modular Design Rules](#modular-design-rules)

---

## Style Overview

### Vision

> A dark, realistic hard sci-fi aesthetic for a AAA production RTS game. Buildings are chunky industrial architectures with large readable silhouettes, believable engineering, and military-industrial design language.

### Core Identity

| Aspect | Description |
|--------|-------------|
| **Genre** | Hard Sci-Fi / Military Industrial / RTS |
| **Mood** | Dark, functional, imposing, grounded |
| **Quality Bar** | AAA production concept art |
| **Engine Target** | Unreal Engine 5 RTS |
| **View Style** | Isometric concept sheet quality |

### Style Ratio

The Stil-1 aesthetic combines:

- **80% Dark Realistic Industrial** — Heavy machinery, armored plating, hydraulic systems, military-grade construction
- **20% Grounded Engineering** — Believable proportions, functional design, practical architecture

### What It Feels Like

- Dangerous frontier with active industry
- Military protection and security
- Constant expansion through engineering
- Humans winning through logistics and heavy industry
- NOT dystopian decay — actively operating infrastructure
- Darker and more imposing than the bright semi-realistic variant

---

## Camera Specifications

### Standard View Angles

| Property | Value |
|----------|-------|
| **Primary Angle** | 40° isometric (2.5D) |
| **Secondary Angles** | 90°, 180°, 270° for modular assets |
| **Camera Type** | High three-quarter isometric |
| **Perspective** | Orthographic/isometric projection |

### Camera Rules

- Fixed camera angle — do not bake motion or perspective distortion
- Assets designed for RTS top-down camera readability
- Centered composition on transparent background
- No terrain, characters, text, or UI elements
- Isolated asset presentation only

---

## Visual Language

### Architecture Style

**Chunky Industrial Architecture:**
- Large readable silhouettes
- Believable engineering principles
- Reinforced armor plating
- Thick structural beams
- Hydraulic machinery presence
- Visible maintenance platforms
- Exposed motors and mechanical systems

### Required Visual Elements

Elements that should appear across Stil-1 assets:

| Element | Purpose |
|---------|---------|
| Conveyors | Resource transport indication |
| Pipes | Industrial utility communication |
| Vents | Active machinery implication |
| Hydraulic systems | Mechanical complexity |
| Exposed motors | Engineering authenticity |
| Maintenance platforms | Scale and accessibility |
| Warning lights | Gameplay signaling |
| Armor plating | Military aesthetic |

### Shape Language

**Preferred Shapes:**
- Rectangles and chamfered edges
- Industrial cylinders (tanks, pipes, stacks)
- Support frames and trusses
- Reinforced corners and structural nodes
- Boxier, more angular than bright variant

**Avoid:**
- Fantasy curves
- Organic architecture
- Overly rounded designs
- Decorative or non-functional shapes
- Excessive micro-details at RTS scale

### Readability Requirements

For RTS gameplay:
- Strong silhouette identifiable from distance
- Primary function visible within 1 second
- Large readable shapes over fine detail
- Distinct color accents for gameplay elements
- Unique roof/top shapes for camera recognition

---

## Color Palette

### Primary Colors

| Color | Usage |
|-------|-------|
| Dark Gunmetal Steel | Primary building material |
| Matte Painted Metal (Dark) | Surface finish |
| Charcoal / Near-Black | Structural elements |
| Dark Gray | Concrete and foundations |
| Weathered Steel | Accent weathering |

### Accent Colors

| Color | Purpose |
|-------|---------|
| Warm Orange | Utility lights, warning indicators |
| Amber | Status lamps, hazard lighting |
| Blue | Energy indicators, reactor glow |
| Red | Alarm lights, critical systems |
| Green | Status LEDs, operational indicators |
| Cyan | Display interfaces (minimal) |
| Purple | Dark matter energy effects |
| Black/Dark Void | Void lab aesthetic |

### Variant Color Schemes

All variant buildings share the same **gray/basic color scheme** as the base Stil-1 aesthetic. Differences are communicated through **glow effects** and **accent lighting** only, maintaining visual consistency across the asset set.

| Variant Name | Base Colors | Glow Effect | Usage |
|-------------|-------------|-------------|-------|
| **Basic Forge** (Default) | Gray/dark gunmetal steel | Small orange/amber indicator lights | Standard production buildings |
| **Energy Lab** | Gray/dark gunmetal steel | Blue energy glow | Energy-related facilities, reactor rooms |
| **Dark Matter Lab** | Gray/dark gunmetal steel | Purple energy glow | Advanced research, dark matter processing |
| **Void Lab** | Near-black/void black | Dark void effect with subtle purple edges | Experimental facilities, void research |

#### Variant Color Implementation Guidelines

```
┌──────────────────────────────────────────────────────────┐
│  VARIANT COLOR REFERENCE                                 │
├──────────────┬──────────────────┬───────────────────────┤
│ Variant      │ Base Colors      │ Glow/Accent           │
├──────────────┼──────────────────┼───────────────────────┤
│ Basic Forge  │ Gray, gunmetal   │ Small orange/amber    │
│ Energy Lab   │ Gray, gunmetal   │ Blue energy glow      │
│ Dark Matter  │ Gray, gunmetal   │ Purple energy glow    │
│ Void Lab     │ Near-black void  │ Subtle purple edges   │
└──────────────┴──────────────────┴───────────────────────┘
```

**Implementation Rules:**
- Base structure colors remain consistent (gray/dark gunmetal) across all variants
- Only glow effects, emissive materials, and small accent lights change per variant
- Void Lab uses near-black base with subtle purple edge lighting for void aesthetic
- Energy Lab uses blue glow for energy-related facilities
- Dark Matter Lab uses purple glow for advanced research facilities
- All variants must remain visually cohesive when placed adjacent to each other

### Saturation & Tone

- **Low to medium saturation** — avoid cartoon colors
- **High contrast** between light and shadow areas
- **Controlled edge wear** — not heavily corroded
- Subtle weathering only — actively maintained infrastructure
- Dark overall tone with strategic bright accents

---

## Materials & Textures

### Metal Surfaces

| Material | Appearance |
|----------|------------|
| Gunmetal Steel | Primary structural material, dark matte finish |
| Painted Steel | Coated surfaces with controlled edge wear |
| Brushed Aluminum | Secondary panels and housings |
| Titanium | High-stress structural components |
| Composite Armor | Military-grade plating |

### Surface Treatment

**Allowed Wear:**
- Subtle weathering on edges
- Controlled edge wear
- Minor scratches from operation
- Light dust accumulation
- Heat discoloration near exhausts
- Oil stains near machinery

**Not Allowed:**
- Heavy corrosion or rust
- Post-apocalyptic decay appearance
- Abandoned aesthetics
- Overly destroyed look
- Excessive dirt buildup

### PBR Materials

All materials should follow realistic PBR-inspired workflow:
- Realistic metal roughness values
- Proper specular response
- Subsurface scattering for glass elements
- Normal mapping for panel detail
- ORM (Occlusion-Roughness-Metallic) packed textures

---

## Lighting Requirements

### Primary Lighting Setup

| Property | Value |
|----------|-------|
| **Ambient** | Overcast diffuse illumination |
| **Key Light** | Low-key cinematic directional |
| **Fill Light** | Warm orange utility lights |
| **Contrast** | High contrast ratio |
| **Mood** | Dark, atmospheric, functional |

### Lighting Rules

- **Overcast ambient** — soft diffused fill, no harsh shadows from sky
- **Warm orange utility lights** — interior and exterior accent lighting
- **Low-key cinematic** — controlled highlights, deep but not black shadows
- **High contrast** — clear separation between elements
- **Crisp sharp details** — no bloom or motion blur baked in
- VFX effects (smoke, steam, particles) are animation layers, NOT baked

### Light Placement Guidelines

| Element | Light Type | Color |
|---------|-----------|-------|
| Warning beacons | Point light, small radius | Orange/Amber |
| Status indicators | Emissive material | Green/Blue/Red |
| Interior glow | Area light through windows | Warm white |
| Exhaust vents | VFX socket (no baked particles) | N/A |
| Reactor cores | Emissive blue glow (contained) | Blue |

---

## Architecture & Design Principles

### Building Categories

#### Power Generation
- Solar arrays with tracking mechanisms
- Wind turbines with rotating blades
- Coal reactors with visible fuel input
- Water turbines on aquatic tiles
- Central power plants with transformer yards

#### Resource Extraction
- Oil pumps (pumpjacks on service skids)
- Metal ore mines (massive drills entering ground)
- Harvester posts (passive collection structures)
- Mineral-specific VFX indicators (black=coal, yellow=sulfur, green=uranium)

#### Infrastructure
- Barracks with armored walls and blast doors
- Habitation modules (comfortable, modern appearance)
- Modular wall segments (4-directional for base围护)
- Gates with barrier mechanisms
- Observation towers

#### Storage
- Lockers (1×1, basic personal storage)
- Containers (2×2 or 4×1 shape-changing)
- Haul storage facilities (4×4 large-scale)

### Design Philosophy

Every asset should answer these questions instantly:
- What is its purpose?
- Who built it?
- How does it function?
- Is it civilian, industrial, or military?
- Can the player recognize it from a high camera angle?

If the answer to all five is "yes," the asset fits the visual style.

---

## AI Image Generation Guide

### Master Prompt Template

Use this prompt as the foundation for all Stil-1 asset generation:

```
Dark realistic hard sci-fi RTS building, 2.5D isometric view (40°), 
AAA production concept art, chunky industrial architecture with large 
readable silhouettes, believable engineering, reinforced armor plating, 
hydraulic machinery, conveyors, exposed motors, vents, pipes, maintenance 
platforms, warning lights, thick structural beams, dark gunmetal steel 
with subtle weathering, matte painted metal, controlled edge wear, realistic 
PBR-inspired materials, low-key cinematic lighting, overcast ambient 
illumination, warm orange utility lights, high contrast, crisp sharp details, 
restrained color palette, grounded military-industrial aesthetic, designed 
for Unreal Engine 5 RTS, isolated asset on transparent background, centered 
composition, no terrain, no characters, no text, no UI, production-ready 
concept sheet quality.
```

### Prompt Customization Rules

1. **Always include** the master prompt as base context
2. **Add specific asset description** BEFORE the master prompt
3. **Specify modular requirements** if asset is part of a set
4. **Note biome variants** if applicable (Desert, Snow, Swamp, Ice)
5. **State animation states** if generating keyframes

### Example Customized Prompts

#### Oil Pump
```
Generate a basic oil pump in the style of dark realistic hard sci-fi RTS assets. Compact pumpjack on a rectangular service skid: near-black plated steel, broad readable machinery, tiny amber lamps, and a high three-quarter isometric camera.
[+ master prompt]
```

#### Power Plant
```
Build a power plant asset matching the established oil pump style. Squat armored generator hall with twin capped stacks, external transformer hardware, and the same amber-on-gunmetal treatment.
[+ master prompt]
```

#### Modular Wall Segment
```
Generate a wall segment that could be used to build a defense around the base. Modular connector ends so segments can form a perimeter, clean armor plating with mirrored connection posts.
[+ master prompt]
```

### Generation Best Practices

| Practice | Reason |
|----------|--------|
| Use chroma-key background | Enables clean alpha extraction |
| Green or magenta key | Choose based on asset color content |
| Generate at highest resolution | RTS assets need detail at distance |
| Validate transparency edges | Prevent green spill in-engine |
| Keep VFX separate | Smoke, steam, particles are animation layers |

### Chroma-Key Extraction

- **Green screen (#00FF00)** — Standard extraction for most assets
- **Magenta screen (#FF00FF)** — Use when asset contains green elements (vegetation, murky water)
- Always validate alpha matte after extraction
- Check thin geometry edges (antennas, blades, rods) for matte holes

---

## Asset Naming Convention

### File Naming Pattern

```
[Category]-[Type]-[Variant]-[Detail].[extension]
```

### Category Codes

| Code | Category |
|------|----------|
| PB | Production Building (main assets) |
| PB-PG | Power Generation |
| PB-EX | Resource Extraction |
| PB-IN | Infrastructure / Buildings |
| PB-ST | Storage |
| PB-MF | Military / Factory |

### Type Codes

| Code | Meaning |
|------|---------|
| 01, 02, 03... | Sequential asset identifier |
| Main | Primary structure |
| Wall | Wall/segment components |
| Tower | Tower structures |

### Variant Suffixes

| Suffix | Meaning |
|--------|---------|
| -desert | Desert biome variant |
| -snow | Snow/Ice biome variant |
| -ice | Ice accumulation variant |
| -swamp | Swamp contamination variant |
| -090, -180, -270 | Rotation angle for modular assets |

### Component Suffixes

| Suffix | Meaning |
|--------|---------|
| -Main | Primary structure |
| -Prop-* | Propellant/prop component |
| -Mech-* | Mechanical state (closed/open) |
| -Anim-* | Animation keyframe state |
| -VFX-* | VFX attachment point indicator |
| -Config-* | Configuration variant |
| -Damage-* | Damage/destruction state |

### Examples

```
PB-PG-01-solar-array.png           — Main solar array
PB-PG-01-solar-array-desert.png    — Desert variant
PB-IN-04-wall-segment-090.png      — Wall segment, 90° rotation
PB-ST-02-Anim-ShapeChange.png      — Shape change animation keyframe
PB-EX-03-Main.png                  — Main extraction post
B-PG-04-water-turbine.png          — Water turbine (alternate prefix)
```

---

## Technical Specifications

### Output Format

| Property | Value |
|----------|-------|
| **Format** | PNG (RGBA) |
| **Alpha Channel** | Required — transparent background |
| **Resolution** | 1254×1254 to 1600×1536 (varies by asset proportions) |
| **Color Space** | sRGB |
| **Compression** | Lossless |

### Grid Footprints

| Footprint Size | Usage | Examples |
|----------------|-------|----------|
| 1×1 | Small structures, posts, towers | Oil pump, gate, water turbine |
| 2×1 | Extended structures | Solar array, wind turbine |
| 2×2 | Medium buildings | Barracks, power plant, coal reactor |
| 4×4 | Large facilities | Haul storage, major factories |

### Modular Asset Rules

For assets that form perimeters or extended structures:

- Generate all cardinal directions (0°, 90°, 180°, 270°)
- Connector ends must be mirrored and compatible
- Same geometry across rotations — only orientation changes
- Lighting direction stays consistent across all views
- Maintain identical height and base alignment

### Animation Preparation

Assets designed for animation should follow these rules:

| Rule | Implementation |
|------|----------------|
| No baked motion | Never include motion blur or particle effects |
| Clear attachment points | VFX outlets visible but empty |
| Keyframe states | Generate open/closed/intermediate as separate assets |
| Separate mechanical parts | Blast doors, rotors, and moving elements identifiable |

---

## Asset Catalog

### Generated Assets

#### Power Generation (PB-PG)

| Asset ID | Name | Footprint | Status |
|----------|------|-----------|--------|
| PB-PG-01 / B-PG-01 | Solar Array | 2×1 | ✅ Generated |
| PB-PG-01-desert / B-PG-01-desert | Solar Array (Desert Variant) | 2×1 | ✅ Generated |
| PB-PG-01-snow / B-PG-01-snow | Solar Array (Snow Variant) | 2×1 | ✅ Generated |
| PB-PG-02 / B-PG-02 | Wind Turbine | 2×1 | ✅ Generated |
| PB-PG-02-ice / B-PG-02-ice | Wind Turbine (Ice Variant) | 2×1 | ✅ Generated |
| PB-PG-03 / B-PG-03 | Coal Reactor | 2×2 | ✅ Generated |
| PB-PG-06 | Power Plant | — | ✅ Generated |
| B-PG-04 | Water Turbine | 1×1 | ✅ Generated |
| B-PG-04-swamp | Water Turbine (Swamp Variant) | 1×1 | ✅ Generated |

#### Resource Extraction (PB-EX)

| Asset ID | Name | Footprint | Status |
|----------|------|-----------|--------|
| PB-EX-01 | Metal Ore Mine | 1×1 | ✅ Generated |
| PB-EX-02 | Oil Pump | 1×1 | ✅ Generated |
| PB-EX-03-Main | Harvester Post Main | 1×1 | ✅ Generated |
| PB-EX-03-Prop-DustCollector | Dust Collector Prop | — | ✅ Generated |
| PB-EX-03-Prop-CondensationTower | Condensation Tower Prop | — | ✅ Generated |
| PB-EX-03-Prop-Antenna | Antenna Prop | — | ✅ Generated |
| PB-EX-03-Prop-StorageContainer | Storage Container Prop | — | ✅ Generated |

#### Infrastructure (PB-IN)

| Asset ID | Name | Footprint | Status |
|----------|------|-----------|--------|
| PB-IN-01-Main | Barracks Main Building | 2×2 | ✅ Generated |
| PB-IN-01-Wall-Armored | Armored Wall Segment | — | ✅ Generated |
| PB-IN-01-Prop-VehicleParking | Vehicle Parking Prop | — | ✅ Generated |
| PB-IN-01-Prop-TrainingEquipment | Training Equipment Prop | — | ✅ Generated |
| PB-IN-01-Tower-Observation | Observation Tower | — | ✅ Generated |
| PB-IN-01-Mech-BlastDoors | Blast Doors (Closed) | — | ✅ Generated |
| PB-IN-01-Prop-FlagPole | Flag Pole Prop | — | ✅ Generated |
| PB-IN-01-Anim-UnitEntrance | Unit Entrance (Open State) | — | ✅ Generated |
| PB-IN-02-Main | Habitation Module Main | 2×2 | ✅ Generated |
| PB-IN-02-Roof-Rounded | Rounded Roof Component | — | ✅ Generated |
| PB-IN-02-Prop-Windows | Illuminated Windows Prop | — | ✅ Generated |
| PB-IN-02-Prop-AirConditioning | AC Units Prop | — | ✅ Generated |
| PB-IN-02-Prop-WalkwayConnectors | Walkway Connector Prop | — | ✅ Generated |
| PB-IN-02-Prop-RooftopGarden | Rooftop Garden Prop | — | ✅ Generated |
| PB-IN-02-Prop-SolarPanelMount | Solar Panel Mount Prop | — | ✅ Generated |
| PB-IN-04-wall-segment | Wall Segment (0°) | 1×1 | ✅ Generated |
| PB-IN-04-wall-segment-090 | Wall Segment (90°) | 1×1 | ✅ Generated |
| PB-IN-04-wall-segment-180 | Wall Segment (180°) | 1×1 | ✅ Generated |
| PB-IN-04-wall-segment-270 | Wall Segment (270°) | 1×1 | ✅ Generated |

#### Storage (PB-ST)

| Asset ID | Name | Footprint | Status |
|----------|------|-----------|--------|
| PB-ST-01-Main | Locker Main | 1×1 | ✅ Generated |
| PB-ST-01-Anim-DoorOpen | Door Open Animation State | — | ✅ Generated |
| PB-ST-01-VFX-StatusLight | Status Light VFX Prop | — | ✅ Generated |
| PB-ST-01-Prop-LabelSlot | Label Slot Prop | — | ✅ Generated |
| PB-ST-02-Config-2x2 | Container 2×2 Config | 2×2 | ✅ Generated |
| PB-ST-02-Config-4x1 | Container 4×1 Config | 4×1 | ✅ Generated |
| PB-ST-02-Anim-Rotate | Rotation Mechanism | — | ✅ Generated |
| PB-ST-02-Anim-ShapeChange | Shape Change Keyframe | — | ✅ Generated |
| PB-ST-02-Mech-LoadingDoor | Loading Door (Mech) | — | ✅ Generated |
| PB-ST-02-Prop-CornerPosts | Corner Posts Prop | — | ✅ Generated |
| PB-ST-03-Main | Haul Storage Main | 4×4 | ✅ Generated |
| PB-ST-03-Prop-ContainerSlots | Container Slots Prop | — | ✅ Generated |
| PB-ST-03-Prop-CraneForkliftPath | Crane/Forklift Path Prop | — | ✅ Generated |
| PB-ST-03-Prop-LoadingDock | Loading Dock Prop | — | ✅ Generated |
| PB-ST-03-Prop-SecurityFencing | Security Fencing Prop | — | ✅ Generated |
| PB-ST-03-VFX-LightingSystem | Lighting System VFX | — | ✅ Generated |

---

## Modular Design Rules

### Module Library

Every building consists of reusable modules:

| Module Type | Description | Usage |
|-------------|-------------|-------|
| Wall | Standard wall segment with connectors | Perimeter, interior division |
| Corner | 90° corner piece | Building corners |
| Roof | Flat/sloped roof sections | Top coverage |
| Door | Standard/hatch doors | Entry points |
| Window | Illuminated window panels | Civil buildings |
| Pipe | Connection pipes | Industrial utilities |
| Vent | Exhaust/cooling vents | Industrial buildings |
| Platform | Maintenance walkways | Access points |
| Support Beam | Structural supports | Framework |
| Foundation | Base platform | Ground connection |
| Power Module | Generator/reactor housing | Energy production |
| Storage Module | Container/storage unit | Resource holding |
| Tower Module | Vertical observation/comms | Surveillance |

### Module Integration Rules

- All modules share the same material language (gunmetal steel, dark painted metal)
- Connector points are standardized across wall and platform modules
- Height increments follow 50cm/100cm grid alignment
- Color accents (warning lights, indicators) appear at consistent locations
- Modular assets maintain identical base height for seamless placement

---

## Quick Reference Card

### Stil-1 in One Glance

```
┌─────────────────────────────────────────────┐
│  STYLE: Dark Realistic Hard Sci-Fi          │
│  QUALITY: AAA Production Concept Art        │
│  CAMERA: 40° Isometric (2.5D)               │
│  ENGINE: Unreal Engine 5 RTS                │
├─────────────────────────────────────────────┤
│  COLORS:                                    │
│  Primary: Dark gunmetal, charcoal           │
│  Accent: Orange/amber lights, blue glow     │
│  Tone: Low saturation, high contrast        │
├─────────────────────────────────────────────┤
│  MATERIALS:                                 │
│  Gunmetal steel, painted metal, PBR         │
│  Subtle weathering only                     │
├─────────────────────────────────────────────┤
│  LIGHTING:                                  │
│  Overcast ambient + warm orange utility     │
│  Cinematic contrast, crisp details          │
├─────────────────────────────────────────────┤
│  AVOID:                                     │
│  Heavy corrosion, fantasy curves,           │
│  Organic architecture, baked VFX            │
└─────────────────────────────────────────────┘
```

---

## Appendix: Comparison with Other Styles

| Feature | Stil-1 (Dark) | Stil-4 (Bright) |
|---------|---------------|-----------------|
| **Tone** | Dark, imposing | Bright, optimistic |
| **Primary Colors** | Gunmetal, charcoal | White, light steel gray |
| **Accent Colors** | Orange/amber warning lights | Blue holograms, cyan displays |
| **Weathering** | Subtle operational wear | Minimal, clean appearance |
| **Lighting** | Low-key cinematic | Natural daylight dominant |
| **Architecture** | Chunky military industrial | Clean modern functional |
| **Mood** | Dangerous frontier | Thriving colony |

---

*End of Stil-1 Style Guide v1.0*
from __future__ import annotations

import os
from pathlib import Path

from PIL import Image


# Resolve project root: from olc-game/scripts/ go up 2 to olc-game/, then up 1 to Projects/,
# then into ourlastchance/. Prefer env var override for flexibility.
if os.environ.get("OURLASTCHANCE_ROOT"):
    ROOT = Path(os.environ["OURLASTCHANCE_ROOT"])
else:
    # Default: assume sibling repos under a common parent
    _script_dir = Path(__file__).resolve().parent
    _projects_root = _script_dir.parents[2]  # olc-game/scripts/ → olc-game/ → Projects/
    ROOT = _projects_root / "ourlastchance"

if not ROOT.exists():
    print(f"WARNING: ourlastchance project root not found at {ROOT}")
    print(f"Set OURLASTCHANCE_ROOT env var to the correct path.")

# Hardcoded user-specific path from original generation session — override via env var.
_SHEET_DEFAULT = r"C:\Users\chris\.codex\generated_images\01a038ce-bfbe-79f2-8fd5-20ec4b14a430\call_ZSLUNQzcUuDayjGYVnrseE2I.png"
SHEET = Path(os.environ.get("WELCOME_FRAME_SHEET", _SHEET_DEFAULT))

OUT = ROOT / "Assets/UI/WelcomeScreen"


PARTS = {
    "Frame_Corner_TL.png": ((42, 36, 296, 274), (256, 256)),
    "Frame_Edge_Top.png": ((338, 68, 1198, 156), (768, 96)),
    "Frame_Corner_TR.png": ((1238, 36, 1488, 274), (256, 256)),
    "Frame_Edge_Left.png": ((64, 304, 142, 636), (96, 768)),
    "Frame_Edge_Right.png": ((1374, 304, 1456, 636), (96, 768)),
    "Frame_Corner_BL.png": ((50, 672, 292, 856), (256, 256)),
    "Frame_Edge_Bottom.png": ((318, 798, 1206, 876), (768, 96)),
    "Frame_Corner_BR.png": ((1248, 672, 1488, 856), (256, 256)),
    "Frame_Control_Panel.png": ((1024, 900, 1332, 1002), (420, 118)),
}


def remove_checker_background(image: Image.Image) -> Image.Image:
    result = image.convert("RGBA")
    pixels = result.load()
    for y in range(result.height):
        for x in range(result.width):
            r, g, b, a = pixels[x, y]
            # The image model rendered the transparent area as a pale checker.
            # Remove only the low-chroma light field; keep metal highlights.
            if r > 214 and g > 214 and b > 214 and max(r, g, b) - min(r, g, b) < 18:
                pixels[x, y] = (0, 0, 0, 0)
            elif a == 0:
                pixels[x, y] = (0, 0, 0, 0)
    return result


def trim_to_content(image: Image.Image, pad: int = 8) -> Image.Image:
    alpha = image.getchannel("A")
    box = alpha.getbbox()
    if not box:
        return image
    left, top, right, bottom = box
    left = max(0, left - pad)
    top = max(0, top - pad)
    right = min(image.width, right + pad)
    bottom = min(image.height, bottom + pad)
    return image.crop((left, top, right, bottom))


def clear_transparent_rgb(image: Image.Image) -> Image.Image:
    pixels = image.load()
    for y in range(image.height):
        for x in range(image.width):
            r, g, b, a = pixels[x, y]
            if a == 0:
                pixels[x, y] = (0, 0, 0, 0)
    return image


def main() -> None:
    if not SHEET.exists():
        print(f"WARNING: Input sheet not found at {SHEET}")
        print(f"Set WELCOME_FRAME_SHEET env var or place the generated image at the default path.")
        return

    sheet = Image.open(SHEET).convert("RGBA")
    OUT.mkdir(parents=True, exist_ok=True)
    for name, (box, size) in PARTS.items():
        part = remove_checker_background(sheet.crop(box))
        part = trim_to_content(part)
        part = part.resize(size, Image.Resampling.LANCZOS)
        clear_transparent_rgb(part).save(OUT / name)


if __name__ == "__main__":
    main()

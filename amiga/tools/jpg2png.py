#!/usr/bin/env python3
"""jpg2png.py IN.jpg OUT.png: the Amiga drawer's copy of a JPEG backdrop, as
an RGBA PNG (amiga/sdl_image_lite.c reads PNG only; RGBA keeps rows a
multiple of 4 bytes). The AmiCradle Team. GPL 3 or later."""
import sys
from PIL import Image

src, dst = sys.argv[1:3]
Image.open(src).convert("RGBA").save(dst, optimize=True)

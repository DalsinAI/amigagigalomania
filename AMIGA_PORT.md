Gigalomania for AmigaOS 3.x
===========================

Gigalomania 1.0.1, Mark Harman's free real-time strategy game in the style
of Mega-Lo-Mania: lead your tribe through the ages from 10000 BC to 2100 AD,
invent weapons, mine elements, build towers and conquer each island's
sectors. This is the AmiCradle Team's port to AmigaOS 3.x.

What you need
-------------

- A 68040 or 68060 with an FPU, AmigaOS 3.2, an RTG card (or the
  AmigaChrome lab).
- OpenGPU 0.6 or later: opengpu.library and LIBS:OpenGPU/SDL2.module
  (the game uses SDL 1.2, which the OpenGPU kit provides on SDL 2).
- AHI for sound.
- About 25 MB of disk (the music is most of it).

Playing
-------

Double-click Gigalomania, or from a Shell run it from anywhere: it moves
to its own drawer first. It opens a 640x480 screen (the game's 320x240
drawn at twice the size). Everything is played with the mouse: the left
button picks and places, the right button cancels and sends men back.
P pauses, Return answers the game's questions, and Esc or Q asks to quit
(in a game, first back to the menus).

Prefs, saved games and log.txt are written into the drawer.

How the port is made
--------------------

- Upstream's SDL 1.2 code path, unchanged except for small `__amigaos__`
  blocks in game.cpp, image.cpp and main.cpp.
- Screen: upstream tries 4x, then 3x, then 2x of 320x240 in full screen.
  On the Amiga it starts at 2x (640x480), then 1x.
- SDL_image 1.2: amiga/sdl_image_lite.c (from the Wizznic port) reads
  PNG with libpng. The game's two JPEG backdrops (starfield, stars) are
  turned into PNG for the drawer, and game.cpp asks for the .png names.
- SDL_mixer 1.2: amiga/sdl_mixer_lite.c (from the Wizznic port), with
  looping, pausing and fading channels added for the aircraft sounds.
  It plays WAV only, so the Ogg Vorbis effects become 16-bit PCM WAV and
  the music becomes IMA ADPCM WAV on the host (amiga/tools/ogg2wav.py),
  and game.cpp asks for the .wav names.
- amiga/amiga_rwops.c: SDL_RWFromFile on plain stdio (linked with
  --wrap). OpenGPU's sdl12-compat passes the call to SDL2.module, and in
  the lab it never returned for the first picture.
- The map list is sorted with a comparator that is now consistent when
  an island is compared with itself ("0mega"); libnix's qsort looped
  forever on upstream's.
- The screen is asked for as a 32-bit software surface (no
  SDL_HWPALETTE).
- amiga/amiga_start.c: moves into PROGDIR: and, at exit, deals with any
  screens SDL2.module 0.6 leaves (amiga/amiga_screens.c).
- amiga/pthread_stubs.c: what libstdc++'s exception code asks of POSIX
  threads, for a program with one thread.
- 256 KB stack (amiga/stack.c and the icon).

Known limits
------------

- Status: lab-tested (Instance-45, an emulated 68040 with an FPU, 5 Nov
  2026, after the 24-bit mask fix below): it loads in about 13 seconds,
  the menus answer clicks, a Single Island game on Alpha starts and
  runs (the men multiply), and "Save game and quit to desktop" writes
  autosave.sav and exits cleanly. The game screen draws at about 4 to 6
  frames a second there.
- As upstream: on the "choose how many men" page the right mouse button
  (held) raises the number of men and the left button lowers it.

- 24-bit surfaces: on a big-endian machine CreateMask() in image.cpp
  gave RGBA masks (red at 0xff000000) even for 24-bit surfaces, which do
  not fit in 24 bits, so sdl12-compat made them 8-bit paletted. Every
  "hi colour" picture and every popup background was then paletted, and
  each SDL_MapRGB and blit into one searched the palette (SDL_FindColor):
  loading took about three minutes and a hovered button's help popup
  stopped the game. CreateMask() now takes the depth and gives 24-bit
  masks (no alpha) for 24-bit surfaces.

- Network play (upstream's multiplayer test modes) is not built in.
- Speed on a real 68040 is not measured yet; the lab is an emulated
  68040.

Building
--------

    make -f amiga/Makefile.amigaos -j2

The defaults find GFOpenAmigaGCC 16.2.0-open4 (STOVE), the NDK, the
OpenGPU SDK (SDLKIT) and the Team's static libpng and zlib (DEPS); override
them on the command line as needed. The drawer is made in
dist/Gigalomania, with dist/Gigalomania.info beside it. Making the drawer
needs Python 3 with PIL and GStreamer (gst-launch-1.0 with vorbisdec,
audioconvert, audioresample and wavenc).

Data and licences
-----------------

The game is GPL 2 or later (COPYING); TinyXML is zlib-licensed. All the
data in the drawer ships with Gigalomania under free licences: the speech
samples CC0, the effects CC0, CC BY 3.0 or GPL, the music CC BY 3.0
(Sindre Froyn; "Warm Ink" by _ghost), the graphics CC0, CC BY 3.0,
CC BY-SA 3.0 or GPL. ReadMe-Gigalomania.html in the drawer, upstream's
readme, lists every file with its author and licence.

The amiga/ files are the AmiCradle Team's, GPL 3 or later (pthread_stubs.c
GPL 2 or later).

Upstream: https://sourceforge.net/projects/gigalomania/ (git:
https://git.code.sf.net/p/gigalomania/code), base v1.0.1,
f8832eab7c99ed2b89eb4dadb453b02613a79a86.

Gigalomania for AmigaOS 3.x
===========================

A port of Gigalomania 1.0.1 (Mark Harman's remake of Mega-Lo-Mania, SDL 2,
https://sourceforge.net/projects/gigalomania/, git commit
3bb9e19c7ab515d853fb5057be30e73dd27839d8), for AmigaOS 3.2 on a 68040 or
68060 with an FPU and an RTG screen, on GFOpenGPU's SDL 2.

Status: runs in an AmiCradle lab (a copy of Instance-32, AmigaOS 3.2);
see "Testing" at the end.


What you need
-------------

- A 68040 or 68060 with an FPU (not an LC040 or EC040), AmigaOS 3.2, an RTG
  card (or an AmiCradle instance).
- GFOpenGPU: gfopengpu.library and LIBS:GFOpenGPU/SDL2.module,
  SDL2_image.module (the PNG and JPEG pictures) and SDL2_mixer.module (the
  Ogg Vorbis music and the sounds), installed by OpenUp.
- About 28 MB of disk (13 MB of it music, 7.5 MB sounds), and plenty of
  free memory: the game keeps all its pictures (1897 of them) in memory.
- Patience at start: on the lab's 68040 the game takes about a minute to
  load its pictures and turn them into textures (the loading bar).


How to play
-----------

Run Install-Gigalomania from its icon (it asks where the Gigalomania
drawer goes, checks the CPU, the FPU and GFOpenGPU's modules, and keeps
your saved games on an update), or copy the drawer anywhere. Double-click
gigalomania, or from a Shell:

    cd Gigalomania
    gigalomania [fullscreen] [onemousebutton]

It opens a 640x480 window (the game's 320x240 twice the size);
"fullscreen" opens a 640x480 screen of its own instead. "onemousebutton"
plays with the left button only (as upstream describes in readme.html).

Pick a game type (tutorial, single island, all islands), then a player,
then an island and the number of men for it: right-click the man icon to
add men, left-click to take them away, then click the map. Build, research,
mine and fight as in Mega-Lo-Mania; readme.html (in the drawer) is
upstream's full manual.

- The mouse plays the game; P pauses.
- Esc, Q or Right Amiga+Q bring up the quit menu: "Save game and quit to
  desktop" leaves the game and keeps it in User/autosave.sav (it carries on
  from there next time), "Exit battle" gives the island up, "Cancel" goes
  back. The window's close gadget saves and quits at once.

Preferences (sound and music on or off), saved games and a log
(log.txt, log_old.txt) go into the drawer's User drawer.


Building
--------

GFOpenAmigaGCC 16.2.0-open6 (the os32-gcc16-open6 stove, which carries the
NDK and GFOpenGPU's SDL 2 kit):

    make -f amiga/Makefile.amigaos -j2 STOVE=/path/to/os32-gcc16-open6

The default STOVE is ~/AmiCradle/stoves/os32-gcc16-open6. Output:
build/amigaos/Gigalomania/ (gigalomania, gfx/, sound/, music/, islands/,
readme.html, ReadMe, COPYING.txt, Licences.txt, User/, with GlowIcons),
build/amigaos/Gigalomania.info and Install-Gigalomania with its icon
beside them. The icons come from amiga/icons/icons.json and the Installer
script from amiga/installer/installer.json (amicradle's
scripts/port_icons.py and port_installer.py).


What was changed
----------------

Upstream's files keep their Windows line ends. The changes are under
`__amigaos__` (or, for the 24-bit masks, `SDL_BIG_ENDIAN`), except the
RWops and qsort fixes below, which are right on every platform.

- amiga/Makefile.amigaos: upstream's file list, with the kit's SDL2,
  SDL2_image and SDL2_mixer. amiga/sdl.h answers stdafx.h's `<sdl.h>`.
- main.cpp, amiga/amiga_progdir.c: the program's drawer is made the
  current directory at start (the game names its data gfx/..., sound/...,
  music/..., islands/...), and given back at the end; the empty screens
  SDL2.module leaves are closed after SDL_Quit() (amiga/amiga_screens.c,
  from the Seven Kingdoms and Abbaye ports).
- utils.cpp: the player's files go to PROGDIR:User/ (made when first
  needed); "Platform: AmigaOS 3" in the log.
- game.cpp: a window by default and at most 640x480, as upstream does for
  AmigaOS 4; "fullscreen" opens a 640x480 screen rather than one of the
  Workbench's size (screen.cpp: SDL_WINDOW_FULLSCREEN, not _DESKTOP).
  The texture step logs its progress every 200 pictures.
- game.cpp, gamestate.cpp: the game called its RWops' read, write, size
  and close function pointers itself. Those functions are SDL2.module's and
  need its data register, so here they crashed or did nothing (the islands
  were never read). They now go through SDL_RWread, SDL_RWwrite,
  SDL_RWsize and SDL_RWclose, which is the same call on every platform.
- game.cpp, sortMapsFunc(): qsort's comparison answered "less" for an
  island compared with itself when its name starts with "0" (the island
  Ohm, 0mega.map); libnix's qsort compares an element with itself, and
  never finished. It now answers 0 there.
- image.cpp, convertToHiColor(): on a big-endian machine CreateMask()
  gives 32-bit masks (R in the top byte), which SDL_CreateRGBSurface()
  refuses for a 24-bit surface; every picture came out NULL, so the first
  texture failed and the game ended. 24-bit surfaces get 24-bit masks
  now, and a failure is reported rather than used.
- amiga/stack.c: 256 KB of stack (the icon asks for it too) and a $VER
  string.

Frame pacing is upstream's: Application::wait() holds each frame to at
least 16 ms with SDL_Delay, and the game runs on elapsed time.

Big-endian: apart from convertToHiColor() above, the picture code takes
its masks from CreateMask(), which has a big-endian case, or from the
surfaces themselves; the maps and saved games are text (XML); the sounds
and music go through SDL2_mixer. Saved games are interchangeable with
other platforms'.


Known limits
------------

- Start-up takes about a minute on a 68040, and the game wants a lot of
  free memory (it wasn't measured).
- Upstream looks for some optional pictures (gfx/attacker_walking_N.png)
  before the per-direction ones; their absence is logged and is normal.
- All drawing goes through SDL's renderer on GFOpenGPU; with many units on
  screen a 68040 will run slower.
- Network play (upstream's "server"/"client" options) was not tried.


Testing
-------

In an AmiCradle lab (t4-port-b, a copy of Instance-32), 5 November 2026,
from SYS:Games/Gigalomania:

- Start: the loading bar, then (after about a minute) "Choose a game type"
  over the planet and starfield, in the right colours, in a 640x480 window.
- Single island, then "Controller of the Green People", then the island
  screen of the first age; right-clicking the man icon added men,
  clicking the map started the battle: the island with its trees, the
  tower, the side panel, the clock, the shield and research icons. The
  research button opened the designs panel.
- Esc and Right Amiga+Q both opened the quit menu; "Save game and quit to
  desktop" ended the program, the Workbench came back with nothing left
  over, and User/autosave.sav was written. Started again, the game loaded
  the saved battle and carried on with it.
- Not heard: the lab is muted, so the music and sounds were not checked by
  ear; the log shows gamemusic.ogg being loaded.
- Not tried: a whole battle to its end, the tutorial, "all islands",
  "fullscreen", "onemousebutton", network play.

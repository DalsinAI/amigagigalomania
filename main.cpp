#include "stdafx.h"

#if defined(__ANDROID__)
#include <android/log.h>
#endif

#include "game.h"

#if defined(__amigaos__)
extern "C" {
void amiga_enter_progdir(void);
void amiga_screens_snapshot(void);
void amiga_screens_close_leftovers(void);
}
#endif

int main(int argc, char *argv[])
{
#if defined(__amigaos__)
	// the data (gfx/, sound/, music/, islands/) is found from the program's
	// drawer, wherever the game was started from; the empty screens
	// SDL2.module leaves behind are closed when the game ends
	amiga_enter_progdir();
	amiga_screens_snapshot();
	atexit(amiga_screens_close_leftovers);
#endif
#if defined(__ANDROID__)
    __android_log_print(ANDROID_LOG_INFO, "Gigalomania", "started main");
#endif

	playGame(argc, argv);
#if defined(__ANDROID__)
    __android_log_print(ANDROID_LOG_INFO, "Gigalomania", "about to exit main");
#endif
    return 0;
}

/* The game finds its data with names relative to the current directory
 * (gfx/, sound/, music/, islands/): it is made the program's drawer at
 * start, and given back at the end.
 * The AmiCradle Team. GPL 2 or later, like Gigalomania. */
#include <exec/types.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <stdlib.h>

static BPTR old_dir;
static int moved;

static void amiga_leave_progdir(void)
{
	if (moved)
	{
		BPTR ours = CurrentDir(old_dir);
		UnLock(ours);
		moved = 0;
	}
}

void amiga_enter_progdir(void)
{
	BPTR home = GetProgramDir();
	BPTR lock;

	if (!home)
		return;
	lock = DupLock(home);
	if (!lock)
		return;
	old_dir = CurrentDir(lock);
	moved = 1;
	atexit(amiga_leave_progdir);
}

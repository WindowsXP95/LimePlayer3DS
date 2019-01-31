/**
 * ctrmus - 3DS Music Player
 * Copyright (C) 2016 Mahyar Koshkouei
 *
 * This program comes with ABSOLUTELY NO WARRANTY and is free software. You are
 * welcome to redistribute it under certain conditions; for details see the
 * LICENSE file.
 */

#include <3ds.h>

#ifndef ctrmus_main_h
#define ctrmus_main_h

/* Default folder */
#define DEFAULT_DIR		"sdmc:/"

/* Maximum number of lines that can be displayed */


typedef struct
{
	char**	files;
	int		fileNum;

	char**	directories;
	int		dirNum;

	char*	currentDir;
} dirList_t;

/**
 * Get number of files in current working folder
 *
 * \return	Number of files in current working folder, -1 on failure with
 *			errno set.
 */
int getNumberFiles(void);

#endif

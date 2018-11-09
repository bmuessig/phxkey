/* phxkey.h - Extract the cryptographic key for encoding album art from the remote management utility (WADM)
 * Copyright (C) 2018 0xbmuessig
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef PHXKEY_H
#define PHXKEY_H

#include "sha1.h"

#define VERSION "1.0"
#define YEAR "2018"
#define AUTHOR "0xbmuessig"
#define DEFAULT_OUTPUT_NAME "./crypto.key"

#if defined(__MINGW32__) || defined(__MINGW32__) || defined(WIN32)
	#define DEFAULT_EXEC_NAME "phxkey.exe"
#else
	#define DEFAULT_EXEC_NAME "phxkey"
#endif

enum
{
	ARG_EXEC_FILE,
	ARG_INPUT_FILE,
	ARG_OUTPUT_FILE
};

unsigned long fileAnalyze(FILE *file, SHA1_CONTEXT *shaCtx);

#endif

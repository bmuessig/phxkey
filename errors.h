/* errors.h - Error return codes for phxkey
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

#ifndef ERRORS_H
#define ERRORS_H

enum
{
	ERROR_NONE,
	ERROR_NUMARGS,
	ERROR_INPUT_FILE,
	ERROR_OUTPUT_FILE,
	ERROR_NO_SIGNATURE,
	ERROR_INVALID_SIGNATURE,
	ERROR_INSUFFICIENT_MEMORY,
	ERROR_PROGRAM
};

#endif

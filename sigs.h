/* sigs.h - Signatures collection
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

#ifndef SIGS_H
#define SIGS_H

#include <stdint.h>

#define FILE_HASH_SIZE 20
#define KEY_HASH_SIZE FILE_HASH_SIZE
#define SOURCE_NAME_SIZE 40

typedef struct {
	uint32_t fileSize;
	uint32_t keyOffset;
	uint32_t keySize;
	uint8_t fileHash[FILE_HASH_SIZE];
	uint8_t keyHash[KEY_HASH_SIZE];
	uint8_t sourceName[SOURCE_NAME_SIZE];
} SIGNATURE;

int signatureCount(void);
int signatureRead(unsigned int index, int doCopyArrays, SIGNATURE* targetSignature);

#endif

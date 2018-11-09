/* sigs.c - Signatures collection
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

#include <stdint.h>
#include <string.h>

#include "sigs.h"

#define NUM_SIGNATURES 1

const uint32_t fileSizes[NUM_SIGNATURES] =
{
	0x481035
};

const uint32_t keyOffsets[NUM_SIGNATURES] =
{
	0x120E28
};

const uint32_t keySizes[NUM_SIGNATURES] =
{
	0x1000
};

const uint8_t fileHashes[NUM_SIGNATURES][FILE_HASH_SIZE] =
{
	{0x59, 0x4d, 0x4a, 0x05, 0x80, 0xd1, 0x26, 0x87, 0xa4, 0xfe, 0xb8, 0x1f, 0x2d, 0xf4, 0x64, 0xe0, 0x43, 0x2c, 0x1f, 0x00}
};

const uint8_t keyHashes[NUM_SIGNATURES][KEY_HASH_SIZE] =
{
	{0xc7, 0xec, 0x72, 0x04, 0xca, 0x98, 0x82, 0xe8, 0x56, 0x05, 0xa1, 0x93, 0xeb, 0x50, 0xa2, 0x22, 0xca, 0x5e, 0x79, 0xd0}
};

const uint8_t sourceNames[NUM_SIGNATURES][SOURCE_NAME_SIZE] =
{
	"WADM.exe 2.0.1 (2007/10/29)"
};

int signatureCount(void)
{
	return NUM_SIGNATURES;
}

// This returns zero on success and 1 on failure
int signatureRead(unsigned int index, int doCopyArrays, SIGNATURE* targetSignature)
{
	// Sanity check the inputs
	if (index >= signatureCount() || targetSignature == NULL)
		return 1;
	
	// Perform some sanity checks
	if (fileSizes[index] <= keyOffsets[index] || !keySizes[index] || !fileSizes[index])
		return 1;
	
	// Copy the integer parameters
	targetSignature->fileSize = fileSizes[index];
	targetSignature->keyOffset = keyOffsets[index];
	targetSignature->keySize = keySizes[index];
	
	// On demand, copy the arrays
	if (doCopyArrays)
	{
		memcpy(targetSignature->fileHash, fileHashes[index], FILE_HASH_SIZE);
		memcpy(targetSignature->keyHash, keyHashes[index], KEY_HASH_SIZE);
		memcpy(targetSignature->sourceName, sourceNames[index], SOURCE_NAME_SIZE);
	}
	
	return 0;
}

/* phxkey.c - Extract the cryptographic key for encoding album art from the remote management utility (WADM)
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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "sha1.h"
#include "errors.h"
#include "phxkey.h"
#include "sigs.h"

int main(int argc, char **argv)
{
	// Variables
	SHA1_CONTEXT fileShaCtx, keyShaCtx;
	SIGNATURE sig;
	FILE *inputFile, *outputFile;
	uint8_t *keyBuffer, *outputPath;
	unsigned long fileSize;
	int currentSig, numSigs, ptr;
	
	// First, perform a quick sanity check on the number of parameters
	if (argc < 2 || argc > 3 || argv == NULL)
	{
		char *appName = (argc > 0) ? argv[ARG_EXEC_FILE] : "phxkey";
		fprintf(stderr, "phxkey v" VERSION "\nCopyright (C) " YEAR " " AUTHOR "\n");
		fprintf(stderr, "This utility is designed to extract the private crypto-key from either WADM client or server binary.\n\n");
		fprintf(stderr, "Usage: %s <Input-file> [Output-file | -]\nHint: Use - instead of a filename to refer to stdout.\n", appName);
		fprintf(stderr, "      If no output file name is given, %s is used.\n", DEFAULT_OUTPUT_NAME);
		return ERROR_NUMARGS;
	}
	
	// Also check if any of our parameters is null
	if (argv[ARG_INPUT_FILE] == NULL || (argc == 3 && argv[ARG_OUTPUT_FILE] == NULL))
	{
		fprintf(stderr, "Error: Nullpointer argument detected!\n");
		return ERROR_NUMARGS;
	}
	
	// Then, open the files and get some pointers
	// Start with the input first
	if (strcmp(argv[ARG_INPUT_FILE], "-") != 0)
	{
		if((inputFile = fopen(argv[ARG_INPUT_FILE], "rb")) == NULL)
		{
			perror("Error opening input file");
			return ERROR_INPUT_FILE;
		}
	}
	else
	{
		// Yes, this may be slightly 
		fprintf(stderr, "Error: This utility can not read from stdin!\n");
		return ERROR_INPUT_FILE;
	}
	
	// Then continue with the output
	// First, check if an output file is specified and if not, set it to the default
	outputPath = (argc == 2) ? DEFAULT_OUTPUT_NAME : argv[ARG_OUTPUT_FILE];
	
	// Check, if the output is stdout
	if (strcmp(outputPath, "-") != 0)
	{
		if ((outputFile = fopen(outputPath, "wb")) == NULL)
		{
			perror("Error opening output file");
			return ERROR_OUTPUT_FILE;
		}
	}
	else
		outputFile = stdout;
	
	// Next, check the number of available signatures
	if (!(numSigs = signatureCount()))
	{
		fprintf(stderr, "Error: No signatures are installed! Please re-compile with at least one target signature.\n");
		return ERROR_NO_SIGNATURE;
	}
	
	// Now, evaluate the input file size and hash
	if (!(fileSize = fileAnalyze(inputFile, &fileShaCtx)))
	{
		fprintf(stderr, "Error: The file size is zero or cannot be determined or hashing failed!\n");
		return ERROR_INPUT_FILE;
	}
	
	// Print the gained info
	fprintf(stderr, "Info: Input file is 0x%lx bytes long and it's SHA1 is: ", fileSize);
	for (ptr = 0; ptr < 20; ptr++)
		fprintf(stderr, "%02x", fileShaCtx.buf[ptr]);
	fprintf(stderr, "\n");
	
	// Then, loop through the signatures until we find something that matches
	for (currentSig = 0; currentSig < numSigs; currentSig++)
	{
		// Try to read the signature
		if (signatureRead(currentSig, 0, &sig))
		{
			fprintf(stderr, "Warning: Could not copy signature %d! Skipping.\n", currentSig);
			continue;
		}
		
		// Now, see if the size matches
		if (sig.fileSize == fileSize)
		{
			// Now we have enough reasons to copy the entire signature
			if (signatureRead(currentSig, 1, &sig))
			{
				fprintf(stderr, "Error: Could not copy signature %d after one successful copy!\n", currentSig);
				return ERROR_INVALID_SIGNATURE;
			}
			
			// Compare the file hash next
			if (memcmp(sig.fileHash, fileShaCtx.buf, FILE_HASH_SIZE) != 0)
				continue;
			
			// Display that we found something
			if (strlen(sig.sourceName) > 0)
				fprintf(stderr, "Info: Input file matches following known signature: %s\n", sig.sourceName);
			else
				fprintf(stderr, "Info: Input file matches a known signature!\n");
			
			// Now, allocate space for the key
			if ((keyBuffer = malloc(sig.keySize)) == NULL)
			{
				fprintf(stderr, "Error: Could not allocate 0x%x bytes of buffer memory!\n", sig.keySize);
				return ERROR_INSUFFICIENT_MEMORY;
			}
			
			// Next, try to seek to the key offset
			if (fseek(inputFile, sig.keyOffset, SEEK_SET) != 0)
			{
				perror("Error seeking to the beginning of the input file");
				return ERROR_INPUT_FILE;
			}
			
			// Then, copy the entire key
			if (fread(keyBuffer, sig.keySize, 1, inputFile) != 1)
			{
				perror("Error reading the key from the input file");
				fprintf(stderr, "Error attempting to read 0x%x bytes from the input file!\n", sig.keySize);
				return ERROR_INPUT_FILE;
			}
			
			// Initialize the sha1 library again for the key
			sha1_init(&keyShaCtx);
			
			// SHA1 the key
			sha1_write(&keyShaCtx, keyBuffer, sig.keySize);
			
			// And finalize it
			sha1_final(&keyShaCtx);
			
			// Now, verify the key
			if (memcmp(sig.keyHash, keyShaCtx.buf, KEY_HASH_SIZE) != 0)
			{
				fprintf(stderr, "Error: The file hash matches while the key has does not! Read key hash: ");
				for (ptr = 0; ptr < 20; ptr++)
					fprintf(stderr, "%02x", keyShaCtx.buf[ptr]);
				fprintf(stderr, "\n");
				return ERROR_PROGRAM;
			}
			
			// The key has been verified successfully
			fprintf(stderr, "Info: The key has been found successfully and it's SHA1 is: ");
			for (ptr = 0; ptr < 20; ptr++)
				fprintf(stderr, "%02x", keyShaCtx.buf[ptr]);
			fprintf(stderr, "\n");
			
			// Finally, try to write the signature to the output stream
			if (fwrite(keyBuffer, sig.keySize, 1, outputFile) != 1)
			{
				perror("Error writing the key to the output file");
				return ERROR_OUTPUT_FILE;
			}
			
			// Success!
			return ERROR_NONE;
		}
	}
	
	// If we end up here, we have not found a signature
	fprintf(stderr, "Error: No signature was found that matched the input file!\n");
	return ERROR_NO_SIGNATURE;
}

// Returns the file size and a shaContext on success or zero on failure
unsigned long fileAnalyze(FILE *file, SHA1_CONTEXT *shaCtx)
{
	// Allocate a temporary buffer
	uint8_t buffer[0x1000];
	unsigned long numBytes, endPosition;
	
	// First, seek to the beginning
	if (fseek(file, 0, SEEK_SET) != 0)
	{
		perror("Error seeking to the beginning of the input file");
		return 0;
	}
	
	// Init the sha1 lib first
	sha1_init(shaCtx);
	
	// Read the file and calculate the sha1
	while ((numBytes = fread(buffer, 1, sizeof(buffer), file)))
        sha1_write(shaCtx, buffer, numBytes);
	
	// Check if an error occured
	if (ferror(file))
    {
		perror("Error hashing input file");
		return 0;
    }
	
	// Copy the current position
	endPosition = ftell(file);
	
	// Finally, seek back to the beginning
	if (fseek(file, 0, SEEK_SET) != 0)
	{
		perror("Error seeking to the beginning of the input file");
		return 0;
	}
	
	// And finalize the sha1
	sha1_final(shaCtx);
	
	return endPosition;
}

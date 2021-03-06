# phxkey
Tool for extracting the cryptographic key used for encoding album art from the remote management utility (WADM) on the MCI500H and it's siblings. This tool also supports extracting the key from the mcp_MCI500 application of the firmware.

## Usage
`Usage: ./phxkey <Input-file> [Output-file | -]`

Hint: Use `-` instead of a filename to refer to stdout. If no output file name is given, `./crypto.key` is used. In Windows, you can just drag & drop the `WADM.exe` or the `mcp_MCI500` file onto `phxkey.exe` to extract the key to `crypto.key`.

## Requirements
### Compilation
For compilation, a Linux installation with Make and GCC is required. For Windows cross-compilation, mingw-gcc as well as mingw-winpthreads is required.

### Usage
At runtime, no special libraries are needed on either OS. On either OS, a file called `WADM.exe` that is part of the WADM management application for the MCI500H and it's siblings, is required. To get this file, WADM has to be installed on Windows and then the file needs to be copied from the program directory.
For Linux systems, instead of WADM, a file called `mcp_MCI500` can be extracted from the device or it's firmware image and used instead.

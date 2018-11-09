# phxkey
Tool for extracting the cryptographic key used for encoding album art from the remote management utility (WADM) on the MCI500H and it's siblings

# Usage
`Usage: ./phxkey <Input-file> [Output-file | -]`

Hint: Use `-` instead of a filename to refer to stdout. If no output file name is given, `./crypto.key` is used. In Windows, you can just drag & drop the `WADM.exe` file onto `phxkey.exe` to extract the key to `crypto.key`.

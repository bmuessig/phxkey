# Makefile - Makefile for building phxkey targeting *nix or MS-Windows
# Copyright (C) 2018 0xbmuessig
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

CC=gcc
CCARGS=
LDARGS=-s
BINARY=phxkey

# Uncomment lines below to compile for MS-Windows
# Leave them commented-out to compile for *NIX
CC=i686-w64-mingw32-gcc
CCARGS=
LDARGS=-s -static
BINARY=phxkey.exe

.PHONY: clean all test

all: $(BINARY)

clean:
	rm -f *.o
	rm -f $(BINARY)
	rm -f mci500.key

test: mci500.key

mci500.key: $(BINARY) WADM.exe
	./$(BINARY) WADM.exe mci500.key

$(BINARY): phxkey.o sha1.o sigs.o
	$(CC) $(LDARGS) -o $@ $^

phxkey.o: phxkey.c phxkey.h errors.h sigs.h
	$(CC) $(CCARGS) -c -o $@ $<

sha1.o: sha1.c sha1.h
	$(CC) $(CCARGS) -c -o $@ $<
	
sigs.o: sigs.c sigs.h
	$(CC) $(CCARGS) -c -o $@ $<

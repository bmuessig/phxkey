/* sha1.h - print SHA-1 Message-Digest Algorithm (taken from gnupg 1.3.92)
 * Copyright (C) 1998, 1999, 2000, 2001 Free Software Foundation, Inc.
 * Copyright (C) 2004 g10 Code GmbH
 * Copyright (C) 2018 Modified by 0xbmuessig
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

#ifndef SHA1_H
#define SHA1_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t h0,h1,h2,h3,h4;
    uint32_t nblocks;
    uint8_t  buf[64];
    uint32_t count;
} SHA1_CONTEXT;

void sha1_init(SHA1_CONTEXT *hd);
void transform(SHA1_CONTEXT *hd, uint8_t *data);
void sha1_write(SHA1_CONTEXT *hd, uint8_t *inbuf, size_t inlen);
void sha1_final(SHA1_CONTEXT *hd);

#endif

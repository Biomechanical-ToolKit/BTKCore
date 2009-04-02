/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __btkASCIIConverter_h
#define __btkASCIIConverter_h

namespace btk
{
  static const char ASCIIConverter[256] = {
    '_', /* 0x00 */ 
    '_', /* 0x01 */
    '_', /* 0x02 */
    '_', /* 0x03 */
    '_', /* 0x04 */
    '_', /* 0x05 */
    '_', /* 0x06 */
    '_', /* 0x07 */
    '_', /* 0x08 */
    '_', /* 0x09 */
    '_', /* 0x0a */
    '_', /* 0x0b */
    '_', /* 0x0c */
    '_', /* 0x0d */
    '_', /* 0x0e */
    '_', /* 0x0f */
    '_', /* 0x10 */
    '_', /* 0x11 */
    '_', /* 0x12 */
    '_', /* 0x13 */
    '_', /* 0x14 */
    '_', /* 0x15 */
    '_', /* 0x16 */
    '_', /* 0x17 */
    '_', /* 0x18 */
    '_', /* 0x19 */
    '_', /* 0x1a */
    '_', /* 0x1b */
    '_', /* 0x1c */
    '_', /* 0x1d */
    '_', /* 0x1e */
    '_', /* 0x1f */
    '_', /* 0x20 */
    '_', /* 0x21 */
    '_', /* 0x22 */
    '_', /* 0x23 */
    '_', /* 0x24 */
    '_', /* 0x25 */
    '_', /* 0x26 */
    '_', /* 0x27 */
    '_', /* 0x28 */
    '_', /* 0x29 */
    '_', /* 0x2a */
    '_', /* 0x2b */
    '_', /* 0x2c */
    '_', /* 0x2d */
    '_', /* 0x2e */
    '_', /* 0x2f */
    '0', /* 0x30 */
    '1', /* 0x31 */
    '2', /* 0x32 */
    '3', /* 0x33 */
    '4', /* 0x34 */
    '5', /* 0x35 */
    '6', /* 0x36 */
    '7', /* 0x37 */
    '8', /* 0x38 */
    '9', /* 0x39 */
    '_', /* 0x3a */
    '_', /* 0x3b */
    '_', /* 0x3c */
    '_', /* 0x3d */
    '_', /* 0x3e */
    '_', /* 0x3f */
    '_', /* 0x40 */
    'A', /* 0x41 */
    'B', /* 0x42 */
    'C', /* 0x43 */
    'D', /* 0x44 */
    'E', /* 0x45 */
    'F', /* 0x46 */
    'G', /* 0x47 */
    'H', /* 0x48 */
    'I', /* 0x49 */
    'J', /* 0x4a */
    'K', /* 0x4b */
    'L', /* 0x4c */
    'M', /* 0x4d */
    'N', /* 0x4e */
    'O', /* 0x4f */
    'P', /* 0x50 */
    'Q', /* 0x51 */
    'R', /* 0x52 */
    'S', /* 0x53 */
    'T', /* 0x54 */
    'U', /* 0x55 */
    'V', /* 0x56 */
    'W', /* 0x57 */
    'X', /* 0x58 */
    'Y', /* 0x59 */
    'Z', /* 0x5a */
    '_', /* 0x5b */
    '_', /* 0x5c */
    '_', /* 0x5d */
    '_', /* 0x5e */
    '_', /* 0x5f */
    '_', /* 0x60 */
    'a', /* 0x61 */
    'b', /* 0x62 */
    'c', /* 0x63 */
    'd', /* 0x64 */
    'e', /* 0x65 */
    'f', /* 0x66 */
    'g', /* 0x67 */
    'h', /* 0x68 */
    'i', /* 0x69 */
    'j', /* 0x6a */
    'k', /* 0x6b */
    'l', /* 0x6c */
    'm', /* 0x6d */
    'n', /* 0x6e */
    'o', /* 0x6f */
    'p', /* 0x70 */
    'q', /* 0x71 */
    'r', /* 0x72 */
    's', /* 0x73 */
    't', /* 0x74 */
    'u', /* 0x75 */
    'v', /* 0x76 */
    'w', /* 0x77 */
    'x', /* 0x78 */
    'y', /* 0x79 */
    'z', /* 0x7a */
    '_', /* 0x7b */
    '_', /* 0x7c */
    '_', /* 0x7d */
    '_', /* 0x7e */
    '_', /* 0x7f */
    'C', /* 0x80 */
    'u', /* 0x81 */
    'e', /* 0x82 */
    'a', /* 0x83 */
    'a', /* 0x84 */
    'a', /* 0x85 */
    'a', /* 0x86 */
    'c', /* 0x87 */
    'e', /* 0x88 */
    'e', /* 0x89 */
    'e', /* 0x8a */
    'i', /* 0x8b */
    'i', /* 0x8c */
    'i', /* 0x8d */
    'A', /* 0x8e */
    'A', /* 0x8f */
    'E', /* 0x90 */
    'a', /* 0x91 : ae */
    'A', /* 0x92 : AE */
    'o', /* 0x93 */
    'o', /* 0x94 */
    'o', /* 0x95 */
    'u', /* 0x96 */
    'u', /* 0x97 */
    'y', /* 0x98 */
    'O', /* 0x99 */
    'U', /* 0x9a */
    '_', /* 0x9b */
    '_', /* 0x9c */
    '_', /* 0x9d */
    '_', /* 0x9e */
    '_', /* 0x9f */
    'a', /* 0xa0 */
    'i', /* 0xa1 */
    'u', /* 0xa2 */
    'u', /* 0xa3 */
    'n', /* 0xa4 */
    'N', /* 0xa5 */
    '_', /* 0xa6 */
    '_', /* 0xa7 */
    '_', /* 0xa8 */
    '_', /* 0xa9 */
    '_', /* 0xaa */
    '_', /* 0xab */
    '_', /* 0xac */
    '_', /* 0xad */
    '_', /* 0xae */
    '_', /* 0xaf */
    '_', /* 0xb0 */
    '_', /* 0xb1 */
    '_', /* 0xb2 */
    '_', /* 0xb3 */
    '_', /* 0xb4 */
    '_', /* 0xb5 */
    '_', /* 0xb6 */
    '_', /* 0xb7 */
    '_', /* 0xb8 */
    '_', /* 0xb9 */
    '_', /* 0xba */
    '_', /* 0xbb */
    '_', /* 0xbc */
    '_', /* 0xbd */
    '_', /* 0xbe */
    '_', /* 0xbf */
    '_', /* 0xc0 */
    '_', /* 0xc1 */
    '_', /* 0xc2 */
    '_', /* 0xc3 */
    '_', /* 0xc4 */
    '_', /* 0xc5 */
    '_', /* 0xc6 */
    '_', /* 0xc7 */
    '_', /* 0xc8 */
    '_', /* 0xc9 */
    '_', /* 0xca */
    '_', /* 0xcb */
    '_', /* 0xcc */
    '_', /* 0xcd */
    '_', /* 0xce */
    '_', /* 0xcf */
    '_', /* 0xd0 */
    '_', /* 0xd1 */
    '_', /* 0xd2 */
    '_', /* 0xd3 */
    '_', /* 0xd4 */
    '_', /* 0xd5 */
    '_', /* 0xd6 */
    '_', /* 0xd7 */
    '_', /* 0xd8 */
    '_', /* 0xd9 */
    '_', /* 0xda */
    '_', /* 0xdb */
    '_', /* 0xdc */
    '_', /* 0xdd */
    '_', /* 0xde */
    '_', /* 0xdf */
    '_', /* 0xe0 */
    '_', /* 0xe1 */
    '_', /* 0xe2 */
    '_', /* 0xe3 */
    '_', /* 0xe4 */
    '_', /* 0xe5 */
    '_', /* 0xe6 */
    '_', /* 0xe7 */
    '_', /* 0xe8 */
    '_', /* 0xe9 */
    '_', /* 0xea */
    '_', /* 0xeb */
    '_', /* 0xec */
    '_', /* 0xed */
    '_', /* 0xee */
    '_', /* 0xff */
    '_', /* 0xf0 */
    '_', /* 0xf1 */
    '_', /* 0xf2 */
    '_', /* 0xf3 */
    '_', /* 0xf4 */
    '_', /* 0xf5 */
    '_', /* 0xf6 */
    '_', /* 0xf7 */
    '_', /* 0xf8 */
    '_', /* 0xf9 */
    '_', /* 0xfa */
    '_', /* 0xfb */
    '_', /* 0xfc */
    '_', /* 0xfd */
    '_', /* 0xfe */
    '_'  /* 0xff */
	};
};

#endif

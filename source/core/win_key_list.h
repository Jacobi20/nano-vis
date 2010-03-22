/*
	The MIT License

	Copyright (c) 2010 IFMO/GameDev Studio

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/
// win_key_list.h - win32 keyboard mapping

#pragma once


/*-----------------------------------------------------------------------------
	KEY CODES :
-----------------------------------------------------------------------------*/

#define KEY_LBUTTON              0x1
#define KEY_RBUTTON              0x2
#define KEY_MBUTTON              0x5
#define KEY_BACKSPACE            0x8
#define KEY_TAB                  0x9
#define KEY_ENTER                0xD
#define KEY_SHIFT                0x10
#define KEY_CTRL                 0x11
#define KEY_ALT                  0x12
#define KEY_PAUSE                0x13
#define KEY_CAPS_LOCK            0x14
#define KEY_ESCAPE               0x1B
#define KEY_SPACE                0x20
#define KEY_PGUP                 0x21
#define KEY_PGDN                 0x22
#define KEY_END                  0x23
#define KEY_HOME                 0x24
#define KEY_LEFT                 0x25
#define KEY_UP                   0x26
#define KEY_RIGHT                0x27
#define KEY_DOWN                 0x28
#define KEY_PRINTSCR             0x2C
#define KEY_INS                  0x2D
#define KEY_DEL                  0x2E
#define KEY_0                    0x30
#define KEY_1                    0x31
#define KEY_2                    0x32
#define KEY_3                    0x33
#define KEY_4                    0x34
#define KEY_5                    0x35
#define KEY_6                    0x36
#define KEY_7                    0x37
#define KEY_8                    0x38
#define KEY_9                    0x39
#define KEY_A                    0x41
#define KEY_B                    0x42
#define KEY_C                    0x43
#define KEY_D                    0x44
#define KEY_E                    0x45
#define KEY_F                    0x46
#define KEY_G                    0x47
#define KEY_H                    0x48
#define KEY_I                    0x49
#define KEY_J                    0x4A
#define KEY_K                    0x4B
#define KEY_L                    0x4C
#define KEY_M                    0x4D
#define KEY_N                    0x4E
#define KEY_O                    0x4F
#define KEY_P                    0x50
#define KEY_Q                    0x51
#define KEY_R                    0x52
#define KEY_S                    0x53
#define KEY_T                    0x54
#define KEY_U                    0x55
#define KEY_V                    0x56
#define KEY_W                    0x57
#define KEY_X                    0x58
#define KEY_Y                    0x59
#define KEY_Z                    0x5A
#define KEY_NUM_0                0x60
#define KEY_NUM_1                0x61
#define KEY_NUM_2                0x62
#define KEY_NUM_3                0x63
#define KEY_NUM_4                0x64
#define KEY_NUM_5                0x65
#define KEY_NUM_6                0x66
#define KEY_NUM_7                0x67
#define KEY_NUM_8                0x68
#define KEY_NUM_9                0x69
#define KEY_NUM_MUL              0x6A
#define KEY_NUM_ADD              0x6B
#define KEY_NUM_SUB              0x6D
#define KEY_NUM_DEC              0x6E
#define KEY_NUM_DIV              0x6F
#define KEY_F1                   0x70
#define KEY_F2                   0x71
#define KEY_F3                   0x72
#define KEY_F4                   0x73
#define KEY_F5                   0x74
#define KEY_F6                   0x75
#define KEY_F7                   0x76
#define KEY_F8                   0x77
#define KEY_F9                   0x78
#define KEY_F10                  0x79
#define KEY_F11                  0x7A
#define KEY_F12                  0x7B
#define KEY_NUM_LOCK             0x90
#define KEY_SCROLL_LOCK          0x91
#define KEY_SEMICOLON            0xBA
#define KEY_ADD                  0xBB
#define KEY_COMMA                0xBC
#define KEY_SUB                  0xBD
#define KEY_POINT                0xBE
#define KEY_SLASH                0xBF
#define KEY_TILDE                0xC0
#define KEY_LEFT_SQR_BRACKET     0xDB
#define KEY_BACKSLASH            0xDC
#define KEY_RIGHT_SQR_BRACKET    0xDD
#define KEY_QUOTE                0xDE


/*-----------------------------------------------------------------------------
	KEY NAMES
-----------------------------------------------------------------------------*/

const char *const OS_KEY_LIST[256] = {
  /* 0x00 */  NULL,
  /* 0x01 */  "LBUTTON",
  /* 0x02 */  "RBUTTON",
  /* 0x03 */  NULL,
  /* 0x04 */  NULL,
  /* 0x05 */  "MBUTTON",
  /* 0x06 */  NULL,
  /* 0x07 */  NULL,
  /* 0x08 */  "BACKSPACE",
  /* 0x09 */  "TAB",
  /* 0x0A */  NULL,
  /* 0x0B */  NULL,
  /* 0x0C */  NULL,
  /* 0x0D */  "ENTER",
  /* 0x0E */  NULL,
  /* 0x0F */  NULL,

  /* 0x10 */  "SHIFT",
  /* 0x11 */  "CTRL",
  /* 0x12 */  "ALT",
  /* 0x13 */  "PAUSE",
  /* 0x14 */  "CAPS_LOCK",
  /* 0x15 */  NULL,
  /* 0x16 */  NULL,
  /* 0x17 */  NULL,
  /* 0x18 */  NULL,
  /* 0x19 */  NULL,
  /* 0x1A */  NULL,
  /* 0x1B */  "ESCAPE",
  /* 0x1C */  NULL,
  /* 0x1D */  NULL,
  /* 0x1E */  NULL,
  /* 0x1F */  NULL,

  /* 0x20 */  "SPACE",
  /* 0x21 */  "PGUP",
  /* 0x22 */  "PGDN",
  /* 0x23 */  "END",
  /* 0x24 */  "HOME",
  /* 0x25 */  "LEFT",
  /* 0x26 */  "UP",
  /* 0x27 */  "RIGHT",
  /* 0x28 */  "DOWN",
  /* 0x29 */  NULL,
  /* 0x2A */  NULL,
  /* 0x2B */  NULL,
  /* 0x2C */  "PRINTSCR",
  /* 0x2D */  "INS",
  /* 0x2E */  "DEL",
  /* 0x2F */  NULL,

  /* 0x30 */  "0",
  /* 0x31 */  "1",
  /* 0x32 */  "2",
  /* 0x33 */  "3",
  /* 0x34 */  "4",
  /* 0x35 */  "5",
  /* 0x36 */  "6",
  /* 0x37 */  "7",
  /* 0x38 */  "8",
  /* 0x39 */  "9",
  /* 0x3A */  NULL,
  /* 0x3B */  NULL,
  /* 0x3C */  NULL,
  /* 0x3D */  NULL,
  /* 0x3E */  NULL,
  /* 0x3F */  NULL,

  /* 0x40 */  NULL,
  /* 0x41 */  "A",
  /* 0x42 */  "B",
  /* 0x43 */  "C",
  /* 0x44 */  "D",
  /* 0x45 */  "E",
  /* 0x46 */  "F",
  /* 0x47 */  "G",
  /* 0x48 */  "H",
  /* 0x49 */  "I",
  /* 0x4A */  "J",
  /* 0x4B */  "K",
  /* 0x4C */  "L",
  /* 0x4D */  "M",
  /* 0x4E */  "N",
  /* 0x4F */  "O",

  /* 0x50 */  "P",
  /* 0x51 */  "Q",
  /* 0x52 */  "R",
  /* 0x53 */  "S",
  /* 0x54 */  "T",
  /* 0x55 */  "U",
  /* 0x56 */  "V",
  /* 0x57 */  "W",
  /* 0x58 */  "X",
  /* 0x59 */  "Y",
  /* 0x5A */  "Z",
  /* 0x5B */  NULL,
  /* 0x5C */  NULL,
  /* 0x5D */  NULL,
  /* 0x5E */  NULL,
  /* 0x5F */  NULL,

  /* 0x60 */  "NUM_0",
  /* 0x61 */  "NUM_1",
  /* 0x62 */  "NUM_2",
  /* 0x63 */  "NUM_3",
  /* 0x64 */  "NUM_4",
  /* 0x65 */  "NUM_5",
  /* 0x66 */  "NUM_6",
  /* 0x67 */  "NUM_7",
  /* 0x68 */  "NUM_8",
  /* 0x69 */  "NUM_9",
  /* 0x6A */  "NUM_MUL",
  /* 0x6B */  "NUM_ADD",
  /* 0x6C */  NULL,
  /* 0x6D */  "NUM_SUB",
  /* 0x6E */  "NUM_DEC",
  /* 0x6F */  "NUM_DIV",

  /* 0x70 */  "F1",
  /* 0x71 */  "F2",
  /* 0x72 */  "F3",
  /* 0x73 */  "F4",
  /* 0x74 */  "F5",
  /* 0x75 */  "F6",
  /* 0x76 */  "F7",
  /* 0x77 */  "F8",
  /* 0x78 */  "F9",
  /* 0x79 */  "F10",
  /* 0x7A */  "F11",
  /* 0x7B */  "F12",
  /* 0x7C */  NULL,
  /* 0x7D */  NULL,
  /* 0x7E */  NULL,
  /* 0x7F */  NULL,

  /* 0x80 */  NULL,
  /* 0x81 */  NULL,
  /* 0x82 */  NULL,
  /* 0x83 */  NULL,
  /* 0x84 */  NULL,
  /* 0x85 */  NULL,
  /* 0x86 */  NULL,
  /* 0x87 */  NULL,
  /* 0x88 */  NULL,
  /* 0x89 */  NULL,
  /* 0x8A */  NULL,
  /* 0x8B */  NULL,
  /* 0x8C */  NULL,
  /* 0x8D */  NULL,
  /* 0x8E */  NULL,
  /* 0x8F */  NULL,

  /* 0x90 */  "NUM_LOCK",
  /* 0x91 */  "SCROLL_LOCK",
  /* 0x92 */  NULL,
  /* 0x93 */  NULL,
  /* 0x94 */  NULL,
  /* 0x95 */  NULL,
  /* 0x96 */  NULL,
  /* 0x97 */  NULL,
  /* 0x98 */  NULL,
  /* 0x99 */  NULL,
  /* 0x9A */  NULL,
  /* 0x9B */  NULL,
  /* 0x9C */  NULL,
  /* 0x9D */  NULL,
  /* 0x9E */  NULL,
  /* 0x9F */  NULL,

  /* 0xA0 */  NULL,
  /* 0xA1 */  NULL,
  /* 0xA2 */  NULL,
  /* 0xA3 */  NULL,
  /* 0xA4 */  NULL,
  /* 0xA5 */  NULL,
  /* 0xA6 */  NULL,
  /* 0xA7 */  NULL,
  /* 0xA8 */  NULL,
  /* 0xA9 */  NULL,
  /* 0xAA */  NULL,
  /* 0xAB */  NULL,
  /* 0xAC */  NULL,
  /* 0xAD */  NULL,
  /* 0xAE */  NULL,
  /* 0xAF */  NULL,

  /* 0xB0 */  NULL,
  /* 0xB1 */  NULL,
  /* 0xB2 */  NULL,
  /* 0xB3 */  NULL,
  /* 0xB4 */  NULL,
  /* 0xB5 */  NULL,
  /* 0xB6 */  NULL,
  /* 0xB7 */  NULL,
  /* 0xB8 */  NULL,
  /* 0xB9 */  NULL,
  /* 0xBA */  "SEMICOLON",
  /* 0xBB */  "ADD",
  /* 0xBC */  "COMMA",
  /* 0xBD */  "SUB",
  /* 0xBE */  "POINT",
  /* 0xBF */  "SLASH",

  /* 0xC0 */  "TILDE",
  /* 0xC1 */  NULL,
  /* 0xC2 */  NULL,
  /* 0xC3 */  NULL,
  /* 0xC4 */  NULL,
  /* 0xC5 */  NULL,
  /* 0xC6 */  NULL,
  /* 0xC7 */  NULL,
  /* 0xC8 */  NULL,
  /* 0xC9 */  NULL,
  /* 0xCA */  NULL,
  /* 0xCB */  NULL,
  /* 0xCC */  NULL,
  /* 0xCD */  NULL,
  /* 0xCE */  NULL,
  /* 0xCF */  NULL,

  /* 0xD0 */  NULL,
  /* 0xD1 */  NULL,
  /* 0xD2 */  NULL,
  /* 0xD3 */  NULL,
  /* 0xD4 */  NULL,
  /* 0xD5 */  NULL,
  /* 0xD6 */  NULL,
  /* 0xD7 */  NULL,
  /* 0xD8 */  NULL,
  /* 0xD9 */  NULL,
  /* 0xDA */  NULL,
  /* 0xDB */  "LEFT_SQR_BRACKET",
  /* 0xDC */  "BACKSLASH",
  /* 0xDD */  "RIGHT_SQR_BRACKET",
  /* 0xDE */  "QUOTE",	 
  /* 0xDF */  NULL,

  /* 0xE0 */  NULL,
  /* 0xE1 */  NULL,
  /* 0xE2 */  NULL,
  /* 0xE3 */  NULL,
  /* 0xE4 */  NULL,
  /* 0xE5 */  NULL,
  /* 0xE6 */  NULL,
  /* 0xE7 */  NULL,
  /* 0xE8 */  NULL,
  /* 0xE9 */  NULL,
  /* 0xEA */  NULL,
  /* 0xEB */  NULL,
  /* 0xEC */  NULL,
  /* 0xED */  NULL,
  /* 0xEE */  NULL,
  /* 0xEF */  NULL,

  /* 0xF0 */  NULL,
  /* 0xF1 */  NULL,
  /* 0xF2 */  NULL,
  /* 0xF3 */  NULL,
  /* 0xF4 */  NULL,
  /* 0xF5 */  NULL,
  /* 0xF6 */  NULL,
  /* 0xF7 */  NULL,
  /* 0xF8 */  NULL,
  /* 0xF9 */  NULL,
  /* 0xFA */  NULL,
  /* 0xFB */  NULL,
  /* 0xFC */  NULL,
  /* 0xFD */  NULL,
  /* 0xFE */  NULL,
  /* 0xFF */  NULL,
};

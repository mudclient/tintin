/******************************************************************************
*   This file is part of TinTin++                                             *
*                                                                             *
*   Copyright 2004-2019 Igor van den Hoven                                    *
*                                                                             *
*   TinTin++ is free software; you can redistribute it and/or modify          *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 3 of the License, or         *
*   (at your option) any later version.                                       *
*                                                                             *
*   This program is distributed in the hope that it will be useful,           *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*   GNU General Public License for more details.                              *
*                                                                             *
*                                                                             *
*   You should have received a copy of the GNU General Public License         *
*   along with TinTin++.  If not, see https://www.gnu.org/licenses.           *
******************************************************************************/

/******************************************************************************
*                                                                             *
*              (T)he K(I)cki(N) (T)ickin D(I)kumud Clie(N)t                   *
*                                                                             *
*                        coded by peter unold 1992                            *
*                       modified by Bill Reiss 1993                           *
*                    recoded by Igor van den Hoven 2004                       *
******************************************************************************/
	
#include <stdio.h>
#include <zlib.h>
#include <signal.h>
#include <ctype.h>
#include <stdarg.h>
#include <termios.h>
#include <pcre.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>

/******************************************************************************
*   Autoconf patching by David Hedbor                                         *
*******************************************************************************/

#include "config.h"

#if defined(HAVE_STRING_H)
#include <string.h>
#elif defined(HAVE_STRINGS_H)
#include <strings.h>
#endif


#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif


#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif


#ifdef HAVE_TIME_H
#include <time.h>
#endif


#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#endif


#ifdef SOCKS
#include <socks.h>
#endif

#ifndef BADSIG
#define BADSIG (RETSIGTYPE (*)(int))-1
#endif


#ifdef HAVE_NET_ERRNO_H
#include <net/errno.h>
#endif


#ifdef HAVE_GNUTLS_H
#include <gnutls/gnutls.h>
#include <gnutls/x509.h>
#else
#define gnutls_session_t int
#endif


#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif


#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifndef __TINTIN_H__
#define __TINTIN_H__

/*
	A bunch of constants
*/

#define FALSE                            0
#define TRUE                             1

#define GET_ONE                          0 /* stop at spaces */
#define GET_ALL                          1 /* stop at semicolon */
#define GET_NST                          2 /* nest square brackets */
#define GET_VBT                          4 /* ignore semicolon for verbatim mode */

#define TEL_N                            0
#define TEL_Y                            1
#define TEL_I                            2

#define SCREEN_WIDTH                    80
#define SCREEN_HEIGHT                   24

#define SORT_PRIORITY                    0
#define SORT_ALPHA                       1
#define SORT_APPEND                      2
#define SORT_DELAY                       3

#define DEFAULT_OPEN                   '{'
#define DEFAULT_CLOSE                  '}'

#define COMMAND_SEPARATOR              ';'

#define TINTIN_DIR               ".tintin"
#define HISTORY_FILE         "history.txt"

#define STRING_SIZE                  64000
#define BUFFER_SIZE                  32000
#define STACK_SIZE                    1000
#define NUMBER_SIZE                    100
#define LEGEND_SIZE                     50
#define COLOR_SIZE                      25
#define LIST_SIZE                        2

#define CLIENT_NAME              "TinTin++"
#define CLIENT_VERSION           "2.01.90 "

#define XT_E                            0x27
#define XT_C                            0x5B
#define XT_O                            0x5D
#define XT_W                            0x74
#define XT_T                            0x07
#define XT_CS                           0x73
#define XT_S                            "\073"
#define XT_V                            "\000"

/*
	<faa> - red
	<afa> - green
	<ffa> - yellow
	<aaf> - blue
	<faf> - magenta
	<aff> - cyan
	<fff> - white
	<acf> - Azure
	<afc> - Jade
	<caf> - Violet
	<cfa> - Lime
	<fac> - Pink
	<fca> - Orange
*/

#define COLOR_BRACE         "\e[38;5;164m" // "<eae>" // magenta
#define COLOR_COMMAND       "\e[38;5;044m" // "<aee>" // cyan
#define COLOR_RESET         "\e[0m"        // "<088>" // reset
#define COLOR_SEPARATOR     "\e[38;5;160m" // "<eaa>" // red
#define COLOR_STATEMENT     "\e[38;5;040m" // "<aea>" // green
#define COLOR_STRING        "\e[38;5;188m" // "<eee>" // white
#define COLOR_TEXT          "\e[0m"        // "<ddd>" // grey
#define COLOR_TINTIN        "\e[38;5;184m" // "<eea>" // yellow


#define ESCAPE                          27

#define TIMER_POLL_INPUT                 0
#define TIMER_POLL_SESSIONS              1
#define TIMER_POLL_CHAT                  2
#define TIMER_POLL_PORT                  3
#define TIMER_UPDATE_TICKS               4
#define TIMER_UPDATE_DELAYS              5
#define TIMER_UPDATE_PACKETS             6
#define TIMER_UPDATE_CHAT                7
#define TIMER_UPDATE_TERMINAL            8
#define TIMER_UPDATE_TIME                9
#define TIMER_UPDATE_MEMORY             10
#define TIMER_STALL_PROGRAM             11
#define TIMER_CPU                       12


#define PULSE_PER_SECOND                20

#define PULSE_POLL_INPUT                 1
#define PULSE_POLL_SESSIONS              1
#define PULSE_POLL_CHAT                  2
#define PULSE_POLL_PORT                  2
#define PULSE_UPDATE_TICKS               1
#define PULSE_UPDATE_DELAYS              1
#define PULSE_UPDATE_PACKETS             2
#define PULSE_UPDATE_CHAT                2
#define PULSE_UPDATE_TERMINAL            1
#define PULSE_UPDATE_MEMORY              2
#define PULSE_UPDATE_TIME                2

/*
	Index for lists used by tintin
*/


#define LIST_ACTION                      0
#define LIST_ALIAS                       1
#define LIST_CLASS                       2
#define LIST_COMMAND                     3
#define LIST_CONFIG                      4
#define LIST_DELAY                       5
#define LIST_EVENT                       6
#define LIST_FUNCTION                    7
#define LIST_GAG                         8
#define LIST_HIGHLIGHT                   9
#define LIST_HISTORY                    10
#define LIST_MACRO                      11
#define LIST_PATH                       12
#define LIST_PATHDIR                    13
#define LIST_PROMPT                     14
#define LIST_SUBSTITUTE                 15
#define LIST_TAB                        16
#define LIST_TICKER                     17
#define LIST_VARIABLE                   18
#define LIST_MAX                        19

/*
	Command type
*/

enum operators
{
	TOKEN_TYPE_BREAK,
	TOKEN_TYPE_CASE,
	TOKEN_TYPE_COMMAND,
	TOKEN_TYPE_CONTINUE,
	TOKEN_TYPE_DEFAULT,
	TOKEN_TYPE_END,
	TOKEN_TYPE_ELSE,
	TOKEN_TYPE_ELSEIF,
	TOKEN_TYPE_FOREACH,
	TOKEN_TYPE_BROKEN_FOREACH,
	TOKEN_TYPE_IF,
	TOKEN_TYPE_LOOP,
	TOKEN_TYPE_BROKEN_LOOP,
	TOKEN_TYPE_PARSE,
	TOKEN_TYPE_BROKEN_PARSE,
	TOKEN_TYPE_REGEX,
	TOKEN_TYPE_RETURN,
	TOKEN_TYPE_SESSION,
	TOKEN_TYPE_STRING,
	TOKEN_TYPE_SWITCH,
	TOKEN_TYPE_WHILE,
	TOKEN_TYPE_BROKEN_WHILE
};

/*
	Various flags
*/

#define BV01 (1   <<  0)
#define BV02 (1   <<  1)
#define BV03 (1   <<  2)
#define BV04 (1   <<  3)
#define BV05 (1   <<  4)
#define BV06 (1   <<  5)
#define BV07 (1   <<  6)
#define BV08 (1   <<  7)
#define BV09 (1   <<  8)
#define BV10 (1   <<  9)
#define BV11 (1   << 10)
#define BV12 (1   << 11)
#define BV13 (1   << 12)
#define BV14 (1   << 13)
#define BV15 (1   << 14)
#define BV16 (1   << 15)
#define BV17 (1   << 16)
#define BV18 (1   << 17)
#define BV19 (1   << 18)
#define BV20 (1   << 19)
#define BV21 (1   << 20)
#define BV22 (1   << 21)
#define BV23 (1   << 22)
#define BV24 (1   << 23)
#define BV25 (1   << 24)
#define BV26 (1   << 25)
#define BV27 (1   << 26)
#define BV28 (1   << 27)
#define BV29 (1   << 28)
#define BV30 (1   << 29)
#define BV31 (1   << 30)
#define BV32 (1LL << 31)
#define BV33 (1LL << 32)
#define BV34 (1LL << 33)
#define BV35 (1LL << 34)
#define BV36 (1LL << 35)
#define BV37 (1LL << 36)
#define BV38 (1LL << 37)
#define BV39 (1LL << 38)

#define COL_BLD                       BV01
#define COL_UND                       BV02
#define COL_BLK                       BV03
#define COL_REV                       BV04
#define COL_XTF                       BV05
#define COL_XTF_5                     BV06
#define COL_XTF_R                     BV07
#define COL_XTB                       BV08
#define COL_XTB_5                     BV09
#define COL_XTB_R                     BV10
#define COL_TCF                       BV11
#define COL_TCF_2                     BV12
#define COL_TCF_R                     BV13
#define COL_TCB                       BV14
#define COL_TCB_2                     BV15
#define COL_TCB_R                     BV16

#define CHAT_NAME_CHANGE                 1
#define CHAT_REQUEST_CONNECTIONS         2
#define CHAT_CONNECTION_LIST             3
#define CHAT_TEXT_EVERYBODY              4
#define CHAT_TEXT_PERSONAL               5
#define CHAT_TEXT_GROUP                  6
#define CHAT_MESSAGE                     7
#define CHAT_DO_NOT_DISTURB              8

#define CHAT_SEND_ACTION                 9
#define CHAT_SEND_ALIAS                 10
#define CHAT_SEND_MACRO                 11
#define CHAT_SEND_VARIABLE              12
#define CHAT_SEND_EVENT                 13
#define CHAT_SEND_GAG                   14
#define CHAT_SEND_HIGHLIGHT             15
#define CHAT_SEND_LIST                  16
#define CHAT_SEND_ARRAY                 17
#define CHAT_SEND_BARITEM               18

#define CHAT_VERSION                    19
#define CHAT_FILE_START                 20
#define CHAT_FILE_DENY                  21
#define CHAT_FILE_BLOCK_REQUEST         22
#define CHAT_FILE_BLOCK                 23     
#define CHAT_FILE_END                   24
#define CHAT_FILE_CANCEL                25
#define CHAT_PING_REQUEST               26 
#define CHAT_PING_RESPONSE              27 

#define CHAT_PEEK_CONNECTIONS           28
#define CHAT_PEEK_LIST                  29
#define CHAT_SNOOP_START                30
#define CHAT_SNOOP_DATA                 31

#define CHAT_END_OF_COMMAND            255

#define CHAT_FLAG_PRIVATE             BV01
#define CHAT_FLAG_REQUEST             BV02
#define CHAT_FLAG_SERVE               BV03
#define CHAT_FLAG_IGNORE              BV04
#define CHAT_FLAG_FORWARD             BV05
#define CHAT_FLAG_FORWARDBY           BV06
#define CHAT_FLAG_FORWARDALL          BV07
#define CHAT_FLAG_DND                 BV08
#define CHAT_FLAG_LINKLOST            BV09

#define PORT_FLAG_PRIVATE             BV01
#define PORT_FLAG_REQUEST             BV02
#define PORT_FLAG_SERVE               BV03
#define PORT_FLAG_IGNORE              BV04
#define PORT_FLAG_FORWARD             BV05
#define PORT_FLAG_FORWARDBY           BV06
#define PORT_FLAG_FORWARDALL          BV07
#define PORT_FLAG_DND                 BV08
#define PORT_FLAG_LINKLOST            BV09

#define PORT_RANK_SPY                    0
#define PORT_RANK_SCOUT                  1

#define RANK_FLAG_SCOUT               BV01

#define COMM_FLAG_DISCONNECT          BV01
#define COMM_FLAG_PASSWORD            BV02
#define COMM_FLAG_REMOTEECHO          BV03
#define COMM_FLAG_EOR                 BV04
#define COMM_FLAG_MSDPUPDATE          BV05
#define COMM_FLAG_256COLORS           BV06
#define COMM_FLAG_UTF8                BV07
#define COMM_FLAG_GMCP                BV08

#define MSDP_FLAG_COMMAND             BV01
#define MSDP_FLAG_LIST                BV02
#define MSDP_FLAG_SENDABLE            BV03
#define MSDP_FLAG_REPORTABLE          BV04
#define MSDP_FLAG_CONFIGURABLE        BV05
#define MSDP_FLAG_REPORTED            BV06
#define MSDP_FLAG_UPDATED             BV07

#define MTTS_FLAG_ANSI                BV01
#define MTTS_FLAG_VT100               BV02
#define MTTS_FLAG_UTF8                BV03
#define MTTS_FLAG_256COLORS           BV04
#define MTTS_FLAG_MOUSETRACKING       BV05
#define MTTS_FLAG_COLORPALETTE        BV06
#define MTTS_FLAG_SCREENREADER        BV07
#define MTTS_FLAG_PROXY               BV08
#define MTTS_FLAG_TRUECOLOR           BV09

#define SCREEN_FLAG_CSIP              BV01
#define SCREEN_FLAG_OSCT              BV02
#define SCREEN_FLAG_OMIT              BV03
#define SCREEN_FLAG_GET_ONE           BV04
#define SCREEN_FLAG_GET_ALL           BV05

#define SUB_NONE                      0
#define SUB_ARG                       (1 <<  0)
#define SUB_VAR                       (1 <<  1)
#define SUB_FUN                       (1 <<  2)
#define SUB_COL                       (1 <<  3)
#define SUB_ESC                       (1 <<  4)
#define SUB_CMD                       (1 <<  5)
#define SUB_SEC                       (1 <<  6)
#define SUB_EOL                       (1 <<  7)
#define SUB_LNF                       (1 <<  8)
#define SUB_FIX                       (1 <<  9)
#define SUB_CMP                       (1 << 10)
#define SUB_LIT                       (1 << 11)

#define TINTIN_FLAG_RESETBUFFER       (1 <<  0)
#define TINTIN_FLAG_CONVERTMETACHAR   (1 <<  1)
#define TINTIN_FLAG_HISTORYBROWSE     (1 <<  2)
#define TINTIN_FLAG_HISTORYSEARCH     (1 <<  3)
#define TINTIN_FLAG_PROCESSINPUT      (1 <<  4)
#define TINTIN_FLAG_INHERITANCE       (1 <<  5)
#define TINTIN_FLAG_INSERTINPUT       (1 <<  6)
#define TINTIN_FLAG_CHILDLOCK         (1 <<  7)
#define TINTIN_FLAG_TERMINATE         (1 <<  8)
#define TINTIN_FLAG_MOUSETRACKING     (1 <<  9)

#define SES_FLAG_ECHOCOMMAND          BV01
#define SES_FLAG_SNOOP                BV02
#define SES_FLAG_MCCP                 BV03
#define SES_FLAG_PATHMAPPING          BV04
#define SES_FLAG_SPLIT                BV05
#define SES_FLAG_SPEEDWALK            BV06
#define SES_FLAG_READMUD              BV08
#define SES_FLAG_WORDWRAP             BV09
#define SES_FLAG_VERBATIM             BV10
#define SES_FLAG_CONNECTED            BV11
#define SES_FLAG_REPEATENTER          BV12
#define SES_FLAG_VERBOSE              BV13
#define SES_FLAG_LOGNEXT              BV14
#define SES_FLAG_LOGLEVEL             BV15
#define SES_FLAG_LOGPLAIN             BV16
#define SES_FLAG_LOGHTML              BV17
#define SES_FLAG_GAG                  BV18
#define SES_FLAG_UPDATEVTMAP          BV19
#define SES_FLAG_COLORPATCH           BV20
#define SES_FLAG_SCROLLLOCK           BV21
//#define SES_FLAG_SCROLLSTOP           BV22
#define SES_FLAG_SCAN                 BV23
#define SES_FLAG_SCANABORT            BV24
#define SES_FLAG_CONVERTMETA          BV25
#define SES_FLAG_RUN                  BV26
#define SES_FLAG_BIG5                 BV27
#define SES_FLAG_UTF8                 BV28
#define SES_FLAG_BIG5TOUTF8           BV29
#define SES_FLAG_KOI8TOUTF8           BV30
#define SES_FLAG_FANSITOUTF8          BV31
#define SES_FLAG_CHARSETS             SES_FLAG_BIG5|SES_FLAG_UTF8|SES_FLAG_KOI8TOUTF8|SES_FLAG_BIG5TOUTF8|SES_FLAG_FANSITOUTF8
#define SES_FLAG_CLOSED               BV32
#define SES_FLAG_PORT                 BV33
#define SES_FLAG_SCREENREADER         BV34
#define SES_FLAG_ANSICOLOR            BV35
#define SES_FLAG_256COLOR             BV36
#define SES_FLAG_TRUECOLOR            BV37
#define SES_FLAG_MOUSETRACKING        BV38
#define SES_FLAG_AUTOPATCH            BV39

#define TELOPT_FLAG_TELNET            BV01
#define TELOPT_FLAG_SGA               BV02
#define TELOPT_FLAG_ECHO              BV03
#define TELOPT_FLAG_NAWS              BV04
#define TELOPT_FLAG_PROMPT            BV05
#define TELOPT_FLAG_DEBUG             BV06
#define TELOPT_FLAG_TSPEED            BV07
#define TELOPT_FLAG_TTYPE             BV08
#define TELOPT_FLAG_MTTS              BV09

#define LIST_FLAG_IGNORE              (1 <<  0)
#define LIST_FLAG_MESSAGE             (1 <<  1)
#define LIST_FLAG_DEBUG               (1 <<  2)
#define LIST_FLAG_INFO                (1 <<  3)
#define LIST_FLAG_LOG                 (1 <<  4)
#define LIST_FLAG_CLASS               (1 <<  5)
#define LIST_FLAG_READ                (1 <<  6)
#define LIST_FLAG_WRITE               (1 <<  7)
#define LIST_FLAG_HIDE                (1 <<  8)
#define LIST_FLAG_INHERIT             (1 <<  9)
#define LIST_FLAG_REGEX               (1 << 10)
#define LIST_FLAG_NEST                (1 << 11)
#define LIST_FLAG_DEFAULT             LIST_FLAG_MESSAGE

#define NODE_FLAG_META                (1 <<  0)

#define LOG_FLAG_LINEFEED             BV01
#define LOG_FLAG_PLAIN                BV02
#define LOG_FLAG_RAW                  BV03
#define LOG_FLAG_HTML                 BV04

// Saved in map files, so don't swap around

// keep synced with exit flags

#define ROOM_FLAG_AVOID               BV01
#define ROOM_FLAG_HIDE                BV02
#define ROOM_FLAG_LEAVE               BV03
#define ROOM_FLAG_VOID                BV04
#define ROOM_FLAG_STATIC              BV05
#define ROOM_FLAG_CURVED              BV06
#define ROOM_FLAG_PATH                BV07
#define ROOM_FLAG_NOGLOBAL            BV08
#define ROOM_FLAG_INVISIBLE           BV09


// keep synced with room flags

#define EXIT_FLAG_HIDE                BV01
#define EXIT_FLAG_AVOID               BV02
#define EXIT_FLAG_INVISIBLE           BV03
#define EXIT_FLAG_ALL                 BV01|BV02|BV03

#define GRID_FLAG_HIDE                BV02
#define GRID_FLAG_INVISIBLE           BV03

#define MAP_FLAG_STATIC               BV01
#define MAP_FLAG_VTMAP                BV02
#define MAP_FLAG_DIRECTION            BV03
#define MAP_FLAG_ASCIIGRAPHICS        BV04
#define MAP_FLAG_ASCIIVNUMS           BV05
#define MAP_FLAG_MUDFONT              BV06
#define MAP_FLAG_NOFOLLOW             BV07
#define MAP_FLAG_SYMBOLGRAPHICS       BV08
#define MAP_FLAG_UNICODEGRAPHICS      BV09
#define MAP_FLAG_BLOCKGRAPHICS        BV10

#define MAP_SEARCH_NAME                0
#define MAP_SEARCH_EXITS               1
#define MAP_SEARCH_DESC                2
#define MAP_SEARCH_AREA                3
#define MAP_SEARCH_NOTE                4
#define MAP_SEARCH_TERRAIN             5
#define MAP_SEARCH_FLAG                6
#define MAP_SEARCH_MAX                 7

#define MAP_EXIT_N                     1
#define MAP_EXIT_E                     2
#define MAP_EXIT_S                     4
#define MAP_EXIT_W                     8
#define MAP_EXIT_U                    16
#define MAP_EXIT_D                    32
#define MAP_EXIT_NE                   64
#define MAP_EXIT_NW                  128
#define MAP_EXIT_SE                  256
#define MAP_EXIT_SW                  512

#define MAP_DIR_N                     (1LL << MAP_EXIT_N)
#define MAP_DIR_E                     (1LL << MAP_EXIT_E)
#define MAP_DIR_S                     (1LL << MAP_EXIT_S)
#define MAP_DIR_W                     (1LL << MAP_EXIT_W)
#define MAP_DIR_U                     (1LL << MAP_EXIT_U)
#define MAP_DIR_D                     (1LL << MAP_EXIT_D)
#define MAP_DIR_NE                    (1LL << (MAP_EXIT_N|MAP_EXIT_E))
#define MAP_DIR_NW                    (1LL << (MAP_EXIT_N|MAP_EXIT_W))
#define MAP_DIR_SE                    (1LL << (MAP_EXIT_S|MAP_EXIT_E))
#define MAP_DIR_SW                    (1LL << (MAP_EXIT_S|MAP_EXIT_W))

#define MAP_UNDO_MOVE                 (1 <<  0)
#define MAP_UNDO_CREATE               (1 <<  1)
#define MAP_UNDO_LINK                 (1 <<  2)
#define MAP_UNDO_INSERT               (1 <<  3)

#define STR_HASH_FLAG_NOGREP          (1 <<  0)

#define MAX_STR_HASH                  50000


#define MOUSE_FLAG_BUTTON_A             1
#define MOUSE_FLAG_BUTTON_B             2
#define MOUSE_FLAG_SHIFT                4
#define MOUSE_FLAG_ALT                  8
#define MOUSE_FLAG_CTRL                16
#define MOUSE_FLAG_MOTION              32
#define MOUSE_FLAG_WHEEL               64
#define MOUSE_FLAG_EXTRA              128
#define MOUSE_FLAG_UNKNOWN            256

#define CURSOR_FLAG_ALWAYS              1
//#define CURSOR_FLAG_NEVER               2
#define CURSOR_FLAG_GET_ONE             4
#define CURSOR_FLAG_GET_ALL             8

#define STARTUP_FLAG_NOGREETING         1
#define STARTUP_FLAG_SCREENREADER       2
#define STARTUP_FLAG_NORESET            4
#define STARTUP_FLAG_ARGUMENT           8


#define LEGEND_ASCII                    0
#define LEGEND_ASCII_MISC              16
#define LEGEND_ASCII_CURVED            20
#define LEGEND_ASCII_DIRS              24
#define LEGEND_UNICODE                 32
#define LEGEND_UNICODE_MISC            48
#define LEGEND_UNICODE_CURVED          52
#define LEGEND_UNICODE_DIRS            56
#define LEGEND_MUDFONT                 64
#define LEGEND_MUDFONT_NWS             64
#define LEGEND_MUDFONT_NES             96
#define LEGEND_MUDFONT_CURVED         192
#define LEGEND_MAX                    200

#define MAP_COLOR_AVOID                 0
#define MAP_COLOR_BACK                  1
#define MAP_COLOR_EXIT                  2
#define MAP_COLOR_HIDE                  3
#define MAP_COLOR_INVIS                 4
#define MAP_COLOR_PATH                  5
#define MAP_COLOR_ROOM                  6
#define MAP_COLOR_SYMBOL                7
#define MAP_COLOR_USER                  8
#define MAP_COLOR_MAX                   9

/*
	Some macros to deal with double linked lists
*/

#define LINK(link, head, tail) \
{ \
	if ((head) == NULL) \
	{ \
		(head) = (link); \
	} \
	else \
	{ \
		(tail)->next = (link); \
	} \
	(link)->next = NULL; \
	(link)->prev = (tail); \
	(tail)				    = (link); \
}


#define INSERT_LEFT(link, right, head) \
{ \
	(link)->prev = (right)->prev; \
	(right)->prev = (link); \
	(link)->next = (right); \
	\
	if ((link)->prev) \
	{ \
		(link)->prev->next = (link); \
	} \
	else \
	{ \
		(head) = (link); \
	} \
}


#define INSERT_RIGHT(link, left, tail) \
{ \
	(link)->next = (left)->next; \
	(left)->next = (link); \
	(link)->prev = (left); \
\
	if ((link)->next) \
	{ \
		(link)->next->prev = (link); \
	} \
	else \
	{ \
		(tail) = (link); \
	} \
}

#define UNLINK(link, head, tail) \
{ \
	if (((link)->prev == NULL && (link) != head) \
	||  ((link)->next == NULL && (link) != tail)) \
	{ \
		tintin_printf2(NULL, "#UNLINK ERROR in file %s on line %d", __FILE__, __LINE__); \
		dump_stack(); \
	} \
	if ((link)->prev == NULL) \
	{ \
		(head)			   = (link)->next; \
	} \
	else \
	{ \
		(link)->prev->next	  = (link)->next; \
	} \
	if ((link)->next == NULL) \
	{ \
		(tail)			    = (link)->prev; \
	} \
	else \
	{ \
		(link)->next->prev	  = (link)->prev; \
	} \
	(link)->next = NULL; \
	(link)->prev = NULL; \
}

/*
	string allocation
*/

#define RESTRING(point, value) \
{ \
	if (point) free(point); \
	point = strdup((value)); \
}

#define FREE(point) \
{ \
	free((point)); \
	point = NULL; \
}

/*
	Bit operations
*/

#define HAS_BIT(bitvector, bit)   ((bitvector)  & (bit))
#define SET_BIT(bitvector, bit)   ((bitvector) |= (bit))
#define DEL_BIT(bitvector, bit)   ((bitvector) &= (~(bit)))
#define TOG_BIT(bitvector, bit)   ((bitvector) ^= (bit))
#define FFS_BIT(bitvector)        ((ffs(bitvector) - 1)) 

/*
	Generic
*/

#define URANGE(a, b, c)           ((b) < (a) ? (a) : (b) > (c) ? (c) : (b))
#define UMAX(a, b)                ((a) > (b) ? (a) : (b))
#define UMIN(a, b)                ((a) < (b) ? (a) : (b))

#define next_arg(u)               (u < 99 ? u++ : u)

#define IS_SPLIT(ses)             (gtd->screen->rows != (ses)->bot_row)
#define SCROLL(ses)               ((ses)->cur_row == 0 || ((ses)->cur_row >= (ses)->top_row && (ses)->cur_row <= (ses)->bot_row) || (ses)->cur_row == gtd->screen->rows)
#define VERBATIM(ses)             (gtd->verbatim_level || (gtd->input_level == 0 && HAS_BIT((ses)->flags, SES_FLAG_VERBATIM)))

#define DO_ARRAY(array) struct session *array (struct session *ses, struct listnode *list, char *arg, char *var)
#define DO_BUFFER(buffer) void buffer (struct session *ses, char *arg)
#define DO_CHAT(chat) void chat (char *arg1, char *arg2)
#define DO_CLASS(class) struct session *class (struct session *ses, char *arg1, char *arg2)
#define DO_COMMAND(command) struct session  *command (struct session *ses, char *arg)
#define DO_CONFIG(config) struct session *config (struct session *ses, char *arg, int index)
#define DO_CURSOR(cursor) void cursor (struct session *ses, char *arg)
#define DO_HISTORY(history) void history (struct session *ses, char *arg)
#define DO_LINE(line) struct session *line (struct session *ses, char *arg)
#define DO_MAP(map) void map (struct session *ses, char *arg, char *arg1, char *arg2)
#define DO_PATH(path) void path (struct session *ses, char *arg)
#define DO_PORT(port) struct session *port (struct session *ses, char *arg1, char *arg2, char *arg)
#define DO_SCREEN(screen) void screen (struct session *ses, int ind, char *arg, char *arg1, char *arg2)

/*
	Compatibility
*/


#define atoll(str) (strtoll(str, NULL, 10))


/************************ structures *********************/

struct listroot
{
	struct listnode      ** list;
	struct session        * ses;
	int                     size;
	int                     used;
	int                     type;
	int                     update;
	int                     flags;
};


struct listnode
{
	struct listroot       * root;
	char                  * arg1;
	char                  * arg2;
	char                  * arg3;
	char                  * arg4;
	char                  * group;
	pcre                  * regex;
	long long               data;
};


struct session
{
	struct session        * next;
	struct session        * prev;
	struct map_data       * map;
	struct port_data      * port;
	z_stream              * mccp;
	z_stream              * mccp3;
	gnutls_session_t        ssl;
	struct termios          cur_terminal;
	struct scroll_data    * scroll;
	char                  * name;
	char                  * group;
	FILE                  * logfile;
	FILE                  * lognext_file;
	char                  * lognext_name;
	time_t                  lognext_time;
	FILE                  * logline_file;
	char                  * logline_name;
	time_t                  logline_time;
	struct listroot       * list[LIST_MAX];
	int                     created;
	int                     top_split;
	int                     bot_split;
	int                     top_row;
	int                     bot_row;
	int                     cur_row;
	int                     sav_row;
	int                     cur_col;
	int                     sav_col;
	int                     fgc;
	int                     bgc;
	int                     vtc;
	int                     socket;
	int                     telopts;
	int                     telopt_flag[8];
	long long               flags;
	char                  * session_host;
	char                  * session_ip;
	char                  * session_port;
	char                  * cmd_color;
	unsigned char         * read_buf;
	int                     read_len;
	int                     read_max;
	unsigned long long      connect_retry;
	int                     connect_error;
	char                    more_output[BUFFER_SIZE];
	char                    color[100];
	unsigned long long      check_output;
	int                     auto_tab;
	unsigned long long      rand;
};


struct tintin_data
{
	struct session        * ses;
	struct session        * update;
	struct session        * all;
	struct session        * dispose_next;
	struct session        * dispose_prev;
	struct chat_data      * chat;
	struct termios          old_terminal;
	struct screen_data    * screen;
	char                  * buf;
	char                  * out;
	char                  * mud_output_buf;
	int                     mud_output_max;
	int                     mud_output_len;
	unsigned char         * mccp_buf;
	int                     mccp_len;
	char                    input_buf[BUFFER_SIZE];
	char                    input_tmp[BUFFER_SIZE];
	char                    macro_buf[BUFFER_SIZE];
	char                    paste_buf[BUFFER_SIZE];
	int                     input_off;
	int                     input_len;
	int                     input_cur;
	int                     input_pos;
	int                     input_hid;
	int                     input_tab;
	char                  * home;
	char                  * lang;
	char                  * term;
	time_t                  time;
	struct tm		calendar;
	unsigned long long      utime;
	long long               timer[TIMER_CPU][5];
	long long               total_io_ticks;
	long long               total_io_exec;
	long long               total_io_delay;
	int                     str_size;
	int                     str_hash_size;
	int                     history_size;
	int                     command_ref[26];
	int                     msdp_table_size;
	int                     flags;
	int                     quiet;
	int                     debug_level;
	int                     input_level;
	int                     verbose_level;
	int                     verbatim_level;
	int                     ignore_level;
	int                     scroll_level;
	char                    tintin_char;
	char                    verbatim_char;
	char                    repeat_char;
	char                  * vars[100];
	char                  * cmds[100];
	int                     args[100];
};

struct scroll_data
{
	char                 ** buffer;
	int                     base;
	int                     line;
	int                     max;
	int                     row;
	char                  * input;
};

struct chat_data
{
	struct chat_data      * next;
	struct chat_data      * prev;
	struct chat_data      * update;
	char                  * name;
	char                  * ip;
	char                  * version;
	char                  * download;
	char                  * reply;
	char                  * prefix;
	char                  * paste_buf;
	char                  * color;
	char                  * group;
	int                     port;
	int                     fd;
	time_t                  timeout;
	int                     flags;
	unsigned long long      paste_time;
	FILE                  * file_pt;
	char                  * file_name;
	long long               file_size;
	int                     file_block_cnt;
	int                     file_block_tot;
	int                     file_block_patch;
	unsigned long long      file_start_time;
};

struct port_data
{
	struct port_data      * next;
	struct port_data      * prev;
	struct port_data      * update;
	char                  * name;
	char                  * ip;
	char                  * prefix;
	char                  * color;
	char                  * group;
	int                     port;
	int                     fd;
	int                     flags;
	int                     comm_flags;
	long long               mtts_flags;
	struct msdp_data     ** msdp_data;
	char                  * proxy;
	char                  * ttype;
	char                    telbuf[BUFFER_SIZE];
	int                     teltop;
	char                    inbuf[BUFFER_SIZE];
	int                     intop;
	int                     cols;
	int                     rows;
	int                     total;
	int                     rank;
	z_stream              * mccp2;
	z_stream              * mccp3;
};

struct link_data
{
	struct link_data     * next;
	struct link_data     * prev;
	char                 * str1;
	char                 * str2;
	char                 * str3;
};

/*
	Typedefs
*/

typedef struct session *ARRAY   (struct session *ses, struct listnode *list, char *arg, char *var);
typedef void            CHAT    (char *left, char *right);
typedef struct session *CLASS   (struct session *ses, char *left, char *right);
typedef struct session *CONFIG  (struct session *ses, char *arg, int index);
typedef struct session *COMMAND (struct session *ses, char *arg);
typedef void            MAP     (struct session *ses, char *arg, char *arg1, char *arg2);
typedef void            CURSOR  (struct session *ses, char *arg);
typedef void            PATH    (struct session *ses, char *arg);
typedef struct session *PORT    (struct session *ses, char *arg1, char *arg2, char *arg);
typedef struct session *LINE    (struct session *ses, char *arg);
typedef void            HISTORY (struct session *ses, char *arg);
typedef void            BUFFER  (struct session *ses, char *arg);
typedef void            MSDP    (struct session *ses, struct port_data *buddy, int index);
typedef void            SCREEN  (struct session *ses, int ind, char *arg, char *arg1, char *arg2);

/*
	Structures for tables.c
*/

struct array_type
{
	char                  * name;
	ARRAY                 * array;
	int                     lval;
	int                     rval;
};

struct chat_type
{
	char                  * name;
	CHAT                  * fun;
	int                     lval;
	int                     rval;
	char                  * desc;
};

struct class_type
{
	char                  * name;
	CLASS                 * group;
};

struct color_type
{
	char                  * name;
	char                  * code;
};

struct command_type
{
	char                  * name;
	COMMAND               * command;
	int                     type;
};

struct config_type
{
	char                  * name;
	char                  * msg_on;
	char                  * msg_off;
	CONFIG                * config;
};

struct event_type
{
	char                  * name;
	char                  * desc;
};

struct list_type
{
	char                  * name;
	char                  * name_multi;
	int                     mode;
	int                     args;
	int                     flags;
};

struct port_type
{
	char                  * name;
	PORT                  * fun;
	int                     lval;
	int                     rval;
	char                  * desc;
};

struct rank_type
{
	char                  * name;
	int                     flags;
};

struct substitution_type
{
	char                  * name;
	int                     bitvector;
};

struct map_type
{
	char                  * name;
	MAP                   * map;
	int                     flags;
	int                     check;
};

struct cursor_type
{
	char                  * name;
	char                  * desc;
	char                  * code;
	int                     flags;
	CURSOR                * fun;
};

struct timer_type
{
	char                  * name;
};

struct path_type
{
	char                  * name;
	PATH                  * fun;
	char                  * desc;
};

struct line_type
{
	char                  * name;
	LINE                  * fun;
};

struct history_type
{
	char                  * name;
	HISTORY               * fun;
	char                  * desc;
};

struct buffer_type
{
	char                  * name;
	BUFFER                * fun;
	char                  * desc;
};

struct telopt_type
{
	char                  * name;
	int                     want;
	int                     flags;
};

struct msdp_type
{
	char                 * name;
	int                    flags;
	int                    rank;
	MSDP                 * fun;
};

struct screen_type
{
	char                  * name;
	char                  * desc;
	int                     get1;
	int                     get2;
	int                     flags;
	SCREEN                * fun;
};

struct map_legend_type
{
	char                  * name;
	char                  * group;
	char                  * min;
	char                  * max;
};

struct map_group_type
{
	char                  * name;
	char                  * group;
	int                     min_row;
	int                     min_col;
	int                     max_row;
	int                     max_col;
	int                     start;
	int                     end;
	char                  * reset;
};

struct msdp_data
{
	char                 * value;
	int                    flags;
};

struct str_data
{
	int                       max;
	int                       len;
};

struct str_hash_data
{
	struct str_hash_data    * next;
	struct str_hash_data    * prev;
	unsigned int              count;
	int                       flags;
	int                       lines;
	int                       length;
	int                       hash;
};

struct str_hash_index_data
{
	struct str_hash_data    * f_node;
	struct str_hash_data    * l_node;
};

struct row_data
{
	char                  * str;
};

struct screen_data
{
	struct row_data      ** lines;
	int                     rows;
	int                     cols;
	int                     height;
	int                     width;
	int                     pos_height;
	int                     pos_width;
	int                     minimized;
	int                     focus;
	int                     char_height;
	int                     char_width;
	int                     desk_height;
	int                     desk_width;
	int                     top_row;
	int                     bot_row;
	int                     cur_row;
	int                     cur_col;
	int                     sav_row;
	int                     sav_col;
	int                     max_row;
};

struct map_data
{
	struct room_data     ** room_list;
	struct room_data     ** grid_rooms;
	int                   * grid_flags;
	FILE                  * logfile;
	struct link_data      * undo_head;
	struct link_data      * undo_tail;
	struct search_data    * search;
	char                  * buf;
	char                  * out;
	char                    color[MAP_COLOR_MAX][COLOR_SIZE];
	int                     max_grid_x;
	int                     max_grid_y;
	int                     top_row;
	int                     top_col;
	int                     bot_row;
	int                     bot_col;
	int                     rows;
	int                     cols;
	int                     undo_size;
	int                     dir;
	int                     size;
	int                     flags;
	int                     in_room;
	int                     at_room;
	int                     last_room;
	int                     global_vnum;
	struct exit_data      * global_exit;
	int                     version;
	int                     display_stamp;
	int                     nofollow;
	char                    legend[LEGEND_MAX][LEGEND_SIZE];
	char                    legend_raw[LEGEND_MAX][LEGEND_SIZE];
};

struct room_data
{
	struct exit_data        * f_exit;
	struct exit_data        * l_exit;
	int                       vnum;
	int                       exit_size;
	long long                 exit_dirs;
	float                     length;
	float                     weight;
	int                       search_stamp;
	int                       display_stamp;
	int                       flags;
	int                       w;
	int                       x;
	int                       y;
	int                       z;
	char                    * color;
	char                    * name;
	char                    * symbol;
	char                    * desc;
	char                    * area;
	char                    * note;
	char                    * terrain;
	char                    * data;
};

struct exit_data
{
	struct exit_data        * next;
	struct exit_data        * prev;
	int                       vnum;
	int                       dir;
	int                       flags;
	float                     weight;
	char                    * name;
	char                    * cmd;
	char                    * data;
};

struct search_data
{
	int                     vnum;
	short                   stamp;
	pcre                  * name;
	int                     exit_size;
	long long               exit_dirs;
	char                  * exit_list;
	pcre                  * desc;
	pcre                  * area;
	pcre                  * note;
	pcre                  * terrain;
	long long               flag;
};

#endif


/*
	Function declarations
*/

#ifndef __ACTION_H__
#define __ACTION_H__

extern struct session *do_action(struct session *ses, char *arg);
extern struct session *do_unaction(struct session *ses, char *arg);

extern void check_all_actions(struct session *ses, char *original, char *line);

#endif


#ifndef __ADVERTISE_H__
#define __ADVERTISE_H__

extern DO_COMMAND(do_advertise);

#endif


#ifndef __CURSOR_H__
#define __CURSOR_H__

extern DO_COMMAND(do_cursor);

extern DO_CURSOR(cursor_backspace);
extern DO_CURSOR(cursor_brace_open);
extern DO_CURSOR(cursor_brace_close);
extern DO_CURSOR(cursor_buffer_down);
extern DO_CURSOR(cursor_buffer_end);
extern DO_CURSOR(cursor_buffer_home);
extern DO_CURSOR(cursor_buffer_lock);
extern DO_CURSOR(cursor_buffer_up);
extern DO_CURSOR(cursor_check_line);
extern DO_CURSOR(cursor_check_line_modified);
extern DO_CURSOR(cursor_clear_left);
extern DO_CURSOR(cursor_clear_line);
extern DO_CURSOR(cursor_clear_right);
extern DO_CURSOR(cursor_convert_meta);
extern DO_CURSOR(cursor_delete);
extern DO_CURSOR(cursor_delete_or_exit);
extern DO_CURSOR(cursor_delete_word_left);
extern DO_CURSOR(cursor_delete_word_right);
extern DO_CURSOR(cursor_echo);
extern DO_CURSOR(cursor_end);
extern DO_CURSOR(cursor_enter);
extern DO_CURSOR(cursor_exit);
extern DO_CURSOR(cursor_get);
extern DO_CURSOR(cursor_history_find);
extern DO_CURSOR(cursor_history_next);
extern DO_CURSOR(cursor_history_prev);
extern DO_CURSOR(cursor_history_search);
extern DO_CURSOR(cursor_home);
extern DO_CURSOR(cursor_info);
extern DO_CURSOR(cursor_insert);
extern DO_CURSOR(cursor_left);
extern DO_CURSOR(cursor_left_word);
extern DO_CURSOR(cursor_paste_buffer);
extern DO_CURSOR(cursor_redraw_input);
extern DO_CURSOR(cursor_redraw_line);
extern DO_CURSOR(cursor_right);
extern DO_CURSOR(cursor_right_word);
extern DO_CURSOR(cursor_set);
extern DO_CURSOR(cursor_suspend);

extern DO_CURSOR(cursor_tab_backward);
extern DO_CURSOR(cursor_tab_forward);
extern DO_CURSOR(cursor_auto_tab_backward);
extern DO_CURSOR(cursor_auto_tab_forward);
extern DO_CURSOR(cursor_mixed_tab_backward);
extern DO_CURSOR(cursor_mixed_tab_forward);

extern DO_CURSOR(cursor_screen_focus_in);
extern DO_CURSOR(cursor_screen_focus_out);

#endif


#ifndef __INPUT_H__
#define __INPUT_H__

extern void  process_input(void);
extern void  read_line();
extern void  read_key(void);
extern void  convert_meta(char *input, char *output, int eol);
extern char *str_convert_meta(char *input, int eol);
extern void  unconvert_meta(char *input, char *output);
extern void  echo_command(struct session *ses, char *line);
extern void  input_printf(char *format, ...);
extern void  modified_input(void);

#endif


#ifndef __ARRAY_H__
#define __ARRAY_H__

extern DO_COMMAND(do_list);
extern DO_ARRAY(array_add);
extern DO_ARRAY(array_clear);
extern DO_ARRAY(array_create);
extern DO_ARRAY(array_insert);
extern DO_ARRAY(array_delete);
extern DO_ARRAY(array_find);
extern DO_ARRAY(array_get);
extern DO_ARRAY(array_simplify);
extern DO_ARRAY(array_size);
extern DO_ARRAY(array_set);
extern DO_ARRAY(array_sort);
extern DO_ARRAY(array_tokenize);

#endif


#ifndef __CHAT_H__
#define __CHAT_H__


extern DO_COMMAND(do_chat);
extern DO_CHAT(chat_accept);
extern DO_CHAT(chat_call);
extern DO_CHAT(chat_cancelfile);
extern DO_CHAT(chat_color);
extern DO_CHAT(chat_decline);
extern DO_CHAT(chat_dnd);
extern DO_CHAT(chat_downloaddir);
extern DO_CHAT(chat_emote);
extern DO_CHAT(chat_filestat);
extern DO_CHAT(chat_group);
extern DO_CHAT(chat_forward);
extern DO_CHAT(chat_forwardall);
extern DO_CHAT(chat_ignore);
extern DO_CHAT(chat_initialize);
extern DO_CHAT(chat_info);
extern DO_CHAT(chat_ip);
extern DO_CHAT(chat_message);
extern DO_CHAT(chat_name);
extern DO_CHAT(chat_paste);
extern DO_CHAT(chat_peek);
extern DO_CHAT(chat_ping);
extern DO_CHAT(chat_prefix);
extern DO_CHAT(chat_private);
extern DO_CHAT(chat_public);
extern DO_CHAT(chat_reply);
extern DO_CHAT(chat_request);
extern DO_CHAT(chat_send);
extern DO_CHAT(chat_sendfile);
extern DO_CHAT(chat_transfer);
extern DO_CHAT(chat_serve);
extern DO_CHAT(chat_uninitialize);
extern DO_CHAT(chat_who);
extern DO_CHAT(chat_zap);

extern  int chat_new(int s);
extern void close_chat(struct chat_data *buddy, int unlink);
extern void process_chat_connections(fd_set *read_set, fd_set *write_set, fd_set *exc_set);
extern void chat_forward_session(struct session *ses, char *linelog);
extern void chat_socket_printf(struct chat_data *buddy, char *format, ...);
extern void chat_printf(char *format, ...);
extern  int process_chat_input(struct chat_data *buddy);
extern void get_chat_commands(struct chat_data *buddy, char *buf, int len);
extern void chat_name_change(struct chat_data *buddy, char *txt);
extern void chat_receive_text_everybody(struct chat_data *buddy, char *txt);
extern void chat_receive_text_personal(struct chat_data *buddy, char *txt);
extern void chat_receive_text_group(struct chat_data *buddy, char *txt);
extern void chat_receive_message(struct chat_data *buddy, char *txt);
extern void chat_receive_snoop_data(struct chat_data *buddy, char *txt);

extern void ping_response(struct chat_data *ch, char *time);

extern void request_response(struct chat_data *requester);
extern void parse_requested_connections(struct chat_data *buddy, char *txt);

extern void peek_response(struct chat_data *peeker);
extern void parse_peeked_connections(struct chat_data *buddy, char *txt);


extern void chat_receive_file(char *arg, struct chat_data *ch);
extern void send_block(struct chat_data *ch);
extern  int receive_block(unsigned char *s, struct chat_data *ch, int size);
extern void deny_file(struct chat_data *ch, char *arg);
extern void file_denied(struct chat_data *ch, char *txt);
extern void file_cleanup(struct chat_data *buddy);

extern int get_file_size(char *fpath);

extern void chat_puts(char *arg);
extern struct chat_data *find_buddy(char *arg);
extern struct chat_data *find_group(char *arg);
extern char *fix_file_name(char *name);

#endif


#ifndef __TERMINAL_H__
#define __TERMINAL_H__

extern void init_terminal(struct session *ses);
extern void reset_terminal(struct session *ses);

extern void save_session_terminal(struct session *ses);
extern void refresh_session_terminal(struct session *ses);

extern void echo_on(struct session *ses);
extern void echo_off(struct session *ses);
extern void init_terminal_size(struct session *ses);
extern int get_scroll_size(struct session *ses);

#endif


#ifndef __CLASS_H__
#define __CLASS_H__

extern DO_COMMAND(do_class);
extern int count_class(struct session *ses, struct listnode *group);
extern DO_CLASS(class_open);
extern DO_CLASS(class_close);
extern DO_CLASS(class_list);
extern DO_CLASS(class_read);
extern DO_CLASS(class_size);
extern DO_CLASS(class_write);
extern DO_CLASS(class_kill);
extern void parse_class(struct session *ses, char *input, struct listnode *group);

#endif


#ifndef __MAPPER_H__
#define __MAPPER_H__

extern DO_COMMAND(do_map);

extern DO_MAP(map_at);
extern DO_MAP(map_color);
extern DO_MAP(map_create);
extern DO_MAP(map_debug);
extern DO_MAP(map_delete);
extern DO_MAP(map_destroy);
extern DO_MAP(map_dig);
extern DO_MAP(map_exit);
extern DO_MAP(map_exitflag);
extern DO_MAP(map_explore);
extern DO_MAP(map_find);
extern DO_MAP(map_flag);
extern DO_MAP(map_get);
extern DO_MAP(map_global);
extern DO_MAP(map_goto);
extern DO_MAP(map_info);
extern DO_MAP(map_insert);
extern DO_MAP(map_jump);
extern DO_MAP(map_leave);
extern DO_MAP(map_legend);
extern DO_MAP(map_link);
extern DO_MAP(map_list);
extern DO_MAP(map_map);
extern DO_MAP(map_move);
extern DO_MAP(map_name);
extern DO_MAP(map_offset);
extern DO_MAP(map_read);
extern DO_MAP(map_resize);
extern DO_MAP(map_return);
extern DO_MAP(map_roomflag);
extern DO_MAP(map_run);
extern DO_MAP(map_set);
extern DO_MAP(map_travel);
extern DO_MAP(map_undo);
extern DO_MAP(map_uninsert);
extern DO_MAP(map_unlink);
extern DO_MAP(map_update);
extern DO_MAP(map_vnum);
extern DO_MAP(map_write);


extern void create_map(struct session *ses, char *arg);
extern void delete_map(struct session *ses);
extern int  create_room(struct session *ses, char *format, ...);
extern void delete_room(struct session *ses, int room, int exits);
extern struct exit_data *create_exit(struct session *ses, int room, char *format, ...);
extern void delete_exit(struct session *ses, int room, struct exit_data *exit);
extern void search_keywords(struct session *ses, char *arg, char *out, char *var);
extern void map_search_compile(struct session *ses, char *arg, char *var);
extern int match_room(struct session *ses, int room, struct search_data *search);
extern int  find_room(struct session *ses, char *arg);
extern void goto_room(struct session *ses, int room);
extern struct exit_data *find_exit(struct session *ses, int room, char *arg);
extern int  get_exit_dir(struct session *ses, char *arg);
extern void set_room_exits(struct session *ses, int room);
extern int  get_room_exits(struct session *ses, int room);
extern void displaygrid_build(struct session *ses, int room, int x, int y, int z);
extern int  follow_map(struct session *ses, char *argument);
extern void add_undo(struct session *ses, char *format, ...);
extern void del_undo(struct session *ses, struct link_data *link);
extern char *draw_room(struct session *ses, struct room_data *room, int line, int x, int y);
extern int searchgrid_find(struct session *ses, int from, struct search_data *search);
extern int searchgrid_walk(struct session *ses, int offset, int from, int dest);
extern void shortest_path(struct session *ses, int run, char *delay, char *arg);
extern void explore_path(struct session *ses, int run, char *left, char *right);
extern int tunnel_void(struct session *ses, int from, int room, int dir);
extern int check_global(struct session *ses, int room);
extern int find_coord(struct session *ses, char *arg);
extern int spatialgrid_find(struct session *ses, int vnum, int x, int y, int z);
extern void show_vtmap(struct session *ses);
extern void map_mouse_handler(struct session *ses, char *left, char *right, int row, int col);

#endif


#ifndef __TT_MATH_H__
#define __TT_MATH_H__

extern DO_COMMAND(do_math);
extern int is_math(struct session *ses, char *str);
extern long double get_number(struct session *ses, char *str);
extern long double get_double(struct session *ses, char *str);
extern void get_number_string(struct session *ses, char *str, char *result);
extern long double mathswitch(struct session *ses, char *left, char *right);
extern void mathexp(struct session *ses, char *str, char *result, int seed);
extern int mathexp_tokenize(struct session *ses, char *str, int seed, int debug);
extern void mathexp_level(struct session *ses, struct link_data *node);
extern void mathexp_compute(struct session *ses, struct link_data *node);
extern long double tintoi(char *str);
extern long double tincmp(char *left, char *right);
extern long double tineval(struct session *ses, char *left, char *right);
extern long double tindice(struct session *ses, char *left, char *right);

#endif


#ifndef __PROMPT_H__
#define __PROMPT_H__

extern DO_COMMAND(do_split);
extern DO_COMMAND(do_unsplit);
extern void init_split(struct session *ses, int top, int bot);
extern void reset_screen(struct session *ses);
extern void dirty_screen(struct session *ses);
extern DO_COMMAND(do_prompt);
extern DO_COMMAND(do_unprompt);
extern void check_all_prompts(struct session *ses, char *original, char *line);
extern void do_one_prompt(struct session *ses, char *prompt, int row, int col);

#endif


#ifndef __TINEXP_H__
#define __TINEXP_H__

extern int substitute(struct session *ses, char *string, char *result, int flags);

extern int match(struct session *ses, char *str, char *exp, int flags);
extern int find(struct session *ses, char *str, char *exp, int sub);
DO_COMMAND(do_regexp);
extern int regexp_compare(pcre *regex, char *str, char *exp, int option, int flag);
extern int check_one_regexp(struct session *ses, struct listnode *node, char *line, char *original, int option);
extern int tintin_regexp_check(struct session *ses, char *exp);
extern int tintin_regexp(struct session *ses, pcre *pcre, char *str, char *exp, int option, int flag);
extern pcre *regexp_compile(char *exp, int option);
extern pcre *tintin_regexp_compile(struct session *ses, struct listnode *node, char *exp, int option);

#endif


#ifndef __CONFIG_H__
#define __CONFIG_H__

extern DO_COMMAND(do_configure);

extern DO_CONFIG(config_speedwalk);
extern DO_CONFIG(config_verbatim);
extern DO_CONFIG(config_repeatenter);
extern DO_CONFIG(config_commandecho);
extern DO_CONFIG(config_commandcolor);
extern DO_CONFIG(config_verbose);
extern DO_CONFIG(config_wordwrap);
extern DO_CONFIG(config_log);
extern DO_CONFIG(config_buffersize);
extern DO_CONFIG(config_scrolllock);
extern DO_CONFIG(config_historysize);
extern DO_CONFIG(config_connectretry);
extern DO_CONFIG(config_packetpatch);
extern DO_CONFIG(config_tintinchar);
extern DO_CONFIG(config_verbatimchar);
extern DO_CONFIG(config_repeatchar);
extern DO_CONFIG(config_debugtelnet);
extern DO_CONFIG(config_convertmeta);
extern DO_CONFIG(config_loglevel);
extern DO_CONFIG(config_colorpatch);
extern DO_CONFIG(config_mccp);
extern DO_CONFIG(config_autotab);
extern DO_CONFIG(config_charset);
extern DO_CONFIG(config_colormode);
extern DO_CONFIG(config_screenreader);
extern DO_CONFIG(config_inheritance);
extern DO_CONFIG(config_randomseed);
extern DO_CONFIG(config_mousetracking);
extern DO_CONFIG(config_childlock);

#endif


#ifndef __MACRO_H__
#define __MACRO_H__

extern DO_COMMAND(do_macro);
extern DO_COMMAND(do_unmacro);
extern void macro_update(void);

#endif


#ifndef __STRHASH_H__
#define __STRHASH_H__


extern unsigned long long generate_hash_key(char *str);
extern char *str_hash(char *str, int lines);
extern char *str_unhash(char *str);
extern int str_hash_lines(char *str);
extern int str_hash_length(char *str);
extern int str_hash_grep(char *str, int write);
extern void reset_hash_table(void);
extern void str_hash_info(struct session *ses);

#endif


#ifndef __BUFFER_H__
#define __BUFFER_H__

extern void init_buffer(struct session *ses, int size);
extern void add_line_buffer(struct session *ses, char *line, int more_output);
extern DO_COMMAND(do_buffer);
extern DO_COMMAND(do_grep);
extern int show_buffer(struct session *ses);
extern DO_BUFFER(buffer_up);
extern DO_BUFFER(buffer_clear);
extern DO_BUFFER(buffer_down);
extern DO_BUFFER(buffer_get);
extern DO_BUFFER(buffer_home);
extern DO_BUFFER(buffer_end);
extern DO_BUFFER(buffer_lock);
extern DO_BUFFER(buffer_find);
extern DO_BUFFER(buffer_write);
extern DO_BUFFER(buffer_info);

#endif


#ifndef __DATA_H__
#define __DATA_H__

extern struct listroot *init_list(struct session *ses, int type, int size);
extern void kill_list(struct listroot *root);
extern void free_list(struct listroot *root);
extern struct listroot *copy_list(struct session *ses, struct listroot *sourcelist, int type);

extern struct listnode *insert_node_list(struct listroot *root, char *arg1, char *arg2, char *arg3, char *arg4);
extern struct listnode *update_node_list(struct listroot *root, char *arg1, char *arg2, char *arg3, char *arg4);
extern struct listnode *insert_index_list(struct listroot *root, struct listnode *node, int index);

extern  int show_node_with_wild(struct session *ses, char *cptr, struct listroot *root);
extern void show_node(struct listroot *root, struct listnode *node, int level);
extern void show_nest(struct listnode *node, char *result);
extern void show_list(struct listroot *root, int level);

extern struct listnode *search_node_list(struct listroot *root, char *text);

extern void delete_node_list(struct session *ses, int type, struct listnode *node);
extern void delete_node_with_wild(struct session *ses, int index, char *string);

extern void delete_index_list(struct listroot *root, int index);
extern  int search_index_list(struct listroot *root, char *text, char *priority);
extern  int locate_index_list(struct listroot *root, char *text, char *priority);


extern int bsearch_alpha_list(struct listroot *root, char *text, int seek);
extern int bsearch_priority_list(struct listroot *root, char *text, char *priority, int seek);
extern int nsearch_list(struct listroot *root, char *text);

extern DO_COMMAND(do_kill);
extern DO_COMMAND(do_message);
extern DO_COMMAND(do_ignore);
extern DO_COMMAND(do_debug);
#endif


#ifndef __DEBUG_H__
#define __DEBUG_H__

extern int push_call(char *f, ...);
extern void pop_call(void);
extern void dump_stack(void);
extern void dump_full_stack(void);

#endif


#ifndef __EVENT_H__
#define __EVENT_H__

extern DO_COMMAND(do_event);
extern DO_COMMAND(do_unevent);
extern int check_all_events(struct session *ses, int flags, int args, int vars, char *fmt, ...);
extern void mouse_handler(struct session *ses, int val1, int val2, int val3, char type);

#endif


#ifndef __ALIAS_H__
#define __ALIAS_H__

extern struct session *do_alias(struct session *ses, char *arg);
extern struct session *do_unalias(struct session *ses, char *arg);
int                    check_all_aliases(struct session *ses, char *input);

#endif


#ifndef __FILES_H__
#define __FILES_H__

extern DO_COMMAND(do_read);
extern DO_COMMAND(do_write);

extern void write_node(struct session *ses, int mode, struct listnode *node, FILE *file);

#endif 


#ifndef __FUNCTION_H__
#define __FUNCTION_H__

extern DO_COMMAND(do_function);
extern DO_COMMAND(do_unfunction);

#endif


#ifndef __GAG_H__
#define __GAG_H__

extern DO_COMMAND(do_gag);
extern DO_COMMAND(do_ungag);
extern void check_all_gags(struct session *ses, char *original, char *line);

#endif


#ifndef __HELP_H__
#define __HELP_H__

extern DO_COMMAND(do_help);

#endif


#ifndef __HIGHLIGHT_H__
#define __HIGHLIGHT_H__

extern DO_COMMAND(do_highlight);
extern int is_high_arg(char *s);
extern DO_COMMAND(do_unhighlight);
extern void check_all_highlights(struct session *ses, char *original, char *line);
extern int get_highlight_codes(struct session *ses, char *htype, char *result);

#endif


#ifndef __HISTORY_H__
#define __HISTORY_H__

extern DO_COMMAND(do_history);
extern void add_line_history(struct session *ses, char *line);
extern void insert_line_history(struct session *ses, char *line);
extern void search_line_history(struct session *ses, char *line);
extern int write_history(struct session *ses, char *filename);
extern int read_history(struct session *ses, char *filename);

DO_HISTORY(history_character);
DO_HISTORY(history_delete);
DO_HISTORY(history_insert);
DO_HISTORY(history_list);
DO_HISTORY(history_size);
DO_HISTORY(history_read);
DO_HISTORY(history_write);


#endif


#ifndef __LINE_H__
#define __LINE_H__
extern DO_COMMAND(do_line);
extern DO_LINE(line_gag);
extern DO_LINE(line_ignore);
extern DO_LINE(line_log);
extern DO_LINE(line_logverbatim);
extern DO_LINE(line_quiet);
extern DO_LINE(line_strip);
extern DO_LINE(line_substitute);
extern DO_LINE(line_verbatim);
extern DO_LINE(line_verbose);

#endif


#ifndef __LOG_H__
#define __LOG_H__

extern void logit(struct session *ses, char *txt, FILE *file, int newline);
extern DO_COMMAND(do_log);
extern void write_html_header(struct session *ses, FILE *fp);
extern void vt102_to_html(struct session *ses, char *txt, char *out);
#endif


#ifndef __MAIN_H__
#define __MAIN_H__

extern struct session *gts;
extern struct tintin_data *gtd;


extern void winch_handler(int signal);
extern void abort_handler(int signal);
extern void pipe_handler(int signal);
extern void suspend_handler(int signal);
extern void trap_handler(int signal);

extern int main(int argc, char **argv);
extern void init_tintin(int greeting);
extern void quitmsg(char *message);
extern void syserr_fatal(int signal, char *msg);
extern void syserr_printf(struct session *ses, char *fmt, ...);

#endif

#ifndef __MCCP_H__
#define __MCCP_H__

void *zlib_alloc(void *opaque, unsigned int items, unsigned int size);
void  zlib_free(void *opaque, void *address);

#endif

#ifndef __MEMORY_H__
#define __MEMORY_H__

extern char *restring(char *point, char *string);
extern char *restringf(char *point, char *fmt, ...);

extern int str_len(char *str);

extern char *str_alloc(int len);
extern void str_clone(char **clone, char *original);
extern char *str_mim(char *original);
extern char *str_dup(char *original);
extern char *str_dup_printf(char *fmt, ...);
extern char *str_cpy(char **ptr, char *str);
extern char *str_cpy_printf(char **ptr, char *fmt, ...);
extern char *str_ndup(char *original, int len);
extern char *str_ncpy(char **ptr, char *str, int len);
extern char *str_cat(char **ptr, char *str);
extern char *str_cat_chr(char **ptr, char chr);
extern char *str_cat_printf(char **ptr, char *fmt, ...);
extern void str_fix(char *str);
extern void str_free(char *ptr);

#endif


#ifndef __MISC_H__
#define __MISC_H__

extern DO_COMMAND(do_all);
extern DO_COMMAND(do_bell);
extern DO_COMMAND(do_commands);
extern DO_COMMAND(do_cr);
extern DO_COMMAND(do_echo);
extern DO_COMMAND(do_end);
extern DO_COMMAND(do_forall);
extern DO_COMMAND(do_info);
extern DO_COMMAND(do_nop);
extern DO_COMMAND(do_send);
extern DO_COMMAND(do_snoop);
extern DO_COMMAND(do_suspend);
extern DO_COMMAND(do_test);
extern DO_COMMAND(do_zap);

#endif

#ifndef __MSDP_H__
#define __MSDP_H__

extern void init_msdp_table(void);
extern int msdp_find(char *var);
extern void arachnos_devel(struct session *ses, char *fmt, ...);
extern void arachnos_mudlist(struct session *ses, char *fmt, ...);
extern void msdp_update_all(char *var, char *fmt, ...);
extern void msdp_update_var(struct session *ses, struct port_data *buddy, char *var, char *str);
extern void msdp_update_varf(struct session *ses, struct port_data *buddy, char *var, char *fmt, ...);
extern void msdp_update_var_instant(struct session *ses, struct port_data *buddy, char *var, char *fmt, ...);
extern void msdp_send_update(struct session *ses, struct port_data *buddy);
extern char *msdp_get_var(struct session *ses, struct port_data *buddy, char *var);
extern void process_msdp_varval(struct session *ses, struct port_data *buddy, char *var, char *val );
extern void msdp_command_list(struct session *ses, struct port_data *buddy, int index);
extern void msdp_command_report(struct session *ses, struct port_data *buddy, int index);
extern void msdp_command_reset(struct session *ses, struct port_data *buddy, int index);
extern void msdp_command_send(struct session *ses, struct port_data *buddy, int index);
extern void msdp_command_unreport(struct session *ses, struct port_data *buddy, int index);
extern void msdp_configure_arachnos(struct session *ses, struct port_data *buddy, int index);
extern void write_msdp_to_descriptor(struct session *ses, struct port_data *buddy, char *src, int length);
extern int msdp2json(unsigned char *src, int srclen, char *out);
extern int json2msdp(unsigned char *src, int srclen, char *out);
extern void arachnos_devel(struct session *ses, char *fmt, ...);
extern void arachnos_mudlist(struct session *ses, char *fmt, ...);

extern struct msdp_type msdp_table[];

#endif

#ifndef __NEST_H__
#define __NEST_H__

extern struct listroot *search_nest_root(struct listroot *root, char *arg);
extern struct listnode *search_base_node(struct listroot *root, char *variable);
extern struct listnode *search_nest_node(struct listroot *root, char *variable);
extern int search_nest_index(struct listroot *root, char *variable);
extern struct listroot *update_nest_root(struct listroot *root, char *arg);
extern void update_nest_node(struct listroot *root, char *arg);
extern int delete_nest_node(struct listroot *root, char *variable);
extern int get_nest_size_key(struct listroot *root, char *variable, char **result);
extern int get_nest_size_val(struct listroot *root, char *variable, char **result);
extern struct listnode *get_nest_node_key(struct listroot *root, char *variable, char **result, int def);
extern struct listnode *get_nest_node_val(struct listroot *root, char *variable, char **result, int def);
extern int get_nest_index(struct listroot *root, char *variable, char **result, int def);
extern void show_nest_node(struct listnode *node, char **result, int initialize);
extern void view_nest_node(struct listnode *node, char **str_result, int nest, int initialize);
extern struct listnode *set_nest_node(struct listroot *root, char *arg1, char *format, ...);
extern struct listnode *add_nest_node(struct listroot *root, char *arg1, char *format, ...);
extern void copy_nest_node(struct listroot *dst_root, struct listnode *dst, struct listnode *src);

#endif


#ifndef __NET_H__
#define __NET_H__

extern int connect_mud(struct session *ses, char *host, char *port);
extern void write_line_mud(struct session *ses, char *line, int size);
extern int read_buffer_mud(struct session *ses);
extern void readmud(struct session *ses);
extern void process_mud_output(struct session *ses, char *linebuf, int prompt);

#endif

#ifndef __PARSE_H__
#define __PARSE_H__

extern int is_abbrev(char *s1, char *s2);
extern void filename_string(char *input, char *output);
extern struct session *parse_input(struct session *ses, char *input);
extern struct session *parse_command(struct session *ses, char *input);

extern int is_speedwalk(struct session *ses, char *input);
extern void process_speedwalk(struct session *ses, char *input);
extern struct session *parse_tintin_command(struct session *ses, char *input);
extern char *get_arg_all(struct session *ses, char *string, char *result, int verbatim);
extern char *get_arg_in_braces(struct session *ses, char *string, char *result, int flag);
extern char *sub_arg_in_braces(struct session *ses, char *string, char *result, int flag, int sub);
extern char *get_arg_with_spaces(struct session *ses, char *string, char *result, int flag);
extern char *get_arg_stop_spaces(struct session *ses, char *string, char *result, int flag);
extern char *space_out(char *string);
extern char *get_arg_to_brackets(struct session *ses, char *string, char *result);
extern char *get_arg_at_brackets(struct session *ses, char *string, char *result);
extern char *get_arg_in_brackets(struct session *ses, char *string, char *result);
extern void write_mud(struct session *ses, char *command, int flags);
extern void do_one_line(char *line, struct session *ses);

#endif


#ifndef __PATH_H__
#define __PATH_H__

extern DO_COMMAND(do_path);
extern DO_PATH(path_create);
extern DO_PATH(path_describe);
extern DO_PATH(path_delete);
extern DO_PATH(path_destroy);
extern DO_PATH(path_goto);
extern DO_PATH(path_insert);
extern DO_PATH(path_load);
extern DO_PATH(path_map);
extern DO_PATH(path_move);
extern DO_PATH(path_run);
extern DO_PATH(path_save);
extern DO_PATH(path_start);
extern DO_PATH(path_stop);
extern DO_PATH(path_swap);
extern DO_PATH(path_undo);
extern DO_PATH(path_unzip);
extern DO_PATH(path_walk);
extern DO_PATH(path_zip);

extern DO_COMMAND(do_pathdir);
extern DO_COMMAND(do_unpathdir);
extern void check_append_path(struct session *ses, char *forward, char *backward, int follow);

// old

extern DO_PATH(path_new);
extern DO_PATH(path_end);

#endif


#ifndef __PORT_H__
#define __PORT_H__


extern DO_COMMAND(do_port);
extern DO_PORT(port_call);
extern DO_PORT(port_color);
extern DO_PORT(port_flag);
extern DO_PORT(port_group);
extern DO_PORT(port_ignore);
extern DO_PORT(port_initialize);
extern DO_PORT(port_info);
extern DO_PORT(port_message);
extern DO_PORT(port_name);
extern DO_PORT(port_prefix);
extern DO_PORT(port_rank);
extern DO_PORT(port_send);
extern DO_PORT(port_uninitialize);
extern DO_PORT(port_who);
extern DO_PORT(port_zap);

extern  int port_new(struct session *ses, int s);
extern void close_port(struct session *ses, struct port_data *buddy, int unlink);
extern void process_port_connections(struct session *ses, fd_set *read_set, fd_set *write_set, fd_set *exc_set);
extern void port_forward_session(struct session *ses, char *linelog);
extern void port_socket_printf(struct session *ses, struct port_data *buddy, char *format, ...);
extern void port_telnet_printf(struct session *ses, struct port_data *buddy, size_t length, char *format, ...);
extern void port_log_printf(struct session *ses, struct port_data *buddy, char *format, ...);
extern void port_printf(struct session *ses, char *format, ...);
extern  int process_port_input(struct session *ses, struct port_data *buddy);
extern void get_port_commands(struct session *ses, struct port_data *buddy, char *buf, int len);
extern void port_name_change(struct session *ses, struct port_data *buddy, char *txt);
extern void port_receive_message(struct session *ses, struct port_data *buddy, char *txt);

extern void port_puts(struct session *ses, char *arg);
extern struct port_data *find_port_buddy(struct session *ses, char *arg);
extern struct port_data *find_port_group(struct session *ses, char *arg);

#endif


#ifndef __TELOPT_H__
#define __TELOPT_H__

extern  int client_translate_telopts(struct session *ses, unsigned char *src, int cplen);

extern  int client_write_compressed(struct session *ses, char *txt, int length);
extern  int client_send_sb_naws(struct session *ses, int cplen, unsigned char *cpsrc);

extern void announce_support(struct session *ses, struct port_data *buddy);
extern  int server_translate_telopts(struct session *ses, struct port_data *buddy, unsigned char *src, int srclen, unsigned char *out, int outlen);

extern void write_mccp2(struct session *ses, struct port_data *buddy, char *txt, int length);

extern void end_mccp2(struct session *ses, struct port_data *buddy);
extern void end_mccp3(struct session *ses, struct port_data *buddy);

extern void init_msdp_table(void);

#endif


#ifndef __TAB_H__
#define __TAB_H__

extern DO_COMMAND(do_tab);
extern DO_COMMAND(do_untab);

#endif


#ifndef __SCREEN_H__
#define __SCREEN_H__

extern DO_COMMAND(do_screen);

extern DO_SCREEN(screen_blur);
extern DO_SCREEN(screen_clear);
extern DO_SCREEN(screen_focus);
extern DO_SCREEN(screen_fullscreen);
extern DO_SCREEN(screen_info);
extern DO_SCREEN(screen_get);
extern DO_SCREEN(screen_cursor);
extern DO_SCREEN(screen_scrollbar);
extern DO_SCREEN(screen_load);
extern DO_SCREEN(screen_save);
extern DO_SCREEN(screen_set);
extern DO_SCREEN(screen_maximize);
extern DO_SCREEN(screen_minimize);
extern DO_SCREEN(screen_refresh);
extern DO_SCREEN(screen_resize);
extern DO_SCREEN(screen_move);
extern DO_SCREEN(screen_rescale);
extern DO_SCREEN(screen_raise);

extern int get_row_index(struct session *ses, char *arg);
extern int get_col_index(struct session *ses, char *arg);

extern void csip_handler(int var1, int var2, int var3);
extern void csit_handler(int var1, int var2, int var3);
extern void osc_handler(char ind, char *arg);

extern void print_screen();
extern void init_screen(int rows, int cols, int pix_rows, int pix_cols);
extern void destroy_screen();
extern void add_line_screen(char *str);
extern void set_line_screen(char *str, int row, int col);
extern void erase_scroll_region();
extern void get_line_screen(char *str, int row);
extern void get_word_screen(char *str, int row, int col);

#endif


#ifndef __SESSION_H__
#define __SESSION_H__

extern DO_COMMAND(do_session);
extern struct session *session_command(char *arg, struct session *ses);
extern void show_session(struct session *ses, struct session *ptr);
extern struct session *find_session(char *name);
extern struct session *newactive_session(void);
extern struct session *activate_session(struct session *ses);
extern struct session *new_session(struct session *ses, char *name, char *address, int desc, int ssl);
extern struct session *connect_session(struct session *ses);
extern void cleanup_session(struct session *ses);
extern void dispose_session(struct session *ses);

#endif

#ifndef __SHOW_H__
#define __SHOW_H__

extern DO_COMMAND(do_showme);

extern void show_message(struct session *ses, int index, char *format, ...);
extern void show_error(struct session *ses, int index, char *format, ...);
extern void show_debug(struct session *ses, int index, char *format, ...);
extern void show_info(struct session *ses, int index, char *format, ...);
extern void show_lines(struct session *ses, int flags, char *format, ...);
extern void tintin_header(struct session *ses, char *format, ...);
extern void socket_printf(struct session *ses, size_t length, char *format, ...);
extern void telnet_printf(struct session *ses, size_t length, char *format, ...);

extern void tintin_printf2(struct session *ses, char *format, ...);
extern void tintin_printf(struct session *ses, char *format, ...);

extern void tintin_puts3(struct session *ses, char *string);
extern void tintin_puts2(struct session *ses, char *string);
extern void tintin_puts(struct session *ses, char *string);

#endif

#ifndef __SSL_H__
#define __SSL_H__

extern DO_COMMAND(do_ssl);
extern gnutls_session_t ssl_negotiate(struct session *ses);

#endif


#ifndef __SUBSTITUTE_H__
#define __SUBSTITUTE_H__

extern DO_COMMAND(do_substitute);
extern DO_COMMAND(do_unsubstitute);
extern void check_all_substitutions(struct session *ses, char *original, char *line);

#endif


#ifndef __SYSTEM_H__
#define __SYSTEM_H__

extern DO_COMMAND(do_run);
extern DO_COMMAND(do_scan);
extern DO_COMMAND(do_script);
extern DO_COMMAND(do_system);
extern DO_COMMAND(do_textin);

#endif


#ifndef __TABLES_H__
#define __TABLES_H__

extern struct array_type array_table[];
extern struct buffer_type buffer_table[];
extern struct chat_type chat_table[];
extern struct class_type class_table[];
extern struct color_type color_table[];
extern struct color_type map_color_table[];
extern struct command_type command_table[];
extern struct config_type config_table[];
extern struct cursor_type cursor_table[];
extern struct event_type event_table[];
extern struct history_type history_table[];
extern struct line_type line_table[];
extern struct list_type list_table[LIST_MAX];
extern struct map_type map_table[];
extern struct path_type path_table[];
extern struct port_type port_table[];
extern struct rank_type rank_table[];
extern struct substitution_type substitution_table[];
extern struct telopt_type telopt_table[];
extern char *telcmds[];
extern struct timer_type timer_table[];
extern struct screen_type screen_table[];
extern struct map_legend_type map_legend_table[];
extern struct map_group_type map_group_table[];

#endif


#ifndef __TEXT_H__
#define __TEXT_H__

extern void print_line(struct session *ses, char **str, int isaprompt);
int word_wrap(struct session *ses, char *textin, char *textout, int display);
int word_wrap_split(struct session *ses, char *textin, char *textout, int display, int skip, int keep);

#endif


#ifndef __TICKS_H__
#define __TICKS_H__

extern DO_COMMAND(do_tick);
extern DO_COMMAND(do_untick);
extern DO_COMMAND(do_delay);
extern DO_COMMAND(do_undelay);

#endif


#ifndef __TOKENIZE_H__
#define __TOKENIZE_H__

extern void init_local(struct session *ses);
extern struct listroot *local_list(struct session *ses);
extern struct session *script_driver(struct session *ses, int list, char *str);
extern char *script_writer(struct session *ses, char *str);
extern char *script_viewer(struct session *ses, char *str);
#endif


#ifndef __UPDATE_H__
#define __UPDATE_H__

extern void mainloop(void);
extern void poll_input(void);
extern void poll_sessions(void);
extern void poll_chat(void);
extern void poll_port(void);
extern void tick_update(void);
extern void delay_update(void);
extern void packet_update(void);
extern void chat_update(void);
extern void terminal_update(void);
extern void memory_update(void);
extern void time_update(void);

extern void show_cpu(struct session *ses);
extern long long display_timer(struct session *ses, int timer);
extern void open_timer(int timer);
extern void close_timer(int timer);

#endif


#ifndef __UTILS_H__
#define __UTILS_H__


extern int is_color_code(char *str);
extern int is_number(char *str);
extern unsigned long long hex_number_64bit(char *str);
extern int hex_number_8bit(char *str);
extern int oct_number(char *str);
extern int unicode_16_bit(char *str, char *out);
extern int unicode_21_bit(char *str, char *out);
extern unsigned long long utime(void);
extern unsigned long long generate_rand(struct session *ses);
extern void seed_rand(struct session *ses, unsigned long long seed);
extern char *capitalize(char *str);
extern char *ntos(long long number);
extern char *indent(int len);
extern int cat_sprintf(char *dest, char *fmt, ...);
extern void ins_sprintf(char *dest, char *fmt, ...);
extern int str_suffix(char *str1, char *str2);



#endif


#ifndef __UTF8_H__
#define __UTF8_H__

extern int is_utf8_head(char *str);
extern int is_utf8_tail(char *str);
extern int get_utf8_size(char *str);
extern int get_utf8_width(char *str, int *width);
extern int get_utf8_index(char *str, int *index);
extern int unicode_to_utf8(int index, char *out);
extern int utf8_strlen(char *str);
extern int is_big5(char *str);
extern int big5_to_utf8(char *input, char *output);
extern int utf8_to_big5(char *input, char *output);
extern int koi8_to_utf8(char *input, char *output);
extern int utf8_to_koi8(char *input, char *output);
extern int fansi_to_utf8(char *input, char *output);

#endif

#ifndef __VARIABLE_H__
#define __VARIABLE_H__

extern DO_COMMAND(do_variable);
extern DO_COMMAND(do_unvariable);

extern DO_COMMAND(do_local);

extern void lowerstring(char *str);
extern void numbertocharacter(struct session *ses, char *str);
extern void charactertonumber(struct session *ses, char *str);

extern int delete_variable(struct session *ses, char *variable);
extern struct listnode *search_variable(struct session *ses, char *variable);

extern struct listnode *get_variable(struct session *ses, char *variable, char *result);
extern struct listnode *set_variable(struct session *ses, char *variable, char *format, ...);

extern int get_variable_index(struct session *ses, char *variable, char *result);

extern void format_string(struct session *ses, char *format, char *arg, char *out);

extern DO_COMMAND(do_format);
extern DO_COMMAND(do_tolower);
extern DO_COMMAND(do_toupper);
extern DO_COMMAND(do_postpad);
extern DO_COMMAND(do_prepad);
extern DO_COMMAND(do_replace);

#endif


#ifndef __VT102_H__
#define __VT102_H__

extern void save_pos(struct session *ses);
extern void restore_pos(struct session *ses);
extern void goto_rowcol(struct session *ses, int row, int col);

extern void erase_toeol(void);
extern void erase_scroll_region(struct session *ses);
extern void reset(void);
extern void scroll_region(struct session *ses, int top, int bottom);
extern void reset_scroll_region(struct session *ses);
extern int skip_vt102_codes(char *str);
extern int skip_vt102_codes_non_graph(char *str);
extern void strip_vt102_codes(char *str, char *buf);
extern void strip_vt102_codes_non_graph(char *str, char *buf);
extern void strip_non_vt102_codes(char *str, char *buf);
extern void get_color_codes(char *old, char *str, char *buf);
extern int strip_vt102_strlen(struct session *ses, char *str);
extern int strip_color_strlen(struct session *ses, char *str);
extern char *strip_vt102_strstr(char *str, char *buf, int *len);
extern int interpret_vt102_codes(struct session *ses, char *str, int real);

#endif
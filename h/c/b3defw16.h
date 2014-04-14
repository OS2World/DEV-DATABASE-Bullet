//
// b3defw16.h
// 1-Jan-1999
// Bullet 3 include, defines for Windows 16-bit
// Copyright (C)1999 Cornel Huth        http://40th.com/
//

#include "windows.h"
//#include "windowsx.h"

#ifndef CDECL
 #define CDECL __cdecl
#endif

#define LONG long
#define SHORT short
#define VOID void

typedef int INT;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;

typedef UCHAR   TBLT_FNCHAR;
typedef ULONG   TBLT_FO;
typedef UINT    TBLT_RETC;
typedef ULONG   TBLT_RETVAL;

typedef INT     TBLT_CMPC;              // compare code (result used only to check if LT 0, = 0 or GT> 0)
typedef VOID*   TBLT_RETPTR;            // mem allocate return (far ptr)
typedef ULONG   TBLT_HANDLE;            // handle type

#define TBLT_CALLBACK   CALLBACK        // calling convention for all xCallback type functions
#define TBLT_EXP        WINAPI          // calling convention for all ll_* functions, and x*.c routines
#define TBLT_ENTRY      WINAPI          // calling convention for the rest (WINAPI is PASCAL)

#define ALLOC_WIN16_FIXPTR  0
#define ALLOC_DEFAULT   ALLOC_WIN16_FIXPTR

// end b3defw16.h

//
// b3defw32.h
// 1-Jan-1999
// Bullet 3 include, defines for Windows 32-bit
// Copyright (C)1999 Cornel Huth        http://40th.com/
//

//#define WIN32_LEAN_AND_MEAN
#include "windows.h"

#define BLT_FLATMM

typedef UCHAR   TBLT_FNCHAR;

#ifndef BLT_FILE64BIT
 typedef ULONG  TBLT_FO;
#else
 typedef INT64  TBLT_FO;                // if INT64 is available (and need 64-bit file sizes)
#endif

typedef UINT    TBLT_RETC;
typedef ULONG   TBLT_RETVAL;

typedef INT     TBLT_CMPC;              // compare code (result used only to check if LT 0, = 0 or GT 0)
typedef VOID*   TBLT_RETPTR;            // mem allocate return
typedef VOID*   TBLT_HANDLE;            // handle type

#define TBLT_CALLBACK   CALLBACK        // calling convention for all xCallback type functions
#define TBLT_EXP        APIENTRY        // calling convention for all ll_* functions, and x*.c routines
#define TBLT_ENTRY      APIENTRY        // calling convention for the rest (APIENTRY is stdcall)

#define ALLOC_WIN32_FIXPTR  0
#define ALLOC_DEFAULT   ALLOC_WIN32_FIXPTR

// end b3defw32.h

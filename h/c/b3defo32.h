//
// b3defo32.h
// 1-Jan-1999
// Bullet 3 include, defines for OS/2 32-bit
// Copyright (C)1999 Cornel Huth        http://40th.com/
//

#define INCL_BASE
#include "os2.h"                        // 32-bit OS/2 header from toolkit 4

#define BLT_FLATMM

#ifndef CDECL
 #define CDECL __cdecl
#endif

#define LONG long
#define SHORT short
#define VOID void

typedef UCHAR   TBLT_FNCHAR;
typedef ULONG   TBLT_FO;
typedef UINT    TBLT_RETC;
typedef ULONG   TBLT_RETVAL;

typedef INT     TBLT_CMPC;              // compare code (result used only to check if LT 0, = 0 or GT 0)
typedef VOID*   TBLT_RETPTR;            // mem allocate return
typedef ULONG   TBLT_HANDLE;            // handle type

#define TBLT_CALLBACK   APIENTRY        // calling convention for all xCallback type functions
#define TBLT_EXP        APIENTRY        // calling convention for all ll_* functions, and x*.c routines
#define TBLT_ENTRY      APIENTRY        // calling convention for the rest (APIENTRY is syscall)

#define ALLOC_OS2_COMMITTED  0
#define ALLOC_DEFAULT   ALLOC_OS2_COMMITTED

// end b3defo32.h

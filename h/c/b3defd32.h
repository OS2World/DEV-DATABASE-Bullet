//
// b3defd32.h
// 1-Jan-1999
// Bullet 3 include, defines for DOS 32-bit
// Copyright (C)1999 Cornel Huth        http://40th.com/
//

#define BLT_FLATMM

#ifndef CDECL
 #define CDECL __cdecl
#endif

typedef unsigned long ULONG;
typedef unsigned short USHORT;
typedef signed char CHAR;
typedef unsigned char UCHAR;

typedef signed char SCHAR;
typedef signed char SBYTE;
typedef unsigned char UBYTE;

#define LONG long
#define SHORT short
#define VOID void

typedef unsigned int UINT;
typedef int INT;

typedef UCHAR   TBLT_FNCHAR;
typedef ULONG   TBLT_FO;
typedef ULONG   TBLT_RETC;
typedef ULONG   TBLT_RETVAL;

typedef INT     TBLT_CMPC;              // compare code (result used only to check if LT 0, = 0 or GT 0)
typedef VOID*   TBLT_RETPTR;            // mem allocate return
typedef ULONG   TBLT_HANDLE;            // handle type

#define TBLT_CALLBACK   CDECL           // calling convention for all xCallback type functions
#define TBLT_EXP        CDECL           // calling convention for all ll_* functions, and x*.c routines
#define TBLT_ENTRY      CDECL           // calling convention for the rest

#define ALLOC_C_MALLOC  0
#define ALLOC_DEFAULT   ALLOC_C_MALLOC

// end b3defd32.h

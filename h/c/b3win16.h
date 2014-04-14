//
// b3win16.h
// 1-Jan-1999
// Bullet 3 include for Windows 16-bit
// Copyright (C)1999 Cornel Huth        http://40th.com/
//
// For 16-bit, all pointers are far (sel16:offset16)
// This requires large|huge memory model builds, or for
// all pointers to be explicitly typed to "far" in all
// header and source files.
//

#ifndef BLT3WIN16_H
#define BLT3WIN16_H

#ifdef __cplusplus
extern "C" {
#endif

#include "b3defw16.h"
#include "b3define.h"
#include "b3types.h"
#ifndef BLT_DYNALOAD
 #include "b3proto.h"
 TBLT_RETC TBLT_EXP ll_testCallBack(TBLT_KH *khPtr);
#else
 #include "b3protod.h"
 typedef TBLT_RETC (TBLT_ENTRY *LL_TESTCALLBACK)(TBLT_KH *khPtr);
#endif

#ifdef __cplusplus
};
#endif
#endif

// end b3win16.h

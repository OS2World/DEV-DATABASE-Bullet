//
// b3dos16.h
// 1-Jan-1999
// Bullet 3 include for DOS 16-bit
// Copyright (C)1999 Cornel Huth        http://40th.com/
//
// For 16-bit, all pointers are far (seg16:offset16)
// This requires large|huge memory model builds, or for
// all pointers to be explicitly typed to "far" in all
// header and source files.
//

#ifndef BLT3DOS16_H
#define BLT3DOS16_H

#ifdef __cplusplus
extern "C" {
#endif

#include "b3defd16.h"
#include "b3define.h"
#include "b3types.h"
#include "b3proto.h"

TBLT_RETC TBLT_EXP ll_testCallBack(TBLT_KH *khPtr);

#ifdef __cplusplus
};
#endif
#endif

// end b3dos16.h

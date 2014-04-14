//
// 1-Jan-1999
// Demo include
//
// demo requires 32-bit pointers (large|huge|flat memory model)
//
// -*- See ADDRIX.C, and other examples, for more notes -*-
//
// most of these demo examples expect the key to be the empID field

#ifndef DEMO_H
#define DEMO_H

// define only one of the following

//#define TEST_MODULE_ADDRECS
#define TEST_MODULE_ADDRIX
//#define TEST_MODULE_CREATE
//#define TEST_MODULE_CRYPTO
//#define TEST_MODULE_GETEQ
//#define TEST_MODULE_GETFWD
//#define TEST_MODULE_GETREV
//#define TEST_MODULE_INSRECS
//#define TEST_MODULE_LIUEX
//#define TEST_MODULE_LOCK
//#define TEST_MODULE_LOCK_RT
//#define TEST_MODULE_MEMOADD
//#define TEST_MODULE_MEMODEL
//#define TEST_MODULE_MEMOUPD
//#define TEST_MODULE_OPEN
//#define TEST_MODULE_UPDRECS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

TBLT_RETC DemoCreateFiles(void);
TBLT_RETC DemoOpenFiles(TBLT_DH **dhPtr, TBLT_KH **khPtr, ULONG asModeData, ULONG asModeIx4);
TBLT_RETC DemoAddRecords(TBLT_DH *dhPtr, ULONG recsToAdd, UINT addMode);
TBLT_RETC DemoReindex(TBLT_KH *khPtr);
TBLT_RETC DemoGetForward(TBLT_KH *khPtr, ULONG expected);
TBLT_RETC DemoGetReverse(TBLT_KH *khPtr, ULONG expected);
TBLT_RETC DemoInsertRecords(TBLT_KH *khPtr, ULONG recsToAdd, UINT addMode);
TBLT_RETC DemoUpdateRecords(TBLT_KH *khPtr, ULONG recsToUpd);
TBLT_RETC DemoGetEqual(TBLT_KH *khPtr, UINT pFlag);
TBLT_RETC DemoAddMemos(TBLT_DH *dhPtr, ULONG memosToAdd, ULONG memoSize);
TBLT_RETC DemoDeleteMemos(TBLT_DH *dhPtr, ULONG memosToDelete);
TBLT_RETC DemoUpdateMemos(TBLT_DH *dhPtr, ULONG memosToUpdate, UINT updateMode);


LONG GetMeOne(LONG seed);


#define DBF_FILENAME    "$DEMO_01.DBF"
#define DBF_FILENAME2   "$DEMO_02.DBF"
#define DBT_FILENAME    "$DEMO_01.DBT"   // used only for BltDeleteFile()
#define DBT_FILENAME2   "$DEMO_02.DBT"
#define DBF_MAM         ALLOC_DEFAULT
#define DBF_ASMODE      (OPENFLAGS_NOCRIT | OPENFLAGS_DENY_NONE | OPENFLAGS_ACCESS_RW)
#define DBF_OPENMODE    0  // mode: FLAGS_READ_THROUGH_LOCK_BIT; FLAGS_READONLY_BIT; FLAGS_SKIP_DELETED_BIT


typedef struct _DATARECORD {
 UCHAR delTag;
 UCHAR empID[9];
 UCHAR lName[18];
 UCHAR fName[18];
 UCHAR salary[8];
 UCHAR personalMemoNo[10];
} DATARECORD; // 64 bytes

#define DBF_FILEID      0x8B
#define DBF_FIELDS      5

#define DBF_FIELD_01    "EMPID",'C',0,9
#define DBF_FIELD_02    "LAST_NAME",'C',0,18
#define DBF_FIELD_03    "FIRST_NAME",'C',0,18
#define DBF_FIELD_04    "SALARY",'N',0,8,2
#define DBF_FIELD_05    "PERSONAL",'M',0,10

#define DBT_MEMOSIZE    (512-8)

#define IX4_FILENAME    "$DEMO_01.IX4"
#define IX4_FILENAME2   "$DEMO_02.IX4"
#define IX4_FILENAME3   "$DEMO_03.IX4"
#define IX4_FILENAME4   "$DEMO_04.IX4"
#define IX4_MAM         ALLOC_DEFAULT
#define IX4_ASMODE      (OPENFLAGS_NOCRIT | OPENFLAGS_DENY_NONE | OPENFLAGS_ACCESS_RW)
#define IX4_OPENMODE    0 // mode: FLAGS_READ_THROUGH_LOCK_BIT; FLAGS_READONLY_BIT; FLAGS_ATOMIC_BIT

#define IX4_NODESIZE    512
//#define IX4_SORTCMPCODE SORT_ASCII
#define IX4_SORTCMPCODE (SORT_ASCII | SORT_DUPS_ALLOWED)
#define IX4_CP          0
#define IX4_CC          0
#define IX4_SORT_TABLE  0
#define IX4_KEYEXP      "SUBSTR(EMPID,1,8)"
#define IX4_PARSER_FNPTR 0

#define ADD_RND 0
#define ADD_SEQ 1


#endif

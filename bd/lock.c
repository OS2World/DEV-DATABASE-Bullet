
//
// 26-Aug-1998
// demo, lock records/keys
//

#include "bullet_3.h"
#include "blt3demo.h"

#define LOCK_ALL_DBF 0

#ifdef TEST_MODULE_LOCK
 #define RECS_TO_INSERT 100
#endif


// ------------------------------------------------
// Lock using the various lock mechanisms available

// BltLockDataHeader(TBLT_DH *dhPtr, ULONG mode, ULONG timeout);
// BltUnlockDataHeader(TBLT_DH *dhPtr);
// BltLockDataRecord(TBLT_DH *dhPtr, ULONG mode, ULONG recNoStart, ULONG recCount, ULONG timeout);
// BltUnlockDataRecord(TBLT_DH *dhPtr, ULONG recNoStart, ULONG recCount);
//
// BltLockIx4File(TBLT_KH *khPtr, ULONG mode, ULONG timeout);
// BltUnlockIx4File(TBLT_KH *khPtr);
//
//#define LOCK_MODE_EXCLUSIVE 0
//#define LOCK_MODE_SHARED    1
//#define LOCK_MODE_ATOMIC    2
//#define LOCK_MODE_SH2EX (LOCK_MODE_EXCLUSIVE | LOCK_MODE_ATOMIC) // 2
//#define LOCK_MODE_EX2SH (LOCK_MODE_SHARED | LOCK_MODE_ATOMIC)    // 3
//
//FLAGS_READ_THROUGH_LOCK_BIT


TBLT_RETC DemoLock(TBLT_KH *khPtr, ULONG recsInserted) {

 TBLT_RETC rc = 0;
 TBLT_FO offset;
 ULONG bytes, exitNext;
 UCHAR readBuffer[256];
 TBLT_HANDLE h2_DBF=0, h2_IX4=0;

 // These second opens are done using OS opens (i.e., not Bullet opened and not Bullet controlled)
 // to demonstrate that locking is at the OS level -- the opens succeed since the files
 // were previously opened (here, too) with OPENFLAGS_DENY_NONE, but access into the files
 // from these handles will fail on writing, and reading (since FLAGS_READ_THROUGH_LOCK_BIT
 // is not set)
 //
 // *** IMPORTANT:
 //
 // To prevent writing you must attempt to lock the file before writing and continue
 // to write only if the lock succeeded.  The read-through locks are intended only to
 // allow reading to the file even if a lock in place, but r-t locks do NOT safeguard
 // against writes unless the write is successful in locking the region it will be
 // writing to.  In any case, once written to, the file is in an inconsistent state
 // since the file may have changed structure (especially an index file).  Therefore,
 // in nearly all cases it is recommended that read-through lock NOT be used, and to
 // always lock a file before use, and to unlock it immediately after use.  Doing this
 // ensures that you have a consistent file structure, both before and after access.
 //
 // In OS/2 and NT you can have safe read-through locks by using LOCK_MODE_SHARED rather
 // than LOCK_MODE_EXCLUSIVE.  This allows read access but prevents write access.

 // these opens are to get handles to the already-opened database files

 rc = BltOpenFile(DBF_FILENAME,DBF_ASMODE,&h2_DBF,0);
 printf("Secondary open of DBF file (raw), rc=%u\n",rc);
 if (rc == 0) {
    rc = BltOpenFile(IX4_FILENAME,IX4_ASMODE,&h2_IX4,0);
    printf("Secondary open of IX4 file (raw), rc=%u\n",rc);
 }
 if (rc) goto ExitNow;


 // This demo has already written to the database files without previous locking, which is not
 // what would normally be done in production code.  Since this is the case, it is a sure bet
 // that the database files need to be flushed (an operation performed by the Bullet unlock routines)
 // if a Bullet file were 'dirty', the header reload done in the lock call would fail since you
 // cannot reload a file header if the file header in memory is still dirty, so...

 // flush the changes made to the database files in the demo setup

 rc = BltDataFlushHeader(khPtr->xbLinkPtr);
 if (rc) {
    printf("BltDataFlushHeader failed, rc=%u\n");
    goto ExitNow;
 }
 rc = BltIx4FlushHeader(khPtr);
 if (rc) {
    printf("BltIx4FlushHeader failed, rc=%u\n");
    goto ExitNow;
 }


 // full lock DBF and IX4 files already opened by the caller
 //
 // NOTE: a full lock ALWAYS automatically reloads the file header info into memory,
 //       ensuring that the in-memory state (TBLT_KH/TBLT_DH) is consistent with the disk state

 rc = BltLockDataRecord(khPtr->xbLinkPtr, LOCK_MODE_EXCLUSIVE, LOCK_ALL_DBF, 0, 1000);
 printf("Locking entire DBF file (LOCK_MODE_EXCLUSIVE), rc=%u\n",rc);
 if (rc == 0) {
    rc = BltLockIx4File(khPtr, LOCK_MODE_EXCLUSIVE, 1000);
    printf("Locking IX4 file (LOCK_MODE_EXCLUSIVE), rc=%u\n",rc);
 }
 if (rc) goto ExitNow;

 // This attempts to read first/last bytes of the DBF and IX4 files, using the handles from the
 // second opens.  The execution is performed twice; first with the locks active and the with
 // the locks released.

 // note: full locks lock all current and all possible bytes of a file, so full locking extends
 // not just to the EOF, but to the max possible file size (varies by OS, and if READ_THROUGH locks

 printf("\nThis first run is with locks active (reads will fail)\n");
 exitNext = 0;
 while(1) {

    // attempt to read first and last byte of DBF

    offset = 0;
    bytes = 1;
    rc = BltReadAtFile(h2_DBF,&bytes,readBuffer,&offset,0);  // attempt to read the first byte
    printf("Reading offset %lu of DBF from second open, rc=%u",offset,rc);
    if (rc) printf(" (failed)");
    printf("\n");

    BltLengthFile(h2_DBF,&offset,0);
    if (offset) offset--;    // 0-base (size is 1-based)
    bytes = 1;
    rc = BltReadAtFile(h2_DBF,&bytes,readBuffer,&offset,0);  // attempt to read the last byte
    printf("Reading offset %lu of DBF from second open, rc=%u",offset,rc);
    if (rc) printf(" (failed)");
    printf("\n");

    // attempt to read first and last byte of IX4

    offset = 0;
    bytes = 1;
    rc = BltReadAtFile(h2_IX4,&bytes,readBuffer,&offset,0);  // attempt to read the first byte
    printf("Reading offset %lu of IX4 from second open, rc=%u",offset,rc);
    if (rc) printf(" (failed)");
    printf("\n");

    BltLengthFile(h2_IX4,&offset,0);
    if (offset) offset--;    // 0-base (size is 1-based)
    bytes = 1;
    rc = BltReadAtFile(h2_IX4,&bytes,readBuffer,&offset,0);  // attempt to read the last byte
    printf("Reading offset %lu of IX4 from second open, rc=%u",offset,rc);
    if (rc) printf(" (failed)");
    printf("\n\n");

    if (exitNext) break;

    rc = BltUnlockDataRecord(khPtr->xbLinkPtr, LOCK_ALL_DBF, 0);
    printf("DBF unlock, rc=%u\n",rc);
    rc = BltUnlockIx4File(khPtr);
    printf("IX4 unlock, rc=%u\n",rc);
    if (rc) break;

    printf("\nThis second run is with locks off (reads will succeed)\n");
    exitNext = 1;

 }

ExitNow:

 // since may have gotten here before unlocking, check if full locked and unlock if so
 // (will also be released when files closed (in caller's close, not the closes below)

 // NOTE: an unlock of a full lock automatically flushes the file header info from memory
 //       to disk (if needed), ensuring that the disk state is updated to any changes made.

 if (khPtr->xbLinkPtr->flags & FLAGS_FULL_LOCK_BIT) {
    rc = BltUnlockDataRecord(khPtr->xbLinkPtr, LOCK_ALL_DBF, 0);
    if (rc) printf("- DBF unlock, rc=%u\n",rc);
 }

 if (khPtr->flags & FLAGS_FULL_LOCK_BIT) {
    rc = BltUnlockIx4File(khPtr);
    if (rc) printf("- IX4 unlock, rc=%u\n",rc);
 }

 // close the second opens

 if (h2_DBF) rc = BltCloseFile(&h2_DBF,0);
 if (rc) printf("h2_DBF close failed, rc=%u\n",rc);
 if (h2_IX4) rc = BltCloseFile(&h2_IX4,0);
 if (rc) printf("h2_IX4 close failed, rc=%u\n",rc);

 return rc;
 recsInserted;
}


#ifdef TEST_MODULE_LOCK

extern ULONG dbfOpenMode;
extern ULONG ix4OpenMode;

int main(void) {

 TBLT_RETC rc;
 TBLT_DH *dhPtr = 0;
 TBLT_KH *khPtr = 0;
 ULONG orgOpenMode[2];

 setbuf(stdout,NULL);

 printf("lock.c count=%u\n",RECS_TO_INSERT);
 rc = DemoCreateFiles();
 printf("Create Files, rc=%u\n",rc);
 if (rc == 0) {

    orgOpenMode[0] = dbfOpenMode;  // save current (default) open modes
    orgOpenMode[1] = ix4OpenMode;  // (these globals are in open.c)

    // ensure read through is off

    dbfOpenMode = dbfOpenMode & ~FLAGS_READ_THROUGH_LOCK_BIT;
    ix4OpenMode = ix4OpenMode & ~FLAGS_READ_THROUGH_LOCK_BIT;

    rc = DemoOpenFiles(&dhPtr,&khPtr,DBF_ASMODE,IX4_ASMODE);
    printf(" Open Files, rc=%u\n",rc);

    dbfOpenMode = orgOpenMode[0];
    ix4OpenMode = orgOpenMode[1];

    if (rc == 0) {
       rc = DemoInsertRecords(khPtr, RECS_TO_INSERT, ADD_RND);
       printf("Insert Records, rc=%u\n",rc);

       if (rc == 0) {
          rc = DemoLock(khPtr, RECS_TO_INSERT);
          printf("Lock, rc=%u\n",rc);
       }

       rc = BltIx4CloseFile(khPtr);
       printf("Index Close, rc=%u\n",rc);
       rc = BltDataCloseFile(dhPtr);
       printf(" Data Close, rc=%u\n",rc);

       free(dhPtr);
       free(khPtr);
    }
 }

 printf("Done.\n");
 return (int)rc;
}
#endif


//
// 16-Aug-1998
// demo, update records/keys
//

#include "bullet_3.h"
#include "blt3demo.h"

#ifdef TEST_MODULE_UPDRECS
 #define RECS_TO_UPDATE 5000
#endif


// ------------------------------------------------------
// Update records to the data file and keys into the index file using BltIx4Update()
// for update, the first byte of each empID key is modified so that a new, unique key is generated
//
// For very large update counts, instead of using BltIx4Update(), you should look into using
// using BltDataUpdateRecord() and follow that up with BltFuncIx4Reindex()
//

TBLT_RETC DemoUpdateRecords(TBLT_KH *khPtr, ULONG recsToUpd) {

 TBLT_RETC rc = 0;
 ULONG cnt = 0;
 ULONG i, recNo, action, thisKey, orgAF;

 DATARECORD dataRec;
 UCHAR keyBuffer[MAX_KEY_LENGTH];
 UCHAR newBuffer[MAX_KEY_LENGTH];
 UCHAR t;

 keyBuffer[khPtr->keyLength] = 0;  // 0T, esp. to keep sscanf() reliable
 newBuffer[khPtr->keyLength] = 0;  // 0T

 // After doing an update, Bullet repositions the index file to the updated key (the old key, where
 // the index file was positioned before the update, no longer exists unless the update was such
 // that the key did not change).  Therefore, atmomic access has to be activated, and so GetNext
 // starts at the last gotten key (in keyBuffer/recNo) rather than the current, internal index position.
 // True, that last gotten key/recNo combo no longer exists, but Bullet can still locate to that
 // position for continued processing of the key following that position.  An alternate method is
 // to open two handles, reading from one while updating to the other.

 orgAF = khPtr->flags & FLAGS_ATOMIC_BIT;
 if (orgAF == 0) khPtr->flags = khPtr->flags | FLAGS_ATOMIC_BIT;

 rc = BltIx4GetFirst(khPtr, keyBuffer, &recNo, &dataRec);
 for (i=0; i < recsToUpd; i++) {
    if (rc) break;
    cnt++;
    thisKey = 0;
    sscanf(dataRec.empID,"%X",&thisKey);        // first byte originally is '0'-'7' only,
    t = dataRec.empID[0];
    if (t > '1') {
       t = t + ('A' - '2');                     // char 2-7 become A-F
    }
    else {
       t = t + 8;                               // char 0,1 become 8,9
    }
    dataRec.empID[0] = t;

    rc = BltIx4Update(khPtr, newBuffer, recNo, &dataRec, &action); // use newBuffer to preserve atomic key

    // action performed (rc has any error code):
    //  0=nothing done (new record and disk record are the same)
    //  1=updated data record only (key did not change)
    //  3=updated index and updated data record (key did change)
    // 16=could not store new key (reindex required)
    // 17=record update failed (data is unchanged) and could not restore original key (reindex required)

    if (action != 3) {
       printf("all keys were expected to change, action (action=%lu) says this one did not\n", action);
       if (rc == 0) rc = 1;
    }

    if (rc == 0) {
       rc = BltIx4GetNext(khPtr, keyBuffer, &recNo, &dataRec);
    }
 }
 if (rc == EXB_END_OF_FILE) rc = 0;

 if (rc == 0 && cnt != recsToUpd) {
    printf("Expected to Update %lu records, processed %lu\n",recsToUpd,cnt);
    rc = 1;
 }

 // if was not atomic before turn atomic off, otherwise leave it as is

 if (orgAF == 0) khPtr->flags = khPtr->flags & ~FLAGS_ATOMIC_BIT;

 return rc;
}


#ifdef TEST_MODULE_UPDRECS

int main(void) {

 TBLT_RETC rc;
 TBLT_DH *dhPtr = 0;
 TBLT_KH *khPtr = 0;

 setbuf(stdout,NULL);

 printf("updrecs.c count=l%u\n",RECS_TO_UPDATE);
 rc = DemoCreateFiles();
 printf("Create Files, rc=%u\n",rc);
 if (rc == 0) {
    rc = DemoOpenFiles(&dhPtr,&khPtr,DBF_ASMODE,IX4_ASMODE);
    printf(" Open Files, rc=%u\n",rc);

    if (rc == 0) {
       rc = DemoInsertRecords(khPtr, RECS_TO_UPDATE, ADD_RND);
       printf("Insert Records, rc=%u\n",rc);

       if (rc == 0) {
          rc = DemoUpdateRecords(khPtr, RECS_TO_UPDATE);
          printf("Update Records, rc=%u\n",rc);

          if (rc == 0) {
             rc = DemoGetForward(khPtr, RECS_TO_UPDATE);
             printf("Get Forward, rc=%u\n",rc);

             rc = DemoGetReverse(khPtr, RECS_TO_UPDATE);
             printf("Get Reverse, rc=%u\n",rc);
          }
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

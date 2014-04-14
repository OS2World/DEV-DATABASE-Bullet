
//
// 16-Aug-1998
// demo, get certain records in key order
//

#include "bullet_3.h"
#include "blt3demo.h"

#ifdef TEST_MODULE_GETEQ
 #define RECS_TO_ADD 1000
#endif

// ------------------------------------------------------
// Get equal* records in key order

TBLT_RETC DemoGetEqual(TBLT_KH *khPtr, UINT pFlag) {

 TBLT_RETC rc;

 ULONG recNo;
 DATARECORD dataRec;
 UCHAR keyBuffer[MAX_KEY_LENGTH];

 strcpy(keyBuffer,"00000000");
 rc = BltIx4GetEqualOrGreater(khPtr, keyBuffer, &recNo, &dataRec);
 if (pFlag) printf("first key equal or greater than 00000000 is '%s' (rc=%u)\n",keyBuffer,rc);
 if (rc == 0) {

    strcpy(keyBuffer,"FFFFFFFF");
    rc = BltIx4GetEqualOrLesser(khPtr, keyBuffer, &recNo, &dataRec);
    if (pFlag) printf("first key equal or lesser than  FFFFFFFF is '%s' (rc=%u)\n",keyBuffer, rc);

    if (rc == 0) {

       recNo = khPtr->xbLinkPtr->noRecords;
       if (recNo > 1) recNo = recNo >> 1;
       rc = BltDataGetRecord(khPtr->xbLinkPtr, recNo, &dataRec);
       if (rc == 0) {

          // if key exists in index file for dataRec/recNo combo then that key is returned
          // (BltIx4KeyForRecord() typically is used when you need to know the key for
          // a record, where you will follow up by deleting the key (for backing out
          // changes made to the index file)

          rc = BltIx4KeyForRecord(khPtr, keyBuffer, recNo, &dataRec);
          if (pFlag) printf("key for recNo=%lu is: '%s' (rc=%u)\n", recNo, keyBuffer, rc);
          if (rc == 0) {

             // Iff SORT_DUPS_ALLOWED then recNo is used as criterion in locating the exact key
             // -- unless recNo = 0, then the key returned will match the key proper, but the
             // order of that key within its duplicates is undetermined (i.e. it could be the
             // first of "1234" key, or the last "1234", or anywhere in between).

             rc = BltIx4GetEqual(khPtr, keyBuffer, &recNo, &dataRec);
             if (pFlag) printf("key equal is '%s', recNo=%lu (rc=%u)\n",keyBuffer, recNo, rc);

          }
       }
    }
 }

 if (rc == EXB_END_OF_FILE) rc = 0;

 return rc;
}


#ifdef TEST_MODULE_GETEQ

int main(void) {

 TBLT_RETC rc;
 TBLT_DH *dhPtr = 0;
 TBLT_KH *khPtr = 0;

 setbuf(stdout,NULL);

 printf("geteq.c count=%u\n",RECS_TO_ADD);
 rc = DemoCreateFiles();
 printf("Create Files, rc=%u\n",rc);
 if (rc == 0) {
    rc = DemoOpenFiles(&dhPtr,&khPtr,DBF_ASMODE,IX4_ASMODE);
    printf(" Open Files, rc=%u\n",rc);

    if (rc == 0) {
       rc = DemoAddRecords(dhPtr, RECS_TO_ADD, ADD_RND);
       printf("Add Records, rc=%u\n",rc);

       if (rc == 0) {
          rc = DemoReindex(khPtr);
          printf("Reindex, rc=%u\n",rc);

          if (rc == 0) {
             rc = DemoGetEqual(khPtr,1);
             printf("Get Equal, rc=%u\n",rc);
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

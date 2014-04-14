
//
// 16-Aug-1998
// demo, get records in key order, reverse
//

#include "bullet_3.h"
#include "blt3demo.h"

#ifdef TEST_MODULE_GETREV
 #define RECS_TO_ADD 20000
#endif

// ------------------------------------------------------
// Get records in key order, reverse

TBLT_RETC DemoGetReverse(TBLT_KH *khPtr, ULONG expected) {

 TBLT_RETC rc;
 ULONG lastKey, thisKey;
 ULONG cnt = 0;

 ULONG recNo;
 DATARECORD dataRec;
 UCHAR keyBuffer[MAX_KEY_LENGTH];

 keyBuffer[khPtr->keyLength] = 0;  // 0T, esp. to keep sscanf() reliable

 rc = BltIx4GetLast(khPtr, keyBuffer, &recNo, &dataRec);
 lastKey = 0;
 sscanf(keyBuffer,"%X",&lastKey);
 while (rc == 0) {

    cnt++;

    // the validation logic does NOT expect DESCEND() (or change to "if (thisKey < lastKey)" if single component)

    rc = BltIx4GetPrev(khPtr, keyBuffer, &recNo, &dataRec);
    if (rc == 0) {
       thisKey = 0;
       sscanf(keyBuffer,"%X",&thisKey);
       if (thisKey > lastKey) {
          printf("GetPrev order is invalid (last key=0x%X, this key=0x%X, cnt=%lu)\n",lastKey, thisKey, cnt);
          printf("(test logic fails if using DESCEND()\n");
          rc = 1;
       }
       lastKey = thisKey;
    }
 }

 if (rc == EXB_TOP_OF_FILE) rc = 0;

 if (rc == 0 && cnt != expected) {
    printf("Get Reverse expected %lu, got %lu\n",expected,cnt);
    rc = 1;
 }

 return rc;
}


#ifdef TEST_MODULE_GETREV

int main(void) {

 TBLT_RETC rc;
 TBLT_DH *dhPtr = 0;
 TBLT_KH *khPtr = 0;

 setbuf(stdout,NULL);

 printf("getrev.c count=%u\n",RECS_TO_ADD);
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
             rc = DemoGetReverse(khPtr, RECS_TO_ADD);
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

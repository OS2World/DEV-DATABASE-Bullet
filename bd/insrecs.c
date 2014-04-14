
//
// 15-Sep-1998
// demo, insert records/keys
//

#include "bullet_3.h"
#include "blt3demo.h"

#ifndef TEST_MODULE_INSRECS
 extern LONG GetMeOne(LONG seed);

#else
 #define RECS_TO_INSERT 10000

static LONG GetMeOne(LONG seed) {

 ULONG lo = 16807 * (seed & 0xFFFF);  // cheap RNG
 ULONG hi = 16807 * ((ULONG)seed >> 16);

 lo = lo + ((hi & 0x7FFF) << 16);
 if (lo > 0x7FFFFFFF) lo = lo & 0x7FFFFFFF;
 lo = lo + (hi >> 15);
 if (lo > 0x7FFFFFFF) lo = lo & 0x7FFFFFFF;

 return (LONG)lo;
}

#endif


// ------------------------------------------------------
// Add records to the data file and keys into the index file using BltIx4Insert()
// random (addMode=ADD_RND) or sequential (addMode=ADD_SEQ) empID is generated

TBLT_RETC DemoInsertRecords(TBLT_KH *khPtr, ULONG recsToAdd, UINT addMode) {

 TBLT_RETC rc = 0;
 LONG rnd;
 ULONG i, recNo, action;
 DATARECORD dataRec;
 UCHAR keyBuffer[MAX_KEY_LENGTH];

 memset(&dataRec,0,sizeof(dataRec));
 keyBuffer[khPtr->keyLength] = 0;  // 0T

 dataRec.delTag = 32;
 strcpy(dataRec.empID,"anyEmpID");
 strcpy(dataRec.lName,"lastname...");
 strcpy(dataRec.fName,"firstname...");
 strncpy(dataRec.salary,"12345.78",8);
 strncpy(dataRec.personalMemoNo,"          ",10);

 if (addMode == ADD_RND) {
    rnd = GetMeOne(1);
 }
 else {
    recNo = 0;
 }

 for (i=0; i < recsToAdd; i++) {

    if (addMode == ADD_RND) {
       rnd = GetMeOne(rnd);
       sprintf(dataRec.empID,"%8.8X",rnd);
    }
    else {
       sprintf(dataRec.empID,"%8.8X",recNo+1);
    }

    rc = BltIx4Insert(khPtr, keyBuffer, &recNo, &dataRec, &action);

    // action performed (rc has any error code):
    //   0=nothing done (error)
    //   3=added record and stored key (normal)
    //  16=key build or key store failed but could not remove data record added
    //     (should mark record as deleted at next opportunity and perform a pack)

    if (action != 3) {
       printf("Insert action=%lu (rc was %u, if 0 setting rc=1)\n", action,rc);
       if (rc == 0) rc = 1;
    }

    if (rc) break;
 }
 return rc;
}


#ifdef TEST_MODULE_INSRECS

int main(void) {

 TBLT_RETC rc;
 TBLT_DH *dhPtr = 0;
 TBLT_KH *khPtr = 0;

 setbuf(stdout,NULL);

 printf("insrecs.c count=%u\n",RECS_TO_INSERT);
 rc = DemoCreateFiles();
 printf("Create Files, rc=%u\n",rc);
 if (rc == 0) {
    rc = DemoOpenFiles(&dhPtr,&khPtr,DBF_ASMODE,IX4_ASMODE);
    printf(" Open Files, rc=%u\n",rc);

    if (rc == 0) {
       rc = DemoInsertRecords(khPtr, RECS_TO_INSERT, ADD_RND);
       printf("Insert Records, rc=%u\n",rc);

       if (rc == 0) {
          rc = DemoGetForward(khPtr, RECS_TO_INSERT);
          printf("Get Forward, rc=%u\n",rc);

          rc = DemoGetReverse(khPtr, RECS_TO_INSERT);
          printf("Get Reverse, rc=%u\n",rc);
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

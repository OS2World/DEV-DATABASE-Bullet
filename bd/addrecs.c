
//
// 16-Aug-1998
// demo, add records
//
// has public GetMeOne() random number generator

#include "bullet_3.h"
#include "blt3demo.h"

#ifdef TEST_MODULE_ADDRECS
 #define RECS_TO_ADD 10000
#endif

LONG GetMeOne(LONG seed);

// ------------------------------------------------------
// Add records to the data file, using BltDataAddRecord()
// random (addMode=ADD_RND) or sequential (addMode=ADD_SEQ) empID is generated

TBLT_RETC DemoAddRecords(TBLT_DH *dhPtr, ULONG recsToAdd, UINT addMode) {

 TBLT_RETC rc = 0;
 LONG rnd;
 ULONG i, recNo;
 DATARECORD dataRec;

 memset(&dataRec,0,sizeof(dataRec));

 dataRec.delTag = 32;
 strcpy(dataRec.lName,"lastname...");
 strcpy(dataRec.fName,"firstname...");
 strncpy(dataRec.salary,"12345.78",8);
 strncpy(dataRec.personalMemoNo,"          ",10);

 if (addMode == ADD_RND) {
    rnd = GetMeOne(1);
    for (i=0; i < recsToAdd; i++) {
       rnd = GetMeOne(rnd);
       sprintf(dataRec.empID,"%8.8X",rnd);
       rc = BltDataAddRecord(dhPtr, &dataRec, &recNo);
       if (rc) break;
    }
 }
 else {
    recNo = 0;
    for (i=0; i < recsToAdd; i++) {
       sprintf(dataRec.empID,"%8.8X",recNo+1);
       rc = BltDataAddRecord(dhPtr, &dataRec, &recNo);
       if (rc) break;
    }

 }
 return rc;
}

LONG GetMeOne(LONG seed) {

 ULONG lo = 16807 * (seed & 0xFFFF);
 ULONG hi = 16807 * ((ULONG)seed >> 16);

 lo = lo + ((hi & 0x7FFF) << 16);
 if (lo > 0x7FFFFFFF) lo = lo & 0x7FFFFFFF;
 lo = lo + (hi >> 15);
 if (lo > 0x7FFFFFFF) lo = lo & 0x7FFFFFFF;

 return (LONG)lo;
}


#ifdef TEST_MODULE_ADDRECS

int main(void) {

 TBLT_RETC rc;
 TBLT_DH *dhPtr = 0;
 TBLT_KH *khPtr = 0;

 setbuf(stdout,NULL);

 printf("addrecs.c count=%u\n",RECS_TO_ADD);
 rc = DemoCreateFiles();
 printf("Create Files, rc=%u\n",rc);
 if (rc == 0) {
    rc = DemoOpenFiles(&dhPtr,&khPtr,DBF_ASMODE, IX4_ASMODE);
    printf(" Open Files, rc=%u\n",rc);

    if (rc == 0) {
       rc = DemoAddRecords(dhPtr, RECS_TO_ADD, ADD_RND);
       printf("Add Records, rc=%u\n",rc);

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


//
// 16-Aug-1998
// demo, add memo
//

#include "bullet_3.h"
#include "blt3demo.h"

#ifdef TEST_MODULE_MEMOADD
 #define MEMOS_TO_ADD 10000
#endif


// ------------------------------------------------------
// Add memos/records to the data files
// adds records as well as memos

TBLT_RETC DemoAddMemos(TBLT_DH *dhPtr, ULONG memosToAdd, ULONG memoSize) {

 TBLT_RETC rc = 0;
 LONG rnd;
 ULONG i, recNo, memoNo, dataBytes;

                     //0123456789-123456789-123456789-123456789-123456789-123456789-12
 UCHAR memoData[64] = "Memo data...12345678 .........................................Z";
 UCHAR tmpBuffer[16];
 DATARECORD dataRec;

 memset(&dataRec,0,sizeof(dataRec));

 dataRec.delTag = 32;
 strcpy(dataRec.lName,"lastname...");
 strcpy(dataRec.fName,"firstname...");
 strncpy(dataRec.salary,"12345.78",8);

 dataBytes = strlen(memoData)+1;          // +1 to store zero terminator also
 if (memoSize == 0) memoSize = dataBytes; // simply for demonstration

 rnd = GetMeOne(1);
 for (i=0; i < memosToAdd; i++) {

    sprintf(&memoData[12],"%8u",i+1);   // simply for demonstration and easy view in .DBT
    rc = BltMemoAdd(dhPtr, memoSize, dataBytes, &memoData, &memoNo, 0);
    if (rc == 0) {
       rnd = GetMeOne(rnd);
       sprintf(dataRec.empID,"%8.8X",rnd);      // can use sprintf() since empID is C string
       sprintf(tmpBuffer,"%10.10u",memoNo);     // cannot for personalMemoNo since it's exactly 10 bytes
       strncpy(dataRec.personalMemoNo,tmpBuffer,10); // copy 10 bytes (no zero terminator, so use strncpy)

       rc = BltDataAddRecord(dhPtr, &dataRec, &recNo);
    }
    if (rc) break;
 }

 return rc;
}


#ifdef TEST_MODULE_MEMOADD

int main(void) {

 TBLT_RETC rc;
 TBLT_DH *dhPtr = 0;
 TBLT_KH *khPtr = 0;
 time_t startTime, endTime;

 setbuf(stdout,NULL);

 printf("memoadd.c count=%u\n",MEMOS_TO_ADD);
 rc = DemoCreateFiles();
 printf("Create Files, rc=%u\n",rc);
 if (rc == 0) {
    rc = DemoOpenFiles(&dhPtr,&khPtr,DBF_ASMODE,IX4_ASMODE);
    printf(" Open Files, rc=%u\n",rc);

    if (rc == 0) {
       printf("Adding Memos/Records...");
       time(&startTime);
       rc = DemoAddMemos(dhPtr, MEMOS_TO_ADD, DBT_MEMOSIZE);
       time(&endTime);
       printf("\rAdd %u memos/records took %u secs, rc=%u\n",MEMOS_TO_ADD,(endTime - startTime),rc);

#if 0
{
 DATARECORD dataRec;
 ULONG recNo;

 for (recNo=1; recNo <= MEMOS_TOADD; recNo++) {

   printf("recNo=%u\n",recNo);
   rc = BltDataGetRecord(dhPtr, recNo, &dataRec);
   if (rc == 0) {
      strncpy(dataRec.personalMemoNo,"          ",10);
      rc = BltDataUpdateRecord(dhPtr, recNo, &dataRec);
      if (rc == 0) {
         rc = BltMemoDelete(dhPtr, recNo);
      }
   }
 }
 printf("memo/delete, rc=%u \n", rc);

}
#endif

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


//
// 16-Aug-1998
// demo, delete memo
//

#include "bullet_3.h"
#include "blt3demo.h"

#ifdef TEST_MODULE_MEMODEL
 #define MEMOS_TO_ADD 10000
#endif


// ------------------------------------------------------
// Delete memos (DBF records remains active)

TBLT_RETC DemoDeleteMemos(TBLT_DH *dhPtr, ULONG memosToDelete) {

 TBLT_RETC rc = 0;
 ULONG recNo, memoNo;

 UCHAR tmpBuffer[16];
 DATARECORD dataRec;

 for (recNo=1; recNo <= memosToDelete; recNo++) {
    if (rc) break;

    rc = BltDataGetRecord(dhPtr, recNo, &dataRec);
    if (rc == 0) {

       strncpy(tmpBuffer,dataRec.personalMemoNo,10);
       tmpBuffer[10] = 0;
       memoNo = atol(tmpBuffer);

       if (memoNo) {

          // remove memo number from DBF record

          strncpy(dataRec.personalMemoNo,"          ",10);
          rc = BltDataUpdateRecord(dhPtr, recNo, &dataRec);
          if (rc == 0) {

             // remove memo from DBT file

             rc = BltMemoDelete(dhPtr, recNo);
             if (rc) {

                // if failed memo delete, restore memoNo in DBF record (see memoadd.c for more)

                sprintf(tmpBuffer,"%10.10u",memoNo);
                strncpy(dataRec.personalMemoNo,tmpBuffer,10);
                BltDataUpdateRecord(dhPtr, recNo, &dataRec);  // rc already set to original problem code
             }
          }
       }
    }
 }

 return rc;
}


#ifdef TEST_MODULE_MEMODEL

int main(void) {

 TBLT_RETC rc;
 ULONG cnt;
 TBLT_DH *dhPtr = 0;
 TBLT_KH *khPtr = 0;
 time_t startTime, endTime;

 setbuf(stdout,NULL);

 printf("memodel.c count=%u\n",MEMOS_TO_ADD);
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

       printf("Deleting Memos/Records...");
       cnt = 0;
       time(&startTime);
       rc = DemoDeleteMemos(dhPtr, MEMOS_TO_ADD);
       time(&endTime);
       printf("\rDel %u memos/records took %u secs, rc=%u\n",MEMOS_TO_ADD,(endTime - startTime),rc);

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

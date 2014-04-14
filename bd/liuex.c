
//
// 7-Nov-1998
// demo, lockEx, insertEx, updateEx
//

#include "bullet_3.h"
#include "blt3demo.h"

#ifdef TEST_MODULE_LIUEX
 #define RECS_TO_DO 5
#endif

#define SHOW_ALL

// ------------------------------------------------------
// Un/LockEx (mode=0 then lock)
//

TBLT_RETC DemoLockEx(ULONG flag, TBLT_LOCKEX *lockExPtr, ULONG *slotPtr) {

 TBLT_RETC rc = 0;

 if (flag == 0) {
    rc = BltLockEx(lockExPtr, slotPtr, 0);
 }
 else {
    rc = BltUnlockEx(lockExPtr, slotPtr);
 }

#ifdef SHOW_ALL
 {
  INT i;
  for (i=0; i < 4; i++) {
     if (flag == 0) {
        printf("lockEx[%u] action:",i);
     }
     else {
        printf("unlockEx[%u] action:",i);
     }
     printf(" %lu \n",lockExPtr->action);
     lockExPtr++;
  }
 }
#endif

 return rc;
}



// ------------------------------------------------------
// InsertEx

TBLT_RETC DemoInsertEx(TBLT_IUEX *iuexPtr, ULONG *slotPtr) {

 TBLT_RETC rc = 0;

 rc = BltIx4InsertEx(iuexPtr, slotPtr);

#ifdef SHOW_ALL // note that .action members after *slotPtr have undefined values since they weren't touched by Bullet
 {
  INT i;
  for (i=0; i < 4; i++) {
     printf("Insert iuEx[%u] action:  %lu \n",i,iuexPtr->action);
     iuexPtr++;
  }
 }
#endif

 return rc;
}


// ------------------------------------------------------
// UpdateEx

TBLT_RETC DemoUpdateEx(TBLT_IUEX *iuexPtr, ULONG *slotPtr) {

 TBLT_RETC rc = 0;

 rc = BltIx4UpdateEx(iuexPtr, slotPtr, 0);

#ifdef SHOW_ALL // note that .action members after *slotPtr have undefined values since they weren't touched by Bullet
 {
  INT i;
  for (i=0; i < 4; i++) {
     printf("Update iuEx[%u] action:  %lu \n",i,iuexPtr->action);
     iuexPtr++;
  }
 }
#endif

 return rc;
}


#ifdef TEST_MODULE_LIUEX

int main(void) {

 TBLT_RETC rc;
 TBLT_DH *dhPtr[2] = {0};
 TBLT_KH *khPtr[4] = {0};

 TBLT_LOCKEX lockPack[4];
 TBLT_IUEX iuPack[4];

 UCHAR keyBuff[4][16]; // key length is just 8 bytes
 DATARECORD dataRec[2];

 ULONG slot, empID, recNo1, recNo2;
 UINT i;

 setbuf(stdout,NULL);

 printf("liuex.c * iuEx pack has two DBF and four IX4 files\n");

 rc = DemoCreateFiles();
 printf("Create Files, rc=%u\n",rc);
 if (rc == 0) {
    rc = DemoOpenFiles(&dhPtr[0],&khPtr[0],DBF_ASMODE,IX4_ASMODE);
    if (rc == 0) {

       // have 1 data and 1 index file created and open
       // create 1 more data and 3 more index (identical for simplicity in this demo)

       ULONG dhSize = sizeof(TBLT_DH) + (sizeof(TBLT_FD)*((dhPtr[0]->noFields)-1));
       ULONG khSize = sizeof(TBLT_KH) + khPtr[0]->nodeSize-512;

       dhPtr[1] = malloc(dhSize);
       if (dhPtr[1]) khPtr[1] = malloc(khSize);
       if (khPtr[1]) khPtr[2] = malloc(khSize);
       if (khPtr[2]) khPtr[3] = malloc(khSize);

       if (khPtr[3] == 0) {
          rc = 8;
       }
       else {

          BltDeleteFile(DBF_FILENAME2,0);
          BltDeleteFile(DBT_FILENAME2,0);
          BltDeleteFile(IX4_FILENAME2,0);
          BltDeleteFile(IX4_FILENAME3,0);
          BltDeleteFile(IX4_FILENAME4,0);

          rc = BltDataCopyHeader(dhPtr[0], DBF_FILENAME2);
          if (rc == 0) rc = BltIx4CopyHeader(khPtr[0], IX4_FILENAME2);
          if (rc == 0) rc = BltIx4CopyHeader(khPtr[0], IX4_FILENAME3);
          if (rc == 0) rc = BltIx4CopyHeader(khPtr[0], IX4_FILENAME4);

          if (rc) {
             printf("A copy header operation failed, rc=%u\n",rc);
          }
          else {

             memset(dhPtr[1],0,dhSize);
             dhPtr[1]->filenamePtr = DBF_FILENAME2;
             dhPtr[1]->asMode = dhPtr[0]->asMode;
             dhPtr[1]->noFields = dhPtr[0]->noFields;
             rc = BltDataOpenFile(dhPtr[1], DBF_OPENMODE);

             if (rc == 0) {
                memset(khPtr[1],0,khSize);
                memset(khPtr[2],0,khSize);
                memset(khPtr[3],0,khSize);
                khPtr[1]->filenamePtr = IX4_FILENAME2;     // this and the first ix4 index the first DBF
                khPtr[1]->asMode = khPtr[0]->asMode;
                khPtr[1]->nodeSize = khPtr[0]->nodeSize;
                khPtr[1]->xbLinkPtr = dhPtr[0]
                ;
                khPtr[2]->filenamePtr = IX4_FILENAME3;     // these two index the second DBF
                khPtr[2]->asMode = khPtr[0]->asMode;
                khPtr[2]->nodeSize = khPtr[0]->nodeSize;
                khPtr[2]->xbLinkPtr = dhPtr[1];
                ;
                khPtr[3]->filenamePtr = IX4_FILENAME4;
                khPtr[3]->asMode = khPtr[0]->asMode;
                khPtr[3]->nodeSize = khPtr[0]->nodeSize;
                khPtr[3]->xbLinkPtr = dhPtr[1];

                rc = BltIx4OpenFile(khPtr[1], IX4_OPENMODE);
                if (rc == 0) rc = BltIx4OpenFile(khPtr[2], IX4_OPENMODE);
                if (rc == 0) rc = BltIx4OpenFile(khPtr[3], IX4_OPENMODE);
             }
             printf(" Open Files, rc=%u\n",rc);
          }
       }
    }

    if (rc == 0) {

       lockPack[0].khdhPtr = khPtr[0];
       lockPack[0].mode = LOCK_MODE_EXCLUSIVE;
       lockPack[0].recNoStart = 0;
       lockPack[0].recCount = 0;
       lockPack[0].flags = FLAGSEX_LOCK_DUAL;   // lock khPtr[0]->xbLinkPtr->handle also
       lockPack[0].rsvPtr = 0;
       lockPack[0].nextPtr = &lockPack[1];

       lockPack[1].khdhPtr = khPtr[1];
       lockPack[1].mode = LOCK_MODE_EXCLUSIVE;
       lockPack[1].recNoStart = 0;
       lockPack[1].recCount = 0;
       lockPack[1].flags = 0;                   // lock only the index handle in khPtr[1]->handle
       lockPack[1].rsvPtr = 0;
       lockPack[1].nextPtr = &lockPack[2];

       lockPack[2].khdhPtr = khPtr[2];
       lockPack[2].mode = LOCK_MODE_EXCLUSIVE;
       lockPack[2].recNoStart = 0;
       lockPack[2].recCount = 0;
       lockPack[2].flags = FLAGSEX_LOCK_DUAL;   // lock khPtr[2]->xbLinkPtr->handle also
       lockPack[2].rsvPtr = 0;
       lockPack[2].nextPtr = &lockPack[3];

       lockPack[3].khdhPtr = khPtr[3];
       lockPack[3].mode = LOCK_MODE_EXCLUSIVE;
       lockPack[3].recNoStart = 0;
       lockPack[3].recCount = 0;
       lockPack[3].flags = 0;                   // lock only the index handle in khPtr[3]->handle
       lockPack[3].rsvPtr = 0;
       lockPack[3].nextPtr = 0;

       printf("LockEx...\n");
       rc = DemoLockEx(0,lockPack,&slot);
       if (rc) printf(" ...lock failed at slot #%lu, rc=%u\n",slot, rc);

       if (rc == 0) {

          // BltInsertEx

          empID = 1;

          dataRec[0].delTag = 32;
          strcpy(dataRec[0].lName,"lastname1..");
          strcpy(dataRec[0].fName,"firstname1..");
          strncpy(dataRec[0].salary,"12345.78",8);
          strncpy(dataRec[0].personalMemoNo,"          ",10);
          sprintf(dataRec[0].empID,"%8.8X",0x10000000+empID);  // empID field is a C)har field, 9 bytes (8+0T)

          dataRec[1].delTag = 32;
          strcpy(dataRec[1].lName,"lastname2..");
          strcpy(dataRec[1].fName,"firstname2..");
          strncpy(dataRec[1].salary,"12345.78",8);
          strncpy(dataRec[1].personalMemoNo,"          ",10);
          sprintf(dataRec[1].empID,"%8.8X",0x20000000+empID);


          iuPack[0].khPtr = khPtr[0];
          iuPack[0].keyBufferPtr = &keyBuff[0];
          iuPack[0].recordPtr = &dataRec[0];
          iuPack[0].flags = 0;
          iuPack[0].nextPtr = &iuPack[1];

          iuPack[1].khPtr = khPtr[1];
          iuPack[1].keyBufferPtr = &keyBuff[1];
          iuPack[1].recordPtr = &dataRec[0];
          iuPack[1].flags = FLAGSEX_IU_SKIPDATA;        // don't add data since done in iuPack[0]
          iuPack[1].nextPtr = &iuPack[2];

          iuPack[2].khPtr = khPtr[2];
          iuPack[2].keyBufferPtr = &keyBuff[2];
          iuPack[2].recordPtr = &dataRec[1];
          iuPack[2].flags = 0;
          iuPack[2].nextPtr = &iuPack[3];

          iuPack[3].khPtr = khPtr[3];
          iuPack[3].keyBufferPtr = &keyBuff[3];
          iuPack[3].recordPtr = &dataRec[1];
          iuPack[3].flags = FLAGSEX_IU_SKIPDATA;        // don't add data since done in iuPack[2]
          iuPack[3].nextPtr = 0;

          printf("InsertEx'ing %u times...\n",RECS_TO_DO);
          for (i=0; i < RECS_TO_DO; i++) {

             //if (i == (RECS_TO_DO-1)) {
             //   printf("*** test rollback\n");
             //   iuPack[3].nextPtr = -1;
             //}

             iuPack[1].recNo = 0;                       // if skipping data these .recNo must be 0
             iuPack[3].recNo = 0;                       // to automatically use recNo of previous add
             rc = DemoInsertEx(iuPack,&slot);
             if (rc) {
                printf(" ...InsertEx #%u at slot #%lu, rc=%u \n",(i+1),slot,rc);
                break;
             }
             empID++;
             sprintf(dataRec[0].empID,"%8.8X",0x10000000+empID);
             sprintf(dataRec[1].empID,"%8.8X",0x20000000+empID);
          }

          if (rc == 0) {

             // BltUpdateEx

             empID++;   // this will be a new empID, so a new key is generated

             dataRec[0].delTag = 32;
             strcpy(dataRec[0].lName,"lastname1u.");
             strcpy(dataRec[0].fName,"firstname1u.");
             strncpy(dataRec[0].salary,"12345.78",8);
             strncpy(dataRec[0].personalMemoNo,"          ",10);
             sprintf(dataRec[0].empID,"%8.8X",0x10000000+empID);

             dataRec[1].delTag = 32;
             strcpy(dataRec[1].lName,"lastname2u.");
             strcpy(dataRec[1].fName,"firstname2u.");
             strncpy(dataRec[1].salary,"12345.78",8);
             strncpy(dataRec[1].personalMemoNo,"          ",10);
             sprintf(dataRec[1].empID,"%8.8X",0x20000000+empID);

             iuPack[0].khPtr = khPtr[0];
             iuPack[0].keyBufferPtr = &keyBuff[0];
             iuPack[0].recordPtr = &dataRec[0];
             iuPack[0].flags = 0;
             iuPack[0].nextPtr = &iuPack[1];

             iuPack[1].khPtr = khPtr[1];
             iuPack[1].keyBufferPtr = &keyBuff[1];
             iuPack[1].recordPtr = &dataRec[0];
             iuPack[1].flags = FLAGSEX_IU_SKIPDATA;        // don't update data since done in iuPack[0]
             iuPack[1].nextPtr = &iuPack[2];               // (do update key, if needed)

             iuPack[2].khPtr = khPtr[2];
             iuPack[2].keyBufferPtr = &keyBuff[2];
             iuPack[2].recordPtr = &dataRec[1];
             iuPack[2].flags = 0;
             iuPack[2].nextPtr = &iuPack[3];

             iuPack[3].khPtr = khPtr[3];
             iuPack[3].keyBufferPtr = &keyBuff[3];
             iuPack[3].recordPtr = &dataRec[1];
             iuPack[3].flags = FLAGSEX_IU_SKIPDATA;        // don't update data since done in iuPack[2]
             iuPack[3].nextPtr = 0;

             printf("UpdateEx'ing %u times...\n",RECS_TO_DO);

             recNo1 = 1;                                   // update all previously inserted data records
             recNo2 = 1;                                   // recNo1 is for first DBF, recNo2 is for second
             for (i=0; i < RECS_TO_DO; i++) {

                //if (i == (RECS_TO_DO-1)) {
                //   printf("*** test rollback\n");
                //   iuPack[3].nextPtr = -1;
                //}

                iuPack[0].recNo = recNo1;
                iuPack[1].recNo = recNo1;
                iuPack[2].recNo = recNo2;
                iuPack[3].recNo = recNo2;
                rc = DemoUpdateEx(iuPack,&slot);
                if (rc) {
                   printf("UpdateEx #%u at slot #%lu, rc=%u \n",(i+1),slot,rc);
                   break;
                }
                recNo1++;                                  // next record to update in first DBF
                recNo2++;                                  // next record to update in second DBF

                empID++;                                   // and modify key field so new key is created
                sprintf(dataRec[0].empID,"%8.8X",0x10000000+empID);  // for this data record in the first DBF
                sprintf(dataRec[1].empID,"%8.8X",0x20000000+empID);  // same for second DBF

             }

             printf("UnlockEx...\n");
             rc = DemoLockEx(1,lockPack,&slot);
             if (rc) printf(" ...unlock failed at slot #%lu, rc=%u \n",slot, rc);
          }
       }
    }

    printf("Close Files...\n");
    for (i=0; i < 4; i++) {
       rc = BltIx4CloseFile(khPtr[i]);
#ifndef SHOW_ALL
       if (rc)
#endif
          printf("Index #%u Close, rc=%u\n",i,rc);
       if (khPtr[i]) free(khPtr[i]);
    }

    for (i=0; i < 2; i++) {
       rc = BltDataCloseFile(dhPtr[i]);
#ifndef SHOW_ALL
       if (rc)
#endif
          printf(" Data #%u Close, rc=%u\n",i,rc);
       if (dhPtr[i]) free(dhPtr[i]);
    }

 }

 printf("Done.\n");
 return (int)rc;
}

#endif

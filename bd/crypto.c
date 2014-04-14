
//
// 2-Nov-1998
// demo, crypto
// (see addrix.c for additional notes)
// 25-Jul-99: moved keyCount assignment in bulk add to inside of node loop

#include <stddef.h>

#include "bullet_3.h"
#include "blt3demo.h"

#ifdef BLT_WIN16
 #define XFARDATA __far   // one way to get compiler to generate full pointer code to get
#else                     // data seen in a DLL's callback (16-bit DLLs only)
 #define XFARDATA
#endif

#ifdef TEST_MODULE_CRYPTO
 #define RECS_TO_CRYPTO 100
 static ULONG XFARDATA counts[8];
#endif

//static TBLT_RETC TBLT_CALLBACK MyDataCrypto(ULONG mode, TBLT_DH *dhPtr, VOID *recordPtr, ULONG recNo);
//static TBLT_RETC TBLT_CALLBACK MyIx4Crypto(ULONG mode, TBLT_KH *khPtr, VOID *keyPtr, ULONG recNoPlus);
//static TBLT_RETC TBLT_CALLBACK MyFuncCallBack(TBLT_KH *khPtr, ULONG info);

#ifdef TEST_MODULE_CRYPTO

//------------------------------------
// data crypto handler
// simple demonstration of crypto:  adds 1 to each character to encrypt, subtracts 1 from each to decrypt

static TBLT_RETC TBLT_CALLBACK MyDataCrypto(ULONG mode, TBLT_DH *dhPtr, VOID *recordPtr, ULONG recNo) {

 UINT i;
 UCHAR *tPtr = recordPtr;
 UINT chars = (UINT)dhPtr->recordLength;

 for (i=0; i < chars; i++) {
    switch (mode) {
    case CRYPTO_FROM_RECGET:
    case CRYPTO_FROM_XRECADD:
    case CRYPTO_FROM_XRECUPD:
       *tPtr = *tPtr - 1;
       break;
    case CRYPTO_FROM_RECADD:
    case CRYPTO_FROM_RECUPD:
       *tPtr = *tPtr + 1;
       break;
    }
    tPtr++;
 }

 switch (mode) {
 case CRYPTO_FROM_RECGET:
    counts[0]++;
    break;
 case CRYPTO_FROM_RECADD:
    counts[1]++;
    break;
 case CRYPTO_FROM_RECUPD:
    counts[2]++;
    break;
 case CRYPTO_FROM_XRECADD:
    counts[3]++;
    break;
 case CRYPTO_FROM_XRECUPD:
    counts[4]++;
    break;
 }

 return 0;
 recNo;
}


//------------------------------------
// IX4 crypto handler
// simple demonstration of crypto:  adds 1 to each character to encrypt, subtracts 1 from each to decrypt

static TBLT_RETC TBLT_CALLBACK MyIx4Crypto(ULONG mode, TBLT_KH *khPtr, VOID *keyPtr, ULONG recNoPlus) {

 if (mode == CRYPTO_FROM_KEYADDBULK) {

    UINT i, j, k;
    UCHAR *tPtr;
    UCHAR *t2Ptr = keyPtr;
    ULONG nodes = recNoPlus;
    UINT nodeSize = (UINT)khPtr->nodeSize;
    UINT keyLength = (UINT)khPtr->keyLength;
    UINT keyCount;

    for (i=0; i < nodes; i++) {

       keyCount =  (UINT)*t2Ptr;  // first byte of node is key count

       tPtr = t2Ptr + 5;                // position to first key on node
       for (j=0; j < keyCount; j++) {
          for (k=0; k < keyLength; k++) {
             *tPtr = *tPtr + 1;
             tPtr++;
          }
          tPtr = tPtr + 8;              // position to next key
       }
       t2Ptr = t2Ptr + nodeSize;
    }
 }
 else {

    UINT i;
    UCHAR *tPtr = keyPtr;
    UINT chars = (UINT)khPtr->keyLength;

    for (i=0; i < chars; i++) {
       switch(mode) {
       case CRYPTO_FROM_KEYGET:
          *tPtr = *tPtr - 1;
          break;
       case CRYPTO_FROM_KEYADD:
          *tPtr = *tPtr + 1;
          break;
       }
       tPtr++;
    }
 }

 switch (mode) {
 case CRYPTO_FROM_KEYADDBULK:
    counts[5]++;
    break;
 case CRYPTO_FROM_KEYGET:
    counts[6]++;
    break;
 case CRYPTO_FROM_KEYADD:
    counts[7]++;
    break;
 }

 return 0;
}


//------------------------------------
// callback handler (reindex)

static TBLT_RETC TBLT_CALLBACK MyFuncCallBack(TBLT_KH *khPtr, ULONG info) {

 static UINT lastPct = 0;

 if ((UINT)khPtr->reindexPct != lastPct) {
    lastPct = (UINT)khPtr->reindexPct;
    if (lastPct) {
       printf("Reindex pct=%u \r",lastPct);
    }
    else {
       printf("Reindex pct=done  \n");
    }
 }

 return 0;
 info;
}


// ------------------------------------------------------
// Reindex

static TBLT_RETC DemoReindex(TBLT_KH *khPtr) {

 TBLT_RETC rc = 0;

 ULONG recNo;
 TBLT_IX4CALLBACKPTR oldFN;

 UCHAR workFN[] = "$DEMO_01.TMP";
 UCHAR keyBuffer[MAX_KEY_LENGTH];

 keyBuffer[khPtr->keyLength] = 0;  // 0T

 BltDeleteFile(workFN,0);

 oldFN = khPtr->xCallBackPtr;
 khPtr->xCallBackPtr = MyFuncCallBack;
 rc = BltFuncIx4Reindex(khPtr, workFN, 100, keyBuffer, &recNo);
 khPtr->xCallBackPtr = oldFN;

 return rc;
}

#endif  // TEST_MODULE_CRYPTO


#ifdef TEST_MODULE_CRYPTO

int main(void) {

 TBLT_RETC rc;
 TBLT_DH *dhPtr = 0;
 TBLT_KH *khPtr = 0;
 UINT z;
 time_t startTime, endTime;

 ULONG action;
 ULONG recNo;
 DATARECORD dataRec;
 UCHAR keyBuffer[MAX_KEY_LENGTH+1];  // +1 for 0T of max key length

 setbuf(stdout,NULL);

 // compiler will detect if the structure sizes are not correct by this code:

 z = 1/(KH_BUFFER_OFFSET_IS == OFFSET_KH_BUFFER);
 z = 1/(DH_FD1_OFFSET_IS    == OFFSET_DH_FD1);

 printf("sizeof TBLT_KH=%u\n",sizeof(TBLT_KH));    // 2048 bytes w/512-byte node
 printf("sizeof TBLT_DH=%u\n",sizeof(TBLT_DH));    // 224 bytes

 printf("crypto.c count=%u\n",RECS_TO_CRYPTO);
 rc = DemoCreateFiles();
 printf("Create Files, rc=%u\n",rc);
 if (rc == 0) {
    rc = DemoOpenFiles(&dhPtr,&khPtr,DBF_ASMODE,IX4_ASMODE);
    printf(" Open Files, rc=%u\n",rc);

    if (rc == 0) {

       dhPtr->xCryptoPtr = MyDataCrypto;
       khPtr->xCryptoPtr = MyIx4Crypto;

       printf("Adding Records...");
       time(&startTime);
       rc = DemoAddRecords(dhPtr, RECS_TO_CRYPTO, ADD_SEQ); // ADD_RND likes a big disk cache for reads
       time(&endTime);
       printf("\rAdd %u records took %u secs, rc=%u\n",RECS_TO_CRYPTO,(endTime - startTime),rc);

       if (rc == 0) {

          time(&startTime);
          rc = DemoReindex(khPtr);
          time(&endTime);
          printf("Reindex took %u secs, rc=%u\n",(endTime - startTime),rc);
          printf("memory requested by reindex=%7lu bytes (data below is hex, 64hex=100)\n",khPtr->MMUBR);

          if (rc == 0) {
             keyBuffer[khPtr->keyLength] = 0;  // 0T buffer for printf
             rc = BltIx4GetFirst(khPtr, keyBuffer, &recNo, &dataRec);
             while (rc == 0) {
                //printf("recNo=%lu, key='%s', data='%s' \n",recNo, keyBuffer,dataRec.empID);
                rc = BltIx4GetNext(khPtr, keyBuffer, &recNo, &dataRec);
             }
          }

          // the following show the crypto stuff for insert, update, and delete

          strcpy(dataRec.empID,"9999999F");
          rc = BltIx4Insert(khPtr, keyBuffer, &recNo, &dataRec, &action);
          if (rc == 0) {
             rc = BltIx4GetLast(khPtr, keyBuffer, &recNo, &dataRec);
             if (rc == 0) {
                printf("Get(Inserted)Last: recNo=%lu, key='%s', data='%s', action=%lu \n",recNo, keyBuffer,dataRec.empID,action);
             }
          }
          if (rc) printf("Insert failed, rc=%u\n",rc);

          strcpy(dataRec.empID,"9999999E");
          rc = BltIx4Update(khPtr, keyBuffer, recNo, &dataRec, &action);
          if (rc == 0) {
             rc = BltIx4GetLast(khPtr, keyBuffer, &recNo, &dataRec);
             if (rc == 0) {
                printf("Get(Updated)Last: recNo=%lu, key='%s', data='%s', action=%lu \n",recNo, keyBuffer,dataRec.empID,action);
             }
          }
          if (rc) printf("Update failed, rc=%u\n",rc);

          rc = BltIx4DeleteKey(khPtr, keyBuffer, recNo);
          if (rc) {
             printf("Delete key failed, rc=%u\n",rc);
          }
          else {
             rc = BltIx4GetLast(khPtr, keyBuffer, &recNo, &dataRec);
             if (rc == 0) {
                printf("Get(Deleted)Last: recNo=%lu, key='%s', data='%s' \n",recNo, keyBuffer,dataRec.empID);
             }
             else {
                printf("Get(Deleted)Last failed, rc=%u\n",rc);
             }
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

 // times each crypto action was called

 printf("CRYPTO_FROM_RECGET: %lu (times called)\n",counts[0]);
 printf("CRYPTO_FROM_RECADD: %lu\n",counts[1]);
 printf("CRYPTO_FROM_RECUPD: %lu\n",counts[2]);
 printf("CRYPTO_FROM_XRECADD: %lu\n",counts[3]);
 printf("CRYPTO_FROM_XRECUPD: %lu\n",counts[4]);
 printf("CRYPTO_FROM_KEYADDBULK: %lu\n",counts[5]);
 printf("CRYPTO_FROM_KEYGET: %lu\n",counts[6]);
 printf("CRYPTO_FROM_KEYADD: %lu\n",counts[7]);

 printf("Done (data on disk was always encrypted: see files $DEMO_01.DBF/IX4).\n");
 return (int)rc;
}
#endif


//
// 16-Aug-1998
// demo, open database files
//
// 30-Sep-99: have to check if s_dhPtr/skhPtr are valid before using (see if (rc) below)

#include "bullet_3.h"
#include "blt3demo.h"

ULONG dbfOpenMode = DBF_OPENMODE;
ULONG ix4OpenMode = IX4_OPENMODE;


// ----------------------------------------------------
// Opens DBF,DBT,IX4 files as described in blt3demo.h
//

TBLT_RETC DemoOpenFiles(TBLT_DH **dhPtr, TBLT_KH **khPtr, ULONG asModeData, ULONG asModeIx4) {

 TBLT_RETC rc = 0;
 UINT dhdSize, khSize;

 ULONG noFields;
 TBLT_DBFINFO dbfInfo;
 TBLT_IX4INFO ix4Info;

 TBLT_DH *s_dhPtr = 0;
 TBLT_KH *s_khPtr = 0;

 rc = BltDataInfoFile(DBF_FILENAME,
                      asModeData,
                      &dbfInfo,
                      &noFields,
                      0);

 if (rc == 0) {

    // dhdSize is size of TBLT_DH plus ((number of fields-1) * 32)

    dhdSize = sizeof(TBLT_DH) + (sizeof(TBLT_FD)*(noFields-1));
    s_dhPtr = malloc(dhdSize);
    if (s_dhPtr == 0) {
       rc = 8;
    }
    else {
       memset(s_dhPtr,0,dhdSize);
       s_dhPtr->filenamePtr = DBF_FILENAME;
       s_dhPtr->asMode = asModeData;
       s_dhPtr->noFields = DBF_FIELDS;
       rc = BltDataOpenFile(s_dhPtr,
                            dbfOpenMode);
       if (rc == 0) {

          rc = BltIx4InfoFile(IX4_FILENAME,
                              asModeIx4,
                              &ix4Info,
                              0);

          if (rc == 0) {

             // khSize is size of TBLT_KH plus nodeSize-512

             khSize = sizeof(TBLT_KH) + ix4Info.nodeSize-512;
             s_khPtr = malloc(khSize);
             if (s_khPtr == 0) {
                rc = 8;
             }
             else {
                memset(s_khPtr,0,khSize);
                s_khPtr->filenamePtr = IX4_FILENAME;
                s_khPtr->asMode = asModeIx4;
                s_khPtr->nodeSize = ix4Info.nodeSize;
                s_khPtr->xbLinkPtr = s_dhPtr;
                rc = BltIx4OpenFile(s_khPtr,
                                    ix4OpenMode);
             }
          }
       }
    }

 }

 if (rc) {

    // 30-Sep-99: have to check if s_dhPtr/skhPtr are valid before using

    if (s_dhPtr && s_dhPtr->handle) BltDataCloseFile(s_dhPtr);
    if (s_khPtr && s_khPtr->handle) BltIx4CloseFile(s_khPtr);

    if (s_dhPtr) free(s_dhPtr);
    if (s_khPtr) free(s_khPtr);
    *dhPtr = 0;
    *khPtr = 0;
 }
 else {
    *dhPtr = s_dhPtr;
    *khPtr = s_khPtr;
 }

 return rc;
}


#ifdef TEST_MODULE_OPEN

int main(void) {

 TBLT_RETC rc;
 TBLT_DH *dhPtr = 0;
 TBLT_KH *khPtr = 0;

 setbuf(stdout,NULL);

 printf("open.c\n");

 rc = DemoCreateFiles();
 printf("Create Files, rc=%u\n",rc);
 if (rc == 0) {
    rc = DemoOpenFiles(&dhPtr,&khPtr,DBF_ASMODE,IX4_ASMODE);
    printf(" Open Files, rc=%u\n",rc);
    if (rc == 0) {
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

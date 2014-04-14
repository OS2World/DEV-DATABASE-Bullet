
//
// 16-Aug-1998
// demo, create database files
// 28-Jan-2000: make a temp copy of IX4_KEYEXP on stack and pass that
// since Bullet needs to write to the string to remove spaces

#include "bullet_3.h"
#include "blt3demo.h"

// ----------------------------------------------------
// Creates DBF,DBT,IX4 files as described in blt3demo.h
//
// - files are created but are not left open
//

TBLT_RETC DemoCreateFiles(void) {

 TBLT_RETC rc = 0;

 TBLT_DH *dhPtr;
 UINT dhdSize;

 // init field descriptors

 TBLT_FD fList[DBF_FIELDS] = {{DBF_FIELD_01},
                         {DBF_FIELD_02},
                         {DBF_FIELD_03},
                         {DBF_FIELD_04},
                         {DBF_FIELD_05}};

 // delete existing $DEMO_01.* files so can create them

 BltDeleteFile(DBF_FILENAME,0);
 BltDeleteFile(DBT_FILENAME,0);
 BltDeleteFile(IX4_FILENAME,0);

 rc = BltDataCreateFile(DBF_FILEID,
                        DBF_FIELDS,
                        fList,
                        DBF_FILENAME,
                        DBF_MAM,
                        0);

 if (rc == 0) {

    // dhdSize is size of TBLT_DH plus (number of fields-1) * 32

    dhdSize = sizeof(TBLT_DH) + (sizeof(TBLT_FD)*(DBF_FIELDS-1));
    dhPtr = malloc(dhdSize);
    if (dhPtr == 0) {
       rc = 8;
    }
    else {

       // 28-Jan-2000
       // make sure the key expression is in writeable memory
       // since bullet removes spaces from it (a problem when run in the debugger)

       UCHAR keyExp[380];

       strcpy(keyExp, IX4_KEYEXP);


       // to create an index file the DBF file has to first be opened

       memset(dhPtr,0,dhdSize);
       dhPtr->filenamePtr = DBF_FILENAME;
       dhPtr->asMode = DBF_ASMODE;
       dhPtr->noFields = DBF_FIELDS;
       rc = BltDataOpenFile(dhPtr,DBF_OPENMODE);
       if (rc == 0) {

          rc = BltIx4CreateFile(IX4_NODESIZE,
                                IX4_SORTCMPCODE,
                                IX4_CP, IX4_CC, IX4_SORT_TABLE,
                                keyExp, /* 28-Jan-2000, was IX4_KEYEXP */
                                dhPtr,
                                IX4_FILENAME,
                                IX4_PARSER_FNPTR,
                                IX4_MAM,
                                0);

          BltDataCloseFile(dhPtr);
          free(dhPtr);
       }
    }
 }

 return rc;
}


#ifdef TEST_MODULE_CREATE

int main(void) {

 TBLT_RETC rc;

 setbuf(stdout,NULL);

 printf("create.c\n");
 rc = DemoCreateFiles();
 printf("Create Files, rc=%u\n",rc);

 printf("Done.\n");
 return (int)rc;
}
#endif

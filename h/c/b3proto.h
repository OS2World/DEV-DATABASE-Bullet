//
// b3proto.h
// 1-Jan-1999
// Bullet 3 include, protos
// Copyright (C)1999 Cornel Huth        http://40th.com/
//

#if 0
 #define TBLT_IMP __declspec(dllimport)
#else
 #define TBLT_IMP
#endif

TBLT_IMP TBLT_RETC TBLT_ENTRY BltDataInfoFile(TBLT_FNCHAR *filenamePtr, ULONG asMode, TBLT_DBFINFO *dbfInfoPtr, ULONG *noFieldsPtr, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltDataCreateFile(ULONG fileID, ULONG fields, TBLT_FD *fieldListPtr, TBLT_FNCHAR *filenamePtr, ULONG maMode, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltDataOpenFile(TBLT_DH *dhPtr, ULONG mode);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltDataCloseFile(TBLT_DH *dhPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltDataFlushHeader(TBLT_DH *dhPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltDataCopyHeader(TBLT_DH *dhPtr, TBLT_FNCHAR *filenamePtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltDataReadHeader(TBLT_DH *dhPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltDataZapHeader(TBLT_DH *dhPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltDataAddRecord(TBLT_DH *dhPtr, VOID *recordPtr, ULONG *recNoPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltDataDebumpRecord(TBLT_DH *dhPtr, ULONG recNo);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltDataMarkRecord(TBLT_DH *dhPtr, ULONG recNo, ULONG tagChar);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltDataUpdateRecord(TBLT_DH *dhPtr, ULONG recNo, VOID *recordPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltDataGetRecord(TBLT_DH *dhPtr, ULONG recNo, VOID *recordPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltDataSort(TBLT_FNCHAR *indexnamePtr, TBLT_FNCHAR *filenamePtr, TBLT_FNCHAR *newFilenamePtr,TBLT_DATACALLBACKPTR callBackPtr, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltDataPack(TBLT_FNCHAR *filenamePtr, TBLT_FNCHAR *newFilenamePtr,TBLT_DATACALLBACKPTR callBackPtr, TBLT_AUXPACK *apPtr);

#define BltDataDeleteRecord(a,b)    BltDataMarkRecord(a,b,42)
#define BltDataUndeleteRecord(a,b)  BltDataMarkRecord(a,b,32)

TBLT_IMP TBLT_RETC TBLT_ENTRY BltMemoCreateFile(TBLT_DH *dhPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltMemoCloseFile(TBLT_DH *dhPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltMemoOpenFile(TBLT_DH *dhPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltMemoFlushHeader(TBLT_DH *dhPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltMemoReadHeader(TBLT_DH *dhPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltMemoAdd(TBLT_DH *dhPtr, ULONG bytesToSize, ULONG bytesToWrite, VOID *bufferPtr, ULONG *memoNoPtr, ULONG rsvFlag);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltMemoDelete(TBLT_DH *dhPtr, ULONG memoNo);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltMemoGet(TBLT_DH *dhPtr, ULONG memoNo, ULONG startOffset, ULONG *bytesToReadPtr, VOID *bufferPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltMemoGetSize(TBLT_DH *dhPtr, ULONG memoNo, ULONG *availLinkPtr, ULONG *memoSizePtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltMemoUpdate(TBLT_DH *dhPtr, ULONG *memoNoPtr, ULONG startOffset, ULONG bytesToWrite, VOID *bufferPtr);

TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4InfoFile(TBLT_FNCHAR *filenamePtr, ULONG asMode, TBLT_IX4INFO *ix4InfoPtr, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4CreateFile(ULONG nodeSize, ULONG sortCmpCode,
                                      ULONG codePage, ULONG countryCode, UCHAR *sortTablePtr,
                                      VOID *keyExpPtr, TBLT_DH *dhPtr, TBLT_FNCHAR *filenamePtr,
                                      TBLT_IX4PARSEEXPPTR xParseExpPtr,
                                      ULONG maMode, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4OpenFile(TBLT_KH *khPtr, ULONG mode);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4CloseFile(TBLT_KH *khPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4FlushHeader(TBLT_KH *khPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4CopyHeader(TBLT_KH *khPtr, TBLT_FNCHAR *filenamePtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4ReadHeader(TBLT_KH *khPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4ZapHeader(TBLT_KH *khPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4KeyForRecord(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG recNo, VOID *recordPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4DeleteKey(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG recNo);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4StoreKey(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG recNo);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4EqualKey(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG *recNoPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4EqualOrGreaterKey(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG *recNoPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4EqualOrLesserKey(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG *recNoPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4FirstKey(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG *recNoPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4LastKey(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG *recNoPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4NextKey(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG *recNoPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4PrevKey(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG *recNoPtr);

TBLT_IMP TBLT_RETC TBLT_ENTRY BltLockEx(TBLT_LOCKEX *lockExPtr, ULONG *slotPtr, ULONG timeout);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltUnlockEx(TBLT_LOCKEX *lockExPtr, ULONG *slotPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltLockDataHeader(TBLT_DH *dhPtr, ULONG mode, ULONG timeout);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltUnlockDataHeader(TBLT_DH *dhPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltLockDataRecord(TBLT_DH *dhPtr, ULONG mode, ULONG recNoStart, ULONG recCount, ULONG timeout);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltUnlockDataRecord(TBLT_DH *dhPtr, ULONG recNoStart, ULONG recCount);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltLockIx4File(TBLT_KH *khPtr, ULONG mode, ULONG timeout);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltUnlockIx4File(TBLT_KH *khPtr);

TBLT_IMP TBLT_RETVAL TBLT_ENTRY BltGetVar(LONG index, TBLT_RETC *rcPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltSetVar(ULONG *varPtr, LONG index);

TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4Insert(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG *recNoPtr, VOID *recordPtr, ULONG *actionPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4InsertEx(TBLT_IUEX *iuexPtr, ULONG *slotPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4Update(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG recNo, VOID *newRecPtr, ULONG *actionPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4UpdateEx(TBLT_IUEX *iuexPtr, ULONG *slotPtr, ULONG maMode);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4GetFirst(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG *recNoPtr, VOID *recordPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4GetNext(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG *recNoPtr, VOID *recordPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4GetPrev(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG *recNoPtr, VOID *recordPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4GetLast(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG *recNoPtr, VOID *recordPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4GetEqual(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG *recNoPtr, VOID *recordPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4GetEqualOrGreater(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG *recNoPtr, VOID *recordPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIx4GetEqualOrLesser(TBLT_KH *khPtr, VOID *keyBufferPtr, ULONG *recNoPtr, VOID *recordPtr);
TBLT_IMP TBLT_RETC TBLT_CALLBACK BltFuncIx4Reindex(TBLT_KH *khPtr, TBLT_FNCHAR *workFilePtr, ULONG nodePackPct, VOID *keyPtr, ULONG *recNoPtr);
TBLT_IMP TBLT_CMPC TBLT_CALLBACK BltFuncIx4SortCmp(TBLT_KH *khPtr, VOID *arg1ptr, VOID *arg2ptr, UCHAR *hvBufferPtr, ULONG hasRecNo);

TBLT_IMP TBLT_RETC TBLT_ENTRY BltWriteAtFile(TBLT_HANDLE handle, ULONG *bytesPtr, VOID *bufferPtr, TBLT_FO *offsetPtr, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltReadAtFile(TBLT_HANDLE handle, ULONG *bytesPtr, VOID *bufferPtr, TBLT_FO *offsetPtr, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltCloseFile(TBLT_HANDLE *handlePtr, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltCreateDir(TBLT_FNCHAR *dirnamePtr, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltCreateFile(TBLT_FNCHAR *filenamePtr, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltDeleteFile(TBLT_FNCHAR *filenamePtr, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltMoveFile(TBLT_FNCHAR *currNamePtr, TBLT_FNCHAR *newNamePtr, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltLengthFile(TBLT_HANDLE handle, TBLT_FO *lengthPtr, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltOpenFile(TBLT_FNCHAR *filenamePtr, ULONG asMode, TBLT_HANDLE *handlePtr, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltReadFile(TBLT_HANDLE handle, ULONG *bytesPtr, VOID *bufferPtr, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltSeekFile(TBLT_HANDLE handle, ULONG mode, TBLT_FO *posPtr, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltUpdateDirEntry(TBLT_HANDLE handle, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltWriteFile(TBLT_HANDLE handle, ULONG *bytesPtr, VOID *bufferPtr, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltLockFile(TBLT_HANDLE handle, ULONG mode, TBLT_FO *lockOffsetPtr, TBLT_FO *lockBytesPtr, ULONG timeout, TBLT_AUXPACK *apPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltIsRemote(TBLT_HANDLE handle, ULONG drive, ULONG *isRemotePtr, ULONG *flagsPtr, TBLT_AUXPACK *apPtr);

TBLT_IMP TBLT_RETPTR TBLT_ENTRY BltMemAlloc(ULONG mode, ULONG count, TBLT_RETC *rcPtr, ULONG flag0);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltMemFree(ULONG mode, VOID *memHandlePtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltSetHandleCount(ULONG *maxHandlesPtr);

TBLT_IMP TBLT_RETC TBLT_ENTRY BltGetSortSequenceTable(ULONG cpID, ULONG ccID, UCHAR *bufferPtr, ULONG charsetFlag);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltGetCountryInfo(ULONG *cpIDptr, ULONG *ccIDptr, ULONG charsetFlag);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltGetExtendedError(TBLT_RETC *exRCptr, ULONG *xeClassPtr, ULONG *xeActionPtr, ULONG *xeLocusPtr);
TBLT_IMP TBLT_RETVAL TBLT_ENTRY BltGetMemoryAvail(ULONG mode);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltGetVersion(ULONG *verDosPtr);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltUpperCaseO(UCHAR *strgPtr, ULONG maxSize, ULONG flag0);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltCloseMutexSem(TBLT_HANDLE handle);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltCreateMutexSem(UCHAR *namePtr, TBLT_HANDLE *handlePtr, ULONG attrFlags, ULONG stateFlag);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltRequestMutexSem(TBLT_HANDLE handle, ULONG timeout);
TBLT_IMP TBLT_RETC TBLT_ENTRY BltReleaseMutexSem(TBLT_HANDLE handle);

// end b3proto.h

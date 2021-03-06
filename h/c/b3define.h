//
// b3define.h
// 1-Jan-1999
// Bullet 3 include, defines/error codes
// Copyright (C)1999 Cornel Huth        http://40th.com/
//

#define MAX_DBF_FIELDS      1024        // max fields in a DBF
#define MAX_RECORD_LENGTH  65024        // max record length (never more than 65535, recommend max is 1024)
                                        // record length must be LTE bullet_MASZ (max single alloc size)
#define MAX_KEY_LENGTH       196        // max length of a key value, in bytes
#define IX4_MAX_EXP_SIZE     380        // max length of key expression, in bytes (incl. 0T)

#define XACTION_IX4_BIT 1               // TBLT_KH.xactionFlag bit0
#define XACTION_DBF_BIT 1               // TBLT_DH.xactionFlag bit0
#define XACTION_DBT_BIT 2               // TBLT_DH.xactionFlag bit1

#define FLAGS_DIRTY_BIT             1   // TBLT_DH/TBLT_KH.flags, bit0 (R/O)
#define FLAGS_FULL_LOCK_BIT         2   // TBLT_DH/TBLT_KH.flags, bit1 (R/O)
#define FLAGS_FULL_SHARED_LOCK_BIT  4   // TBLT_DH/TBLT_KH.flags, bit2 (R/O)
#define FLAGS_READ_THROUGH_LOCK_BIT 8   // TBLT_DH/TBLT_KH.flags, bit3 (set at open)
#define FLAGS_SKIP_DELETED_BIT      16  // TBLT_DH.flags, bit4 (set at data open)
#define FLAGS_ATOMIC_BIT            16  // TBLT_KH.flags, bit4 (set at ix4 open)
#define FLAGS_HDR_LOCK_BIT          32  // TBLT_DH.flags, bit5 (R/O)
#define FLAGS_HDR_SHARED_LOCK_BIT   64  // TBLT_DH.flags, bit6 (R/O)
#define FLAGS_READONLY_BIT          128 // TBLT_DH/TBLT_KH.flags, bit7 (set at open)

#define FLAGSEX_IU_SKIPDATA         1   // insertEx/updateEx skips data record add/update
#define FLAGSEX_LOCK_DUAL           128 // lockEx has KH handle with dual operation on its xbLink handle

#define SORT_ASCII 1    // sort by: ASCII value  (values 1-255 for low-byte of sortCmpCode)
#define SORT_NLS   2    // NLS                   (up to 16 components allowed but all are to be...
#define SORT_S16   3    // 16-bit signed integer (...of the same sort type unless SORT_MIXED is used)
#define SORT_U16   4    // 16-bit unsigned integer |-------------------------------------------|
#define SORT_S32   5    // 32-bit signed integer   | See the documentation for important notes |
#define SORT_U32   6    // 32-bit unsigned integer | on 64-bit and _MIXED-data requirements    |
#define SORT_S64   7    // 64-bit signed           |-------------------------------------------|
#define SORT_U64   8    // 64-bit unsigned
#define SORT_MIXED 9    // mixed types allowed (an N|F field's key is stored as a DOUBLE in the index)
                        // 10-31 reserved, 32-255 free

#define SORT_MIXED_S16 '3' // 16-bit signed as part of SORT_MIXED
#define SORT_MIXED_U16 '4' // 16-bit unsigned
#define SORT_MIXED_S32 '5' // 32-bit signed     these are special field types...
#define SORT_MIXED_U32 '6' // 32-bit unsigned   ...not sortCmp codes
#define SORT_MIXED_S64 '7' // 64-bit signed
#define SORT_MIXED_U64 '8' // 64-bit unsigned
#define SORT_MIXED_DBL 'Y' // 8-byte floating-point double
                           // the above FIELD TYPES are for -binary- data fields
                           // dBASE standard field types are: C,N,D,L,M
                           // dBASE extended field types are: F,B,G

#define SORT_DUPS_ALLOWED 0x10000       // bit16 of sortCmpCode
#define SORT_USE_ANSI_SET 0x20000       // bit17

#define LOCK_MODE_EXCLUSIVE 0
#define LOCK_MODE_SHARED    1
#define LOCK_MODE_ATOMIC    2
#define LOCK_MODE_SH2EX (LOCK_MODE_EXCLUSIVE | LOCK_MODE_ATOMIC) // 2
#define LOCK_MODE_EX2SH (LOCK_MODE_SHARED | LOCK_MODE_ATOMIC)    // 3


#define BLTVAR_IFS    (-4)   // IFS pointers (R/O) (apPtr as rcPtr)
#define BLTVAR_DLLH   (-3)   // DLL handle (R/O)
#define BLTVAR_MAF    (-2)   // memory frees (R/O)
#define BLTVAR_MAA    (-1)   // memory allocations (R/O)
#define BLTVAR_VERSION  0    // returns pointer to Bullet version string (R/O)
#define BLTVAR_MPNL     1    // default max pathname length
#define BLTVAR_MPCL     2    // default max pathname component length
#define BLTVAR_MPXL     3    // default max pathname extension length
#define BLTVAR_MFSF     4    // max file size fudge to guard against math overflow near .maxFileSize
#define BLTVAR_MARSV    10   // default bytes to hold back from Bullet reindex routine
#define BLTVAR_MASZRX   11   // default max total memory, in bytes, reindex is allowed to allocate
#define BLTVAR_MASZ     12   // default max single allocation request size, in bytes, that will be made
#define BLTVAR_MCBSZ    13   // memo copy buffer size
#define BLTVAR_MCBMAM   14   // memo copy buffer/data sort memory allocation strategy, also data sort/pack alloc mode


#define CRYPTO_FROM_RECGET  1
#define CRYPTO_FROM_RECADD  2
#define CRYPTO_FROM_RECUPD  4
#define CRYPTO_FROM_XRECADD 3
#define CRYPTO_FROM_XRECUPD 5

#define CRYPTO_FROM_KEYGET  16
#define CRYPTO_FROM_KEYADD  32
#define CRYPTO_FROM_KEYADDBULK 160


#define OPENFLAGS_COMMIT       0x400000 // slow file writes (DOS 4+, all)
#define OPENFLAGS_NOCRIT       0x200000 // critical errors passed to caller (DOS 4+, all)
#define OPENFLAGS_NOCACHE      0x100000 // (OS/2)
#define OPENFLAGS_FOURGB       0x100000 // (FAT32)
#define OPENFLAGS_IO_RNDSEQ    0x030000 // random access, with some sequential access (OS/2)
#define OPENFLAGS_IO_RND       0x020000 // mostly random access (OS/2, NT)
#define OPENFLAGS_IO_SEQ       0x010000 // mostly sequential access (OS/2, NT)
                                        // the above values are SHR 8 internally
#define OPENFLAGS_NOINHERIT    0x000080 // child programs do not inherit asMode
#define OPENFLAGS_DENY_NONE    0x000040 // share: do not deny others
#define OPENFLAGS_DENY_READ    0x000030 // share: deny others read
#define OPENFLAGS_DENY_WRITE   0x000020 // share: deny others write
#define OPENFLAGS_DENY_RW      0x000010 // share: deny others read/write
#define OPENFLAGS_ACCESS_READ7 0x000004 // access: read-only, file timestamp not altered (DOS7)
#define OPENFLAGS_ACCESS_RW    0x000002 // access: read/write
#define OPENFLAGS_ACCESS_WRITE 0x000001 // access: write only
#define OPENFLAGS_ACCESS_READ  0x000000 // access: read only

// -------------------------------------------------------------------------------------------------------
// Error codes

#define EXB_FUNCTION_NOT_IMPLEMENTED    1   // function is not implemented
#define EXB_NOT_ENOUGH_MEMORY           8   // cannot get memory requested
#define EXB_INVALID_ACCESS              12  // invalid file access flags
#define EXB_INVALID_PARAMETER           13  // invalid parameter
#define EXB_INVALID_DRIVE               15  // not a valid drive letter
#define EXB_UNEXPECTED_EOF              38  // unexpect EOF (bytes read != bytes asked)
#define EXB_DISK_FULL                   39  // disk full on WriteFile
#define EXB_FILE_EXISTS                 80  // cannot create file since it already exists
#define EXB_SEM_OWNER_DIED              105 // in place of Win32 error 80h (mutex, ll_*)
#define EXB_ATOMIC_LOCK_NOT_SUPPORTED   174 // only OS/2 32-bit supports atomic locks
#define EXB_READ_LOCKS_NOT_SUPPORTED    175 // OS/2 and WinNT support read (shared, non-exclusive) locks
#define EXB_TIMEOUT                     640 // in place of Win32 error 102h (mutex, ll_*)

// Other operating system errors are as returned by OS itself

// System/general error codes

#define WRN_USING_ST850         8295 // code page info could not be loaded, using default cp=850 (DOS, create ix4)
#define WRN_USING_ST1252        8296 // cp info could not be loaded, using default cp=1252 (DOS, create ix4)
#define EXB_ILLEGAL_CMD         8300 // function not allowed
#define EXB_SYSTEM_HANDLE       8305 // Bullet won't use or close handles 0-2
#define EXB_FILE_NOT_OPEN       8306 // file not open (not Bullet handle, including xbLink)
#define EXB_FILE_IS_DIRTY       8307 // tried to reload/zap header but current still dirty
#define EXB_BAD_FILETYPE        8308 // tried key op on non-key file, data op on non...
#define EXB_EXPIRED             8313 // evaluation time period has expired
#define EXB_BAD_INDEX           8314 // BltGetVer/SetVar index is not valid
#define EXB_FILE_BOUNDS         8316 // file size GT max file size (or GT half that if read-through locks)
#define EXB_READONLY            8317 // handle has read-only flag active and attempted to write to it
#define WRN_INCOMPLETE_XACTION  8318 // header xactionFlag indicates an incomplete transaction (IX4|DBF|DBT)
#define EXB_FILE_ALREADY_OPEN   8319 // .handle is non-zero so already has an open file handle

#define EXB_BAD_ALLOC_SIZE      8351 // requested allocation size is more than bullet_MASZ, or is 0
#define EXB_IO_WRAP_DETECTED    8352 // file I/O for bytes to/from memory address would cause segment wrap

#define EXB_NULLFNPTR           8395 // null function pointer detected (without default function available)
#define EXB_NULLPTR             8396 // general null pointer detected
#define EXB_INVALID_DLL         8398 // DLL/LIB is not valid

// Multi-access error codes

#define EXB_BAD_LOCK_MODE       8401 // lock mode not valid
#define EXB_NOTHING_TO_UNLOCK   8404 // no outstanding full locks
#define EXB_ALREADY_FULL_LOCK   8405 // already have a full lock so cannot record lock/header lock
#define EXB_ALREADY_HDR_LOCK    8406 // already have a hdr lock so cannot full lock
#define EXB_DIFFERENT_LOCK_MODE 8407 // current lock does not match new lock mode (shared/exclusive)
#define EXB_SAME_LOCK_MODE      8408 // current lock same as new lock mode (relock)

// Index error codes

#define EXB_KEY_NOT_FOUND       8501 // exact match of key not found (also, recNo not same, if specified)
#define EXB_KEY_EXISTS          8502 // key exists already and dups not allowed (or key+recno exists w/dups)
#define EXB_END_OF_FILE         8503 // already at last index order
#define EXB_TOP_OF_FILE         8504 // already at first index order
#define EXB_EMPTY_FILE          8505 // nothing to do since no keys
#define EXB_BAD_INDEX_STACK     8507 // index file is corrupt (stack over/underflow)
#define EXB_BAD_INDEX_READ0     8508 // index file is corrupt (read attempt of node 0)
#define EXB_BAD_INDEX_WRITE0    8509 // index file is corrupt (write attempt to node 0)
#define EXB_SEEK_PREV_FAILED    8510 // seek prev failed
#define EXB_BAD_INDEX_ROOTNODE  8511 // index file is corrupt (root node cannot = 0)
#define EXB_BAD_INDEX_KEYCOUNT  8512 // index file is corrupt (key count GT max keys on node)
#define EXB_BAD_INDEX_NOTAVAIL  8513 // index file is corrupt (avail node is not so marked)
#define EXB_RECNO_IS_ZERO       8514 // cannot delete a key when passed a recNo of 0

#define EXB_UNKNOWN_INDEX       8522 // not a Bullet IX4 index file
#define EXB_KEY_TOO_LONG        8523 // keylength GT MAX_KEY_LENGTH, or is 0
#define EXB_BAD_XLATE_INFO      8524 // build key xlate.info is invalid
#define EXB_SORT_STACK_OV       8525 // sort stack exceeded max depth

#define EXB_BAD_NODE_SIZE       8534 // nodeSize is not 512,1024,1536,2048, or 2560 (must be specified at open)
#define EXB_FILENAME_TOO_LONG   8535 // filename GT max path length (or null ptr)
#define EXB_BAD_SORT_CMP_CODE   8538 // sortCmp code (among those less than 32) is not valid
#define EXP_REQUIRES_FPU        8539 // converting text to floating-pointing double requires FPU support

#define EXB_KEYX_NULL           8541 // expression is effectively NULL
#define EXB_KEYX_TOO_LONG       8542 // TBLT_KH.expression GT IX4_MAX_EXP_SIZE bytes
#define EXB_KEYX_SYM_TOO_LONG   8543 // fieldname/funcname in expression GT 10 single-byte OEM chars
#define EXB_KEYX_SYM_UNKNOWN    8544 // fieldname/funcname in expression unknown
#define EXB_KEYX_TOO_MANY_SYMS  8545 // too many symbols/fields used in expression
#define EXB_KEYX_BAD_SUBSTR     8546 // invalid SUBSTR() operand in expression
#define EXB_KEYX_BAD_SUBSTR_SZ  8547 // SUBSTR() exceeds field's size
#define EXB_KEYX_BAD_FORM       8548 // didn't match expected symbol in expression or bad nesting
#define EXB_KEYX_BAD_TYPE       8549 // field type not compatible with sortCmp mode

#define EXB_NO_READS_FOR_RUN    8551 // unlikely, use different reindex buffer size
#define EXB_TOO_MANY_RUNS       8552 // unlikely, too many reindex runs (more than 65535 runs (8128 for 16-bit builds))
#define EXB_TOO_MANY_RUNS_FOR_BUFFER 8553 // unlikely, too many reindex runs for run buffer (varies by buffer size, key size)

// Data error codes

#define EXB_EXT_XBLINK          8601 // xbLink handle is not internal DBF (xbLinkPtr is -1)
#define EXB_RECORD_TOO_LONG     8603 // record length is GT 64K or GT bullet_MASZ
#define EXB_BAD_FIELD_COUNT     8605 // fields LTE 0 or GTE MAX_FIELDS (Init,Open)
#define EXB_BAD_HEADER          8606 // bad header
#define EXB_BAD_RECNO           8609 // recNo=0, or GT recs in data file hdr, or not last recNo for debump
#define WRN_RECORD_EXCLUDE      8610 // record's tag byte matches exclude tag or does not match include tag
#define EXB_EMPTY_FIELDLEN      8611 // field length is 0
#define EXB_NO_RECORDS          8612 // no data records

// Memo error codes

#define WRN_CANNOT_OPEN_MEMO    8701 // DBF says memo file but memo open fails
#define EXB_BAD_BLOCKSIZE       8703 // memo blocksize must be at least 24 bytes
#define EXB_MEMO_DELETED        8704 // memo is deleted
#define EXB_MEMO_PAST_END       8705 // memo data requested is past end of record
#define EXB_BAD_MEMONO          8706 // memo number is not valid
#define EXB_MEMO_IN_USE         8707 // memo add encountered likely corrupt memo file
#define EXB_BAD_AVAIL_LINK      8708 // memo avail link (in memoAvailBlock) cannot be valid (is 0)
#define EXB_MEMO_ZERO_SIZE      8709 // memo data has no size or bytes to size LT bytes to write
#define EXB_MEMO_IS_SMALLER     8710 // memo attempt to shrink but already LTE size
#define EXB_TOO_MANY_MEMOS      8712 // memo pack, record has more than MAX_MEMO_FIELDS

// end b3define.h

Option Explicit
'
'  b3vbwin.bas
'  1999-Oct-4
'  Bullet 3.0 Visual Basic Win32 include (defines, types, declares)
'  Copyright (C)1999 Cornel Huth        http://40th.com/
'
' - *** See VB Support section at end for VB-specific stuff ***
'
' NOTE: Trial version uses ... Lib "b3w32d30" ... (d for demo)
'              rather than ... Lib "b3w32_30" ... (use find/replace if needed)
'

Public Const B_MAX_FIELDS = 32               ' hard-code DH.fd[] size
Public Const B_NODE_SIZE = 512               ' hard-code KH.nodeSize, see KH structure (change to max needed)

Public Const MAX_DBF_FIELDS = 1024           ' max fields in a DBF
Public Const MAX_RECORD_LENGTH = 65024       ' max record length (never more than 65535, recommend max is 1024)
                                             ' record length must be LTE bullet_MASZ (max single alloc size)
Public Const MAX_KEY_LENGTH = 196            ' max length of a key value, in bytes
Public Const IX4_MAX_EXP_SIZE = 380          ' max length of key expression, in bytes (incl. 0T)

Public Const XACTION_IX4_BIT = 1             ' BLTKH.xactionFlag bit0
Public Const XACTION_DBF_BIT = 1             ' BLTDH.xactionFlag bit0
Public Const XACTION_DBT_BIT = 2             ' BLTDH.xactionFlag bit1

Public Const FLAGS_DIRTY_BIT = 1             ' BLTDH/BLTKH.flags, bit0 (R/O)
Public Const FLAGS_FULL_LOCK_BIT = 2         ' BLTDH/BLTKH.flags, bit1 (R/O)
Public Const FLAGS_FULL_SHARED_LOCK_BIT = 4  ' BLTDH/BLTKH.flags, bit2 (R/O)
Public Const FLAGS_READ_THROUGH_LOCK_BIT = 8 ' BLTDH/BLTKH.flags, bit3 (set at open)
Public Const FLAGS_SKIP_DELETED_BIT = 16     ' BLTDH.flags, bit4 (set at data open)
Public Const FLAGS_ATOMIC_BIT = 16           ' BLTKH.flags, bit4 (set at ix4 open)
Public Const FLAGS_HDR_LOCK_BIT = 32         ' BLTDH.flags, bit5 (R/O)
Public Const FLAGS_HDR_SHARED_LOCK_BIT = 64  ' BLTDH.flags, bit6 (R/O)
Public Const FLAGS_READONLY_BIT = 128        ' BLTDH/BLTKH.flags, bit7 (set at open)

Public Const FLAGSEX_IU_SKIPDATA = 1         ' insertEx/updateEx skips data record add/update
Public Const FLAGSEX_LOCK_DUAL = 128         ' lockEx has KH handle with dual operation on its xbLink handle


Public Const SORT_ASCII = 1  ' sort by: ASCII value  (values 1-255 for low-byte of sortCmpCode)
Public Const SORT_NLS = 2    ' NLS                   (up to 16 components allowed but all are to be...
Public Const SORT_S16 = 3    ' 16-bit signed integer (...of the same sort type unless SORT_MIXED is used)
Public Const SORT_U16 = 4    ' 16-bit unsigned integer |-------------------------------------------|
Public Const SORT_S32 = 5    ' 32-bit signed integer   | See the documentation for important notes |
Public Const SORT_U32 = 6    ' 32-bit unsigned integer | on 64-bit and _MIXED-data requirements    |
Public Const SORT_S64 = 7    ' 64-bit signed           |-------------------------------------------|
Public Const SORT_U64 = 8    ' 64-bit unsigned
Public Const SORT_MIXED = 9  ' mixed types allowed (an N|F field's key is stored as a DOUBLE in the index)
                      ' 10-31 reserved, 32-255 free

' sortable instrinic extended field types (not including standard types, like 'C', 'D', etc.)

Public Const FTYPE_S16 = 51  ' 16-bit signed as part of FTYPE                        '3'
Public Const FTYPE_U16 = 52  ' 16-bit unsigned                                       '4'
Public Const FTYPE_S32 = 53  ' 32-bit signed                                         '5'
Public Const FTYPE_U32 = 54  ' 32-bit unsigned                                       '6'
Public Const FTYPE_S64 = 55  ' 64-bit signed                                         '7'
Public Const FTYPE_U64 = 56  ' 64-bit unsigned                                       '8'
Public Const FTYPE_DBL = 89  ' 8-byte floating-point double                          'Y'

Public Const FTYPE_C = 67    ' std C-haracter field type (any size: is not necessarily a C string)
Public Const FTYPE_D = 68    ' std D-ate field type (8 bytes: YYYYMMDD)
Public Const FTYPE_L = 76    ' std L-ogical field type (1 byte: blank, 'Y|y|T|t', 'N|n|F|f')
Public Const FTYPE_N = 78    ' std N-umeric field type (1 to 19 bytes, text, as in " 123.56")
Public Const FTYPE_M = 77    ' std M-emo field type (10 bytes: text record number of DBT block)
Public Const FTYPE_B = 66    ' x-std, same as FTYPE_M, but typically used to indicate DBT stores binary data
Public Const FTYPE_G = 71    ' x-std, same as FTYPE_M, but typically used to indicate DBT stores OLE data
Public Const FTYPE_F = 70    ' x-std, same as FTYPE_N

'the following are obsolete (_S16 to _DBL, use FTYPE_* above instead)
Public Const SORT_MIXED_S16 = 51 ' 16-bit signed as part of SORT_MIXED ('3')
Public Const SORT_MIXED_U16 = 52 ' 16-bit unsigned                     ('4')
Public Const SORT_MIXED_S32 = 53 ' 32-bit signed     these are special field types... ('5')
Public Const SORT_MIXED_U32 = 54 ' 32-bit unsigned   ...not sortCmp codes             ('6')
Public Const SORT_MIXED_S64 = 55 ' 64-bit signed                       ('7')
Public Const SORT_MIXED_U64 = 56 ' 64-bit unsigned                     ('8')
Public Const SORT_MIXED_DBL = 89 ' 8-byte floating-point double        ('Y')
                         ' the above FIELD TYPES are for -binary- data fields
                         ' dBASE standard field types are: C,N,D,L,M
                         ' dBASE extended field types are: F,B,G


Public Const SORT_DUPS_ALLOWED = &H10000  ' bit16 of sortCmpCode (LONG)
Public Const SORT_USE_ANSI_SET = &H20000  ' bit17                (LONG)

Public Const LOCK_MODE_EXCLUSIVE = 0
Public Const LOCK_MODE_SHARED = 1
Public Const LOCK_MODE_ATOMIC = 2
Public Const LOCK_MODE_SH2EX = (LOCK_MODE_EXCLUSIVE Or LOCK_MODE_ATOMIC) ' 2
Public Const LOCK_MODE_EX2SH = (LOCK_MODE_SHARED Or LOCK_MODE_ATOMIC)  ' 3


Public Const BLTVAR_IFS = -4       ' IFS pointers (R/O)
Public Const BLTVAR_DLLH = -3      ' HW=ds, LW=sel is environment (R/O)
Public Const BLTVAR_MAF = -2       ' memory frees (R/O)
Public Const BLTVAR_MAA = -1       ' memory allocations (R/O)
Public Const BLTVAR_VERSION = 0    ' returns pointer to Bullet version string
Public Const BLTVAR_MPNL = 1       ' default max pathname length
Public Const BLTVAR_MPCL = 2       ' default max pathname component length
Public Const BLTVAR_MPXL = 3       ' default max pathname extension length
Public Const BLTVAR_MFSF = 4       ' max file size fudge to guard against math overflow near .maxFileSize
Public Const BLTVAR_MARSV = 10     ' default bytes to hold back from Bullet reindex routine
Public Const BLTVAR_MASZRX = 11    ' default max total memory, in bytes, reindex is allowed to allocate
Public Const BLTVAR_MASZ = 12      ' default max single allocation request size, in bytes, that will be made
Public Const BLTVAR_MCBSZ = 13     ' memo copy buffer size
Public Const BLTVAR_MCBMAM = 14    ' memo copy buffer/data sort memory allocation strategy

Public Const CRYPTO_FROM_RECGET = 1
Public Const CRYPTO_FROM_RECADD = 2
Public Const CRYPTO_FROM_RECUPD = 4
Public Const CRYPTO_FROM_XRECADD = 3
Public Const CRYPTO_FROM_XRECUPD = 5

Public Const CRYPTO_FROM_KEYGET = 16
Public Const CRYPTO_FROM_KEYADD = 32
Public Const CRYPTO_FROM_KEYADDBULK = 160

Public Const STRTYPE_CHAR = 0                   ' uses _A RTL calls (not used in 3.0)

Public Const OPENFLAGS_COMMIT = &H400000        ' slow file writes (DOS 4+, all)
Public Const OPENFLAGS_NOCRIT = &H200000        ' critical errors passed to caller (DOS 4+, all)
Public Const OPENFLAGS_NOCACHE = &H100000       ' (OS/2)
Public Const OPENFLAGS_FOURGB = &H100000        ' (FAT32)
Public Const OPENFLAGS_IO_RNDSEQ = &H30000      ' random access, with some sequential access (OS/2)
Public Const OPENFLAGS_IO_RND = &H20000         ' mostly random access (OS/2)
Public Const OPENFLAGS_IO_SEQ = &H10000         ' mostly sequential access (OS/2)
                                                ' the above values are SHR 8 internally
Public Const OPENFLAGS_NOINHERIT = &H80         ' child programs do not inherit asMode
Public Const OPENFLAGS_DENY_NONE = &H40         ' share: do not deny others
Public Const OPENFLAGS_DENY_READ = &H30         ' share: deny others read
Public Const OPENFLAGS_DENY_WRITE = &H20        ' share: deny others write
Public Const OPENFLAGS_DENY_RW = &H10           ' share: deny others read/write
Public Const OPENFLAGS_ACCESS_READ7 = &H4       ' access: read-only, file timestamp not altered (DOS7)
Public Const OPENFLAGS_ACCESS_RW = &H2          ' access: read/write
Public Const OPENFLAGS_ACCESS_WRITE = &H1       ' access: write only
Public Const OPENFLAGS_ACCESS_READ = &H0        ' access: read only

' -------------------------------------------------------------------------------------------------------
' Error codes

Public Const EXB_FUNCTION_NOT_IMPLEMENTED = 1    ' function is not implemented
Public Const EXB_NOT_ENOUGH_MEMORY = 8           ' cannot get memory requested
Public Const EXB_INVALID_ACCESS = 12             ' invalid file access flags
Public Const EXB_INVALID_PARAMETER = 13          ' parameter is not valid
Public Const EXB_INVALID_DRIVE = 15              ' not a valid drive letter
Public Const EXB_UNEXPECTED_EOF = 38             ' unexpect EOF (bytes read != bytes asked)
Public Const EXB_DISK_FULL = 39                  ' disk full on WriteFile
Public Const EXB_FILE_EXISTS = 80                ' cannot create file since it already exists
Public Const EXB_SEM_OWNER_DIED = 105            ' in place of Win32 error 80h (mutex)
Public Const EXB_ATOMIC_LOCK_NOT_SUPPORTED = 174 ' only OS/2 32-bit supports atomic locks
Public Const EXB_READ_LOCKS_NOT_SUPPORTED = 175  ' OS/2 and WinNT support read (shared, non-exclusive) locks
Public Const EXB_TIMEOUT = 640                   ' in place of Win32 error 102h (mutex)

' Other operating system errors are as returned by OS itself

' System/general error codes

Public Const WRN_USING_ST850 = 8295       ' code page info could not be loaded, using default cp=850 (DOS, create ix4)
Public Const WRN_USING_ST1252 = 8296      ' cp info could not be loaded, using default cp=1252 (DOS, create ix4)
Public Const EXB_ILLEGAL_CMD = 8300       ' function not allowed
Public Const EXB_SYSTEM_HANDLE = 8305     ' Bullet won't use or close handles 0-2
Public Const EXB_FILE_NOT_OPEN = 8306     ' file not open (not Bullet handle, including xbLink)
Public Const EXB_FILE_IS_DIRTY = 8307     ' tried to reload/zap header but current still dirty
Public Const EXB_BAD_FILETYPE = 8308      ' tried key op on non-key file, data op on non...
Public Const EXB_EXPIRED = 8313           ' evaluation time period has expired
Public Const EXB_BAD_INDEX = 8314         ' BltGetVer/SetVar index is not valid
Public Const EXB_FILE_BOUNDS = 8316       ' file size GT max file size (or GT half that if read-through locks)
Public Const EXB_READONLY = 8317          ' handle has read-only flag active and attempted to write to it
Public Const WRN_INCOMPLETE_XACTION = 8318 ' header xactionFlag indicates an incomplete transaction (IX4|DBF|DBT)
Public Const EXB_FILE_ALREADY_OPEN = 8319 ' .handle is non-zero so already has an open file handle
Public Const EXB_NO_XBLINK = 8320         ' .xbLinkPtr is 0

Public Const EXB_BAD_ALLOC_SIZE = 8351    ' requested allocation size is more than bullet.MASZ, or is 0

Public Const EXB_NULLFNPTR = 8395         ' null function pointer detected (without default function available)
Public Const EXB_NULLPTR = 8396           ' general null pointer detected
Public Const EXB_FORCE_ROLLBACK = 8397    ' TBLT_IUEX.nextPtr == -1
Public Const EXB_INVALID_DLL = 8398       ' DLL/LIB is not valid

' Multi-access error codes

Public Const EXB_BAD_LOCK_MODE = 8401     ' lock mode not valid
Public Const EXB_NOTHING_TO_UNLOCK = 8404 ' no outstanding full locks
Public Const EXB_ALREADY_FULL_LOCK = 8405 ' already have a full lock so cannot record lock/header lock
Public Const EXB_ALREADY_HDR_LOCK = 8406  ' already have a hdr lock so cannot full lock
Public Const EXB_DIFFERENT_LOCK_MODE = 8407 ' current lock does not match new lock mode (shared/exclusive)
Public Const EXB_SAME_LOCK_MODE = 8408    ' current lock same as new lock mode (relock)

' Index error codes

Public Const EXB_KEY_NOT_FOUND = 8501     ' exact match of key not found (also, recNo not same, if specified)
Public Const EXB_KEY_EXISTS = 8502        ' key exists already and dups not allowed (or key+recNo exists w/dups)
Public Const EXB_END_OF_FILE = 8503       ' already at last index order
Public Const EXB_TOP_OF_FILE = 8504       ' already at first index order
Public Const EXB_EMPTY_FILE = 8505        ' nothing to do since no keys
Public Const EXB_BAD_INDEX_STACK = 8507   ' index file is corrupt (stack over/underflow)
Public Const EXB_BAD_INDEX_READ0 = 8508   ' index file is corrupt (read attempt of node 0)
Public Const EXB_BAD_INDEX_WRITE0 = 8509  ' index file is corrupt (write attempt to node 0)
Public Const EXB_SEEK_PREV_FAILED = 8510  ' seek prev failed
Public Const EXB_BAD_INDEX_ROOTNODE = 8511 ' index file is corrupt (root node cannot = 0)
Public Const EXB_BAD_INDEX_KEYCOUNT = 8512 ' index file is corrupt (key count GT max keys on node)
Public Const EXB_BAD_INDEX_NOTAVAIL = 8513 ' index file is corrupt (avail node is not so marked)
Public Const EXB_recNo_IS_ZERO = 8514     ' cannot delete a key when passed a recNo of 0

Public Const EXB_UNKNOWN_INDEX = 8522     ' not a Bullet IX4 index file
Public Const EXB_KEY_TOO_LONG = 8523      ' keylength GT MAX_KEY_LENGTH, or is 0
Public Const EXB_BAD_XLATE_INFO = 8524    ' build key xlate.info is invalid
Public Const EXB_SORT_STACK_OV = 8525     ' sort stack exceeded max depth

Public Const EXB_BAD_NODE_SIZE = 8534     ' nodeSize is not 512,1024,1536,2048,2560 (must be specified at open)
Public Const EXB_FILENAME_TOO_LONG = 8535 ' filename GT max path length (or null ptr)
Public Const EXB_BAD_SORT_CMP_CODE = 8538 ' sortCmp code (among those less than 20) is not valid
Public Const EXP_REQUIRES_FPU = 8539      ' converting text to floating-pointing double requires FPU support

Public Const EXB_KEYX_NULL = 8541         ' expression is effectively NULL
Public Const EXB_KEYX_TOO_LONG = 8542     ' KH.expression GT 380 bytes
Public Const EXB_KEYX_SYM_TOO_LONG = 8543 ' fieldname/funcname in expression GT 10 single-byte OEM chars
Public Const EXB_KEYX_SYM_UNKNOWN = 8544  ' fieldname/funcname in expression unknown
Public Const EXB_KEYX_TOO_MANY_SYMS = 8545 ' too many symbols/fields used in expression
Public Const EXB_KEYX_BAD_SUBSTR = 8546   ' invalid SUBSTR() operand in expression
Public Const EXB_KEYX_BAD_SUBSTR_SZ = 8547 ' SUBSTR() exceeds field's size
Public Const EXB_KEYX_BAD_FORM = 8548     ' didn't match expected symbol in expression or bad nesting
Public Const EXB_KEYX_BAD_TYPE = 8549     ' field type not compatible with sortCmp mode

Public Const EXB_NO_READS_FOR_RUN = 8551  ' unlikely, use different reindex buffer size
Public Const EXB_TOO_MANY_RUNS = 8552     ' unlikely, too many reindex runs (more than 65535 runs (8128 for 16-bit builds))
Public Const EXB_TOO_MANY_RUNS_FOR_BUFFER = 8553 ' unlikely, too many reindex runs for run buffer (varies by buffer size, key size)

Public Const WRN_NOTHING_TO_UPDATE = 8571 ' accumulated UpdateEx record lengths is 0
Public Const EXB_UPDATE_TOO_LARGE = 8576  ' accumulated UpdateEx record lengths is GT bullet_MASZ
Public Const EXB_UPDATE_EMPTY_START = 8577 ' UpdateEx cannot skip the update for the first slot in the pack

' Data error codes

Public Const EXB_EXT_XBLINK = 8601        ' xbLink handle is not internal DBF (xbLinkPtr is -1)
Public Const EXB_RECORD_TOO_LONG = 8603   ' record length is GT 64K or GT bullet.MASZ
Public Const EXB_BAD_FIELD_COUNT = 8605   ' fields LTE 0 or GT MAX_FIELDS (Init,Open)
Public Const EXB_BAD_HEADER = 8606        ' bad header
Public Const EXB_BAD_recNo = 8609         ' recNo=0, or GT recs in data file hdr, or not last recNo for debump
Public Const WRN_RECORD_EXCLUDE = 8610    ' record's tag byte matches exclude tag or does not match include tag
Public Const EXB_EMPTY_FIELDLEN = 8611    ' field length is 0
Public Const EXB_NO_RECORDS = 8612        ' no data records

' Memo error codes

Public Const WRN_CANNOT_OPEN_MEMO = 8701  ' DBF says memo file but memo open fails
Public Const EXB_BAD_BLOCKSIZE = 8703     ' memo blocksize must be at least 24 bytes
Public Const EXB_MEMO_DELETED = 8704      ' memo is deleted
Public Const EXB_MEMO_PAST_END = 8705     ' memo data requested is past end of record
Public Const EXB_BAD_MEMONO = 8706        ' memo number is not valid
Public Const EXB_MEMO_IN_USE = 8707       ' memo add encountered likely corrupt memo file
Public Const EXB_BAD_AVAIL_LINK = 8708    ' memo avail link (in memoAvailBlock) cannot be valid (is 0)
Public Const EXB_MEMO_ZERO_SIZE = 8709    ' memo data has no size
Public Const EXB_MEMO_IS_SMALLER = 8710   ' memo attempt to shrink but already LTE size
Public Const EXB_MEMO_TOO_LARGE = 8711    ' memo has more than 65535 blocks (16-bit build) or GT 2GB
Public Const EXB_TOO_MANY_MEMOS = 8712    ' memo pack, record has more than MAX_MEMO_FIELDS


' -------------------------------
' DBF header info
' first 32 bytes of DBF file

Type BLTDBFINFO
 fileID         As Byte         ' 000 file id byte (this is the first 32 bytes of a DBF file)
 lastUpdateYR   As Byte         ' 001 binary
 lastUpdateMO   As Byte         ' 002 binary
 lastUpdateDA   As Byte         ' 003 binary
 noRecords      As Long         ' 004 total number of records
 headerLength   As Integer      ' 008 length of data header
 recordLength   As Integer      ' 010 record length
 offsetFirstRec As Integer      ' 012 offset to first record if align512 checks, otherwise not used/0
 xactionFlag    As Byte         ' 014 flag indicating incomplete dBase transaction
 encryptFlag    As Byte         ' 015 flag indicating encrypted
 rsv16          As Long         ' 016 rsv
 rsv20          As Long         ' 020 rsv
 rsv24          As Long         ' 024 rsv
 filler28       As Long         ' 028 fill to 32 bytes
End Type                        '  32 bytes


Type BLTFD                      ' field descriptor
 fieldName(1 To 11) As Byte     ' 000 ASCII/OEM, UPPER, underscore, zero-filled, (0T)
 fieldType      As Byte         ' 011 UPPER characters: C,N,D,L,M (F,G,B) [Y,3,4,5,6,7,8]
 fieldDA        As Long         ' 012 field start offset (delete tag=0, fld1=1, fld2=fld1+fld1's size..., non-std)
 fieldLength    As Byte         ' 016 field length
 fieldDC        As Byte         ' 017 decimal count (high byte of length if fieldType=C)
 rez16          As Integer      ' 018 (obsolete)
 filler20       As Long         ' 020 not used
 filler24       As Long
 filler28       As Long
End Type                        '  32 bytes


Type BLTDH                      ' Data file structure
 fType          As Long         ' 000  bit0=1 for DBF data file, XB
 flags          As Long         ' 004  bit0=1 is dirty
                                '      bit1=1 full lock (count stored in DHD.lockCount)
                                '      bit2=1 full shared lock (if bit1=1)
                                '      bit3=1 read-through lock (maxFileSize effectively cut in half)
                                '      bit4=1 skip over deleted records in BltIx4Get*() routines
                                '      bit5=1 header is locked (count stored in DHD.hdrLockCount)
                                '      bit6=1 header shared locked (if bit5=1)
                                '      bit7=1 handle is read-only
                                '      bit8-31 reserved (=0)
 handle         As Long         ' 008  file handle
 asMode         As Long         ' 012  access-sharing mode of open
 noFields       As Long         ' 016  number of fields in this data file
 filenamePtr    As Long         ' 020  filename at open, PSZ
 mutexHandle    As Long         ' 024  mutex handle
                                '
                                '      memo file header
 memoAvailBlock As Long         ' 028  next available block (header is block 0)
 memoUnk1       As Long         ' 032  not used
 memoFilename(1 To 8) As Byte   ' 036  filename proper (first 8 of filename proper)
 memoUnk2       As Long         ' 044  not used (apparently)
 memoBlockSize  As Long         ' 048  block size, must be at least 24 to cover header! and less than 64K
                                '
                                '      memo file control
 memoHandle     As Long         ' 052  handle of open memo file
 memoFlags      As Long         ' 056  bit0=1 is dirty
 memoLastNo     As Long         ' 060  last accessed memo number (if not 0)
 memoLastLink   As Long         ' 064  link data for last accessed memo
 memoLastSize   As Long         ' 068  size of last accessed memo (in bytes, w/OH)
 memoHeaderSize As Long         ' 072  size of first block (dBASE same as all, FP just 24, or?, if 0 uses memoBlockSize)
 memoExt(1 To 4) As Byte        ' 076  extension to use for memo file (def='DBT\0')
 memoInUseFlag  As Long         ' 080  default flag value is 0x8FFFF (0x8FFFF used if memoInUseFlag=0)
                                '
 lockCount      As Long         ' 084  only when dec'ed to 0 do full unlock
 hdrLockCount   As Long         ' 088  only when dec'ed to 0 do header unlock
 maxFileSize    As Long         ' 092  see also DHD.flags, bit3
 maxFileSizeH   As Long         ' 096  not used: placeholder when T_BLTFO is 32 bits
 packPct        As Long         ' 100  percent of sort|pack done (0,1-99)
 userWord       As Long         ' 104  user word (available for programmer's use)
                                '
                                '      data function pointers
 xCallBackPtr   As Long         ' 108  xCallBackPtr is callback routine for pack (def=0:none)
 xCryptoPtr     As Long         ' 112  xCryptoPtr is callback routine for crypto (def=0:none)
 rsv116         As Long         ' 116  reserved
 rsv120         As Long         ' 120  reserved
 rsv124         As Long         ' 124  reserved
                                '
                                '      state data:
 MPNL           As Long         ' 128  max pathname length (def=0, use bullet_MPNL)
 MPCL           As Long         ' 132  max pathname component length (def=0, use bullet_MPCL)
 MPXL           As Long         ' 136  max pathname extension length (def=0, use bullet_MPXL)
 MASZ           As Long         ' 140  max single allocation request size, in bytes, that will be made (def=0, use bullet_MASZ)
 MAM            As Long         ' 144  memory allocation strategy (0=default for platform)
 LOMS           As Long         ' 148  flag to leave old memo space (1=leave old space as-is, 0=clear it)
 fdPtr          As Long         ' 152  (not available in 3.0) pointer to field descriptors (or if null, fd[] is used)
 apPtr          As Long         ' 156  auxiliary pack ptr (def=0)
                                ' 160 plus:
                                '      -------------------------------------------
                                '      data file header (actually written to disk)
 fileID         As Byte         ' 000  file id byte
 lastUpdateYR   As Byte         ' 001  binary
 lastUpdateMO   As Byte         ' 002  binary
 lastUpdateDA   As Byte         ' 003  binary
 noRecords      As Long         ' 004  total number of records
 headerLength   As Integer      ' 008  length of data header
 recordLength   As Integer      ' 010  record length (includes tag byte)
 nada           As Integer      ' 012  reserved
 xactionFlag    As Byte         ' 014  flag indicating incomplete dBASE transaction (bit0=1 DBF, bit1=1 DBT)
 encryptFlag    As Byte         ' 015  flag indicating encrypted (not modified by Bullet)
 autoinc        As Long         ' 016  rsv
 checksum       As Long         ' 020  rsv
 rsv24          As Long         ' 024  rsv
 filler28       As Long         ' 028  fill to 32 bytes
 fd(1 To B_MAX_FIELDS) As BLTFD ' 032  adjust fd() as needed (set to max fields you will allow)
                                '      use multiple TYPEs if you need many different-sized fd[]s
End Type                        ' workspace (160) + header proper (32) + descriptors (32 each) bytes


'-------------------------------------------------------------------------------------------------


' ---------------
' IX4 header info

Type BLTIX4INFO
 fileID         As Long         ' 000  file id  (this is the first 44 bytes of an IX4 file)
 nodeSize       As Long         ' 004  size of a node, in bytes (buffer[], at end of KH, must be this size)
 rootNode       As Long         ' 008  root node (1-based)
 noKeys         As Long         ' 012  total number of keys
 availNode      As Long         ' 016  next available node (link to, 0 if none, 1-based)
 freeNode       As Long         ' 020  next free node (always points to last physical node+1, at end of file)
 keyLength      As Long         ' 024  length of key
 maxKeys        As Long         ' 028  maximum number of keys on a node: (nodeSize-5)/(keyLen+8) (255=absolute max)
 codePage       As Long         ' 032  code page from Ix4CreateFile
 countryCode    As Long         ' 036  country code from Ix4CreateFile
 sortCmpCode    As Long         ' 040  (see KH)
End Type                        '  44 bytes


Type BLTXLATEX                  ' (informational use only)
 fType          As Byte         ' 000 field type (see below)
 length         As Byte         ' 001 bytes to use starting at offset (never GT MAX_KEY_LENGTH)
 offset         As Integer      ' 002 byte offset into data record that length bytes are used
 flags          As Integer      ' 004 flags (bit0=1 upper, bit1=1 descend)
End Type                        '   6 bytes (one xlatex per key component)


Type BLTIX4ACCESS
 day            As Byte
 month          As Byte
 year           As Integer
 counter        As Long
End Type                        ' 8 bytes


' ---------
' KeyHeader
' Composed of: workspace(512)+header(1024)+node buffer(512,1024,1536,2048,2560)=2KB to 4KB KH size

Type BLTKH                      '
 fType          As Long         ' 000  bit0=0 for index file, btree
 flags          As Long         ' 004  bit0=1 is dirty
                                '      bit1=1 full lock (count stored in KH.lockCount)
                                '      bit2=1 full shared lock (if bit1=1) (clred as below)
                                '      bit3=1 read-through lock (maxFileSize effectively cut in half)
                                '      bit4=1 atomic access
                                '      bit5-6 reserved (=0)
                                '      bit7=1 handle is read-only
                                '      bit8-11 reserved (=0)
                                '      bit13=1 skip already-sorted check in sorter
                                '      bit14=1 leave workfile
                                '      bit15=1 no coalesce on key delete until node is empty
                                '      bit16-31 reserved (=0)
 handle         As Long         ' 008  file handle
 asMode         As Long         ' 012  access-sharing mode of open
 xbLinkPtr      As Long         ' 016  related XB DHD header (aka dhdPtr)
 filenamePtr    As Long         ' 020  filename at open, PSZ
 mutexHandle    As Long         ' 024  mutex handle
                                '      Btree workspace
 sWork028       As Long         ' 028    "
 sWork032       As Long         ' 032    "
 sWork036       As Long         ' 036    "
 sWork040       As Long         ' 040    "
 sWork044       As Long         ' 044    "
 sWork048(1 To 30) As Long      ' 048    "
 rsv168         As Long         ' 168  reserved
 wsPtr          As Long         ' 172  additional workspace ptr (released automatically at header flush unless KH.flags bit12=1)
 workHandle     As Long         ' 176  open handle of reindex work file, if not 0
                                '      history info (since opened or last cleared)
 searches       As Long         ' 180  keys searched for
 seeks          As Long         ' 184  nodes seeked (sought)
 hits           As Long         ' 188  node seeks satisfied without disk access
 keysDeleted    As Long         ' 192  keys deleted
 keysStored     As Long         ' 196  keys added
 nodesSplit     As Long         ' 200  splits needed on insert
 nodesMadeAvail As Long         ' 204  nodes made available from deleting keys
 tagMarks       As Long         ' 208  used to demark delete tag to exclude/include
 lockCount      As Long         ' 212  active full-lock count
 rsv216         As Long         ' 216  reserved
 maxFileSize    As Long         ' 220  see also KH.flags, bit3
 maxFileSizeH   As Long         ' 224  not used: placeholder when T_BLTFO is 32 bits
 reindexPct     As Long         ' 228  percent of reindex done (0,1-99)
 userWord       As Long         ' 232  user word (available for programmer's use)
                                '      ix4 function pointers
 xCallBackPtr   As Long         ' 236  xCallBackPtr is callback routine for reindex (def=0:none)
 xCryptoPtr     As Long         ' 240  xCryptoPtr is crypto routine (def=0:none)
 xSortPtr       As Long         ' 244  xSortPtr is sort routine (def=0:BltFuncIx4Sort())
 xSortCmpPtr    As Long         ' 248  xSortCmpPtr is sort-compare routine (def=0:BltFuncIx4SortCmp())
 xUserSortCmpPtr As Long        ' 252  xUserSortPtr is handler for sort-cmp codes 20 or above (def=none)
 xBuildKeyPtr   As Long         ' 256  xBuildKeyPtr is build-key routine (def=0:BltFuncIx4BuildKey())
 xParseExpPtr   As Long         ' 260  xParseExpPtr is expression parser (def=0:BltFuncIx4ParseExp())
 xBtreePtr      As Long         ' 264  xBtreePtr is btree manager (def=0:BltFuncIx4BTM())
 xReindexPtr    As Long         ' 268  xReindexPtr is reindex routine (def=0:BltFuncIx4Reindex())
                                '      state data:
 MPNL           As Long         ' 272  max pathname length (def=0, use bullet_MPNL)
 MPCL           As Long         ' 276  max pathname component length (def=0, use bullet_MPCL)
 MPXL           As Long         ' 280  max pathname extension length (def=0, use bullet_MPXL)
 MARSV          As Long         ' 284  bytes to hold back from Bullet reindex routine (def=0, use bullet_MARSV)
 MASZRX         As Long         ' 288  max total memory, in bytes, reindex is allowed to allocate (def=0, use bullet_MASZRX)
 MASZ           As Long         ' 292  max single allocation request size, in bytes, that will be made (def=0, use bullet_MASZ)
 MAM            As Long         ' 296  memory allocation strategy (0=default for platform)
 MMUBR          As Long         ' 300  max memory allocation used by reindex, in bytes (R-O)
 rsv304         As Long         ' 304  reserved
                                '
 currKeyrecNo   As Long         ' 308  current rec number assigned to KH.currKey (01 00 00 00 = 1)
 currKey(1 To 196) As Byte      ' 312  current key value (MAX_KEY_LENGTH=196 bytes)
 zeroT          As Byte         ' 508  allow 196-byte key to be 0T'ed
 decryptRecFlag As Byte         ' 509  checked in BuildKey, if = 0x55 then decrypts record (set by reindex to 0x55)
 rsv510         As Integer      ' 510  reserved
                                ' 512  plus:
                                '      index file header
 fileID         As Long         ' 000  file id
 nodeSize       As Long         ' 004  size of a node, in bytes (buffer[], at end of KH, must be this size)
 rootNode       As Long         ' 008  root node (1-based)
 noKeys         As Long         ' 012  total number of keys
 availNode      As Long         ' 016  next available node (link to, 0 if none, 1-based)
 freeNode       As Long         ' 020  next free node (always points to last physical node+1, at end of file)
 keyLength      As Long         ' 024  length of key
 maxKeys        As Long         ' 028  maximum number of keys on a node: (nodeSize-5)/(keyLen+8) (255=absolute max)
 codePage       As Long         ' 032  code page from CreateIndexFile
 countryCode    As Long         ' 036  country code from CreateIndexFile
 sortCmpCode    As Long         ' 040  system (1-31) or user (32 to 255)        |highbyte has special use
                                '      high word has flags: bit0=1 dups allowed | which comes into play when
                                '                           bit1=0 OEM charset  | no data file used
                                '                           bit1=1 ANSI charset
                                '                           bit2-15 rez
                                '
                                '      translated key expression as done by ParseKey during BltIx4CreateFile()
                                '      for each key part in KH.expression a 6-byte structure is used:
                                '
                                '       UCHAR  ftype   field type (C,N,L, etc)
                                '       UCHAR  length  bytes to use starting at offset (never GT MAX_KEY_LENGTH)
                                '       USHORT offset  byte offset into data record that length bytes are to be used
                                '       USHORT flags   flags (iff bit15=0 then bit0=1 upper, bit1=1 descend)
                                '
 xlateCount     As Long         ' 044  number of key fields (16 max fields)
 xlateExp(1 To 16) As BLTXLATEX ' 048  key construct info (96 bytes, MAX_KEY_FIELDS*6)
 lastWrite      As BLTIX4ACCESS ' 144  last write
 xactionFlag    As Byte         ' 152  flag indicating index file was not flushed (bit0=1 not flushed)
 rsv153         As Byte         ' 153  number of bytes key has been padded to reach even8 length, if requested
 rsv154(1 To 6) As Byte         ' 154  reserved
 rsv160(1 To 224) As Byte       ' 160  reserved
                                '      (IX4_HDR_SIZE_FLUSH to here)
 expression(1 To 380) As Byte   ' 384  key expression, user (ASCIZ) (same as IX4_MAX_EXP_SIZE)
 STsize         As Long         ' 764  size of sort table following (is 0x100 or 0)
 sortTable(1 To 256) As Byte    ' 768  sort table, fill at index create
                                '      disk header ends here, at 1024 bytes (first node is at +1024)
                                '
 buffer(1 To 512) As Byte       '1024  node buffer determined at index file create (nodeSize)
End Type                        ' 512+1024+512 = 2048 bytes (for 512-byte node size)


' ---------------
' misc structures

Type BLTIUEX
 khPtr          As Long         ' I:KH of index file
 keyBufferPtr   As Long         ' O:key buffer (separate buffer for each operation)
 recordPtr      As Long         ' I:record data to add | update with
 recNo          As Long         ' O:recNo that record data was stored on insert | I:recNo to update
 action         As Long         ' O:action performed
 flags          As Long         ' I:operation flags
 prevPtr        As Long         ' set to 0 (used internally)
 nextPtr        As Long         ' I:next TBLT_IUEX, or 0 if no more
End Type                        ' 32 bytes

Type BltLockEx
 khdhPtr        As Long         ' I:KH or DH of file
 mode           As Long         ' I:LOCK_MODE_*
 recNoStart     As Long         ' I:for DH locks
 recCount       As Long         ' I:for DH locks
 action         As Long         ' O:action performed
 flags          As Long         ' I:operation flags
 prevPtr        As Long         ' set to 0 (used internally)
 nextPtr        As Long         ' I:next TBLT_IUEX, or 0 if no more
End Type                        ' 32 bytes


Public Const OFFSET_KH_BUFFER = 1536
Public Const OFFSET_DH_FD1 = 192


' DBF data ---------------------------------------------------------------

Declare Function BltDataInfoFile Lib "b3w32d30" (ByVal filenamePtr As String, ByVal asMode As Long, dbfInfoPtr As BLTDBFINFO, noFieldsPtr As Long, ByVal zero As Long) As Long
Declare Function BltDataCreateFile Lib "b3w32d30" (ByVal fileID As Long, ByVal fields As Long, fieldListPtr As Any, ByVal Filename As String, ByVal maMode As Long, ByVal zero As Long) As Long
Declare Function BltDataOpenFile Lib "b3w32d30" (dhPtr As BLTDH, ByVal mode As Long) As Long
Declare Function BltDataCloseFile Lib "b3w32d30" (dhPtr As BLTDH) As Long
Declare Function BltDataFlushHeader Lib "b3w32d30" (dhPtr As BLTDH) As Long
Declare Function BltDataCopyHeader Lib "b3w32d30" (dhPtr As BLTDH, ByVal Filename As String) As Long
Declare Function BltDataReadHeader Lib "b3w32d30" (dhPtr As BLTDH) As Long
Declare Function BltDataZapHeader Lib "b3w32d30" (dhPtr As BLTDH) As Long
Declare Function BltDataAddRecord Lib "b3w32d30" (dhPtr As BLTDH, recordPtr As Any, recNoPtr As Long) As Long
Declare Function BltDataDebumpRecord Lib "b3w32d30" (dhPtr As BLTDH, ByVal recNo As Long) As Long
Declare Function BltDataMarkRecord Lib "b3w32d30" (dhPtr As BLTDH, ByVal recNo As Long, ByVal tagChar As Long) As Long
Declare Function BltDataUpdateRecord Lib "b3w32d30" (dhPtr As BLTDH, ByVal recNo As Long, recordPtr As Any) As Long
Declare Function BltDataGetRecord Lib "b3w32d30" (dhPtr As BLTDH, ByVal recNo As Long, recordPtr As Any) As Long
Declare Function BltDataSort Lib "b3w32d30" (ByVal indexname As String, ByVal Filename As String, ByVal newFilename As String, ByVal cbPtr As Long, ByVal zero As Long) As Long
Declare Function BltDataPack Lib "b3w32d30" (ByVal Filename As String, ByVal newFilename As String, ByVal cbPtr As Long, ByVal zero As Long) As Long

' DBT memo ---------------------------------------------------------------

Declare Function BltMemoCreateFile Lib "b3w32d30" (dhPtr As BLTDH) As Long
Declare Function BltMemoCloseFile Lib "b3w32d30" (dhPtr As BLTDH) As Long
Declare Function BltMemoOpenFile Lib "b3w32d30" (dhPtr As BLTDH) As Long
Declare Function BltMemoFlushHeader Lib "b3w32d30" (dhPtr As BLTDH) As Long
Declare Function BltMemoReadHeader Lib "b3w32d30" (dhPtr As BLTDH) As Long
Declare Function BltMemoAdd Lib "b3w32d30" (dhPtr As BLTDH, ByVal bytesToSize As Long, ByVal bytesToWrite As Long, bufferPtr As Any, memoNoPtr As Long, ByVal rsvFlag As Long) As Long
Declare Function BltMemoDelete Lib "b3w32d30" (dhPtr As BLTDH, ByVal memoNo As Long) As Long
Declare Function BltMemoGet Lib "b3w32d30" (dhPtr As BLTDH, ByVal memoNo As Long, ByVal startOffset As Long, bytesToReadPtr As Long, bufferPtr As Any) As Long
Declare Function BltMemoGetSize Lib "b3w32d30" (dhPtr As BLTDH, ByVal memoNo As Long, availLinkPtr As Long, memoSizePtr As Long) As Long
Declare Function BltMemoUpdate Lib "b3w32d30" (dhPtr As BLTDH, memoNoPtr As Long, ByVal startOffset As Long, ByVal bytesToWrite As Long, bufferPtr As Any) As Long

' IX4 index mid ----------------------------------------------------------

Declare Function BltIx4InfoFile Lib "b3w32d30" (ByVal Filename As String, ByVal asMode As Long, ix4InfoPtr As BLTIX4INFO, ByVal zero As Long) As Long
Declare Function BltIx4CreateFile Lib "b3w32d30" (ByVal nodeSz As Long, ByVal sortCmpCode As Long, ByVal cp As Long, ByVal cc As Long, ByVal stPtr As Long, ByVal kxPtr As String, dhPtr As BLTDH, ByVal Filename As String, ByVal zero As Long, ByVal maMode As Long, ByVal zero2 As Long) As Long
Declare Function BltIx4OpenFile Lib "b3w32d30" (khPtr As BLTKH, ByVal mode As Long) As Long
Declare Function BltIx4CloseFile Lib "b3w32d30" (khPtr As BLTKH) As Long
Declare Function BltIx4FlushHeader Lib "b3w32d30" (khPtr As BLTKH) As Long
Declare Function BltIx4CopyHeader Lib "b3w32d30" (khPtr As BLTKH, ByVal Filename As String) As Long
Declare Function BltIx4ReadHeader Lib "b3w32d30" (khPtr As BLTKH) As Long
Declare Function BltIx4ZapHeader Lib "b3w32d30" (khPtr As BLTKH) As Long
Declare Function BltIx4KeyForRecord Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, ByVal recNo As Long, recordPtr As Any) As Long
Declare Function BltIx4DeleteKey Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, ByVal recNo As Long) As Long
Declare Function BltIx4StoreKey Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, ByVal recNo As Long) As Long
Declare Function BltIx4EqualKey Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, recNoPtr As Long) As Long
Declare Function BltIx4EqualOrGreaterKey Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, recNoPtr As Long) As Long
Declare Function BltIx4EqualOrLesserKey Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, recNoPtr As Long) As Long
Declare Function BltIx4FirstKey Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, recNoPtr As Long) As Long
Declare Function BltIx4LastKey Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, recNoPtr As Long) As Long
Declare Function BltIx4NextKey Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, recNoPtr As Long) As Long
Declare Function BltIx4PrevKey Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, recNoPtr As Long) As Long
Declare Function BltFuncIx4SortCmp Lib "b3w32d30" (khPtr As BLTKH, arg1ptr As Any, arg2ptr As Any, hvBufferPtr As Any, ByVal zero As Long) As Long

Declare Function BltFuncIx4BuildKey Lib "b3w32d30" (khPtr As BLTKH, recordPtr As Any, keyPtr As Any)
Declare Function BltFuncIx4ParseExp Lib "b3w32d30" (khPtr As BLTKH, keyLenPtr As Long)
Declare Function BltFuncIx4Sort Lib "b3w32d30" (khPtr As BLTKH, bufferPtr As Any, ByVal count As Long)
Declare Function BltFuncIx4BTM Lib "b3w32d30" (ByVal func As Long, khPtr As BLTKH, keyPtr As Any, ByVal recNo As Long)

' IX4 index high ---------------------------------------------------------

Declare Function BltIx4Insert Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr, recNoPtr As Long, recordPtr As Any, actionPtr As Long) As Long
Declare Function BltIx4InsertEx Lib "b3w32d30" (iuexPtr As BLTIUEX, slotPtr As Long) As Long
Declare Function BltIx4Update Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, ByVal recNo As Long, newRecPtr As Any, actionPtr As Long) As Long
Declare Function BltIx4UpdateEx Lib "b3w32d30" (iuexPtr As BLTIUEX, slotPtr As Long, ByVal maMode As Long) As Long
Declare Function BltIx4GetFirst Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, recNoPtr As Long, recordPtr As Any) As Long
Declare Function BltIx4GetNext Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, recNoPtr As Long, recordPtr As Any) As Long
Declare Function BltIx4GetPrev Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, recNoPtr As Long, recordPtr As Any) As Long
Declare Function BltIx4GetLast Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, recNoPtr As Long, recordPtr As Any) As Long
Declare Function BltIx4GetEqual Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, recNoPtr As Long, recordPtr As Any) As Long
Declare Function BltIx4GetEqualOrGreater Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, recNoPtr As Long, recordPtr As Any) As Long
Declare Function BltIx4GetEqualOrLesser Lib "b3w32d30" (khPtr As BLTKH, keyBufferPtr As Any, recNoPtr As Long, recordPtr As Any) As Long
Declare Function BltFuncIx4Reindex Lib "b3w32d30" (khPtr As BLTKH, ByVal workFilePtr As String, ByVal nodePackPct As Long, keyBufferPtr As Any, recNoPtr As Long) As Long

' Lock -------------------------------------------------------------------

Declare Function BltLockEx Lib "b3w32d30" (lockExPtr As BltLockEx, slotPtr As Long, ByVal timeout As Long) As Long
Declare Function BltUnlockEx Lib "b3w32d30" (lockExPtr As BltLockEx, slotPtr As Long) As Long
Declare Function BltLockDataHeader Lib "b3w32d30" (dhPtr As BLTDH, ByVal mode As Long, ByVal timeout As Long) As Long
Declare Function BltUnlockDataHeader Lib "b3w32d30" (dhPtr As BLTDH) As Long
Declare Function BltLockDataRecord Lib "b3w32d30" (dhPtr As BLTDH, ByVal mode As Long, ByVal recNoStart As Long, ByVal recCount As Long, ByVal timeout As Long) As Long
Declare Function BltUnlockDataRecord Lib "b3w32d30" (dhPtr As BLTDH, ByVal recNoStart As Long, ByVal recCount As Long) As Long
Declare Function BltLockIx4File Lib "b3w32d30" (khPtr As BLTKH, ByVal mode As Long, ByVal timeout As Long) As Long
Declare Function BltUnlockIx4File Lib "b3w32d30" (khPtr As BLTKH) As Long

' Vars -------------------------------------------------------------------

Declare Function BltGetVar Lib "b3w32d30" (ByVal index As Long, rcPtr As Long) As Long
Declare Function BltSetVar Lib "b3w32d30" (valPtr As Any, ByVal index As Long) As Long

' OS file ----------------------------------------------------------------

Declare Function BltWriteAtFile Lib "b3w32d30" (ByVal handle As Long, bytesPtr As Long, bufferPtr As Any, offsetPtr As Long, ByVal zero As Long) As Long
Declare Function BltReadAtFile Lib "b3w32d30" (ByVal handle As Long, bytesPtr As Long, bufferPtr As Any, offsetPtr As Long, ByVal zero As Long) As Long
Declare Function BltCloseFile Lib "b3w32d30" (handlePtr As Long, ByVal zero As Long) As Long
Declare Function BltCreateDir Lib "b3w32d30" (ByVal dirnamePtr As String, ByVal zero As Long) As Long
Declare Function BltCreateFile Lib "b3w32d30" (ByVal Filename As String, ByVal zero As Long) As Long
Declare Function BltDeleteFile Lib "b3w32d30" (ByVal Filename As String, ByVal zero As Long) As Long
Declare Function BltMoveFile Lib "b3w32d30" (ByVal currNamePtr As String, ByVal newNamePtr As String, ByVal zero As Long) As Long
Declare Function BltLengthFile Lib "b3w32d30" (ByVal handle As Long, lengthPtr As Long, ByVal zero As Long) As Long
Declare Function BltOpenFile Lib "b3w32d30" (ByVal filenamePtr As String, ByVal mode As Long, handlePtr As Long, ByVal zero As Long) As Long
Declare Function BltReadFile Lib "b3w32d30" (ByVal handle As Long, bytesPtr As Long, bufferPtr As Any, ByVal zero As Long) As Long
Declare Function BltSeekFile Lib "b3w32d30" (ByVal handle As Long, ByVal mode As Long, posPtr As Long, ByVal zero As Long) As Long
Declare Function BltUpdateDirEntry Lib "b3w32d30" (ByVal handle As Long, ByVal zero As Long) As Long
Declare Function BltWriteFile Lib "b3w32d30" (ByVal handle As Long, bytesPtr As Long, bufferPtr As Any, ByVal zero As Long) As Long
Declare Function BltLockFile Lib "b3w32d30" (ByVal handle As Long, ByVal mode As Long, lockOffsetPtr As Long, lockBytesPtr As Long, ByVal timeout As Long, ByVal zero As Long) As Long
Declare Function BltIsRemote Lib "b3w32d30" (ByVal handle As Long, ByVal drive As Long, isRemotePtr As Long, flagsPtr As Long, ByVal zero As Long) As Long

' OS misc ----------------------------------------------------------------

Declare Function BltMemAlloc Lib "b3w32d30" (ByVal mode As Long, ByVal count As Long, rcPtr As Long, ByVal zero As Long) As Long
Declare Function BltMemFree Lib "b3w32d30" (ByVal mode As Long, bmaPtr As Long) As Long
Declare Function BltSetHandleCount Lib "b3w32d30" (maxHandlesPtr As Long) As Long
Declare Function BltGetSortSequenceTable Lib "b3w32d30" (ByVal cpID As Long, ByVal ccID As Long, bufferPtr As Any, ByVal charsetFlag As Long) As Long
Declare Function BltGetCountryInfo Lib "b3w32d30" (cpIDptr As Long, ccIDptr As Long, ByVal charsetFlag As Long) As Long
Declare Function BltGetExtendedError Lib "b3w32d30" (exRCptr As Long, xeClassPtr As Long, xeActionPtr As Long, xeLocusPtr As Long) As Long
Declare Function BltGetMemoryAvail Lib "b3w32d30" (ByVal mode As Long) As Long
Declare Function BltGetVersion Lib "b3w32d30" (verDosPtr As Long) As Long
Declare Function BltUpperCaseO Lib "b3w32d30" (ByVal strgPtr As String, ByVal maxSize As Long, ByVal zero As Long) As Long
Declare Function BltCloseMutexSem Lib "b3w32d30" (ByVal handle As Long) As Long
Declare Function BltCreateMutexSem Lib "b3w32d30" (ByVal namePtr As String, handlePtr As Long, ByVal attrFlags As Long, ByVal stateFlag As Long) As Long
Declare Function BltRequestMutexSem Lib "b3w32d30" (ByVal handle As Long, ByVal timeout As Long) As Long
Declare Function BltReleaseMutexSem Lib "b3w32d30" (ByVal handle As Long) As Long


' ----------------------------------------------------------------------------
' VB support routines
' ----------------------------------------------------------------------------

' use BltVB_StrPtr to return a *temporary* pointer to a basic string passed in Strg:
'
' dim zeroValue as long
' zeroValue = 0
' ...
' khPtr.filenamePtr = BltVB_StrPtr(filename$)

Declare Function BltVB_StrPtr Lib "b3w32d30" Alias "BltTestCallBack" (ByVal Strg As String) As Long

' use BltVB_AnyPtr to return a pointer to a basic variable or structure:
' khPtr.xbLinkPtr = BltVB_AnyPtr(dhStructure, zeroValue)

Declare Function BltVB_AnyPtr Lib "b3w32d30" Alias "BltTestCallBack" (StrucVar As Any) As Long

' use BltVB_FnPtr with an AddressOf argument,
' such as KH.xCallbackPtr = BltVB_FnPtr(AddressOf MyRixCallbackRoutine)

Declare Function BltVB_FuncPtr Lib "b3w32d30" Alias "BltTestCallBack" (ByVal FuncPtr As Long) As Long

Declare Sub BltRtlMemSet Lib "b3w32d30" Alias "BltMemSet" (ByVal destPtr As Long, ByVal value As Long, ByVal length As Long)  'fast zero-fill of BLTKH/BLTDH" ()
Declare Sub BltRtlMemCpy Lib "b3w32d30" Alias "BltMemCpy" (ByVal destPtr As Long, ByVal srcPtr As Long, ByVal length As Long)
Declare Function BltStrLen Lib "kernel32" Alias "lstrlen" (ByVal strPtr As Long) As Long


Public Function BltRtlStrLen(strPtr As Long, strType As Long)

'strType is not used in 3.0

   BltRtlStrLen = BltStrLen(strPtr)

End Function


Public Sub BltVB_CopyStrU2A(ascStr() As Byte, maxBytes As Long, basicStr As String, ff As Long)

' convert basic unicode string (in basicStr) to single-byte ascii (in ascStr() array)
' for up to maxBytes (or length of basicStr, whichever is less)
' if ff is non-zero then uses fast method which assumes array base is 1 (ie, ascStr(1 to nn)) and no bounds checking needed
' ascStr gets a 0 appended if there's room, and possibly filled out to its end size with 0s

Dim tsl As Long, ub As Long
Dim i As Long, j As Long
Dim copy2start As Long, copy2end

If (ff And 1) Then

   tsl = Len(basicStr)
   If (tsl > maxBytes) Then tsl = maxBytes

   For i = 1 To tsl
      ascStr(i) = Asc(Mid$(basicStr, i, 1))
   Next

   If (tsl < maxBytes) Then
      ascStr(i) = 0 'always put a 0T if room
      tsl = tsl + 1 'adjust in case can avoid fill loop below
   End If

   ' fill out rest of ascStr() with zeroes if ff=3

   If (maxBytes < tsl) Then
      If (ff And 2) Then
         For i = tsl + 1 To maxBytes
            ascStr(i) = 0
         Next
      End If
   End If

Else

   ub = UBound(ascStr)

   tsl = Len(basicStr)
   If (tsl > maxBytes) Then tsl = maxBytes

   copy2start = LBound(ascStr)
   copy2end = copy2start + tsl - 1
   If (copy2end > ub) Then copy2end = ub

   j = 0
   For i = copy2start To copy2end
      j = j + 1
      ascStr(i) = Asc(Mid$(basicStr, j, 1))
   Next

   ' fill out rest of ascStr() with zeroes

   If (maxBytes < tsl) Then
      For i = (copy2end + 1) To ((copy2end + 1) + (maxBytes - tsl))
         ascStr(i) = 0
      Next
   End If

End If


End Sub

Public Sub BltVB_CopyStrA2U(basicStr As String, maxBytes As Long, ascStr() As Byte, ff As Long)

' convert single-byte ascii (in ascStr() array) to basic unicode string (in basicStr)
' for up to maxBytes (or first character < 32, whichever is less)
' if ff is non-zero then uses fast method which assumes array base is 1 (ie, ascStr(1 to nn)) and no bounds checking needed

Dim ub As Long
Dim i As Long, j As Long
Dim copy2start As Long, copy2end

basicStr = Space$(maxBytes)

If (ff And 1) Then

   For i = 1 To maxBytes
      If ascStr(i) < 32 Then Exit For
      Mid$(basicStr, i, 1) = Chr$(ascStr(i))
   Next

Else

   ub = UBound(ascStr)

   copy2start = LBound(ascStr)
   copy2end = copy2start + maxBytes - 1
   If (copy2end > ub) Then copy2end = ub

   j = 0
   For i = copy2start To copy2end
      j = j + 1
      If ascStr(i) < 32 Then Exit For
      Mid$(basicStr, j, 1) = Chr$(ascStr(i))
   Next

End If

End Sub

Public Function BltVB_CopyStrA2UEx$(basicStr As String, maxBytes As Long, ascStr() As Byte, ff As Long)

' similar to BltVB_CopyStrA2U subroutine but returns basicStr

Call BltVB_CopyStrA2U(basicStr, maxBytes, ascStr(), ff)

BltVB_CopyStrA2UEx = basicStr

End Function

Public Sub BltVB_CopyStrMem2U(basicStr As String, maxBytes As Long, memPtr As Long, ff As Long)

' convert single-byte ascii (at offset memPtr) to basic unicode string (in basicStr)
' for up to maxBytes (or first character < 32, whichever is less)
' ff not currently used
' returns empty string (0 length) only if first byte at memPtr was binary 0

Dim ub As Long
Dim i As Long, j As Long

Dim tBuff(1 To 260) As Byte  'use stack if maxBytes < 260

basicStr = Space$(maxBytes)

If (maxBytes < 260) Then

   Call BltRtlMemCpy(BltVB_AnyPtr(tBuff(1)), memPtr, maxBytes)
   
   For i = 1 To maxBytes
      If tBuff(i) < 32 Then
         If (i = 1) Then basicStr = ""
         Exit For
      End If
      Mid$(basicStr, i, 1) = Chr$(tBuff(i))

   Next

Else

   ReDim t2Buff(1 To maxBytes) As Byte

   Call BltRtlMemCpy(BltVB_AnyPtr(t2Buff(1)), memPtr, maxBytes)

   For i = 1 To maxBytes
      If t2Buff(i) < 32 Then
         If (i = 1) Then basicStr = ""
         Exit For
      End If
      Mid$(basicStr, i, 1) = Chr$(t2Buff(i))
   Next

End If

End Sub

Public Function BltVB_CopyStrMem2UEx$(basicStr As String, maxBytes As Long, memPtr As Long, ff As Long)

' similar to BltVB_CopyStrMem2U subroutine but returns basicStr

Call BltVB_CopyStrMem2U(basicStr, maxBytes, memPtr, ff)

BltVB_CopyStrMem2UEx = basicStr

End Function

'end b3vbwin.bas


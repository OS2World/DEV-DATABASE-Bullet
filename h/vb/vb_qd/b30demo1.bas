Attribute VB_Name = "Module2"
'b3xdemo1.bas
'4-Oct-99 -chh
'original basic design by Paul Squires

'define the TYPE variable to hold the record
'data for the Bullet 3 demo. This has to be
'done in a module because VB won't let you do
'it in a Form.

''you really have to be careful when using String * n since
''VB makes those wide characters -- it might be a good idea
''to use the BltVB_BtoA and AtoB routines instead since they'll
''probably be much faster (VB does something similar, only
''you don't know about it), though maybe not worth the hassle
''(write a routine to do it, once, and you're set, anyway)

Option Explicit
Option Base 1   'is a good idea since it prevents common start-at errors (for Basic)

Public Const MAX_FILENAME_LEN = 64

Public Const g01dataFilename As String = "01_$$$.dbf"
Public Const g01memoFilename As String = "01_$$$.dbt"
Public Const g01indexFilename As String = "01_$$$.ix4"

Public Const g01fileID As Long = &H3
Public Const g01fields As Long = 4

Public Const g01dbfASMODE As Long = OPENFLAGS_ACCESS_RW + OPENFLAGS_DENY_NONE
Public Const g01ix4ASMODE As Long = OPENFLAGS_ACCESS_RW + OPENFLAGS_DENY_NONE

Public Const g01sortCmpCode As Long = SORT_MIXED
Public Const g01keyExp As String = "EMP_ID"
Public Const g01nodeSize As Long = 512

Public g01busy As Long     'use to flag whether can safely abort (unload)
                           'since you don't want to unload while you're inside Bullet
Public g01bailNow As Long  'flag for callback routine to bail as soon as possible
Public g01testVar As Long  'misc global var

Public g01DH As BLTDH
Public g01KH As BLTKH

Public g01fieldList(1 To g01fields) As BLTFD

'just a simple memory buffer to use whenever a key buffer is needed
'a specific UDT could be used, too, and would be a good idea if the
'key were any more complex than this example (just a simple 9-byte text ID)
 
Type KEY01_STRUC
 key(1 To 196) As Byte
End Type

Type EMP_RECORD
 delTag As Byte
 empID(1 To 9) As Byte
 empName(1 To 34) As Byte
 salary(1 To 12) As Byte
 salBin As Double
End Type  '64 bytes


Public Function RixCallback(tKH As BLTKH, info As Long) As Long

 'callback routines need to be at the module level (not in a form)
 'handle callback from reindex routine
 'as a minimum should call DoEvent to handle msg queue
 'but can also update a window with tKH.reindexPct, for example
 'return non-0 to abort the reindex, 0 to continue

 Dim rc As Long
 Static lastPct As Long

 rc = 0

 If (lastPct <> tKH.reindexPct) Then
    If (tKH.reindexPct = 0) Then
       Form1.PctDone.Text = "done"
    Else
       Form1.PctDone.Text = Str$(tKH.reindexPct)
    End If
    lastPct = tKH.reindexPct
 End If

 rc = g01bailNow  'set to 1 on cancel click (any non-zero causes reindex to abort)

 If (rc = 0) Then
    DoEvents   'always do events (unless bailing)
 End If

 RixCallback = rc

End Function


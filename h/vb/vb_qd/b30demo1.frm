VERSION 5.00
Begin VB.Form Form1 
   BackColor       =   &H00808080&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Bullet 3..."
   ClientHeight    =   7260
   ClientLeft      =   315
   ClientTop       =   3870
   ClientWidth     =   7110
   FillColor       =   &H00FFFFFF&
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   7260
   ScaleWidth      =   7110
   Begin VB.TextBox PctDone 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      CausesValidation=   0   'False
      ForeColor       =   &H0080FF80&
      Height          =   315
      Left            =   2475
      TabIndex        =   1
      TabStop         =   0   'False
      Top             =   750
      Width           =   615
   End
   Begin VB.TextBox txtRecords 
      Alignment       =   2  'Center
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Left            =   1125
      TabIndex        =   4
      Text            =   "10000"
      Top             =   750
      Width           =   1140
   End
   Begin VB.CommandButton cmdQuit 
      Caption         =   "E&xit"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   465
      Left            =   5625
      MaskColor       =   &H00FFFFFF&
      Style           =   1  'Graphical
      TabIndex        =   3
      Top             =   675
      Width           =   1140
   End
   Begin VB.CommandButton cmdDoDemo 
      Caption         =   "&Run"
      Default         =   -1  'True
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   465
      Left            =   3900
      MaskColor       =   &H00FFFFFF&
      Style           =   1  'Graphical
      TabIndex        =   2
      Top             =   675
      Width           =   1140
   End
   Begin VB.ListBox List1 
      BackColor       =   &H00000000&
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H0000FF00&
      Height          =   5940
      Left            =   75
      TabIndex        =   0
      TabStop         =   0   'False
      Top             =   1200
      Width           =   6915
   End
   Begin VB.Label BulletVersion 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      BackColor       =   &H00808080&
      Caption         =   "3.10 G WNT..."
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   6.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00E0E0E0&
      Height          =   240
      Left            =   2700
      TabIndex        =   8
      Top             =   225
      Width           =   4290
   End
   Begin VB.Label Label4 
      BackColor       =   &H00808080&
      Caption         =   "%"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00E0E0E0&
      Height          =   315
      Left            =   3225
      TabIndex        =   7
      Top             =   750
      Width           =   315
   End
   Begin VB.Line Line1 
      X1              =   180
      X2              =   6900
      Y1              =   570
      Y2              =   570
   End
   Begin VB.Label AppName 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      BackColor       =   &H00808000&
      BorderStyle     =   1  'Fixed Single
      Caption         =   "Bullet 3..."
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   315
      Left            =   300
      TabIndex        =   6
      Top             =   150
      Width           =   2340
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      BackColor       =   &H00808080&
      Caption         =   "Records"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00E0E0E0&
      Height          =   315
      Left            =   150
      TabIndex        =   5
      Top             =   750
      Width           =   915
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmdDoDemo_Click()

 Const RixWorkFilename As String = "01_$$$.tmp"

 Const ADDREC_EVENT_RATE As Long = 500
 Const GETNEXT_EVENT_RATE As Long = 500
 Const GETPREV_EVENT_RATE As Long = 500

 'want nice, clean ascii with no hidden VB calls,
 'so using As Byte strings for filenames to pass
 'in --Bullet KH and DH structures--

 Static dataFilename(1 To MAX_FILENAME_LEN) As Byte
 Static indexFilename(1 To MAX_FILENAME_LEN) As Byte


 Dim rc As Long
 Dim i As Long
 Dim tVar As Long
 Dim recs2add As Long, recNo As Long, lastAddPct As Long

 Dim tStr As String

 Dim startTime As Single, endTime As Single

 Dim salary As Double

 Dim empRec As EMP_RECORD

 'can also use a UDT for the key buffer, useful for complex keys

 Dim keyBuffer As KEY01_STRUC, lastBuffer As KEY01_STRUC

 Form1.cmdDoDemo.Enabled = False
 Form1.cmdQuit.Caption = "&Stop"

 ChDir App.Path
 List1.Clear
 Form1.PctDone.Text = ""

 g01busy = 1
 g01bailNow = 0

 'remove any previous data file(s)
  
 rc = BltDeleteFile(g01dataFilename, 0)
 rc = BltDeleteFile(g01memoFilename, 0)
 rc = BltDeleteFile(g01indexFilename, 0)
 rc = BltDeleteFile(RixWorkFilename, 0)

 'create data file(s)
 'note: the fieldLengths must exactly match those of EMP_RECORD

 Call BltVB_CopyStrU2A(g01fieldList(1).fieldName, 11, "EMP_ID", 1)
 g01fieldList(1).fieldType = FTYPE_C
 g01fieldList(1).fieldLength = 9
 g01fieldList(1).fieldDC = 0

 Call BltVB_CopyStrU2A(g01fieldList(2).fieldName, 11, "EMP_NAME", 1)
 g01fieldList(2).fieldType = FTYPE_C
 g01fieldList(2).fieldLength = 34
 g01fieldList(2).fieldDC = 0

 Call BltVB_CopyStrU2A(g01fieldList(3).fieldName, 11, "SALARY", 1)
 g01fieldList(3).fieldType = FTYPE_N
 g01fieldList(3).fieldLength = 12
 g01fieldList(3).fieldDC = 2

 Call BltVB_CopyStrU2A(g01fieldList(4).fieldName, 11, "SALBIN", 1)
 g01fieldList(4).fieldType = FTYPE_DBL
 g01fieldList(4).fieldLength = 12   'when binary data type, fieldLen.DC are
 g01fieldList(4).fieldDC = 2        'for display purposes (eg, used by validate.exe)

 rc = BltDataCreateFile(g01fileID, g01fields, g01fieldList(1), g01dataFilename, 0, 0)
 'If (rc = WRN_ALIGNMENT_CHECK) Then rc = 0
 
 List1.AddItem "Create data, rc=" & rc
 If (rc) Then GoTo BailJump
   
 ' open data file(s)

 Call BltVB_CopyStrU2A(dataFilename(), MAX_FILENAME_LEN, g01dataFilename, 0)
 g01DH.filenamePtr = BltVB_AnyPtr(dataFilename(1))
 g01DH.asMode = g01dbfASMODE
 g01DH.noFields = g01fields

 ' put field descriptor data in this array rather than in the gDH.fd() fixed-sized array (new in 3.1)

#If (BLT_VERSION_310) Then

 g01DH.fdPtr = BltVB_AnyPtr(g01fieldList(1))

#Else

 Call BltRtlMemCpy(BltVB_AnyPtr(g01DH.fd(1)), BltVB_AnyPtr(g01fieldList(1)), (32 * g01fields))

#End If

 rc = BltDataOpenFile(g01DH, 0)

 List1.AddItem "  Open data, rc=" & rc
 If (rc) Then GoTo BailJump


 'create index file

 rc = BltIx4CreateFile(g01nodeSize, g01sortCmpCode, 0, 0, 0, g01keyExp, g01DH, g01indexFilename, 0, 0, 0)
 List1.AddItem " Create ix4, rc=" & rc
 List1.Refresh
 If (rc) Then GoTo BailJump

 ' open index file

 Call BltVB_CopyStrU2A(indexFilename(), MAX_FILENAME_LEN, g01indexFilename, 0)
 g01KH.filenamePtr = BltVB_AnyPtr(indexFilename(1))
 g01KH.asMode = g01ix4ASMODE
 g01KH.nodeSize = (g01nodeSize And 4095)  'remove any flags than may be in g01nodeSize
 g01KH.xbLinkPtr = BltVB_AnyPtr(g01DH)

 rc = BltIx4OpenFile(g01KH, 0)
 List1.AddItem "   Open ix4, rc=" & rc
 List1.Refresh
 If (rc) Then GoTo BailJump

 'list the field descriptor data

 List1.AddItem ""
 List1.AddItem "FIELDNAME    T LEN.DC"
 List1.AddItem " <deltag>    * 1.0"

 If (g01DH.fdPtr) Then

    'if fdPtr then the field descriptor data is in this program's g01fieldList() global array

    For i = 1 To g01DH.noFields

       List1.AddItem " " & BltVB_CopyStrA2UEx$(tStr, 11, g01fieldList(i).fieldName, 1) & " " & _
                                          Chr(g01fieldList(i).fieldType) & " " & _
                                          g01fieldList(i).fieldLength & "." & _
                                          g01fieldList(i).fieldDC

    Next

 Else

    'if fdPtr is 0 then field descriptor data is in g01DH.fd()

    '(and it's also in g01fieldList() in this example, since this example created the file)
    
    For i = 1 To g01DH.noFields

      List1.AddItem " " & BltVB_CopyStrA2UEx$(tStr, 11, g01DH.fd(i).fieldName, 1) & " " & _
                                          Chr(g01DH.fd(i).fieldType) & " " & _
                                          g01DH.fd(i).fieldLength & "." & _
                                          g01DH.fd(i).fieldDC

    Next

 End If
 
 'add recs

 recs2add = Val(txtRecords.Text)

 List1.AddItem ""
 List1.AddItem "Adding " & Format(recs2add, "#,##0") & " recs..."
 List1.Refresh

 'these won't be changing in this example's loop

 empRec.delTag = 32  'same as <space>
 Call BltVB_CopyStrU2A(empRec.empName(), UBound(empRec.empName), "Any name will do", 0)

 lastAddPct = -1
 g01DH.packPct = 1   'set non-zero to prevent cancel until better prepared

 startTime = Timer
 For i = 1 To recs2add

    Call BltVB_CopyStrU2A(empRec.empID, UBound(empRec.empID), Hex$(&H10000000 + i), 0)

    tVar = 10000000 - i
    tStr = Space$(UBound(empRec.salary))
    RSet tStr = Str$(tVar) & ".25"

    Call BltVB_CopyStrU2A(empRec.salary, UBound(empRec.salary), tStr, 0)

    empRec.salBin = Val(tStr)  'not val(empRec.salary) which is array of byte

    rc = BltDataAddRecord(g01DH, empRec, recNo)
    If (rc = 0) Then rc = g01bailNow               '*** AddRecs bail check
    If (rc) Then Exit For

    If ((i Mod ADDREC_EVENT_RATE) = 0) Then
       tVar = (Int((i / recs2add) * 100))

       If (tVar <> lastAddPct) Then
          Form1.PctDone.Text = Str$(tVar)
          lastAddPct = tVar
       End If

       DoEvents

    End If

 Next
 endTime = Timer

 g01DH.packPct = 0   'set back to 0 (check on cancel click)

 Form1.PctDone.Text = "done"

 If (endTime < startTime) Then
   endTime = endTime + (60 * 60 * 24) 'crossed midnight, add in a day's worth of secs
 End If

 If (rc = 1) Then
    List1.AddItem "...add recs aborted, rc=" & rc
 ElseIf (rc) Then
    List1.AddItem "...add recs failed, rc=" & rc
 Else
    List1.AddItem "...took " & Format((endTime - startTime), "#0.00") & " secs"
 End If

 If (rc) Then GoTo BailJump

 'reindex (add all keys at one time, see BltIx4Insert to insert one at a time)

 List1.AddItem "Reindexing " & Format(recs2add, "#,##0") & " recs..."
 List1.Refresh

 'set a callback for the reindex (to at least call DoEvents)
 'very useful when there are lots of recs to deal with

 g01KH.xCallBackPtr = BltVB_FuncPtr(AddressOf RixCallback)

 startTime = Timer
 rc = BltFuncIx4Reindex(g01KH, RixWorkFilename, 100, keyBuffer, recNo)
 endTime = Timer

 g01KH.xCallBackPtr = 0

 If (rc = 1) Then
    List1.AddItem "...reindex aborted, rc=" & rc   '*** Reindex bail check (captured in callback routine)
    List1.AddItem ""
 ElseIf (rc) Then
    List1.AddItem "...reindex failed, rc=" & rc
    List1.AddItem ""
 Else
    List1.AddItem "...took " & Format((endTime - startTime), "#0.00") & " secs"
    List1.AddItem ""
 End If
 List1.Refresh

 If (rc) Then GoTo BailJump

 List1.AddItem "Getting forward key order, all " & Format(recs2add, "#,##0") & " keys+recs..."
 List1.Refresh

 i = 0
 lastAddPct = -1

 startTime = Timer
 rc = BltIx4GetFirst(g01KH, keyBuffer, recNo, empRec)
 Do While (rc = 0)
    i = i + 1
    rc = BltIx4GetNext(g01KH, keyBuffer, recNo, empRec)

    If (rc = 0) Then rc = g01bailNow               '*** GetNext bail check
    If (rc) Then Exit Do

    'key is based on empRec.empID, which can be compared with a simple compare
    'but since Bullet's SortCmp is available, just use that
    'also, this example expects the key to be printable characters (ie, not binary)

    If (BltFuncIx4SortCmp(g01KH, lastBuffer.key(1), keyBuffer.key(1), 0, 0) > 0) Then
       rc = 999999
       Exit Do
    End If

    Call BltRtlMemCpy(BltVB_AnyPtr(lastBuffer.key(1)), BltVB_AnyPtr(keyBuffer.key(1)), g01KH.keyLength)

    If ((i Mod GETNEXT_EVENT_RATE) = 0) Then
       tVar = (Int((i / recs2add) * 100))

       If (tVar <> lastAddPct) Then
          Form1.PctDone.Text = Str$(tVar)
          lastAddPct = tVar
       End If

       DoEvents

    End If

 Loop
 endTime = Timer
 If (rc = EXB_END_OF_FILE) Then rc = 0  'expected rc after getting all keys+recs

 If (rc = 1) Then
    List1.AddItem "...GetNext aborted, rc=" & rc
    List1.AddItem ""
 ElseIf (rc = 999999) Then
    List1.AddItem "...GetNext is not in expected order (count=" & i & "):"
    List1.AddItem "...arg1->" & BltVB_CopyStrA2UEx$(tStr, g01KH.keyLength, lastBuffer.key, 0)
    List1.AddItem "...arg2->" & BltVB_CopyStrA2UEx$(tStr, g01KH.keyLength, keyBuffer.key, 0)
   List1.AddItem ""
 ElseIf (rc) Then
    List1.AddItem "...GetNext failed, rc=" & rc
    List1.AddItem ""
 Else
    List1.AddItem "...took " & Format((endTime - startTime), "#0.00") & " secs (count=" & i & ")"
    Form1.PctDone.Text = "done"
 End If
 List1.Refresh

 If (rc) Then GoTo BailJump

 List1.AddItem "Getting reverse key order, all " & Format(recs2add, "#,##0") & " keys+recs..."
 List1.Refresh

 i = 0
 startTime = Timer
 rc = BltIx4GetLast(g01KH, keyBuffer, recNo, empRec)
 Do While (rc = 0)
    i = i + 1
    rc = BltIx4GetPrev(g01KH, keyBuffer, recNo, empRec)

    If (rc = 0) Then rc = g01bailNow               '*** GetPrev bail check
    If (rc) Then Exit Do

    'key is based on empRec.empID, which can be compared with a simple compare
    'but since Bullet's SortCmp is available, just use that
    'also, this example expects the key to be printable characters (ie, not binary)

    If (BltFuncIx4SortCmp(g01KH, lastBuffer.key(1), keyBuffer.key(1), 0, 0) < 0) Then
       rc = 999999
       Exit Do
    End If
    Call BltRtlMemCpy(BltVB_AnyPtr(lastBuffer.key(1)), BltVB_AnyPtr(keyBuffer.key(1)), g01KH.keyLength)

    If ((i Mod GETPREV_EVENT_RATE) = 0) Then
       tVar = (Int((i / recs2add) * 100))

       If (tVar <> lastAddPct) Then
          Form1.PctDone.Text = Str$(tVar)
          lastAddPct = tVar
       End If

       DoEvents

    End If

 Loop
 endTime = Timer
 If (rc = EXB_TOP_OF_FILE) Then rc = 0  'expected rc after getting all keys+recs

 If (rc = 1) Then
    List1.AddItem "...GetPrev aborted, rc=" & rc
    List1.AddItem ""
 ElseIf (rc = 999999) Then
    List1.AddItem "...GetPrev is not in expected order (count=" & i & "):"
    List1.AddItem "...arg1->" & BltVB_CopyStrA2UEx$(tStr, g01KH.keyLength, lastBuffer.key, 0)
    List1.AddItem "...arg2->" & BltVB_CopyStrA2UEx$(tStr, g01KH.keyLength, keyBuffer.key, 0)

    List1.AddItem ""
 ElseIf (rc) Then
    List1.AddItem "...GetPrev failed, rc=" & rc
    List1.AddItem ""
 Else
    List1.AddItem "...took " & Format((endTime - startTime), "#0.00") & " secs (count=" & i & ")"
    Form1.PctDone.Text = "done"
 End If
 List1.AddItem ""
 List1.Refresh

BailJump:

 If (g01KH.handle) Then
   rc = BltIx4CloseFile(g01KH)
   List1.AddItem "  ix4 close, rc=" & rc
 End If

 If (g01DH.handle) Then
   rc = BltDataCloseFile(g01DH)
   List1.AddItem " data close, rc=" & rc
 End If

 g01busy = 0
 Form1.cmdDoDemo.Enabled = True
 Form1.cmdQuit.Caption = "E&xit"

End Sub

Private Sub cmdQuit_Click()

 'to prevent sudden unloading of form while still inside Bullet, sync unloading
 'with Bullet so that an Exit press unloads only when known to be no longer in Bullet
 '-- this example checks g01bailNow in key points (including the reindex callback routine)
 '-- and bails as soon as possible if g01bailNow is non-zero, at which point Exit can
 '-- be pressed again to actually exit (they may be a better way, but this'll work)
 'unloading while this code is active in Bullet results in a zombie process, which can
 'be killed with the task manager
 
 If ((g01KH.reindexPct = 0) And (g01DH.packPct = 0) And (g01busy = 0)) Then
  Unload Form1
 Else
  g01bailNow = 1
  Beep
 End If

End Sub


Private Sub Form_Load()

 'gets pointer to Bullet version string (ascii)
 'copies the version string to a basic string, then sets form text label

 Dim memPtr As Long
 Dim versionStr As String
 
 memPtr = BltGetVar(BLTVAR_VERSION, 0)

 If (memPtr) Then
    Call BltVB_CopyStrMem2U(versionStr, BltRtlStrLen(memPtr, STRTYPE_CHAR), memPtr, 0)
    Form1.BulletVersion.Caption = versionStr
 Else
    Form1.BulletVersion.Caption = "BltGetVar error (version)"
 End If

 AppName.Caption = "Bullet " + Left$(versionStr, 3) + " Quickie"
 Form1.Caption = AppName.Caption + " Demo"

End Sub

Private Sub Form_Unload(Cancel As Integer)
    
 Dim rc As Long

 If (g01KH.handle) Then rc = BltIx4CloseFile(g01KH)
 If (g01DH.handle) Then rc = BltDataCloseFile(g01DH)
 
End Sub


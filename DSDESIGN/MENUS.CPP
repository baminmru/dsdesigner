VERSION 5.00
Begin VB.Form frmMenus 
   ClientHeight    =   5325
   ClientLeft      =   1035
   ClientTop       =   2505
   ClientWidth     =   6585
   LinkTopic       =   "Form1"
   ScaleHeight     =   5325
   ScaleWidth      =   6585
   Begin VB.Menu mnuRefresh2 
      Caption         =   "��������"
   End
   Begin VB.Menu tvAdd 
      Caption         =   "tvAdd"
      Begin VB.Menu mnuAdd 
         Caption         =   "��������"
      End
      Begin VB.Menu mnuLoadCollection 
         Caption         =   "���������"
      End
      Begin VB.Menu mnuLoadNewObject 
         Caption         =   "��������� ����� ������"
      End
      Begin VB.Menu mnuSaveCollection 
         Caption         =   "���������"
      End
   End
   Begin VB.Menu tvEdit 
      Caption         =   "tvEdit"
      Begin VB.Menu mnuEdit 
         Caption         =   "�������������"
      End
      Begin VB.Menu mnuDel 
         Caption         =   "�������"
      End
      Begin VB.Menu mnuLoadNew 
         Caption         =   "��������� ����� ������"
      End
      Begin VB.Menu mnuCreateLike 
         Caption         =   "������� �����"
      End
      Begin VB.Menu mnuLoadObject 
         Caption         =   "���������"
      End
      Begin VB.Menu mnuSaveObject 
         Caption         =   "���������"
      End
   End
   Begin VB.Menu tvEditOnly 
      Caption         =   "tvEditonly"
      Begin VB.Menu mnuEditOnly 
         Caption         =   "�������������"
      End
      Begin VB.Menu mnuRefresh 
         Caption         =   "��������"
      End
      Begin VB.Menu mnuMakeDoc 
         Caption         =   "��������������"
      End
   End
End
Attribute VB_Name = "frmMenus"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Private Sub mnuAdd_Click()
    Dim f As Form
    NodeText = ""
    Select Case NodeTag
    Case "VARS"
        Set f = frmVar
    Case "FORMS"
        Set f = frmFORM
    Case "REPORTS"
        Set f = frmRPT
    Case "MENUS"
        Set f = frmMenu
    Case "LAYS"
        Set f = frmLayout
    Case Else
    Set f = Nothing
    End Select
    If Not f Is Nothing Then
      f.Show
      f.WindowState = 0
    End If
End Sub

Private Sub mnuCreateLike_Click()
    Dim pos As Long, src As String, o As Object
    pos = 1
    Select Case NodeTag
    Case "VAR"
        src = A.AppVariables.Find(NodeText).oWrite
        Set o = A.AppVariables.Add
        o.Read src, pos
        o.name = o.name & " (�����)" & Now
    Case "FORM"
        src = A.AppForms.Find(NodeText).oWrite
        Set o = A.AppForms.Add
        o.Read src, pos
        o.name = o.name & " (�����)" & Now
    Case "REPORT"
        src = A.AppReports.Find(NodeText).oWrite
        Set o = A.AppReports.Add
        o.Read src, pos
        o.name = o.name & " (�����)" & Now
    Case "MENU"
        src = A.AppMenu.Find(NodeText).oWrite
        Set o = A.AppMenu.Add
        o.Read src, pos
        o.Caption = o.Caption & " (�����)" & Now
    Case "LAY"
        src = A.AppLayouts.Find(NodeText).oWrite
        Set o = A.AppLayouts.Add
        o.Read src, pos
        o.name = o.name & " (�����)" & Now
    End Select
    frmTree.FillForm
End Sub

Public Sub mnuDel_Click()
    Dim V As Variable
    Dim f As AppForm
    Dim r As Report
    Dim m As AppMenu
    Dim l As Layout
    Dim i As Long
    i = 1
    Select Case NodeTag
    Case "VAR"
       For Each V In A.AppVariables
        If V.name = NodeText Then
          A.AppVariables.Remove i
          Exit For
        End If
        i = i + 1
       Next
    Case "FORM"
        For Each f In A.AppForms
        If f.name = NodeText Then
          A.AppForms.Remove i
          Exit For
        End If
        i = i + 1
       Next
    Case "REPORT"
        For Each r In A.AppReports
        If r.name = NodeText Then
          A.AppReports.Remove i
          Exit For
        End If
        i = i + 1
       Next
    Case "MENU"
        For Each m In A.AppMenu
        If m.Caption = NodeText Then
          A.AppMenu.Remove i
          Exit For
        End If
        i = i + 1
       Next
    Case "LAY"
        For Each l In A.AppLayouts
        If l.name = NodeText Then
          A.AppLayouts.Remove i
          Exit For
        End If
        i = i + 1
       Next
    Case Else
    
    End Select
    frmTree.FillForm
End Sub

Private Sub mnuEdit_Click()
    Dim cap As String
    Dim f As Form
    Select Case NodeTag
    Case "APP"
        cap = NodeText & "-����������"
    Case "VAR"
        cap = NodeText & "-����������"
    Case "FORM"
        cap = NodeText & "-�����"
    Case "REPORT"
        cap = NodeText & "-�����"
    Case "MENU"
        cap = NodeText & "-����"
    Case "LAY"
        cap = NodeText & "-���������"
    Case Else
        cap = ""
        Set f = Nothing
    End Select
    
    For Each f In Forms
        If f.Caption = cap Then
            f.ZOrder 0
            f.WindowState = 0
            Exit Sub
        End If
    Next f
    
    Select Case NodeTag
    Case "APP"
        Set f = New frmApp
    Case "VAR"
        Set f = New frmVar
    Case "FORM"
        Set f = New frmFORM
    Case "REPORT"
        Set f = New frmRPT
    Case "MENU"
        Set f = New frmMenu
    Case "LAY"
        Set f = New frmLayout
    Case Else
        Set f = Nothing
    End Select
    
    If Not f Is Nothing Then
        f.Caption = cap
        'f.Show
        f.WindowState = 0
    End If
End Sub

Private Sub mnuEditOnly_Click()
  mnuEdit_Click
End Sub

Private Sub mnuLoadCollection_Click()
    Dim src As String, pos As Long
    pos = 1
        With MDIForm1.CDlg
       ' Set CancelError is True
        .CancelError = True
        On Error GoTo ErrHandler
        ' Set flags
        .Flags = cdlOFNHideReadOnly + cdlOFNExplorer + cdlOFNExtensionDifferent + cdlOFNHelpButton + cdlOFNLongNames + cdlOFNNoReadOnlyReturn + cdlOFNOverwritePrompt
        ' Set filters
        .filename = NodeTag
        .InitDir = A.SourcePath
        
        Select Case NodeTag
        Case "VARS"
          .Filter = "��������� ���������� (*.acv)|*.acv"
        Case "FORMS"
          .Filter = "��������� ���� (*.acf)|*.acf"
        Case "REPORTS"
          .Filter = "��������� ������� (*.acr)|*.acr"
        Case "MENUS"
           .Filter = "��������� ���� (*.acm)|*.acm"
        Case "LAYS"
            .Filter = "��������� ���������� (*.acl)|*.acl"
        End Select
        

        ' Display the Open dialog box
        .ShowOpen
              
        Screen.MousePointer = vbHourglass
        Open .filename For Input As 1
        src = Input(LOF(1), 1)
        Close 1
    End With
    
    Select Case NodeTag
    Case "VARS"
        A.AppVariables.Read src, pos
    Case "FORMS"
        A.AppForms.Read src, pos
    Case "REPORTS"
        A.AppReports.Read src, pos
    Case "MENUS"
        A.AppMenu.Read src, pos
    Case "LAYS"
        A.AppLayouts.Read src, pos
    End Select
    
    Screen.MousePointer = vbNormal
    frmTree.FillForm
ErrHandler:
End Sub

Private Sub mnuLoadNew_Click()
Dim src As String, pos As Long, o As Object
    pos = 1
        With MDIForm1.CDlg
       ' Set CancelError is True
        .CancelError = True
        On Error GoTo ErrHandler
        ' Set flags
        .Flags = cdlOFNHideReadOnly + cdlOFNExplorer + cdlOFNExtensionDifferent + cdlOFNHelpButton + cdlOFNLongNames + cdlOFNNoReadOnlyReturn + cdlOFNOverwritePrompt
        ' Set filters
        .filename = ""
        .InitDir = A.SourcePath
        Select Case NodeTag
        Case "VAR"
          .Filter = "���������� (*.adv)|*.adv"
        Case "FORM"
          .Filter = "����� (*.adf)|*.adf"
        Case "REPORT"
          .Filter = "����� (*.adr)|*.adr"
        Case "MENU"
          .Filter = "���� (*.adm)|*.adm"
        Case "LAY"
          .Filter = "�������� (*.adl)|*.adl"
        End Select
        
        
        ' Display the Open dialog box
        .ShowOpen
          
        Screen.MousePointer = vbHourglass
        Open .filename For Input As 1
        src = Input(LOF(1), 1)
        Close 1
    End With
    
    Select Case NodeTag
    Case "VAR"
        Set o = A.AppVariables.Add
        o.Read src, pos
        o.name = o.name & " >>" & Now
    Case "FORM"
        Set o = A.AppForms.Add
          o.Read src, pos
        o.name = o.name & " >>" & Now
    Case "REPORT"
        Set o = A.AppReports.Add
          o.Read src, pos
        o.name = o.name & " >>" & Now
    Case "MENU"
        Set o = A.AppMenu.Add
        o.Read src, pos
        o.Caption = o.Caption & " >>" & Now
    Case "LAY"
        Set o = A.AppLayouts.Add
          o.Read src, pos
        o.name = o.name & " >>" & Now
    End Select
    
    Screen.MousePointer = vbNormal
    frmTree.FillForm
ErrHandler:

End Sub

Private Sub mnuLoadNewObject_Click()
Dim src As String, pos As Long, o As Object
    pos = 1
        With MDIForm1.CDlg
       ' Set CancelError is True
        .CancelError = True
        On Error GoTo ErrHandler
        ' Set flags
        .Flags = cdlOFNHideReadOnly + cdlOFNExplorer + cdlOFNExtensionDifferent + cdlOFNHelpButton + cdlOFNLongNames + cdlOFNNoReadOnlyReturn + cdlOFNOverwritePrompt
        .filename = ""
        .InitDir = A.SourcePath
        ' Set filters
        Select Case NodeTag
        Case "VAR", "VARS"
          .Filter = "���������� (*.adv)|*.adv"
        Case "FORM", "FORMS"
          .Filter = "����� (*.adf)|*.adf"
        Case "REPORT", "REPORTS"
          .Filter = "����� (*.adr)|*.adr"
        Case "MENU", "MENUS"
          .Filter = "���� (*.adm)|*.adm"
        Case "LAY", "LAYS"
          .Filter = "�������� (*.adl)|*.adl"
        End Select
        ' Display the Open dialog box
        .ShowOpen
          
        Screen.MousePointer = vbHourglass
        Open .filename For Input As 1
        src = Input(LOF(1), 1)
        Close 1
    End With
    
      Select Case NodeTag
    Case "VARS"
        Set o = A.AppVariables.Add
        o.Read src, pos
        o.name = o.name & " >>" & Now
    Case "FORMS"
        Set o = A.AppForms.Add
          o.Read src, pos
        o.name = o.name & " >>" & Now
    Case "REPORTS"
        Set o = A.AppReports.Add
          o.Read src, pos
        o.name = o.name & " >>" & Now
    Case "MENUS"
        Set o = A.AppMenu.Add
          o.Read src, pos
        o.Caption = o.Caption & " >>" & Now
    Case "LAYS"
        Set o = A.AppLayouts.Add
          o.Read src, pos
        o.name = o.name & " >>" & Now
    End Select
    
    Screen.MousePointer = vbNormal
    frmTree.FillForm
ErrHandler:


End Sub

Private Sub mnuLoadObject_Click()
Dim src As String, pos As Long
    pos = 1
        With MDIForm1.CDlg
       ' Set CancelError is True
        .CancelError = True
        On Error GoTo ErrHandler
        ' Set flags
        .Flags = cdlOFNHideReadOnly + cdlOFNExplorer + cdlOFNExtensionDifferent + cdlOFNHelpButton + cdlOFNLongNames + cdlOFNNoReadOnlyReturn + cdlOFNOverwritePrompt
        .filename = NodeTag & "_" & NodeText
        .InitDir = A.SourcePath
        ' Set filters
        Select Case NodeTag
        Case "VAR"
          .Filter = "���������� (*.adv)|*.adv"
        Case "FORM"
          .Filter = "����� (*.adf)|*.adf"
        Case "REPORT"
          .Filter = "����� (*.adr)|*.adr"
        Case "MENU"
          .Filter = "���� (*.adm)|*.adm"
        Case "LAY"
          .Filter = "�������� (*.adl)|*.adl"
        End Select
        ' Display the Open dialog box
        .ShowOpen
          
        Screen.MousePointer = vbHourglass
        Open .filename For Input As 1
        src = Input(LOF(1), 1)
        Close 1
    End With
    
    Select Case NodeTag
    Case "VAR"
        A.AppVariables.Find(NodeText).Read src, pos
    Case "FORM"
        A.AppForms.Find(NodeText).Read src, pos
    Case "REPORT"
        A.AppReports.Find(NodeText).Read src, pos
    Case "MENU"
        A.AppMenu.Find(NodeText).Read src, pos
    Case "LAY"
        A.AppLayouts.Find(NodeText).Read src, pos
    End Select
    
    Screen.MousePointer = vbNormal
    frmTree.FillForm
ErrHandler:
End Sub

Private Sub mnuMakeDoc_Click()
  ' ������� �������� - �������� ���������
  Dim sss As String
  Dim sss1 As String
  
  Dim CTLNAME(0 To 13) As String
  
  Dim m As adi.AppMenu
  Dim sm As adi.AppMenu
  
  Dim f As adi.AppForm
  
  Dim btn As adi.Button
  
  Dim q As adi.Filter
  Dim ff As adi.FilterField
  
  Dim lh As adi.LayoutHolder
  Dim l As adi.Layout
  Dim lf As adi.Field
  
  Dim rh As adi.ReportHolder
  Dim r As adi.Report
  
  CTLNAME(0) = "�� �������������"
  CTLNAME(1) = "�����"
  CTLNAME(2) = "�����"
  CTLNAME(3) = "���������� ������ ��������� ��������"
  CTLNAME(4) = "������ ��������� ��������"
  CTLNAME(5) = "����"
  CTLNAME(6) = "�����"
  CTLNAME(7) = "���� �����"
  CTLNAME(8) = "��/���"
  CTLNAME(9) = "������������� �����"
  CTLNAME(10) = "������"
  CTLNAME(11) = "������������� �����"
  CTLNAME(12) = "�������"
  CTLNAME(13) = "������ ��������� �������� � ������������ ������ ���������� ��������"
  
  ' ���������
  sss = H("�������� ���������� " & A.Caption, 1)
  sss = sss & P("������ ���������� :" & A.Version)
  If A.StartAction <> "" Then
      sss = sss & FormCalls("��� �������", A.StartAction)
  End If
  
  ' ����
  sss = sss & H("��������� ����", 2)
  For Each m In A.AppMenu
    sss = sss & P(b(i(" " & m.Caption)))
    If m.SubMenu.Count <> 0 Then
      For Each sm In m.SubMenu
        sss = sss & UL(LI(i(sm.Caption))) & FormCalls("��� ������", sm.MenuAction)
      Next
    Else
      sss = sss & FormCalls("��� ������", m.MenuAction)
    End If
  Next
  
  '�����
  sss = sss & H("�������� ���� ����������", 2)
  For Each f In A.AppForms
    sss = sss & H("����� :""" & f.Caption & """", 3)
    If f.StartAction <> "" Then
      sss = sss & FormCalls("��� �������", f.StartAction)
    End If
    If f.CloseAction <> "" Then
      sss = sss & FormCalls("��� ��������", f.CloseAction)
    End If
    
    ' ������
    If f.Buttons.Count <> 0 Then
      sss = sss & H("�������� ��������� �� ����� (������)", 4)
      sss1 = ""
      For Each btn In f.Buttons
        sss = sss & UL(LI(b(btn.Caption) & i(" (" & btn.ToolTip & ")")))
        If btn.Action <> "" Then
          sss = sss & FormCalls("��� �������", btn.Action)
        End If
      Next
      sss = sss & sss1
    End If
    
    ' ������� ����������
    If f.FrmFilters.Count <> 0 Then
      sss = sss & H("������� ����������", 5)
      For Each q In f.FrmFilters
        sss = sss & H(q.Caption, 6)
        If q.FilterTip <> "" Then sss = sss & P(q.FilterTip)
        If q.Action <> "" Then
          sss = sss & FormCalls("����� �����������", q.Action)
        End If
        
        sss1 = ROW(HCELL("�������� ����", "") & _
        HCELL("������ �������������", "") & _
        HCELL("��������� ������ ��������", "") & _
        HCELL("����������", ""), "")
        
        For Each ff In q.FilterFields
          If ff.ControlType <> 0 Then
            sss1 = sss1 & ROW( _
            CELL(ff.Caption, "") & CELL(CTLNAME(ff.ControlType), "") & _
             CELL( _
             IIf(ff.AllowNull, "��", "���"), "" _
             ) & _
             CELL( _
             " ", "" _
             ), "" _
             )
          End If
        Next
        sss = sss & TABLE(sss1, " border=2 ")
      Next
    End If
    
    ' ������� ���������
    If f.FrmLayouts.Count <> 0 Then
      sss = sss & H("������� ���������", 4)
      For Each lh In f.FrmLayouts
        Set l = A.AppLayouts.Find(lh.LayoutName)
        sss = sss & H(lh.Caption & "(" & l.Caption & ")", 5)
        
        If Not l Is Nothing Then
          sss = sss & P(b("���������� �������� ��� ������� ���������:"))
          sss1 = ""
          If l.ROWINSERT <> "" Then sss1 = sss1 & LI("���� ������")
          If l.ROWUPDATE <> "" Then sss1 = sss1 & LI("�������������� ������")
          If l.ROWDELETE <> "" Then sss1 = sss1 & LI("�������� ������")
          If sss1 <> "" Then
            sss = sss & UL(sss1)
          Else
            sss = sss & P("���������� �������� �����������")
          End If
        
          sss = sss & P(b("������ ����� � ������� ���������"))
          sss1 = ""
          For Each lf In l.Columns
            If lf.Visible Then sss1 = sss1 & LI(lf.Caption)
          Next
          sss = sss & UL(sss1)
          If l.ROWINSERT <> "" Or l.ROWUPDATE <> "" Then
            sss = sss & P(b("������ ����� � ����� ��������������"))
            sss1 = ROW(HCELL("�������� ����", "") & _
            HCELL("������ �������������", "") & _
            HCELL("����������� �����", "") & _
            HCELL("��������� ������ ��������", "") & _
            HCELL("�������� ������ ��� �������� ������", "") & _
            HCELL("����������", ""), "")
            For Each lf In l.Columns
              If lf.ControlType <> 0 Then
                sss1 = sss1 & ROW( _
                CELL(lf.Caption, "") & CELL(CTLNAME(lf.ControlType), "") & _
                CELL( _
                 IIf(IsStringType(lf.FTYPE) And (lf.FScale > 0), CStr(lf.FScale), "���"), "" _
                 ) & _
                 CELL( _
                 IIf(lf.AllowNull, "��", "���"), "" _
                 ) & _
                 CELL( _
                 IIf(lf.InsertOnly, "��", "���"), "" _
                 ) & _
                 CELL( _
                 " ", "" _
                 ), "" _
                 )
              End If
            Next
            sss = sss & TABLE(sss1, " border=2 ")
          End If
        End If
      Next
    End If
    
    If f.FrmReports.Count <> 0 Then
      sss = sss & H("������ ����������� �� �����", 3)
      sss1 = ""
      For Each rh In f.FrmReports
        sss1 = sss1 & LI(rh.Caption)
      Next
      sss = sss & UL(sss1)
    End If
  Next
  HTMLSaveShow App.Path & "\" & A.AppFile & ".htm", HTML("�������� ���������� " & A.Caption & " ������ " & A.Version, sss)
End Sub

Private Sub mnuRefresh_Click()
  frmTree.FillForm
End Sub

Private Sub mnuRefresh2_Click()
  mnuRefresh_Click
End Sub

Private Sub mnuSaveCollection_Click()
Dim src As String
 With MDIForm1.CDlg
       ' Set CancelError is True
        .CancelError = True
        On Error GoTo ErrHandler
        ' Set flags
        .Flags = cdlOFNHideReadOnly + cdlOFNExplorer + cdlOFNExtensionDifferent + cdlOFNHelpButton + cdlOFNLongNames + cdlOFNNoReadOnlyReturn + cdlOFNOverwritePrompt
        .filename = NodeTag
        .InitDir = A.SourcePath
        ' Set filters
        Select Case NodeTag
        Case "VARS"
          .Filter = "��������� ���������� (*.acv)|*.acv"
        Case "FORMS"
          .Filter = "��������� ���� (*.acf)|*.acf"
        Case "REPORTS"
          .Filter = "��������� ������� (*.acr)|*.acr"
        Case "MENUS"
           .Filter = "��������� ���� (*.acm)|*.acm"
        Case "LAYS"
            .Filter = "��������� ���������� (*.acl)|*.acl"
        End Select
        ' Display the Open dialog box
        .ShowSave
        
        
         Select Case NodeTag
        Case "VARS"
            src = A.AppVariables.oWrite
        Case "FORMS"
            src = A.AppForms.oWrite
        Case "REPORTS"
            src = A.AppReports.oWrite
        Case "MENUS"
            src = A.AppMenu.oWrite
        Case "LAYS"
            src = A.AppLayouts.oWrite
        End Select
               
        Open .filename For Output As #1
        Print #1, src
        Close #1
    End With
ErrHandler:
    'cancel pressed
End Sub

Private Sub mnuSaveObject_Click()
 Dim src As String
 With MDIForm1.CDlg
       ' Set CancelError is True
        .CancelError = True
        On Error GoTo ErrHandler
        ' Set flags
        .Flags = cdlOFNHideReadOnly + cdlOFNExplorer + cdlOFNExtensionDifferent + cdlOFNHelpButton + cdlOFNLongNames + cdlOFNNoReadOnlyReturn + cdlOFNOverwritePrompt
        .filename = NodeTag & "_" & NodeText
        .InitDir = A.SourcePath
        ' Set filters
        Select Case NodeTag
        Case "VAR"
          .Filter = "���������� (*.adv)|*.adv"
        Case "FORM"
          .Filter = "����� (*.adf)|*.adf"
        Case "REPORT"
          .Filter = "����� (*.adr)|*.adr"
        Case "MENU"
          .Filter = "���� (*.adm)|*.adm"
        Case "LAY"
          .Filter = "�������� (*.adl)|*.adl"
        End Select
        ' Display the Open dialog box
        .ShowSave
        
        
         Select Case NodeTag
        Case "VAR"
            src = A.AppVariables.Find(NodeText).oWrite
        Case "FORM"
            src = A.AppForms.Find(NodeText).oWrite
        Case "REPORT"
            src = A.AppReports.Find(NodeText).oWrite
        Case "MENU"
            src = A.AppMenu.Find(NodeText).oWrite
        Case "LAY"
            src = A.AppLayouts.Find(NodeText).oWrite
        End Select
               
        Open .filename For Output As #1
        Print #1, src
        Close #1
        
    End With
ErrHandler:
    'cancel pressed
End Sub


Public Function GetChildForm(s As String, cforms() As String, cmodes() As String) As Long
  Dim P, cnt As Long, CMODE(4) As String, pl, pr, s1, i
  CMODE(1) = "SHOW"
  CMODE(2) = "ACTIVATE"
  CMODE(3) = "REPORT"
  'CMODE(4) = "SHOWMODAL"
  cnt = 0
  P = 0
  If s = "" Then
    GetChildForm = 0
    Exit Function
  End If
  For i = 1 To 3
    Do
      P = InStr(P + 1, UCase(s), CMODE(i))
      If P > 0 Then
        pl = InStr(P + 1, UCase(s), "{")
        If pl > 0 Then
          pr = InStr(pl + 1, UCase(s), "}")
          If pr > 0 Then
            If pr - pl - 2 > 0 Then
              s1 = Trim(Mid(UCase(s), pl + 1, pr - pl - 2))
              If Left(s1, 1) = """" Then Left(s1, 1) = " "
              If Right(s1, 1) = """" Then Right(s1, 1) = " "
              s1 = Trim(s1)
              cnt = cnt + 1
              ReDim Preserve cforms(cnt)
              ReDim Preserve cmodes(cnt)
              cmodes(cnt) = CMODE(i)
              cforms(cnt) = s1
            End If
          End If
        End If
      End If
    Loop While P > 0 And P Is Not Null
  Next
  GetChildForm = cnt
End Function

Private Function FormCalls(msg As String, Action As String) As String
  Dim cforms() As String, cmodes() As String
  Dim s As String, s1 As String, i As Long, fc As Long
  s = ""
  fc = FormsCalled(Action, cforms(), cmodes())
  If fc > 0 Then
    If fc > 1 Then
      s = P(b(msg & " ���������� �������� �����:"))
      s1 = ""
      For i = 1 To fc
        'If cmodes(i) <> "REPORT" Then
          s1 = s1 & LI(A.AppForms.Find(cforms(i)).Caption)
        'End If
      Next
      s = s & UL(s1)
    Else
      s = P((msg & " ���������� �����: """ & A.AppForms.Find(cforms(1)).Caption) & """")
    End If
  End If
  FormCalls = s
End Function

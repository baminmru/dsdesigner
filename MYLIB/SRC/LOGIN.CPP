//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "LogIn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmLogin *frmLogin;
//---------------------------------------------------------------------------
__fastcall TfrmLogin::TfrmLogin(TComponent* Owner)
 : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogin::FormShow(TObject *Sender)
{
 Label1->Caption = "��������� ���������� � ���������� ������: '" + SourceName +
   "'. \r\n ������� ��� ������������ � ������.";
 txtUID->SetFocus();
};
//---------------------------------------------------------------------------



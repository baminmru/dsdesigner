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
 Label1->Caption = "«апрошено соединение с источником данных: '" + SourceName +
   "'. \r\n «адайте им€ пользовател€ и пароль.";
 txtUID->SetFocus();
};
//---------------------------------------------------------------------------



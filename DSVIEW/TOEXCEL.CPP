//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ToEXCEL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "ToolEdit"
#pragma resource "*.dfm"
TfrmToEXCEL *frmToEXCEL;
//---------------------------------------------------------------------------
__fastcall TfrmToEXCEL::TfrmToEXCEL(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmToEXCEL::Button1Click(TObject *Sender)
{
  SaveDialog1->FileName=excelName->Text;
  if(SaveDialog1->Execute()==mrOk)
     excelName->Text=SaveDialog1->FileName;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ToHTML.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "ToolEdit"
#pragma resource "*.dfm"
TfrmToHTML *frmToHTML;
//---------------------------------------------------------------------------
__fastcall TfrmToHTML::TfrmToHTML(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmToHTML::Button1Click(TObject *Sender)
{
  SaveDialog1->FileName=htmlName->Text;
  if(SaveDialog1->Execute()==mrOk)
     htmlName->Text=SaveDialog1->FileName;


}
//---------------------------------------------------------------------------


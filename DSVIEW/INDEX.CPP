//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Index.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "RXCtrls"
#pragma resource "*.dfm"
TfrmIndex *frmIndex;
//---------------------------------------------------------------------------
__fastcall TfrmIndex::TfrmIndex(TComponent* Owner)
 : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmIndex::lstIndexDblClick(TObject *Sender)
{
   ModalResult=mrOk; 
}
//---------------------------------------------------------------------------



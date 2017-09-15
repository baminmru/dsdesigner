//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Report.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CRPrintEngine"
#pragma resource "*.dfm"
TfrmReport *frmReport;
//---------------------------------------------------------------------------
__fastcall TfrmReport::TfrmReport(TComponent* Owner)
 : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmReport::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree; 
}
//---------------------------------------------------------------------------

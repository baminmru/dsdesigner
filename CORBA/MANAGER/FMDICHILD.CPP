//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "fMDIChild.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMDIChild *MDIChild;
//---------------------------------------------------------------------------
__fastcall TMDIChild::TMDIChild(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMDIChild::FormClose(TObject *Sender, TCloseAction &Action)
{
  Action = caFree;  
}
//---------------------------------------------------------------------------


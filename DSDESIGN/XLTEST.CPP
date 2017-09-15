//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "xltest.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Excels"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
 : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
   Excel1->Connect();
   Excel1->InsertAt(1,2,"Пошел ты в пень");
   Excel1->Exec("[SAVE()]");
   Excel1->Disconnect();
}
//---------------------------------------------------------------------------
 
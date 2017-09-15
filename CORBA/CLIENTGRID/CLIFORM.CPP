//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <corba.h>
#include "CliForm.h"
#include "Convertions.h"
#include "stackvalue.h"
#include "mystream.h"
#include "CartrigeObjects.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


Cartrige_ptr __fastcall TForm1::Getcartrige(void)
{
        if (Fcartrige == NULL)
        {
                 Fcartrige = Cartrige::_bind("ODBC");
        }
        return Fcartrige;
}

void __fastcall TForm1::Setcartrige(Cartrige_ptr _ptr)
{
        Fcartrige = _ptr;
}
void __fastcall TForm1::cmdCallClick(TObject *Sender)
{
  try
  {
     MyArray<CartrigeParam> ps;
     CartrigeParam *p;
     p = new CartrigeParam;
     p->Name="DSN";
     p->Value.Set(stString,"aisbp99");
     ps.Add(p);

     p = new CartrigeParam;
     p->Name="UID";
     p->Value.Set(stString,"sa");
     ps.Add(p);

     p = new CartrigeParam;
     p->Name="PWD";
     p->Value.Set(stString,"");
     ps.Add(p);

     p = new CartrigeParam;
     p->Name="NAME";
     p->Value.Set(stString,"aisbp99");
     ps.Add(p);

     p = new CartrigeParam;
     p->Name="TIMEOUT";
     p->Value.SetN(0);
     ps.Add(p);

     MyMemory m ;
     m  << ps;


     long id=cartrige->NewInstance();
     cartrige->Connect(id,*MakeOctets(m));

     if(cartrige->IsConnected(id)){
        long curs = cartrige->OpenCursor(id, txtSQL->Text.c_str(),*MakeOctets(m));
        long cnt=0;
	Octets *oc;
	if(curs){
                Caption = txtSQL->Text;
                Application->ProcessMessages();
                oc = cartrige->GetCursorHeared(id,curs);

                MyMemory * mm =  MakeMemory(*oc);
                MyArray<FieldHeader> fh;
                *mm >> fh;
                delete mm;
                delete oc;
                SG->Visible = false;
                SG->RowCount=2;
                SG->ColCount=fh.Count();
     		for(long i=0;i<fh.Count();i++){
                 SG->Cells[i][0]= fh[i]->Name;
		}
                AnsiString s;
	        while(!cartrige->NoMoreRows(id,curs)){
		        oc=cartrige->GetCursorRow(id,curs);
			mm =MakeMemory(*oc);
			MyArray<StackValue> r;
			*mm>>r;
                        delete mm;
                        delete oc;
			cnt++;
                        SG->RowCount=1+cnt;

			for(long i=0;i<r.Count();i++){
                           s= r[i]->ToStr();
               		   SG->Cells[i][cnt]=s;
			}
		};

		cartrige->CloseCursor(id,curs);
                SG->Visible = true;
	}
		cartrige->DisConnect(id);

      }

           cartrige->CloseInstance(id);
     }

     catch(const CORBA::Exception& e)
     {
		 cerr << e << endl;
                SG->Visible = true;
     }

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
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

void __fastcall TForm1::Button1Click(TObject *Sender)
{

  lines.Clear();
  TRect  r= Form1->GetClientRect();
  long dx =r.Right -r.Left;
  long dy=r.Bottom-r.Top;
  MyLine *l;
  for(long i=0;i<10;i++){
    l = new MyLine();
    l->F.X= rand()%dx;
    l->T.X= rand()%dx;
    l->F.Y= rand()%dy;
    l->T.Y= rand()%dy;
    l->Select = 0;
    lines.Add(l);
 }
 Invalidate();
}


void __fastcall TForm1::PaintTo(HDC DC, int X, int Y){
  TForm::PaintTo(DC, X,Y);

};
//---------------------------------------------------------------------------
void __fastcall TForm1::FormPaint(TObject *Sender)
{
  for(long i=0;i<lines.Count();i++){
    if(!lines[i]->Select)
      Canvas->Pen->Color = (Graphics::TColor) 0;
    else
      Canvas->Pen->Color = (Graphics::TColor) 255;
    Canvas->MoveTo(lines[i]->F.X,lines[i]->F.Y);
    Canvas->LineTo(lines[i]->T.X,lines[i]->T.Y);
  };

}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
  Button1Click(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  for(long i=0;i<lines.Count();i++){
    lines[i]->Select=IsClicked(lines[i],X,Y)
  };
  Invalidate();
}
//---------------------------------------------------------------------------
bool TForm1::IsClicked(MyLine * l,long X,long Y){
  if(!l) return 0;
  l->


};

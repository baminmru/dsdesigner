//---------------------------------------------------------------------------
#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "stackvalue.h"

//---------------------------------------------------------------------------
struct  MyPoint{
  long X;
  long Y;
};

struct  MyLine{
  MyPoint F;
  MyPoint T;
  bool Select;
};


class TForm1 : public TForm
{
__published:	// IDE-managed Components
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall FormPaint(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
  void __fastcall PaintTo(HDC DC, int X, int Y);
  bool IsClicked(MyLine*l,long X,long Y);
public:		// User declarations
 MyArray<MyLine> lines;
  __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

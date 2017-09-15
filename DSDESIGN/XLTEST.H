//---------------------------------------------------------------------------
#ifndef xltestH
#define xltestH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Excels.hpp"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
 TAdvExcel *AdvExcel1;
 TExcel *Excel1;
 TBitBtn *BitBtn1;
 void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
 __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

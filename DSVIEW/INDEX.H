//---------------------------------------------------------------------------
#ifndef IndexH
#define IndexH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include "RXCtrls.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmIndex : public TForm
{
__published:	// IDE-managed Components

 TPanel *Panel1;
 TButton *Button1;
 TButton *Button2;
  TListBox *lstIndex;
 void __fastcall lstIndexDblClick(TObject *Sender);
  
private:	// User declarations
public:		// User declarations
 __fastcall TfrmIndex(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmIndex *frmIndex;
//---------------------------------------------------------------------------
#endif

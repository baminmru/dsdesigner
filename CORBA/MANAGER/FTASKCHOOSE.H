//---------------------------------------------------------------------------
#ifndef fTaskChooseH
#define fTaskChooseH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmTaskChoose : public TForm
{
__published:	// IDE-managed Components
  TButton *btnOK;
  TButton *tbnExit;
  TListBox *lstTask;
private:	// User declarations
public:		// User declarations
  __fastcall TfrmTaskChoose(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmTaskChoose *frmTaskChoose;
//---------------------------------------------------------------------------
#endif

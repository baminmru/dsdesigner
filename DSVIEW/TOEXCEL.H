//---------------------------------------------------------------------------
#ifndef ToEXCELH
#define ToEXCELH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include "ToolEdit.hpp"
#include <Mask.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TfrmToEXCEL : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label1;
  TButton *Button2;
  TButton *btnSave;
  TEdit *excelName;
  TSaveDialog *SaveDialog1;
  TButton *Button1;
  void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmToEXCEL(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmToEXCEL *frmToEXCEL;
//---------------------------------------------------------------------------
#endif

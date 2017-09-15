//---------------------------------------------------------------------------
#ifndef ToHTMLH
#define ToHTMLH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include "ToolEdit.hpp"
#include <Mask.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TfrmToHTML : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label1;
  TButton *btnSave;
  TButton *Button2;
  TEdit *htmlName;
  TButton *Button1;
  TSaveDialog *SaveDialog1;
  void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmToHTML(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmToHTML *frmToHTML;
//---------------------------------------------------------------------------
#endif

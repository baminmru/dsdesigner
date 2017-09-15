//---------------------------------------------------------------------------
#ifndef ChooseTaskH
#define ChooseTaskH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "task.h"
//nclude "RXCtrls.hpp"
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TfrmChooseTask : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TButton *btnCancel;
  TButton *btnOk;
  TListBox *lstTask;
  //TTextListBox *lstTask;
 void __fastcall btnOkClick(TObject *Sender);
  void __fastcall lstTaskDblClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
 __fastcall TfrmChooseTask(TComponent* Owner);
 LMEAppDef *App;
 AnsiString TaskName;
 void __fastcall Init();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmChooseTask *frmChooseTask;
//---------------------------------------------------------------------------
#endif

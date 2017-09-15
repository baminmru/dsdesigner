//---------------------------------------------------------------------------
#ifndef LoadObjH
#define LoadObjH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include "task.h"
//---------------------------------------------------------------------------
class TfrmLoadObject : public TForm
{
__published:	// IDE-managed Components
  TDirectoryListBox *lstDirectory;
  TFileListBox *lstFile;
  TPanel *Panel1;
  TSplitter *Splitter1;
  TDriveComboBox *DriveComboBox1;
  TButton *Button1;
  void __fastcall lstDirectoryChange(TObject *Sender);
  void __fastcall DriveComboBox1Change(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
   AnsiString TaskPath;
public:		// User declarations
  LMEAppDef *App;
  __fastcall TfrmLoadObject(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLoadObject *frmLoadObject;
//---------------------------------------------------------------------------
#endif

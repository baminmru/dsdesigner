//---------------------------------------------------------------------------
#ifndef ObjectListH
#define ObjectListH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include "RXCtrls.hpp"
#include <ExtCtrls.hpp>
//#include "ToolEdit.hpp"
#include <Mask.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TfrmObjectList : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  //TListBox *lstObj;
  TButton *Button1;
  TButton *Button2;
  TDriveComboBox *DriveComboBox1;
  TDirectoryListBox *txtDir;
  TListBox *lstObj;
  //TDirectoryEdit *txtDir;
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall DriveComboBox1Change(TObject *Sender);
private:	// User declarations
  LMEAppDef *App;
public:		// User declarations
  __fastcall TfrmObjectList(TComponent* Owner);
  void __fastcall Init(LMEAppDef *_App);

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmObjectList *frmObjectList;
//---------------------------------------------------------------------------
#endif

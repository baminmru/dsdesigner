//---------------------------------------------------------------------------
#ifndef MenuEDitorH
#define MenuEDitorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "MenuDef.h"

//---------------------------------------------------------------------------
class TfrmMenuEdit : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TTreeView *tvMenu;
  TSplitter *Splitter1;
  TPanel *Panel2;
  TButton *Button1;
  TButton *Button2;
  TButton *Button3;
  TListBox *lstCalls;
  TCheckBox *chkCallModal;
  TComboBox *cmbObjects;
  TButton *btnAddCall;
  TButton *btnDelCall;
  TButton *btnUp;
  TButton *btnDown;
  TEdit *txtCaption;
  TButton *Button4;
  TButton *Button5;
  void __fastcall cmbObjectsClick(TObject *Sender);
  void __fastcall lstCallsClick(TObject *Sender);
  void __fastcall tvMenuClick(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall Button2Click(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);
  void __fastcall txtCaptionChange(TObject *Sender);
  void __fastcall btnAddCallClick(TObject *Sender);
  void __fastcall btnDelCallClick(TObject *Sender);
private:	// User declarations
  MyArray<MenuDef> md;
  bool InTvClick;
  bool InLstClick;
public:		// User declarations

  void SetMenuDef(DataBuffer * db);
  void SetCombo(MyArray<AnsiString> *ss);

  DataBuffer *GetMenuDef();
  __fastcall TfrmMenuEdit(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMenuEdit *frmMenuEdit;
//---------------------------------------------------------------------------
#endif

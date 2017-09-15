//---------------------------------------------------------------------------
#ifndef EditBuildH
#define EditBuildH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "Manager.h"

struct bei{
  AnsiString DataSourceName;
  AnsiString TargetName;
  CParam Info;
};

//---------------------------------------------------------------------------
class TfrmEditBuild : public TForm
{
__published:	// IDE-managed Components
  TComboBox *cmbSources;
  TListBox *lstParam;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TEdit *txtValue;
  TPanel *Panel1;
  TButton *Button1;
  TButton *Button2;
  TComboBox *cmbTarget;
  TLabel *Label4;
  void __fastcall cmbSourcesClick(TObject *Sender);
  void __fastcall cmbTargetChange(TObject *Sender);
  void __fastcall lstParamClick(TObject *Sender);
  void __fastcall txtValueChange(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
  AnsiString System;
  AnsiString Build;
  MyArray<bei> BEI;
  bool InClick;
public:		// User declarations
  void SetBuild(
    AnsiString System,
    AnsiString Build
  );

  __fastcall TfrmEditBuild(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEditBuild *frmEditBuild;
//---------------------------------------------------------------------------
#endif

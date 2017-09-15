//---------------------------------------------------------------------------
#ifndef Form1H
#define Form1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "bamilib.h"
//#include "RXCtrls.hpp"
//---------------------------------------------------------------------------
class TfrmChanges : public TForm
{
__published:	// IDE-managed Components
  TEdit *txtDefName;
  TCheckBox *chkDef;
  TEdit *txtDefValue;
  TGroupBox *UpdateAccess;
  TRadioButton *UpdNoAccess;
  TRadioButton *UpdReadOnly;
  TRadioButton *UpdReadWrite;
  TGroupBox *InsertAccess;
  TRadioButton *InsNoAccess;
  TRadioButton *InsReadOnly;
  TRadioButton *InsReadWrite;
  TEdit *txtField;
  TCheckBox *chkParam;
  TEdit *txtParamName;
  TEdit *txtParamValue;
  TButton *btnAddDef;
  TButton *btnDelDef;
  TButton *btnAddAcces;
  TButton *btnDelAccess;
  TButton *btnAddParam;
  TButton *btnDelParam;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TEdit *txtSubCondition;
  TButton *btnOK;
 //TTextListBox *lstDef;
 //TTextListBox *lstAccess;
 //TTextListBox *lstParam;
  TListBox *lstDef;
  TListBox *lstAccess;
  TListBox *lstParam;
  void __fastcall btnAddDefClick(TObject *Sender);
  void __fastcall btnAddAccesClick(TObject *Sender);
  void __fastcall btnAddParamClick(TObject *Sender);
  void __fastcall lstParamClick(TObject *Sender);
  void __fastcall lstDefClick(TObject *Sender);
  void __fastcall lstAccessClick(TObject *Sender);
 void __fastcall btnDelDefClick(TObject *Sender);
 void __fastcall btnDelAccessClick(TObject *Sender);
 void __fastcall btnDelParamClick(TObject *Sender);
 void __fastcall txtDefNameChange(TObject *Sender);
 void __fastcall txtFieldChange(TObject *Sender);
 void __fastcall txtParamNameChange(TObject *Sender);
 void __fastcall btnOKClick(TObject *Sender);
 void __fastcall txtSubConditionChange(TObject *Sender);
 
private:	// User declarations

    bool OnParam;
    bool OnAccess;
    bool OnDef;

public:		// User declarations
    EditFieldActions *Actions;
    DataManager *Manager;
    void __fastcall Prepare();
  __fastcall TfrmChanges(TComponent* Owner);
};


//---------------------------------------------------------------------------
#endif

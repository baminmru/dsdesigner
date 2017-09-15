//---------------------------------------------------------------------------
#ifndef _FilterDlgH
#define _FilterDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "bamilib.h"
#include <checklst.hpp>
#include "RXCtrls.hpp"

//---------------------------------------------------------------------------
class TFilterDlg : public TForm
{
__published:	// IDE-managed Components
	TComboBox *cmbOperation;
	TEdit *Edit1;
	TEdit *Edit2;
	TLabel *Label2;
	TLabel *Label1;
	TLabel *Label3;
	TButton *BtnOK;
	TButton *Button1;
	TButton *Button2;
	TRxCheckListBox *FieldList;
	TButton *Button3;
	void __fastcall cmbOperationClick(TObject *Sender);
	void __fastcall FieldListClick(TObject *Sender);
	void __fastcall Edit1Change(TObject *Sender);
	void __fastcall Edit2Change(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	FilterDef Storage;
	__fastcall TFilterDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFilterDlg *FilterDlg;
//---------------------------------------------------------------------------
#endif

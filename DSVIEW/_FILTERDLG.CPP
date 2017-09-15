//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "bamilib.h"

#include "_FilterDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RXCtrls"
#pragma resource "*.dfm"
TFilterDlg *FilterDlg;
//---------------------------------------------------------------------------
__fastcall TFilterDlg::TFilterDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFilterDlg::cmbOperationClick(TObject *Sender)
{
	if(FieldList->ItemIndex==-1) return;
	if(cmbOperation->ItemIndex==7){
		Edit2->Enabled=true;
		Label3->Enabled=true;
	}else{
		Edit2->Enabled=false;
		Label3->Enabled=false;
	}
	if(cmbOperation->ItemIndex>=0)
		Storage[FieldList->ItemIndex]->Condition=(FilterCondition)cmbOperation->ItemIndex;

	if(cmbOperation->ItemIndex>0)
		FieldList->State[FieldList->ItemIndex]=cbChecked;
	else
		FieldList->State[FieldList->ItemIndex]=cbUnchecked;


}
//---------------------------------------------------------------------------
void __fastcall TFilterDlg::FieldListClick(TObject *Sender)
{
 /*struct FilterLineDef{
	AnsiString Name;          // имя поля из DataStream !!!
	FilterCondition Condition;
	StackValue From;
	StackValue To;
 };
 */

 if(FieldList->ItemIndex>=0){
	cmbOperation->ItemIndex=Storage[FieldList->ItemIndex]->Condition;
	Edit1->Text=Storage[FieldList->ItemIndex]->From.ToStr();
	Edit1->Text=Storage[FieldList->ItemIndex]->To.ToStr();

	for(long i=0;i<FieldList->Items->Count;i++)
	if(Storage[i]->Condition==fcNone)
		FieldList->State[i]=cbUnchecked;
	else
		FieldList->State[i]=cbChecked;
 }

}
//---------------------------------------------------------------------------
void __fastcall TFilterDlg::Edit1Change(TObject *Sender)
{
	Storage[FieldList->ItemIndex]->From=Edit1->Text;
}
//---------------------------------------------------------------------------
void __fastcall TFilterDlg::Edit2Change(TObject *Sender)
{
	Storage[FieldList->ItemIndex]->To=Edit2->Text;
}
//---------------------------------------------------------------------------
void __fastcall TFilterDlg::Button1Click(TObject *Sender)
{
	for(int i=0;i<FieldList->Items->Count;i++){
		FieldList->State[i]=cbUnchecked;
		Storage[i]->Condition=fcNone;
		Storage[i]->From=StackValue("");
		Storage[i]->To=StackValue("");
	}
}
//---------------------------------------------------------------------------
void __fastcall TFilterDlg::Button3Click(TObject *Sender)
{
	cmbOperation->ItemIndex=0;
	cmbOperationClick(Sender);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "_SortDlg.h"
#pragma package(smart_init)
//#pragma link "CurrEdit"
//#pragma link "ToolEdit"
#pragma link "RXSpin"
#pragma resource "*.dfm"

//---------------------------------------------------------------------
__fastcall TsortDlg::TsortDlg(Classes::TComponent* AOwner)
	: TForm(AOwner)
{
	SortList->Items->Clear();
}
//---------------------------------------------------------------------

void __fastcall TsortDlg::SortListDblClick(TObject *Sender)
{
//(cbChecked), deselected (cbUnchecked), or grayed (cbGrayed).
			/*if(SortList->ItemIndex==-1) return;

			 switch (SortList->State[SortList->ItemIndex]){

			 }
			*/

}
//---------------------------------------------------------------------------


void __fastcall TsortDlg::BitBtn1Click(TObject *Sender)
{
	for(long i=0;i<SortList->Items->Count;i++){
		SortList->State[i]=cbGrayed;
	}
}
//---------------------------------------------------------------------------





void __fastcall TsortDlg::RxSpinButton1BottomClick(TObject *Sender)
{
	// двигаем вниз
	if(SortList->Items->Count && SortList->ItemIndex>=0 && SortList->ItemIndex<SortList->Items->Count-1 ){
		Stdctrls::TCheckBoxState s1=SortList->State[SortList->ItemIndex];
		Stdctrls::TCheckBoxState s2=SortList->State[SortList->ItemIndex+1];
		AnsiString n1 =SortList->Items->Strings[SortList->ItemIndex];
		AnsiString n2 =SortList->Items->Strings[SortList->ItemIndex+1];
		SortList->Items->Strings[SortList->ItemIndex]=n2;
		SortList->Items->Strings[SortList->ItemIndex+1]=n1;
		SortList->State[SortList->ItemIndex+1]=s1;
		SortList->State[SortList->ItemIndex]=s2;
		SortList->ItemIndex++;
	}
}
//---------------------------------------------------------------------------

void __fastcall TsortDlg::RxSpinButton1TopClick(TObject *Sender)
{
	// двигаем вверх
	if(SortList->Items->Count && SortList->ItemIndex>0){
		Stdctrls::TCheckBoxState s1=SortList->State[SortList->ItemIndex];
		Stdctrls::TCheckBoxState s2=SortList->State[SortList->ItemIndex-1];
		AnsiString n1 =SortList->Items->Strings[SortList->ItemIndex];
		AnsiString n2 =SortList->Items->Strings[SortList->ItemIndex-1];
		SortList->Items->Strings[SortList->ItemIndex]=n2;
		SortList->Items->Strings[SortList->ItemIndex-1]=n1;
		SortList->State[SortList->ItemIndex-1]=s1;
		SortList->State[SortList->ItemIndex]=s2;
		SortList->ItemIndex--;
	}

}
//---------------------------------------------------------------------------


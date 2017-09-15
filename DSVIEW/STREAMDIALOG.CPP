//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "StreamGrid.h"
#include "StreamDialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TStreamDlg *StreamDlg;
//---------------------------------------------------------------------------
__fastcall TStreamDlg::TStreamDlg(TComponent* Owner)
	: TForm(Owner)
{
	StreamGrid1 = new TStreamGrid(Owner);
    ((TStreamGrid*)StreamGrid1)->Parent = GridPanel;
    ((TStreamGrid*)StreamGrid1)->Align= alClient;
    //((TStreamGrid*)StreamGrid1)->OnDblClick=Button1Click;
 TranIdx=0;
}

void TStreamDlg::ActivateEditor(){
    // установки параметров редактора
	((TStreamGrid*)StreamGrid1)->SetEditor(Manager,Editor);
    // установка состояния параметров
    ((TStreamGrid*)StreamGrid1)->InitSlave(Actions,CurrentRowState);
    // создание Snap и заполнение
    ((TStreamGrid*)StreamGrid1)->RunEditor();
     ((TStreamGrid*)StreamGrid1)->OnDblClick=Button1Click;
};

StackValue* TStreamDlg::Lookup(AnsiString SyncFieldName,  AnsiString ShowFieldName, AnsiString Text){
     StackValue *v=new StackValue(Text);
     if( ((TStreamGrid*)StreamGrid1)->Sync(ShowFieldName,v) ){
        delete v;
        v=((TStreamGrid*)StreamGrid1)->GetValue(SyncFieldName);
     	return v;
     }
     delete v;
     return 0;
};

AnsiString TStreamDlg::LookupText( AnsiString ShowFieldName, AnsiString Text){
     StackValue *v=new StackValue(Text);
     if( ((TStreamGrid*)StreamGrid1)->Sync(ShowFieldName,v) ){
        delete v;
        v=((TStreamGrid*)StreamGrid1)->GetValue(ShowFieldName);
     	return v->ToStr();
     }
     delete v;
     return "";
};


//---------------------------------------------------------------------------
TModalResult TStreamDlg::RunDialog(AnsiString *TextToFind){

		((TStreamGrid*)StreamGrid1)->TabStop=true;

		// синхронизация
		((TStreamGrid*)StreamGrid1)->Sync(SyncFieldName,&Value);

    Caption =((TStreamGrid*)StreamGrid1)->GetStreamEditor()->Caption +" (Выбор строки)";

    if(TextToFind)
      ((TStreamGrid*)StreamGrid1)->FindData=*TextToFind;
    else
      ((TStreamGrid*)StreamGrid1)->FindData="";

		// запуск формы
    return ShowModal();
};
//---------------------------------------------------------------------------
StackValue TStreamDlg::GetValue( AnsiString FieldName){
	StackValue *ptr =((TStreamGrid*)StreamGrid1)->GetValue(FieldName);
    if(ptr)
    	return *ptr;
    return StackValue();
};
//---------------------------------------------------------------------------


void __fastcall TStreamDlg::Button1Click(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------



void __fastcall TStreamDlg::FormShow(TObject *Sender)
{

   HeaderControl2->Sections->Clear();
   for(long i=0;i<((TStreamGrid*)StreamGrid1)->GetManager()->Trans.Count();i++){
    if(((TStreamGrid*)StreamGrid1)->GetEditorName()==
       ((TStreamGrid*)StreamGrid1)->GetManager()->Trans[i]->EditorName){
        long j=0;
        while( j<HeaderControl2->Sections->Count &&
               HeaderControl2->Sections->Items[j]->Text <
               ((TStreamGrid*)StreamGrid1)->GetManager()->Trans[i]->Caption){
          j++;
        }
        THeaderSection *pSection;
        if( j<HeaderControl2->Sections->Count )
           pSection = (THeaderSection *)HeaderControl2->Sections->Insert(j);
        else
           pSection = HeaderControl2->Sections->Add();
        pSection->Text = ((TStreamGrid*)StreamGrid1)->GetManager()->Trans[i]->Caption;
        pSection->MinWidth=1;
        pSection->MaxWidth=pSection->Text.Length() * HeaderControl2->Font->Size+15;
        pSection->Width=pSection->Text.Length() * HeaderControl2->Font->Size+8;
        pSection->MinWidth = pSection->Text.Length() * HeaderControl2->Font->Size/3;
        pSection->Style = hsText;
        HeaderControl2->Width+=pSection->Width;
     }
   }
   if(HeaderControl2->Sections->Count>0){
     TranHolder->Visible =true;
   }else{
     TranHolder->Visible =false;
   }

   if(!((TStreamGrid*)StreamGrid1)->FindData.IsEmpty())
     ((TStreamGrid*)StreamGrid1)->Find();

   ((TStreamGrid*)StreamGrid1)->SetFocus();

}
//---------------------------------------------------------------------------

void __fastcall TStreamDlg::HeaderControl2SectionClick(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
  if(((TStreamGrid*)StreamGrid1)->GetStream()){
      ((TStreamGrid*)StreamGrid1)->RunTransaction(Section->Text);
  }
}
//---------------------------------------------------------------------------

void __fastcall TStreamDlg::btnTranRightClick(TObject *Sender)
{
    if(TranIdx<HeaderControl2->Sections->Count-1) TranIdx++;
    long l=0;
    for(long i=0;i<TranIdx;i++){
     l+=HeaderControl2->Sections->Items[i]->Width;
    }

    HeaderControl2->Left=-l;
    if(l)
     btnTranLeft->Enabled = true;
    else
     btnTranLeft->Enabled = false;

}
//---------------------------------------------------------------------------

void __fastcall TStreamDlg::btnTranLeftClick(TObject *Sender)
{
    if(TranIdx>0) TranIdx--;

    long l=0;
    for(long i=0;i<TranIdx;i++){
     l+=HeaderControl2->Sections->Items[i]->Width;
    }

    HeaderControl2->Left=-l;
    if(l)
     btnTranLeft->Enabled = true;
    else
     btnTranLeft->Enabled = false;
}

//---------------------------------------------------------------------------

void __fastcall TStreamDlg::HeaderControl2SectionResize(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
 long l=0;
 for(long i=0;i<HeaderControl->Sections->Count;i++){
   l+=HeaderControl->Sections->Items[i]->Width;
 }
 HeaderControl->Width=l;
 Panel6Resize(HeaderControl);
}
//---------------------------------------------------------------------------

void __fastcall TStreamDlg::Panel6Resize(TObject *Sender)
{
 if(HeaderControl2->Width<=Panel6->Width){
    HeaderControl2->Left=0;
    btnTranRight->Enabled = false;
    btnTranLeft->Enabled = false;
    TranIdx=0;
 }else{
     btnTranRight->Enabled = true;
     if(TranIdx>0)
      btnTranLeft->Enabled = true;
     else
      btnTranLeft->Enabled = false;
 }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChooseTask.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "RXCtrls"
#pragma resource "*.dfm"
TfrmChooseTask *frmChooseTask;
//---------------------------------------------------------------------------
__fastcall TfrmChooseTask::TfrmChooseTask(TComponent* Owner)
 : TForm(Owner)
{
  App=0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmChooseTask::Init(){
 if(!App) return;
 long idx=-1;
 lstTask->Items->Clear();
 for(long i=0;i< App->Tasks.Count();i++){
   lstTask->Items->Add(App->Tasks[i]->Caption);
   if(App->Tasks[i]->Name==TaskName)
    idx =i;
 }
 lstTask->ItemIndex=idx;
};
void __fastcall TfrmChooseTask::btnOkClick(TObject *Sender)
{
 if(lstTask->ItemIndex>=0){
   TaskName=App->Tasks[lstTask->ItemIndex]->Name;
   ModalResult = mrOk;
 }else
  TaskName="";

}
//---------------------------------------------------------------------------

void __fastcall TfrmChooseTask::lstTaskDblClick(TObject *Sender)
{
      btnOkClick(Sender);

}
//---------------------------------------------------------------------------

void __fastcall TfrmChooseTask::FormShow(TObject *Sender)
{
  Init();
}
//---------------------------------------------------------------------------


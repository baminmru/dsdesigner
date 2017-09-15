//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "fMDIMain.h"
#include "Login.h"
#include "fTaskChoose.h"
#include "Manager.h"
#include "fTree.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMDIMain *MDIMain;

//---------------------------------------------------------------------------
__fastcall TMDIMain::TMDIMain(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMDIMain::FormCreate(TObject *Sender)
{
  if(!man)
    man = new ObjectManager();

  TfrmLogin * login = new TfrmLogin(this);
  do{
  if(login->ShowModal()!=mrOk){
    delete login;
    Application->Terminate();
  };
  try{
    if(man->LogOn(login->txtUID->Text.c_str(),login->txtPWD->Text.c_str()))
     break;
    }catch(...){}
  }while(1);
  delete login;



  TfrmTaskChoose * task = new TfrmTaskChoose(this);
  task->lstTask->Items->Clear();
  MyArray<Build> *b=man->GetBuilds();
  if(b){
    for(long i=0;i<b->Count();i++){
      task->lstTask->Items->Add(b->Get(i)->SystemName+" "+b->Get(i)->BuildName);
    }

  }


  do{
    if(task->ShowModal()!=mrOk){
      man->LogOff();
      delete man;
      delete task;
      delete b;
      Application->Terminate();
    };
  }while(task->lstTask->ItemIndex==-1);

  MyMemory m;
  m << (*b->Get(task->lstTask->ItemIndex));
  delete b;
  Build bld;
  m >> bld;
  man->RunBuild(&bld);

}
//---------------------------------------------------------------------------
void __fastcall TMDIMain::FormClose(TObject *Sender, TCloseAction &Action)
{
  try{
  if(man){
    man->LogOff();
    delete man ;
    man=0;
  }
  }catch(...) {};
}
//---------------------------------------------------------------------------

void __fastcall TMDIMain::FormDblClick(TObject *Sender)
{
  TfrmTree* frmTree = new TfrmTree(this);
  frmTree->FillTree();
  frmTree->Show();
}
//---------------------------------------------------------------------------


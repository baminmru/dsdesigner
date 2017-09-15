//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Navigator.h"
#include "DSDesigner.h"
#include "TaskEditor.h"
#include "ChooseTask.h"
#include "SlaveGrid.h"
#include "About.h"
#include "Filter.h"
#include "Report.h"
#include "DBSave.h"
#include "HTML.h"
#include "ObjectList.h"
#include "LoadObj.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLMENavigator *LMENavigator;
//---------------------------------------------------------------------------
__fastcall TLMENavigator::TLMENavigator(TComponent* Owner)
 : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::mnuExitClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::mnuLoadClick(TObject *Sender)
{
 if(OpenDialog1->Execute()){
   AppFile=OpenDialog1->FileName;
   MyFile f(AppFile.c_str(),acRead);

   for(int i = 0; i < MDIChildCount; i++)
    		MDIChildren[i]->Close();
   if(MainMenu1->Items->Count>3){
    while(MainMenu1->Items->Count>3){
     MainMenu1->Items->Delete(0);
    }
   }

   StreamSavedFile::PathOnly = false;
   f >> App;
   StreamSavedFile::PathOnly = true;
 }
}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::mnuSaveClick(TObject *Sender)
{
   SaveDialog1->FileName=AppFile;
   if(SaveDialog1->Execute()){
       AppFile=SaveDialog1->FileName;
       MyFile f(AppFile.c_str(),acWrite);
       f << App;

   }

}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::N3Click(TObject *Sender)
{
 if(!frmStreamDesigner){
     frmStreamDesigner = new TfrmStreamDesigner(Application);
     frmStreamDesigner->Manager = & App.Manager;
     frmStreamDesigner->Init();
     frmStreamDesigner->Show();
 }
}
//---------------------------------------------------------------------------


void __fastcall TLMENavigator::mnuRunClick(TObject *Sender)
{

   CurrentTask=0;
   if(!TaskName.IsEmpty()){
     for(long i=0;i<App.Tasks.Count();i++){
       if(App.Tasks[i]->Name==TaskName){
         CurrentTask=App.Tasks[i];
         break;
     }
   }

   if(!CurrentTask){
    MessageBox(0,TaskName.c_str(),"Неверное имя задачи",MB_OK);
    return;
   }

   Caption=CurrentTask->Caption;
   Application->Title=CurrentTask->Caption +" (© ЛМЭИ, 1999)";
   TMenuItem *mnu;
   TMenuItem *sub;
   MenuDef *m;

   if(MainMenu1->Items->Count>3){
    while(MainMenu1->Items->Count>3){
     MainMenu1->Items->Delete(0);
    }
   }

   for(long i=0;i<CurrentTask->Menu.Count();i++){
        m=CurrentTask->Menu[i];
        MainMenu1->Items->Insert(i,mnu =new TMenuItem(Application));
        mnu->Caption=m->Caption;
        if(!m->Items.Count())
          mnu->OnClick =UserMenuClick;
        else
          for(long j=0;j<m->Items.Count();j++){
            mnu->Add( sub=new TMenuItem(Application));
            sub->Caption = m->Items[j]->Caption;
            sub->OnClick =UserMenuClick;
          }
   }
  }
}
//---------------------------------------------------------------------------
void __fastcall TLMENavigator::UserMenuClick(TObject *Sender)
{
 if(!CurrentTask) return;
 TMenuItem* mnu=(TMenuItem*)Sender;
 TMenuItem* sub;
 MenuDef *m=0;
 if(mnu->Parent==(TObject*)(MainMenu1->Items) ){

  for(long i=0;i<CurrentTask->Menu.Count();i++){
   if(CurrentTask->Menu[i]->Caption==mnu->Caption){
     m=CurrentTask->Menu[i];
     break;
   }
  }
 }else{
   sub=mnu;
   mnu=sub->Parent;
   m=0;
  for(long i=0;i<CurrentTask->Menu.Count();i++){
   if(CurrentTask->Menu[i]->Caption==mnu->Caption){
     m=CurrentTask->Menu[i];
     break;
   }
  }
  for(long i=0;i<m->Items.Count();i++){
   if(m->Items[i]->Caption==sub->Caption){
     m=m->Items[i];
     break;
   }
  }
 }
 if(!m){
   MessageBox(0,mnu->Caption.c_str(),"Пункт меню не описан!",MB_OK);
   return;
 }

 AnsiString FSW;
 FilterFormDef *ff=0;
 TfrmFilter *frmFilter=0;
  if(!m->Filter.IsEmpty()){
     frmFilter = new TfrmFilter(Application);
     for(long i=0;i<App.Filters.Count();i++){
       if(m->Filter==App.Filters[i]->Caption){
         ff = App.Filters[i];
       }
     }

     if(ff){
      if(frmFilter->Setup(&App.Manager,ff)==mrOk){
         FSW=frmFilter->MakeSubWhere();
         if(FSW.IsEmpty())
           delete frmFilter;
       }else{
         FSW="";
         delete frmFilter;
         return;
       }
     }
  }

  if(!m->Editor.IsEmpty()){
   TfrmSlave * frmSlave =  new TfrmSlave(Application);
   frmSlave->StreamGrid1->SetEditor(&App.Manager,m->Editor);
   if(ff && !FSW.IsEmpty()){
     ff->Changes.SubCondition = FSW;
     frmSlave->StreamGrid1->InitSlave(&ff->Changes, &frmFilter->NewRow );
     delete frmFilter;
   }
   frmSlave->Caption = m->Caption;
   frmSlave->Init();
   frmSlave->Show();
  }else{
     if(!m->Report.IsEmpty()){
      TfrmReport * rpt = new TfrmReport(Application);
      ReportDef * rep=0;
      for(long i=0;i<App.Reports.Count();i++){
        if(m->Report==App.Reports[i]->Caption){
           rep =App.Reports[i];
           break;
        }

      }

      if(!rep){
       if(frmFilter)   delete frmFilter;
       MessageBox(0,mnu->Caption.c_str(),"Отчет не найден!",MB_OK);
       return;
      }


      if(frmFilter){
       Parser.SetParams(&frmFilter->NewRow);
      }

      rpt->Report1->FileName=rep->ReportFile.Path;


      if(!rep->SQL.IsEmpty())
       rpt->Report1->SQLString=Parser.ParseExpression(rep->SQL);



      if(rep->CaptionExpression.IsEmpty())
       rpt->Caption=rep->Caption;
      else
       rpt->Caption=Parser.ParseExpression(rep->CaptionExpression);

      rpt->Report1->ClearParameters();
      rpt->Report1->ClearFormulas();
      ODBCDataSource *ds;
      ds = App.Manager.Sources.Get(rep->ConnectionName);
      if(!ds){
       if(frmFilter)   delete frmFilter;
       MessageBox(0,mnu->Caption.c_str(),"Описание источника данных не найдено!",MB_OK);
       return;
      }

      for(long i=0;i<rep->Formulas.Count();i++){
       AnsiString Ex;
       Ex=Parser.ParseExpression(rep->Formulas[i]->Formula);
       rpt->Report1->NewFormula(rep->Formulas[i]->Name,Ex);
      }

      if(!ds->FullLogOn(rep->ConnectionName)){
        MessageBox(0,mnu->Caption.c_str(),"Соединение с источником данных не установлено!",MB_OK);
        if(frmFilter)   delete frmFilter;
        return;
      }

      if(!rep->BuildProc.IsEmpty()) {
       try {
         ProcCursor ccccc;
         ds->GetConnection()->Execute(Parser.ParseExpression(rep->BuildProc));
       }catch(  TODBCException *e){
         e->Show();
         if(frmFilter)   delete frmFilter;
         return;
       }
      }

      rpt->Report1->DataServerName=ds->GetDSN();
      rpt->Report1->UserID=ds->GetUID();
      rpt->Report1->Password=ds->GetPWD();
      rpt->Show();
      rpt->Report1->Start();

      if(frmFilter)   delete frmFilter;
     }
  }
}

void __fastcall TLMENavigator::FormShow(TObject *Sender)
{
 AnsiString ServerDSN,ServerTask;
 if(ParamCount() > 1){
   mnuDesign->Visible = false;
  for (int i=1;i<=ParamCount();i++)
  {
    if (LowerCase(ParamStr(i)) == "editor"){
     mnuDesign->Visible = true;
    }else{
     if(ParamStr(i).SubString(1,1)=="@")
       AppFile = ParamStr(i).SubString(2,ParamStr(i).Length()-1);
     if(ParamStr(i).SubString(1,1)=="#")
       TaskName = ParamStr(i).SubString(2,ParamStr(i).Length()-1);
     if(ParamStr(i).SubString(1,1)=="$")
       ServerDSN = ParamStr(i).SubString(2,ParamStr(i).Length()-1);
     if(ParamStr(i).SubString(1,1)=="&")
       ServerTask = ParamStr(i).SubString(2,ParamStr(i).Length()-1);

    }
  }
 }

 if(!AppFile.IsEmpty()){
    ProcCursor ccc;
    MyFile f(AppFile.c_str(),acRead);
    f >> App;
 }

 if(!ServerDSN.IsEmpty()&& !ServerTask.IsEmpty()){
    ProcCursor ccc;
    DbStream *dstream= new DbStream(
           ServerTask,
           ServerDSN,
           &App.Manager,
          daRead
     );

      LMEAppDef DbApp;
       StreamSavedFile::PathOnly = false;
     try{
     *dstream >> DbApp;
     delete dstream;
     }catch(...){}
     StreamSavedFile::PathOnly = true;

      try{
       MyMemory mem;
       mem <<DbApp;
       mem >>App;
      }catch(...){
      }

 }

 if(!AppFile.IsEmpty() && !TaskName.IsEmpty()){
    ProcCursor ccc;
    mnuRunClick(this);
 }

}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::mnuTasksClick(TObject *Sender)
{
  if(!frmTasks){
     frmTasks = new TfrmTasks(Application);
     frmTasks->App = &App;
     frmTasks->Init();
     frmTasks->Show();
 }
}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::N1Click(TObject *Sender)
{
 frmChooseTask = new TfrmChooseTask(Application);
 frmChooseTask->App=&App;
 frmChooseTask->Init();
 frmChooseTask->TaskName=TaskName;
 if(frmChooseTask->ShowModal()==mrOk)
  TaskName=frmChooseTask->TaskName;
}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::ArrangeAll1Click(TObject *Sender)
{
 ArrangeIcons(); 
}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::Cascade1Click(TObject *Sender)
{
 Cascade(); 
}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::Tile1Click(TObject *Sender)
{
 Tile();  
}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::mnuAboutClick(TObject *Sender)
{
   AboutBox = new TAboutBox(Application);
   if(CurrentTask){
     AboutBox->TaskName->Caption = CurrentTask->Caption;
     AboutBox->Version->Caption = "Версия: " +CurrentTask->Version;

   }else{
     AboutBox->TaskName->Caption = "Редактор приложения";
     AboutBox->Version->Caption = "";

   }
   AboutBox->ShowModal();
   delete AboutBox;
}
//---------------------------------------------------------------------------



void __fastcall TLMENavigator::mnuDBSaveClick(TObject *Sender)
{

    TfrmDBSave *fdsn = new TfrmDBSave(Application);
    fdsn->cmbDSN->Items->Clear();
    fdsn->Caption="Данные для сохранения приложения";
    for(long i=0;i<App.Manager.Sources.Count();i++){
      fdsn->cmbDSN->Items->Add(App.Manager.Sources.Name(i));
    }
    if(fdsn->ShowModal()!=mrOk){
     delete fdsn;
     return;
    }

    if(fdsn->cmbDSN->Text.IsEmpty() || fdsn->txtTask->Text.IsEmpty()){
     delete fdsn;
     return;
    }

    ProcCursor ccc;
    DbStream dstream=DbStream(
           fdsn->txtTask->Text,
           fdsn->cmbDSN->Text,
           &App.Manager,
          daWrite
     );
     delete fdsn;
     StreamSavedFile::PathOnly = false;
     try{
     dstream << App;
     }catch(...){}

     StreamSavedFile::PathOnly = true;

}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::mnuDbLoadClick(TObject *Sender)
{


    TfrmDBSave *fdsn = new TfrmDBSave(Application);
    fdsn->cmbDSN->Items->Clear();
    fdsn->Caption="Данные для загрузки приложения";
    for(long i=0;i<App.Manager.Sources.Count();i++){
      fdsn->cmbDSN->Items->Add(App.Manager.Sources.Name(i));
    }
    if(fdsn->ShowModal()!=mrOk){
     delete fdsn;
     return;
    }

    if(fdsn->cmbDSN->Text.IsEmpty() || fdsn->txtTask->Text.IsEmpty()){
     delete fdsn;
     return;
    }

    ProcCursor ccc;
    for(int i = 0; i < MDIChildCount; i++)
    		MDIChildren[i]->Close();
    if(MainMenu1->Items->Count>3){
     while(MainMenu1->Items->Count>3){
      MainMenu1->Items->Delete(0);
     }
    }

    DbStream *dstream=new DbStream(
           fdsn->txtTask->Text,
           fdsn->cmbDSN->Text,
           &App.Manager,
          daRead
     );
     delete fdsn;

     LMEAppDef DbApp;

     StreamSavedFile::PathOnly = false;
     try{
     *dstream >> DbApp;
     }catch(...){}
     StreamSavedFile::PathOnly = true;

     delete dstream;

     MyMemory mem;
     mem <<DbApp;
     mem >>App;

}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::mnuMakeDocClick(TObject *Sender)
{
  Documentator D("","");
  D.MakeHTML(App);
}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::SaveObjectClick(TObject *Sender)
{
  //if(!frmObjectList)

  frmObjectList = new TfrmObjectList(Application);
  frmObjectList->Init(&App);
  frmObjectList->ShowModal();
  delete frmObjectList;
  frmObjectList=0;

}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::LoadObjectClick(TObject *Sender)
{
  frmLoadObject = new TfrmLoadObject(Application);
  frmLoadObject->App=&App;
  if(frmLoadObject->ShowModal()==mrOk)
     for(int i = 0; i < MDIChildCount; i++)
    		MDIChildren[i]->Close();

  delete frmLoadObject;
  frmLoadObject=0;
}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::mnuAppNewClick(TObject *Sender)
{
   MyMemory f;

   for(int i = 0; i < MDIChildCount; i++)
    		MDIChildren[i]->Close();

   if(MainMenu1->Items->Count>3){
    while(MainMenu1->Items->Count>3){
     MainMenu1->Items->Delete(0);
    }
   }
   f >> App;
}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::nmuChangeTaskClick(TObject *Sender)
{
 frmChooseTask = new TfrmChooseTask(Application);
 frmChooseTask->App=&App;
 frmChooseTask->Init();
 frmChooseTask->TaskName=TaskName;
 if(frmChooseTask->ShowModal()==mrOk){
  TaskName=frmChooseTask->TaskName;
  for(int i = 0; i < MDIChildCount; i++)
 		MDIChildren[i]->Close();
  mnuRunClick(Sender);
 }
}
//---------------------------------------------------------------------------

void __fastcall TLMENavigator::N9Click(TObject *Sender)
{
 SaveDialog1->FileName=AppFile;
   if(SaveDialog1->Execute()){
       AppFile=SaveDialog1->FileName;
       MyFile f(AppFile.c_str(),acWrite);
       StreamSavedFile::PathOnly = false;
       try{
         f << App;
       }catch(...){}
       StreamSavedFile::PathOnly = true;
   }
}
//---------------------------------------------------------------------------


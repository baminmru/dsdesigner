//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Task.h"
#include "ObjectList.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "RXCtrls"
//#pragma link "ToolEdit"
#pragma resource "*.dfm"
TfrmObjectList *frmObjectList;
//---------------------------------------------------------------------------
__fastcall TfrmObjectList::TfrmObjectList(TComponent* Owner)
  : TForm(Owner)
{

}
//---------------------------------------------------------------------------

AnsiString FN(AnsiString N){
AnsiString buf;
for(long i=1;i<=N.Length();i++){
  switch (N[i]){
    case ' ':
    case '>':
    case '*':
    case ':':
    case '\\':
    case '?':
    case '.':
      buf=buf+'_';
      break;
    /*

    case '?':
    case '?':
    case '?':
    */
    default:
      buf=buf+N[i];
  }
}
return buf;//DelSpace11( DelChars(DelChars(DelChars(N,'*'),'?'),'\');
}

void __fastcall TfrmObjectList::Init(LMEAppDef *_App){
  App=_App;
  lstObj->Items->Clear();
  for(long i=0;i<App->Tasks.Count();i++){
    lstObj->Items->AddObject("Задача "+App->Tasks[i]->Name,(TObject*)i);
  }
  for(long i=0;i<App->Reports.Count();i++){
    lstObj->Items->AddObject("Отчет "+App->Reports[i]->Caption,(TObject*)i);
  }
  for(long i=0;i<App->Filters.Count();i++){
    lstObj->Items->AddObject("Фильтр "+App->Filters[i]->Caption,(TObject*)i);
  }

  DataManager *Man=&App->Manager;
  for(long i=0;i<Man->Sources.Count();i++){
    lstObj->Items->AddObject("Источник "+Man->Sources.Name(i),(TObject*)i);
  }

  for(long i=0;i<Man->Editors.Count();i++){
    lstObj->Items->AddObject("Редактор "+Man->Editors[i]->Caption,(TObject*)i);
  }
  for(long i=0;i<Man->Trans.Count();i++){
    lstObj->Items->AddObject("Операция "+Man->Trans[i]->EditorName+"__"+Man->Trans[i]->Caption,(TObject*)i);
  }

  for(long i=0;i<Man->Links.Count();i++){
    lstObj->Items->AddObject("Связь "+Man->Links[i]->MasterName+"__"+Man->Links[i]->Caption,(TObject*)i);
  }


};
void __fastcall TfrmObjectList::Button1Click(TObject *Sender)
{
  for(long i=0;i<lstObj->Items->Count;i++){
    if( lstObj->Selected[i]){
      AnsiString ext,p;
      long j=(long)lstObj->Items->Objects[i];
      MyMemory MEM;
      p=lstObj->Items->Strings[i].SubString(1,5);
      if(p=="Задач"){
        MEM << *(App->Tasks[j]);
        ext=".tsk";
      }
      if(p=="Отчет"){
        MEM << *(App->Reports[j]);
        ext=".rep";
      }
      if(p=="Фильт"){
        MEM << *(App->Filters[j]);
        ext=".flt";
      }

      if(p=="Источ"){
        MEM << *(App->Manager.Sources[j]);
        MEM << App->Manager.Sources.Name(j);
        ext=".dsn";
      }

      if(p=="Редак"){
        MEM << *(App->Manager.Editors[j]);
        ext=".sed";
      }
      if(p=="Опера"){
        MEM << *(App->Manager.Trans[j]);
        ext=".trn";
      }
      if(p=="Связь"){
        MEM << *(App->Manager.Links[j]);
        ext=".slk";
      }
      MEM.SaveToFile((txtDir->Directory+"\\"+FN(lstObj->Items->Strings[i])+ext).c_str());
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmObjectList::DriveComboBox1Change(TObject *Sender)
{
  txtDir->Drive=DriveComboBox1->Drive;
}
//---------------------------------------------------------------------------


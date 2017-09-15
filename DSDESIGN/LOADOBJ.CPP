//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "LoadObj.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmLoadObject *frmLoadObject;
//---------------------------------------------------------------------------
__fastcall TfrmLoadObject::TfrmLoadObject(TComponent* Owner)
  : TForm(Owner)
{
  TaskPath=GetCurrentDir();
}
//---------------------------------------------------------------------------

void __fastcall TfrmLoadObject::lstDirectoryChange(TObject *Sender)
{
 lstFile->Drive=lstDirectory->Drive;
 lstFile->Directory=lstDirectory->Directory;
}
//---------------------------------------------------------------------------



void __fastcall TfrmLoadObject::DriveComboBox1Change(TObject *Sender)
{
  lstDirectory->Drive=DriveComboBox1->Drive;
  lstFile->Drive=DriveComboBox1->Drive;
}
//---------------------------------------------------------------------------
void __fastcall TfrmLoadObject::Button1Click(TObject *Sender)
{

  SetCurrentDir(lstFile->Directory);
  for (long i=0;i<lstFile->Items->Count;i++){
    if(lstFile->Selected[i]){
      MyFile MEM(lstFile->Items->Strings[i].c_str(),acRead);
      AnsiString ext =ExtractFileExt(lstFile->Items->Strings[i]);
      if(ext==".tsk"){
        Task *t = new Task();
        MEM >> *t;
        App->Tasks.Add(t);
      }
      if(ext==".rep"){
        ReportDef * r = new ReportDef;
        MEM >> *r;
        App->Reports.Add(r);
      }
      if(ext==".flt"){
        FilterFormDef *f = new FilterFormDef;
        MEM >> *f;
        App->Filters.Add(f);
      }

      if(ext==".dsn"){
        ODBCDataSource *d = new ODBCDataSource;
        AnsiString N;
        MEM >> *d;
        MEM >> N;
        App->Manager.Sources.Add(d,N);

      }

      if(ext==".sed"){
        StreamEditorDef *ed = new StreamEditorDef;
        MEM >> *ed;
        App->Manager.Editors.Add(ed,ed->Caption);
      }
      if(ext==".trn"){
        StreamTransaction *t = new StreamTransaction;
        MEM >> *t;
        App->Manager.Trans.Add(t);
      }

      if(ext==".slk"){
        StreamEditorLink *l = new StreamEditorLink;
        MEM >> *l;
        App->Manager.Links.Add(l);
      }
    }
  }
  SetCurrentDir(TaskPath);
}
//---------------------------------------------------------------------------

void __fastcall TfrmLoadObject::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 SetCurrentDir(TaskPath);
}
//---------------------------------------------------------------------------


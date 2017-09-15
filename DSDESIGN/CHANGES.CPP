//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Changes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "RXCtrls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmChanges::TfrmChanges(TComponent* Owner)
  : TForm(Owner)
{
 OnParam=false;
 OnAccess=false;
 OnDef=false;
};

void __fastcall TfrmChanges::Prepare(){
 txtSubCondition->Text=Actions->SubCondition;
 lstDef->Items->Clear();
 lstAccess->Items->Clear();
 lstParam->Items->Clear();
 for( long i=0;i<Actions->Changes.Count();i++){
   lstParam->Items->Add(Actions->Changes[i]->ParamName);
 }
 for( long i=0;i<Actions->DefaultChanges.Count();i++){
   lstDef->Items->Add(Actions->DefaultChanges[i]->ParamName);
 }
 for( long i=0;i<Actions->Disables.Count();i++){
   lstAccess->Items->Add(Actions->Disables[i]->FieldName);
 }
};

//---------------------------------------------------------------------------
void __fastcall TfrmChanges::btnAddDefClick(TObject *Sender)
{
 lstDef->Items->Add("@@@@@@@@@@" +AnsiString(lstDef->Items->Count));
 Actions->DefaultChanges.Add(new EditFieldChanges());
}
//---------------------------------------------------------------------------

void __fastcall TfrmChanges::btnAddAccesClick(TObject *Sender)
{
 lstAccess->Items->Add("#########" +AnsiString(lstAccess->Items->Count));
 Actions->Disables.Add(new EditFieldDisables());

}
//---------------------------------------------------------------------------

void __fastcall TfrmChanges::btnAddParamClick(TObject *Sender)
{
 lstParam->Items->Add("$$$$$$$$$$" +AnsiString(lstParam->Items->Count));
 Actions->Changes.Add(new EditFieldChanges());


}
//---------------------------------------------------------------------------

void __fastcall TfrmChanges::lstParamClick(TObject *Sender)
{
 OnParam = true;
 EditFieldChanges *fc;
 if(lstParam->ItemIndex==-1) return;
 fc =Actions->Changes[lstParam->ItemIndex];
 txtParamName->Text=fc->ParamName;
 if(fc->SetAsValue)
   chkParam->Checked =cbChecked;
 else
  chkParam->Checked =cbUnchecked;
 txtParamValue->Text=fc->ValueOrName;
 OnParam = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmChanges::lstDefClick(TObject *Sender)
{
 EditFieldChanges *fc;
 if(lstDef->ItemIndex==-1) return;
 OnDef = true;
 fc =Actions->DefaultChanges[lstDef->ItemIndex];
 txtDefName->Text=fc->ParamName;
 if(fc->SetAsValue)
   chkDef->Checked =cbChecked;
 else
  chkDef->Checked =cbUnchecked;
 txtDefValue->Text=fc->ValueOrName;
 OnDef = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmChanges::lstAccessClick(TObject *Sender)
{
 EditFieldDisables *fc;
 if(lstAccess->ItemIndex==-1) return;
 OnAccess = true;
 fc =Actions->Disables[lstAccess->ItemIndex];
 txtField->Text=fc->FieldName;
 switch(fc->InsertAccess){
   case faNone:
     InsNoAccess->Checked = true;
   break;
   case faRead:
     InsReadOnly->Checked = true;
   break;
   case faWrite:
     InsReadWrite->Checked = true;
   break;
 }
 switch(fc->EditAccess){
   case faNone:
     UpdNoAccess->Checked = true;
   break;
   case faRead:
     UpdReadOnly->Checked = true;
   break;
   case faWrite:
     UpdReadWrite->Checked = true;
   break;
 }
 OnAccess = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmChanges::btnDelDefClick(TObject *Sender)
{
 if(lstDef->ItemIndex==-1) return;
 Actions->DefaultChanges.Destroy(lstDef->ItemIndex);
 lstDef->Items->Delete(lstDef->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TfrmChanges::btnDelAccessClick(TObject *Sender)
{

 if(lstAccess->ItemIndex==-1) return;
 Actions->Disables.Destroy(lstAccess->ItemIndex);
 lstAccess->Items->Delete(lstAccess->ItemIndex);

}
//---------------------------------------------------------------------------

void __fastcall TfrmChanges::btnDelParamClick(TObject *Sender)
{
 if(lstParam->ItemIndex==-1) return;
 Actions->Changes.Destroy(lstParam->ItemIndex);
 lstParam->Items->Delete(lstParam->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TfrmChanges::txtDefNameChange(TObject *Sender)
{
 EditFieldChanges *fc;
 if(lstDef->ItemIndex==-1) return;
 if(OnDef) return;
 fc =Actions->DefaultChanges[lstDef->ItemIndex];
 lstDef->Items->Strings[lstDef->ItemIndex]=txtDefName->Text;
 fc->ParamName=txtDefName->Text;
 fc->SetAsValue=( chkDef->Checked ==cbChecked);
 fc->ValueOrName= txtDefValue->Text;
}
//---------------------------------------------------------------------------

void __fastcall TfrmChanges::txtFieldChange(TObject *Sender)
{
 EditFieldDisables *fc;
 if(lstAccess->ItemIndex==-1) return;
 if(OnAccess) return;
 fc =Actions->Disables[lstAccess->ItemIndex];
 fc->FieldName=txtField->Text;
 lstAccess->Items->Strings[lstAccess->ItemIndex]=txtField->Text;;
 if(  InsNoAccess->Checked) fc->InsertAccess=faNone;
 if(  InsReadOnly->Checked) fc->InsertAccess=faRead;
 if(  InsReadWrite->Checked) fc->InsertAccess=faWrite;
 if(  UpdNoAccess->Checked) fc->EditAccess=faNone;
 if(  UpdReadOnly->Checked) fc->EditAccess=faRead;
 if(  UpdReadWrite->Checked) fc->EditAccess=faWrite;
}
//---------------------------------------------------------------------------

void __fastcall TfrmChanges::txtParamNameChange(TObject *Sender)
{
 EditFieldChanges *fc;
 if(lstParam->ItemIndex==-1) return;
 if(OnParam) return;
 fc =Actions->Changes[lstParam->ItemIndex];
 lstParam->Items->Strings[lstParam->ItemIndex]=txtParamName->Text;
 fc->ParamName=txtParamName->Text;
 fc->SetAsValue=( chkParam->Checked ==cbChecked);
 fc->ValueOrName= txtParamValue->Text;
}
//---------------------------------------------------------------------------

void __fastcall TfrmChanges::btnOKClick(TObject *Sender)
{
 Actions->SubCondition=txtSubCondition->Text;
}
//---------------------------------------------------------------------------

void __fastcall TfrmChanges::txtSubConditionChange(TObject *Sender)
{
 Actions->SubCondition=txtSubCondition->Text;
}
//---------------------------------------------------------------------------



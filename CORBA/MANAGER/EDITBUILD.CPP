//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "EditBuild.h"
#include "Manager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmEditBuild *frmEditBuild;
//---------------------------------------------------------------------------
__fastcall TfrmEditBuild::TfrmEditBuild(TComponent* Owner)
  : TForm(Owner)
{
  InClick = false;
}
//---------------------------------------------------------------------------
void TfrmEditBuild::SetBuild(
    AnsiString _System,
    AnsiString _Build
  ){
  InClick = true;
  System=_System;
  Build=_Build;
  cmbSources->Items->Clear();
  Octets * oc ;
  MyMemory *m;
  // получить список всех источников
  MyArray<AnsiString> *sss= man->SourceList(System);
  for(long i=0;i<sss->Count();i++){
    bei *b=new bei;
    b->DataSourceName=*((*sss)[i]);
    cmbSources->Items->Add(*((*sss)[i]));
    BEI.Add(b);

    // получить все настройки источников
    try
    {
     oc =man->GetDS()->GetTargetPackSetup(
            man->GetConnect(),
            Build.c_str(),
            System.c_str(),
            b->DataSourceName.c_str());
     DataSourceSetup dss;
     m=0;
     if(oc){
       m = MakeMemory(*oc);
       delete oc;

       if(m){
         *m >> dss;
         delete m;
       }
     }
     MyMemory m1;
     m1.LoadBuffer(dss.Info.GetBuffer(),dss.Info.Length());
     m1 >> b->Info;
     b->TargetName=dss.TargetName;

    }
     catch(const CORBA::Exception& e)
    {
    }
  }

  delete sss;

  // Заполнить комбик c серверами
  cmbTarget->Items->Clear();
  oc = man->GetDS()->GetTargetList(man->GetConnect());
  m=0;
  if(oc)
    m = MakeMemory(*oc);
  sss = new MyArray<AnsiString>;
  if(m)
    *m >> *sss;
  delete oc;
  delete m;

  for(long i=0;i<sss->Count();i++){
    cmbTarget->Items->Add(*((*sss)[i]));
  }
  InClick=false;
};

void __fastcall TfrmEditBuild::cmbSourcesClick(TObject *Sender)
{

   long idx=cmbSources->ItemIndex;
   if(idx==-1) return;
   InClick= true;
   txtValue->Text="";
   if(BEI[idx]->TargetName!=""){
     cmbTarget->ItemIndex=cmbTarget->Items->IndexOf(BEI[idx]->TargetName);
     DataSource * ds=0;
     ds= new DataSource(BEI[idx]->TargetName,BEI[idx]->DataSourceName);
     MyArray<AnsiString>* ci =ds->GetConnectInfo();
     lstParam->Items->Clear();
     for(long i=0;i<ci->Count();i++){
        lstParam->Items->Add(*(ci->Get(i)));
     }
     delete ci;
     delete ds;
   }else{
     cmbTarget->ItemIndex=-1;
     lstParam->Items->Clear();
   }
   InClick = false;

}
//---------------------------------------------------------------------------
void __fastcall TfrmEditBuild::cmbTargetChange(TObject *Sender)
{

   long idx=cmbSources->ItemIndex;
   long idx2=cmbTarget->ItemIndex;
   if(idx==-1 || idx2==-1) return;
   if(InClick) return;
   txtValue->Text="";
   InClick= true;
   BEI[idx]->TargetName=cmbTarget->Text;
   DataSource * ds=0;
   ds= new DataSource(BEI[idx]->TargetName,BEI[idx]->DataSourceName);
   MyArray<AnsiString>* ci =ds->GetConnectInfo();
   lstParam->Items->Clear();
   for(long i=0;i<ci->Count();i++){
      lstParam->Items->Add(*(ci->Get(i)));
   }
   delete ci;
   delete ds;
   InClick = false;
};
//---------------------------------------------------------------------------
void __fastcall TfrmEditBuild::lstParamClick(TObject *Sender)
{
  long idx=cmbSources->ItemIndex;
  long idx2=cmbTarget->ItemIndex;
  long idx3=lstParam->ItemIndex;
  if(idx==-1 || idx2==-1 || idx3==-1) return;
  if(InClick) return;
  InClick = true;
  txtValue->Text="";
  for(long i=0;i<BEI[idx]->Info.Count();i++){
    if(BEI[idx]->Info[i]->Name==lstParam->Items->Strings[idx3]){
      txtValue->Text=BEI[idx]->Info[i]->Value.ToStr();
      break;
    }
  }
  InClick = false;

}
//---------------------------------------------------------------------------
void __fastcall TfrmEditBuild::txtValueChange(TObject *Sender)
{
  long idx=cmbSources->ItemIndex;
  long idx2=cmbTarget->ItemIndex;
  long idx3=lstParam->ItemIndex;
  if(idx==-1 || idx2==-1 || idx3==-1) return;
  if(InClick) return;



  for(long i=0;i<BEI[idx]->Info.Count();i++){
    if(BEI[idx]->Info[i]->Name==lstParam->Items->Strings[idx3]){
      BEI[idx]->Info[i]->Value.Set(txtValue->Text);
      return;
    }
  }
  CartrigeParam *cp = new CartrigeParam;
  cp->Name=lstParam->Items->Strings[idx3];
  cp->Value.Set(txtValue->Text);
  BEI[idx]->Info.Add(cp);

}
//---------------------------------------------------------------------------
void __fastcall TfrmEditBuild::Button1Click(TObject *Sender)
{
Octets *oc;
for(long i=0;i<BEI.Count();i++){

  MyMemory m;
  m << BEI[i]->Info;
  oc = MakeOctets(m);
  man->GetDS()->SetTargetPackSetup(
      man->GetConnect(),
      Build.c_str(),
      System.c_str(),
      BEI[i]->DataSourceName.c_str(),
      BEI[i]->TargetName.c_str(),
      *oc
      );
   delete oc;
 }
}
//---------------------------------------------------------------------------

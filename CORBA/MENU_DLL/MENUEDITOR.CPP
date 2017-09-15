//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MenuEDitor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMenuEdit *frmMenuEdit;
//---------------------------------------------------------------------------
__fastcall TfrmMenuEdit::TfrmMenuEdit(TComponent* Owner)
  : TForm(Owner)
{
  InTvClick = false;
  InLstClick = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::cmbObjectsClick(TObject *Sender)
{
  if(InLstClick) return;
  if(tvMenu->Selected){
  if(lstCalls->ItemIndex==-1) return;

      long idx =lstCalls->ItemIndex;
      CallMode *cm =md[tvMenu->Selected->AbsoluteIndex]->Calls[idx];
      cm->ObjectName=cmbObjects->Text;
      cm->RunModal = chkCallModal->Checked;
      lstCalls->Items->Strings[idx]=cmbObjects->Text;
      lstCalls->ItemIndex = idx;
  }

}
//---------------------------------------------------------------------------
void __fastcall TfrmMenuEdit::lstCallsClick(TObject *Sender)
{
  //load check and sync combo
  if(InLstClick) return;
  if(tvMenu->Selected){
    if(lstCalls->ItemIndex==-1) return;
      InLstClick = true;
      long idx =lstCalls->ItemIndex;
      CallMode *cm =md[tvMenu->Selected->AbsoluteIndex]->Calls[idx];
      cmbObjects->ItemIndex=cmbObjects->Items->IndexOf(cm->ObjectName);
      chkCallModal->Checked=cm->RunModal;
      InLstClick = false;
  }
}
//---------------------------------------------------------------------------
void TfrmMenuEdit::SetMenuDef(DataBuffer * db){
  MyMemory m;
  if(db)
    m.LoadBuffer( db->GetBuffer(),db->Length());
  m >> md;
  tvMenu->Items->Clear();
  long maxlvl=0;
  for(long i=0;i<md.Count();i++){
  if(maxlvl<md[i]->Level)
    maxlvl=md[i]->Level;
  }
  TTreeNode * *n = new TTreeNode * [maxlvl+2];
  n[0]=0;
  for(long i=0;i<md.Count();i++){
    if(md[i]->Level>0)
     n[md[i]->Level+1]=tvMenu->Items->AddChild(n[md[i]->Level],md[i]->Caption);
    else
    if(md[i]->Level<=0)
     n[1]=tvMenu->Items->Add(n[0],md[i]->Caption);
  }
};

void TfrmMenuEdit::SetCombo(MyArray<AnsiString> *ss){
  cmbObjects->Items->Clear();
  if(ss){
  for(long i=0;i<ss->Count();i++){
    cmbObjects->Items->Add(*ss->Get(i));
  }
  delete ss;
  }


};

DataBuffer *TfrmMenuEdit::GetMenuDef(){
  MyMemory m;
  m << md;
  return m.MakeDataBuffer();
};

void __fastcall TfrmMenuEdit::tvMenuClick(TObject *Sender)
{
  if(tvMenu->Selected){
      txtCaption->Text=tvMenu->Selected->Text;
      lstCalls->Items->Clear();
      MyArray<CallMode> *cm =&(md[tvMenu->Selected->AbsoluteIndex]->Calls);
      for(long i=0;i<cm->Count();i++){
        lstCalls->Items->Add(
        (*cm)[i]->ObjectName
        );
      }
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::Button1Click(TObject *Sender)
{
  TTreeNode * n =tvMenu->Items->Add(0,"Новый пункт меню");
  md.Insert(new MenuDef("Новый пункт меню",n->Level),n->AbsoluteIndex);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::Button2Click(TObject *Sender)
{

if(tvMenu->Selected){
  TTreeNode * n = tvMenu->Items->AddChild(tvMenu->Selected,"Новый пункт меню");
  md.Insert(new MenuDef("Новый пункт меню",n->Level),n->AbsoluteIndex);
 }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::Button3Click(TObject *Sender)
{
  if(tvMenu->Selected){
     TTreeNode * n = tvMenu->Selected;
     long lvl = n->Level;
     md[n->AbsoluteIndex]->Level=-1;
     for (long i=n->AbsoluteIndex+1;i<md.Count();i++)
      if(md[i]->Level>lvl) md[i]->Level =-1;
      else break;

     for (long i=n->AbsoluteIndex;i<md.Count();i++)
      if(md[i]->Level==-1){
        md.Destroy(i);
        i--;
      }
     tvMenu->Items->Delete(tvMenu->Selected);
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::txtCaptionChange(TObject *Sender)
{
  if(tvMenu->Selected){
    tvMenu->Selected->Text=txtCaption->Text;
    md[tvMenu->Selected->AbsoluteIndex]->Caption = txtCaption->Text;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::btnAddCallClick(TObject *Sender)
{
  if(tvMenu->Selected){
   CallMode * cm = new CallMode;
   cm->ObjectName ="Новый объект";
   cm->RunModal = true;
   md[ tvMenu->Selected->AbsoluteIndex]->Calls.Add(cm);
   lstCalls->Items->Add("Новый объект");
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMenuEdit::btnDelCallClick(TObject *Sender)
{
  // удаляем вызов
  if(tvMenu->Selected && lstCalls->ItemIndex>=0){
   md[ tvMenu->Selected->AbsoluteIndex]->
     Calls.Destroy(lstCalls->ItemIndex);
  }
}
//---------------------------------------------------------------------------


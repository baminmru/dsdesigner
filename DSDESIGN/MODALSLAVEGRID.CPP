//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ModalSlaveGrid.h"
#include "Navigator.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "StreamGrid"
#pragma link "RXGrids"
#pragma resource "*.dfm"
TfrmModalSlave *frmModalSlave;
//---------------------------------------------------------------------------
__fastcall TfrmModalSlave::TfrmModalSlave(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TfrmModalSlave::Init()
{
   HeaderControl1->Sections->Clear();
   HeaderControl1->Width=0;
   HeaderControl2->Sections->Clear();
   HeaderControl2->Width=0;

   for(long i=0;i<StreamGrid1->GetManager()->Links.Count();i++){
      if(StreamGrid1->GetEditorName()==StreamGrid1->GetManager()->Links[i]->MasterName){
        long j=0;
        while( j<HeaderControl1->Sections->Count &&
               HeaderControl1->Sections->Items[j]->Text <
               StreamGrid1->GetManager()->Links[i]->Caption){
          j++;
        }
        THeaderSection *pSection;
        if( j<HeaderControl1->Sections->Count )
           pSection = (THeaderSection *)HeaderControl1->Sections->Insert(j);
        else
           pSection = HeaderControl1->Sections->Add();

        pSection->Text = StreamGrid1->GetManager()->Links[i]->Caption;
        pSection->MinWidth=1;
        pSection->MaxWidth=pSection->Text.Length() * HeaderControl1->Font->Size+15;
        pSection->Width=pSection->Text.Length() * HeaderControl1->Font->Size+8;
        pSection->MinWidth = pSection->Text.Length() * HeaderControl1->Font->Size/3;
        pSection->Style = hsText;
        HeaderControl1->Width+=pSection->Width;
      }
  }

   if(HeaderControl1->Sections->Count>0){
     LinkHolder->Visible=true;
   }else{
     LinkHolder->Visible=false;
   }

   for(long i=0;i<StreamGrid1->GetManager()->Trans.Count();i++){
      if(StreamGrid1->GetEditorName()==StreamGrid1->GetManager()->Trans[i]->EditorName){
        long j=0;
        while( j<HeaderControl2->Sections->Count &&
               HeaderControl2->Sections->Items[j]->Text <
               StreamGrid1->GetManager()->Trans[i]->Caption){
          j++;
        }
        THeaderSection *pSection;
        if( j<HeaderControl2->Sections->Count )
           pSection = (THeaderSection *)HeaderControl2->Sections->Insert(j);
        else
           pSection = HeaderControl2->Sections->Add();

        pSection->Text = StreamGrid1->GetManager()->Trans[i]->Caption;
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
  StreamGrid1->RunEditor();
  StreamGrid1->Visible= true;
  LinkIdx=0;
  TranIdx=0;
}

//







void __fastcall TfrmModalSlave::FormClose(TObject *Sender, TCloseAction &Action)
{
 if(Action!=caFree)
    Action=caFree;

}
//---------------------------------------------------------------------------
void __fastcall TfrmModalSlave::btnLinkLeftClick(TObject *Sender)
{
    if(LinkIdx>0) LinkIdx--;
    long l=0;
    for(long i=0;i<LinkIdx;i++){
     l+=HeaderControl1->Sections->Items[i]->Width;
    }

    HeaderControl1->Left=-l;
    if(l)
     btnLinkLeft->Enabled = true;
    else
     btnLinkLeft->Enabled = false;
}

//---------------------------------------------------------------------------

void __fastcall TfrmModalSlave::btnLinkRightClick(TObject *Sender)
{
    if(LinkIdx<HeaderControl1->Sections->Count-1) LinkIdx++;
    long l=0;
    for(long i=0;i<LinkIdx;i++){
     l+=HeaderControl1->Sections->Items[i]->Width;
    }

    HeaderControl1->Left=-l;
    if(l)
     btnLinkLeft->Enabled = true;
    else
     btnLinkLeft->Enabled = false;
}
//---------------------------------------------------------------------------


void __fastcall TfrmModalSlave::HeaderControl1SectionClick(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
if(StreamGrid1->GetStream()  &&  StreamGrid1->GetSnap()->RowCount()){
   Params *p;
   StreamEditorLink *Link;
   for(long i=0;i<StreamGrid1->GetManager()->Links.Count();i++){
         if(StreamGrid1->GetManager()->Links[i]->MasterName==StreamGrid1->GetEditorName() &&
            StreamGrid1->GetManager()->Links[i]->Caption==Section->Text){
            Link=StreamGrid1->GetManager()->Links[i];
            goto LinkFound;
            }
   }
   return;

   LinkFound:
   p=StreamGrid1->GetState();
      if(StreamGrid1->TestLink(Link->MasterCondition)){
            TfrmModalSlave * frmSlave =  new TfrmModalSlave(LMENavigator);
            frmSlave->StreamGrid1->SetEditor(StreamGrid1->GetManager(),Link->SlaveName);
            frmSlave->StreamGrid1->InitSlave(&Link->WhenChanged,p);
            frmSlave->Caption = Link->Caption;
            frmSlave->Init();
            frmSlave->ShowModal();
      }
   delete p;
 }

}
//---------------------------------------------------------------------------

void __fastcall TfrmModalSlave::btnTranRightClick(TObject *Sender)
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

void __fastcall TfrmModalSlave::btnTranLeftClick(TObject *Sender)
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


void __fastcall TfrmModalSlave::HeaderControl2SectionClick(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
  if(StreamGrid1->GetStream()){
      StreamGrid1->RunTransaction(Section->Text);
  }

}
//---------------------------------------------------------------------------

void __fastcall TfrmModalSlave::HeaderControl1SectionResize(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
 long l=0;
 for(long i=0;i<HeaderControl->Sections->Count;i++){
   l+=HeaderControl->Sections->Items[i]->Width;
 }
 HeaderControl->Width=l;
 Panel5Resize(HeaderControl);
 Panel6Resize(HeaderControl);
}
//---------------------------------------------------------------------------

void __fastcall TfrmModalSlave::Panel5Resize(TObject *Sender)
{
 if(HeaderControl1->Width<=Panel5->Width){
    HeaderControl1->Left=0;
    btnLinkRight->Enabled = false;
    btnLinkLeft->Enabled = false;
    LinkIdx=0;
 }else{
     btnLinkRight->Enabled = true;
     if(LinkIdx>0)
      btnLinkLeft->Enabled = true;
     else
      btnLinkLeft->Enabled = false;

 }
}
//---------------------------------------------------------------------------

void __fastcall TfrmModalSlave::Panel6Resize(TObject *Sender)
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


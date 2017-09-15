//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "fTree.h"
#include "Manager.h"
#include "AddObject.h"
#include "EditUser.h"
#include "EditBuild.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmTree *frmTree;
//---------------------------------------------------------------------------
__fastcall TfrmTree::TfrmTree(TComponent* Owner)
  : TForm(Owner)
{
  LastNode=0;
}

//---------------------------------------------------------------------------
void __fastcall TfrmTree::FillTree(void){
  if(!man) return;
  LastNode=0;
  long iiiii=0;
  if(OT->Selected)
    iiiii = OT->Selected->AbsoluteIndex;
  ObjDefStorage_ptr os =  man->GetDS();
  //
  TTreeNode *RootNode,*UNode,*DNode,*PNode,*SNode;
  OT->Items->Clear(); // remove any existing nodes

  // Add a root node
  RootNode=OT->Items->Add(NULL, "APPLICATION SERVER");

  DNode=OT->Items->AddChildObject(RootNode,"DATA SERVERS",mnuDataServers);
  PNode=OT->Items->AddChildObject(RootNode,"PLATFORMS",mnuPlatforms);
  SNode=OT->Items->AddChildObject(RootNode,"SYSTEMS",mnuSystems);
  UNode=OT->Items->AddChildObject(RootNode,"USERS",mnuUsers);

  // USERS
  Octets *oc1,*oc = os->GetUserList(man->GetConnect());
  MyMemory * m=0;
  if(oc)
    m = MakeMemory(*oc);
  MyArray<AnsiString> ss;
  if(m)
    *m >> ss;
  delete oc;
  delete m;
  for(long i=0;i<ss.Count();i++){
    OT->Items->AddChildObject(UNode,*(ss[i]),mnuUser);
  }

  // PLATFORMS
  oc = os->GetPlatformList(man->GetConnect());
  m=0;
  if(oc)
    m = MakeMemory(*oc);
  ss.Clear();
  if(m)
    *m >> ss;
  delete oc;
  delete m;
  for(long i=0;i<ss.Count();i++){
    TTreeNode * CurP=OT->Items->AddChildObject(PNode,*(ss[i]),mnuPlatform);
    oc1= os->GetComponentList(
      man->GetConnect(),
      ss[i]->c_str()
      );
    MyMemory *mm=0;
    if(oc1)
      mm = MakeMemory(*oc1);
    delete oc1;
    MyArray<AnsiString> sss;
    if(mm)
      *mm >> sss;
    delete mm;
    for(long j=0;j<sss.Count();j++){
      OT->Items->AddChildObject(CurP,*(sss[j]),mnuComponent);
    }
  }

  // DATA SERVERS
  oc = os->GetTargetList(man->GetConnect());
  m=0;
  if(oc)
    m = MakeMemory(*oc);
  ss.Clear();
  if(m)
    *m >> ss;
  delete oc;
  delete m;
  for(long i=0;i<ss.Count();i++){
    OT->Items->AddChildObject(DNode,*(ss[i]),mnuDataServer);
  }

  oc = os->GetSystemList(man->GetConnect());
  m=0;
  if(oc)
    m = MakeMemory(*oc);
  ss.Clear();
  if(m)
    *m >> ss;
  delete oc;
  delete m;
  for(long i=0;i<ss.Count();i++){
    TTreeNode *CurSys,*Cur;
    MyMemory *mm=0;
    MyArray<AnsiString> sss;
    CurSys=OT->Items->AddChildObject(SNode,*(ss[i]),mnuSystem);
    /// OBJECTS
    Cur=OT->Items->AddChild(CurSys,"OBJECTS");
    oc1 = os->GetSystemObjectsList(man->GetConnect(),ss[i]->c_str());
    mm=0;
    if(oc1)
      mm = MakeMemory(*oc1);
    delete oc1;
    if(mm)
      *mm >> sss;
    delete mm;
    for(long j=0;j<sss.Count();j++){
      OT->Items->AddChildObject(Cur,*(sss[j]),mnuObject);
    }
    /// DATA SOURCES
    Cur=OT->Items->AddChild(CurSys,"DATA SOURCES");
    oc1 = os->GetDataSourceList(man->GetConnect(),ss[i]->c_str());
    mm=0;
    if(oc1)
      mm = MakeMemory(*oc1);
    delete oc1;

    if(mm)
      *mm >> sss;
    delete mm;
    for(long j=0;j<sss.Count();j++){
      OT->Items->AddChildObject(Cur,*(sss[j]),mnuSource);
    }
    /// BUILDS
    Cur=OT->Items->AddChild(CurSys,"BUILDS");
    oc1 = os->GetSystemBuildList(man->GetConnect(),ss[i]->c_str());
    mm=0;
    if(oc1)
      mm = MakeMemory(*oc1);
    delete oc1;

    if(mm)
      *mm >> sss;
    delete mm;
    for(long j=0;j<sss.Count();j++){
      OT->Items->AddChildObject(Cur,*(sss[j]),mnuBuild);
    }


  }                            //TTreeView
  //OT->FullExpand();
  if(iiiii>=OT->Items->Count)
    iiiii=OT->Items->Count-1;
  if(OT->Items->Count)
    OT->Selected = OT->Items->Item[iiiii];
};

//---------------------------------------------------------------------------

void __fastcall TfrmTree::FormClose(TObject *Sender, TCloseAction &Action)
{
  Action = caFree;
}



//---------------------------------------------------------------------------

void __fastcall TfrmTree::OTMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{

  LastNode = OT->GetNodeAt( X,  Y);
  if(LastNode){
    PopupMenu=(TPopupMenu*)LastNode->Data;
  }
  else
   PopupMenu=0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuAddDataServerClick(TObject *Sender)
{
  AnsiString ds =InputBox("Добавить сервер данных","Имя сервера","");
  if(!ds.IsEmpty()){
   man->GetDS()->AddTarget(man->GetConnect(),ds.c_str());
   FillTree();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuDelTargetClick(TObject *Sender)
{

  if(!LastNode) return;
  TTreeNode *s = LastNode;
  int button;
  button = Application->MessageBox("Удаление сервера данных ... ",s->Text.c_str(), MB_OKCANCEL +
    MB_DEFBUTTON1);
 if (button == IDOK){
   man->GetDS()->DelTarget(man->GetConnect(),s->Text.c_str());
   FillTree();
  }

}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuAddPlatfromClick(TObject *Sender)
{
  AnsiString ds =InputBox("Добавить платформу","Имя платформы","");
  if(!ds.IsEmpty()){
   man->GetDS()->AddPlatform(man->GetConnect(),ds.c_str());
   FillTree();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuDelPlatformClick(TObject *Sender)
{
  if(!LastNode) return;
  TTreeNode *s = LastNode;
  int button;

  button = Application->MessageBox("Удаление платформы ... ",s->Text.c_str(), MB_OKCANCEL +
    MB_DEFBUTTON1);
  if (button == IDOK){
   man->GetDS()->DelPlatform(man->GetConnect(),s->Text.c_str());
   FillTree();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuAddComponentClick(TObject *Sender)
{
  if(!LastNode) return;
  TTreeNode *s = LastNode;

  AnsiString ds =InputBox("Добавить компонент","Имя компонента","");
  if(!ds.IsEmpty()){
   Octets oc;
   man->GetDS()->SetComponent(man->GetConnect(),s->Text.c_str(),ds.c_str(),oc);
   FillTree();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuDelComponentClick(TObject *Sender)
{
  if(!LastNode) return;
  TTreeNode *s = LastNode;
  int button;

  button = Application->MessageBox("Удаление компонента ... ",s->Text.c_str(), MB_OKCANCEL +
    MB_DEFBUTTON1);
  if (button == IDOK){
   man->GetDS()->DelComponent(man->GetConnect(),s->Parent->Text.c_str(),s->Text.c_str());
   FillTree();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuAddSystemClick(TObject *Sender)
{
  AnsiString ds =InputBox("Добавить систему","Имя системы","");
  if(!ds.IsEmpty()){
   man->GetDS()->AddSystem(man->GetConnect(),ds.c_str());
   FillTree();
  }

}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuDelSystemClick(TObject *Sender)
{
  if(!LastNode) return;
  TTreeNode *s = LastNode;
  int button;

  button = Application->MessageBox("Удаление системы ... ",s->Text.c_str(), MB_OKCANCEL +
    MB_DEFBUTTON1);
  if (button == IDOK){
   man->GetDS()->DelSystem(man->GetConnect(),s->Text.c_str());
   FillTree();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuAddObjectClick(TObject *Sender)
{
  if(!LastNode) return;
  TTreeNode *s = LastNode;

  Octets ccc;
  TfrmAddObject *ao = new TfrmAddObject(this);
  ao->cmbComponent->Items->Clear();

  Octets*  oc= man->GetDS()->GetComponentList(
      man->GetConnect(),
      PLATFORM
      );
    MyMemory *mm=0;
    if(oc)
      mm = MakeMemory(*oc);
    delete oc;
    MyArray<AnsiString> sss;
    if(mm)
      *mm >> sss;
    delete mm;
    for(long j=0;j<sss.Count();j++){
      ao->cmbComponent->Items->Add(*(sss[j]));
    }
    do{
      if(ao->ShowModal()==mrCancel) return;

      if(!ao->txtObjName->Text.IsEmpty() &&
          ao->cmbComponent->ItemIndex!=-1){
        man->GetDS()->SetObject(man->GetConnect(),
          s->Text.c_str(),
          ao->txtObjName->Text.c_str(),
          ao->cmbComponent->Items->Strings[ao->cmbComponent->ItemIndex].c_str(),
          ccc
        );
        delete ao;
        FillTree();
        return;

      }
    }while(1);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuAddBuildClick(TObject *Sender)
{
  if(!LastNode) return;
  TTreeNode *s = LastNode;

  AnsiString ds =InputBox("Добавить версию","Имя версии","");
  if(!ds.IsEmpty()){
   man->GetDS()->AddBuild(man->GetConnect(),ds.c_str(),s->Text.c_str());
   FillTree();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuAddSourceClick(TObject *Sender)
{
  if(!LastNode) return;
  TTreeNode *s = LastNode;


  AnsiString ds =InputBox("Добавить источник данных","Имя источника","");
  if(!ds.IsEmpty()){
   man->GetDS()->AddDataSource(man->GetConnect(),s->Text.c_str(),ds.c_str());
   FillTree();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuDelDataSourceClick(TObject *Sender)
{
  if(!(LastNode && LastNode->Parent && LastNode->Parent->Parent)) return;
  TTreeNode *s = LastNode;
  int button;

  button = Application->MessageBox("Удаление источника данных ... ",s->Text.c_str(), MB_OKCANCEL +
    MB_DEFBUTTON1);
  if (button == IDOK){
   man->GetDS()->DelDataSource(man->GetConnect(),s->Parent->Parent->Text.c_str(),s->Text.c_str());
   FillTree();
  }

}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuDelBuildClick(TObject *Sender)
{
  if(!(LastNode && LastNode->Parent && LastNode->Parent->Parent)) return;
  TTreeNode *s = LastNode;
  int button;
  button = Application->MessageBox("Удаление версии системы ... ",s->Text.c_str(), MB_OKCANCEL +
    MB_DEFBUTTON1);
  if (button == IDOK){
   man->GetDS()->DelBuild(man->GetConnect(),s->Text.c_str(),s->Parent->Parent->Text.c_str());
   FillTree();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuDelObjectClick(TObject *Sender)
{
  if(!(LastNode && LastNode->Parent && LastNode->Parent->Parent)) return;
  TTreeNode *s = LastNode;
  int button;

  button = Application->MessageBox("Удаление объекта ... ",s->Text.c_str(), MB_OKCANCEL +
    MB_DEFBUTTON1);
  if (button == IDOK){
   man->GetDS()->DelObject(man->GetConnect(),s->Parent->Parent->Text.c_str(),s->Text.c_str());
   FillTree();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuAddUserClick(TObject *Sender)
{
  AnsiString ds =InputBox("Добавить пользователя","Имя пользователя","");
  if(!ds.IsEmpty()){
   man->GetDS()->SetUser(man->GetConnect(),ds.c_str(),"",0);
   FillTree();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuDelUserClick(TObject *Sender)
{
  if(!LastNode) return;
  TTreeNode *s = LastNode;
  int button;

  button = Application->MessageBox("Удаление пользователя ... ",s->Text.c_str(), MB_OKCANCEL +
    MB_DEFBUTTON1);
  if (button == IDOK){
   man->GetDS()->DelUser(man->GetConnect(),s->Text.c_str());
   FillTree();
  }

}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuEditUserClick(TObject *Sender)
{
  if(!LastNode) return;
  TfrmEditUser * eu =  new TfrmEditUser(this);
  eu->SetUser(LastNode->Text);
  eu->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuObjectCommonClick(TObject *Sender)
{
  TTreeNode *s = LastNode;
  if(!(s && s->Parent && s->Parent->Parent)) return;

  ObjectComponent * c =man->GetComponent(s->Text,s->Parent->Parent->Text);

  Octets *oc=0;
  oc = man->GetDS()->GetObject(
      man->GetConnect(),
      s->Parent->Parent->Text.c_str(),
      s->Text.c_str()
  );

  MyMemory * m;
  if(oc){
    m=MakeMemory(*oc);
    delete oc;
  }
  oc=0;
  ObjectInfo oi;
  *m >> oi;
  delete m;
  DataBuffer *db=c->EditPI(&oi.ObjectDef,s->Parent->Parent->Text);
  if(db){
    oc=MakeOctets(*db);
    delete db;
  }

  if(!oc)
    oc = new Octets;

  man->GetDS()->SetObject(
      man->GetConnect(),
      s->Parent->Parent->Text.c_str(),
      s->Text.c_str(),
      oi.ComponentName.c_str(),
      *oc
  );

  delete oc;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuObjectPlatformClick(TObject *Sender)
{

  TTreeNode *s = LastNode;
  if(!(s && s->Parent && s->Parent->Parent)) return;


  ObjectComponent * c =man->GetComponent(s->Text,s->Parent->Parent->Text);

  Octets *oc=0;
  oc = man->GetDS()->GetObject(
      man->GetConnect(),
      s->Parent->Parent->Text.c_str(),
      s->Text.c_str()
  );
  ObjectInfo oi;
  MyMemory * m=0;
  if(oc){
    m=MakeMemory(*oc);
    delete oc;
    if(m){
      *m >> oi;
      delete m;
    }
  }

  oc=0;
  DataBuffer ps;

  oc =man->GetDS()->GetObjectPlatformSpecifics(
      man->GetConnect(),
      s->Parent->Parent->Text.c_str(),
      s->Text.c_str(),
      "TEST"
  );


  if(oc){
    SetDataBuffer(ps,*oc);
    delete oc;
  }

  oc=0;

  DataBuffer *db=c->EditPD(&ps,s->Parent->Parent->Text);
  if(db){
    oc=MakeOctets(*db);
    delete db;
  }

  if(!oc)
    oc = new Octets;

  man->GetDS()->SetObject(
      man->GetConnect(),
      s->Parent->Parent->Text.c_str(),
      s->Text.c_str(),
      PLATFORM,
      *oc
  );

  delete oc;

}
//---------------------------------------------------------------------------

void __fastcall TfrmTree::mnuEditBuildClick(TObject *Sender)
{
  if(!(LastNode && LastNode->Parent && LastNode->Parent->Parent)) return;
  TfrmEditBuild *eb = new TfrmEditBuild(this);
  eb->SetBuild(LastNode->Parent->Parent->Text,LastNode->Text);
  eb->ShowModal();
  delete eb;
}
//---------------------------------------------------------------------------


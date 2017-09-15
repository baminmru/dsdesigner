//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "EditUser.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"
TfrmEditUser *frmEditUser;
//---------------------------------------------------------------------
__fastcall TfrmEditUser::TfrmEditUser(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void TfrmEditUser::SetUser(AnsiString _UID){
  UID=_UID;
  txtUID->Text=UID;
  Octets *oc=man->GetDS()->GetUser(man->GetConnect(),UID.c_str());
  if(!oc) return;
  MyMemory *m = MakeMemory(*oc);
  delete oc;
  *m>>ui;
  delete m;
  ubr.Clear();
  ur.Clear();
  txtPWD->Text=ui.PWD;
  chkAdmin->Checked = ui.IsAdmin;
  MyArray<AnsiString> S,O,B;

  oc = man->GetDS()->GetSystemList(man->GetConnect());
  m=0;
  if(oc)
    m = MakeMemory(*oc);
  S.Clear();
  if(m)
    *m >> S;
  delete oc;
  delete m;
  for(long i=0;i<S.Count();i++){
    /// OBJECTS
    oc = man->GetDS()->GetSystemObjectsList(man->GetConnect(),S[i]->c_str());
    m=0;
    if(oc)
      m = MakeMemory(*oc);
    delete oc;
    if(m)
      *m >> O;
    delete m;
    for(long j=0;j<O.Count();j++){
        UserRights *pur = new UserRights;
        pur->Execute=false;
        pur->Read=false;
        pur->Write=false;
        pur->SystemName=*S[i];
        pur->UserName=UID;
        pur->ObjectName=*O[j];
        ur.Add(pur);
    }
    /// BUILDS
    oc = man->GetDS()->GetSystemBuildList(man->GetConnect(),S[i]->c_str());
    m=0;
    if(oc)
      m = MakeMemory(*oc);
    delete oc;

    if(m)
      *m >> B;
    delete m;
    for(long j=0;j<B.Count();j++){
        UserBuildRight *pubr = new UserBuildRight;
        pubr->AllowAccess=false;
        pubr->SystemName=*S[i];
        pubr->UserName=UID;
        pubr->BuildName=*B[j];
        ubr.Add(pubr);
    }

  }

  // сканируем ur и ubr
  for(long i=0;i< ubr.Count();i++){
    ubr[i]->AllowAccess =
    man->GetDS()->GetBuildRights(
    man->GetConnect(),
    ubr[i]->SystemName.c_str(),
    ubr[i]->BuildName.c_str(),
    UID.c_str()
    );
    chkBuilds->Checked[
      chkBuilds->Items->Add(ubr[i]->SystemName+" -> " +ubr[i]->BuildName)
    ] = ubr[i]->AllowAccess;
  }

  for(long i=0;i< ur.Count();i++){
    Octets *oc=
    man->GetDS()->GetObjectRights(
    man->GetConnect(),
    ur[i]->SystemName.c_str(),
    ur[i]->ObjectName.c_str(),
    UID.c_str()
    );
    if(oc){
      MyMemory *m=MakeMemory(*oc);
      delete oc;
      UserRights UR;
      *m >>UR;
        ur[i]->Execute=UR.Execute;
        ur[i]->Read=UR.Read;
        ur[i]->Write=UR.Write;

    }

   chkObj->Checked[
      chkObj->Items->Add(ur[i]->SystemName+" -> " +ur[i]->ObjectName)
    ] = ur[i]->Execute;

  }



};
void __fastcall TfrmEditUser::OKBtnClick(TObject *Sender)
{

  man->GetDS()->SetUser(
    man->GetConnect(),
    UID.c_str(),
    txtPWD->Text.c_str(),
    chkAdmin->Checked
  );


 for(long i=0;i< ubr.Count();i++){
    man->GetDS()->SetBuildRights(
    man->GetConnect(),
    ubr[i]->SystemName.c_str(),
    ubr[i]->BuildName.c_str(),
    UID.c_str(),
    chkBuilds->Checked[i]
    );
  }

  for(long i=0;i< ur.Count();i++){
    man->GetDS()->SetObjectRights(
    man->GetConnect(),
    ur[i]->SystemName.c_str(),
    ur[i]->ObjectName.c_str(),
    UID.c_str(),
    chkObj->Checked[i],
    chkObj->Checked[i],
    chkObj->Checked[i]
    );
  }

}
//---------------------------------------------------------------------------


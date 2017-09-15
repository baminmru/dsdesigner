//---------------------------------------------------------------------------
#ifndef fTreeH
#define fTreeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TfrmTree : public TForm
{
__published:	// IDE-managed Components
  TTreeView *OT;
  TPopupMenu *mnuSystems;
  TMenuItem *mnuAddSystem;
  TPopupMenu *mnuObject;
  TMenuItem *mnuDelObject;
  TPopupMenu *mnuPlatforms;
  TMenuItem *mnuAddPlatfrom;
  TPopupMenu *mnuPlatform;
  TMenuItem *mnuDelPlatform;
  TPopupMenu *mnuDataServers;
  TPopupMenu *mnuDataServer;
  TPopupMenu *mnuSystem;
  TMenuItem *mnuAddObject;
  TMenuItem *mnuAddSource;
  TMenuItem *mnuAddBuild;
  TPopupMenu *mnuBuild;
  TMenuItem *mnuDelBuild;
  TMenuItem *mnuEditBuild;
  TPopupMenu *mnuUsers;
  TPopupMenu *mnuUser;
  TMenuItem *mnuAddUser;
  TMenuItem *mnuEditUser;
  TMenuItem *mnuDelUser;
  TPopupMenu *mnuSource;
  TMenuItem *mnuDelDataSource;
  TMenuItem *mnuAddComponent;
  TPopupMenu *mnuComponent;
  TMenuItem *mnuEditComponent;
  TMenuItem *mnuDelComponent;
  TMenuItem *mnuDelSystem;
  TMenuItem *mnuAddDataServer;
  TMenuItem *mnuDelTarget;
  TMenuItem *N1;
  TMenuItem *mnuObjectCommon;
  TMenuItem *mnuObjectPlatform;
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall OTMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall mnuAddDataServerClick(TObject *Sender);
  void __fastcall mnuDelTargetClick(TObject *Sender);
  void __fastcall mnuAddPlatfromClick(TObject *Sender);
  void __fastcall mnuDelPlatformClick(TObject *Sender);
  void __fastcall mnuAddComponentClick(TObject *Sender);
  void __fastcall mnuDelComponentClick(TObject *Sender);
  void __fastcall mnuAddSystemClick(TObject *Sender);
  void __fastcall mnuDelSystemClick(TObject *Sender);
  void __fastcall mnuAddObjectClick(TObject *Sender);
  void __fastcall mnuAddBuildClick(TObject *Sender);
  void __fastcall mnuAddSourceClick(TObject *Sender);
  void __fastcall mnuDelDataSourceClick(TObject *Sender);
  void __fastcall mnuDelBuildClick(TObject *Sender);
  void __fastcall mnuDelObjectClick(TObject *Sender);
  void __fastcall mnuAddUserClick(TObject *Sender);
  void __fastcall mnuDelUserClick(TObject *Sender);
  void __fastcall mnuEditUserClick(TObject *Sender);
  void __fastcall mnuObjectCommonClick(TObject *Sender);
  void __fastcall mnuObjectPlatformClick(TObject *Sender);
  void __fastcall mnuEditBuildClick(TObject *Sender);
private:	// User declarations
  TTreeNode *LastNode;
public:		// User declarations
  __fastcall TfrmTree(TComponent* Owner);
  void __fastcall FillTree(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmTree *frmTree;
//---------------------------------------------------------------------------
#endif

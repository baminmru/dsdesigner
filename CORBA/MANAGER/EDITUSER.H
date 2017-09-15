//----------------------------------------------------------------------------
#ifndef EditUserH
#define EditUserH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include "Manager.h"
//----------------------------------------------------------------------------
class TfrmEditUser : public TForm
{
__published:
  TPanel *Panel1;
  TButton *OKBtn;
  TButton *CancelBtn;
  TPageControl *PageControl1;
  TTabSheet *TabSheet1;
  TTabSheet *TabSheet2;
  TTabSheet *TabSheet3;
  TLabel *Label1;
  TEdit *txtUID;
  TEdit *txtPWD;
  TLabel *Label2;
  TCheckBox *chkAdmin;
  TLabel *Label3;
  TCheckListBox *chkBuilds;
  TCheckListBox *chkObj;
  void __fastcall OKBtnClick(TObject *Sender);
private:
  AnsiString UID;
  UserInfo ui;
  MyArray<UserBuildRight> ubr;
  MyArray<UserRights> ur;
public:
  void SetUser(AnsiString UID);
	virtual __fastcall TfrmEditUser(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TfrmEditUser *frmEditUser;
//----------------------------------------------------------------------------
#endif    

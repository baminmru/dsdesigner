//----------------------------------------------------------------------------
#ifndef AboutH
#define AboutH
//----------------------------------------------------------------------------
#include <System.hpp>
#include <Windows.hpp>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Forms.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//----------------------------------------------------------------------------
class TAboutBox : public TForm
{
__published:
	TPanel *Panel1;
	TImage *ProgramIcon;
	TLabel *Version;
	TButton *OKButton;
 TLabel *TaskName;
  TPanel *Panel2;
  TLabel *lblBuild;
  TLabel *ProductName;
  TLabel *Copyright;
private:
public:
	virtual __fastcall TAboutBox(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TAboutBox *AboutBox;
//----------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#ifndef LoginH
#define LoginH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

//---------------------------------------------------------------------------
class TfrmLogin : public TForm
{
__published:	// IDE-managed Components
  TEdit *txtUID;
  TLabel *Label1;
  TButton *btnConnect;
  TButton *btnExit;
  TEdit *txtPWD;
  TLabel *Label2;
private:	// User declarations
public:		// User declarations
        __fastcall TfrmLogin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLogin *frmLogin;
//---------------------------------------------------------------------------
#endif

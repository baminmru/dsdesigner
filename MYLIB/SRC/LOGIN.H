//---------------------------------------------------------------------------
#ifndef LogInH
#define LogInH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmLogin : public TForm
{
__published:	// IDE-managed Components
 TLabel *Label1;
 TButton *Button1;
 TButton *Button2;
 TLabel *Label2;
 TEdit *txtUID;
 TLabel *Label3;
 TEdit *txtPWD;
 void __fastcall FormShow(TObject *Sender);
 
private:	// User declarations
public:		// User declarations
 __fastcall TfrmLogin(TComponent* Owner);
 AnsiString SourceName;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLogin *frmLogin;
//---------------------------------------------------------------------------
#endif

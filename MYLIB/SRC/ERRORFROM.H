//---------------------------------------------------------------------------
#ifndef ErrorFromH
#define ErrorFromH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmError : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TButton *Button1;
	TMemo *msg;
private:	// User declarations
public:		// User declarations
	__fastcall TfrmError(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmError *frmError;
//---------------------------------------------------------------------------
#endif

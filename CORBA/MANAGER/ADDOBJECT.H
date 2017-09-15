//---------------------------------------------------------------------------
#ifndef AddObjectH
#define AddObjectH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmAddObject : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TButton *Button1;
  TButton *Button2;
  TLabel *Label1;
  TEdit *txtObjName;
  TComboBox *cmbComponent;
  TLabel *Label2;
private:	// User declarations
public:		// User declarations
  __fastcall TfrmAddObject(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAddObject *frmAddObject;
//---------------------------------------------------------------------------
#endif

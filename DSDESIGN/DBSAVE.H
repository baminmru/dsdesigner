//---------------------------------------------------------------------------
#ifndef DBSaveH
#define DBSaveH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmDBSave : public TForm
{
__published:	// IDE-managed Components
 TLabel *Label1;
 TComboBox *cmbDSN;
 TEdit *txtTask;
 TLabel *Label2;
 TButton *Button1;
 TButton *Button2;
private:	// User declarations
public:		// User declarations
 __fastcall TfrmDBSave(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDBSave *frmDBSave;
//---------------------------------------------------------------------------
#endif

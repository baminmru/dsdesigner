//---------------------------------------------------------------------------
#ifndef EditGridH
#define EditGridH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmEditGrid : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label1;
  TButton *Button1;
  TButton *Button2;
  TComboBox *cmbSources;
  TMemo *txtMainQuery;
  TLabel *Label2;
  TEdit *txtCaption;
  TLabel *Label3;
private:	// User declarations
public:		// User declarations
  __fastcall TfrmEditGrid(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEditGrid *frmEditGrid;
//---------------------------------------------------------------------------
#endif

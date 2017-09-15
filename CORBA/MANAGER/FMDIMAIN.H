//---------------------------------------------------------------------------
#ifndef fMDIMainH
#define fMDIMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TMDIMain : public TForm
{
__published:	// IDE-managed Components
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormDblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TMDIMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMDIMain *MDIMain;
//---------------------------------------------------------------------------
#endif

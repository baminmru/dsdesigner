//---------------------------------------------------------------------------
#ifndef fMDIChildH
#define fMDIChildH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TMDIChild : public TForm
{
__published:	// IDE-managed Components
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
  __fastcall TMDIChild(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMDIChild *MDIChild;
//---------------------------------------------------------------------------
#endif

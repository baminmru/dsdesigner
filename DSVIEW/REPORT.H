//---------------------------------------------------------------------------
#ifndef ReportH
#define ReportH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CRPrintEngine.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmReport : public TForm
{
__published:	// IDE-managed Components
 TCRPrintEngine *Report1;
 void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
 __fastcall TfrmReport(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmReport *frmReport;
//---------------------------------------------------------------------------
#endif

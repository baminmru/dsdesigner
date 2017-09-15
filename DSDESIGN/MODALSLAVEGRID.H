//---------------------------------------------------------------------------
#ifndef ModalSlaveGridH
#define ModalSlaveGridH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "StreamGrid.h"
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "RXGrids.hpp"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmModalSlave : public TForm
{
__published:	// IDE-managed Components
 TStreamGrid *StreamGrid1;
 TPanel *LinkHolder;
 TPanel *Panel4;
 TSpeedButton *btnLinkLeft;
 TSpeedButton *btnLinkRight;
 TPanel *Panel5;
 THeaderControl *HeaderControl1;
 TPanel *TranHolder;
 TPanel *Panel3;
 TSpeedButton *btnTranLeft;
 TSpeedButton *btnTranRight;
 TPanel *Panel6;
 THeaderControl *HeaderControl2;void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
 void __fastcall btnLinkRightClick(TObject *Sender);
 void __fastcall btnLinkLeftClick(TObject *Sender);
 
 void __fastcall HeaderControl1SectionClick(THeaderControl *HeaderControl,
          THeaderSection *Section);
 void __fastcall btnTranRightClick(TObject *Sender);
 void __fastcall btnTranLeftClick(TObject *Sender);
 void __fastcall HeaderControl2SectionClick(THeaderControl *HeaderControl,
          THeaderSection *Section);
 void __fastcall HeaderControl1SectionResize(THeaderControl *HeaderControl,
          THeaderSection *Section);
 void __fastcall Panel5Resize(TObject *Sender);
 void __fastcall Panel6Resize(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmModalSlave(TComponent* Owner);
  void __fastcall Init();
  long LinkIdx;
  long TranIdx;

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmModalSlave *frmModalSlave;
//---------------------------------------------------------------------------
#endif

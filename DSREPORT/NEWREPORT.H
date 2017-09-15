//---------------------------------------------------------------------------
#ifndef NewReportH
#define NewReportH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "New_dw.h"
//---------------------------------------------------------------------------
class PACKAGE TNewReport : public TCustomPanel
{
private:
 bool FConnected;
 HDC DC;
 odbcEnvironment *Enviro;
 odbcConnection *Connection;
 ODBCDataStream *DS;
 DataWindow *DW;
 TScrollBar *HBar, *VBar;
 TPrintDialog *PrintDialog;
 TPrinterSetupDialog *PrinterSetup;
 virtual void __fastcall Paint(void);
 void __fastcall ScrollBarScroll(TObject *Sender, TScrollCode ScrollCode, int &ScrollPos);
 void __fastcall MeResize(TObject *Sender);
protected:
public:
 __fastcall TNewReport(TComponent* Owner);
 int __fastcall Connect(AnsiString DSN, AnsiString UID, AnsiString PWD, AnsiString SQL_Select);
 bool __property Connected={read=FConnected};
__published:
 __property Align;
 __property Left;
 __property Top;
 __property Width;
 __property Height;
};
//---------------------------------------------------------------------------
#endif

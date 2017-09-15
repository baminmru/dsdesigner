//---------------------------------------------------------------------------
#ifndef StreamComboH
#define StreamComboH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "StreamGrid.H"
//#include "ToolEdit.hpp"
#include "StreamDialog.h"
//---------------------------------------------------------------------------
class PACKAGE TStreamCombo : public TPanel
{
private:
    bool Inited;
    TEdit *Edit1;
   	TStreamDlg * StreamDialog;
    TSpeedButton *Button1;
protected:
	  void __fastcall SCOnResize(TObject * Sender);
 	  void __fastcall SCOnEnter(TObject * Sender);
	  void __fastcall SCButtonClick(TObject * Sender);
    void __fastcall SCChange(TObject * Sender);
    void __fastcall SCOnExit(TObject * Sender);
	  void __fastcall CreateWnd();
	  void __fastcall DestroyWnd();
    void __fastcall Init();
    void __fastcall TStreamCombo::SCOnKeyDown(
            System::TObject* Sender,
            Word &Key,
            Classes::TShiftState Shift);
public:
    DataManager * Manager;
    AnsiString Editor;
    AnsiString SyncFieldName;
    AnsiString ShowFieldName;
    StackValue Value;
    EditFieldActions *Actions;
    Params *CurrentRowState;
    void __fastcall TextFromValue();
    void __fastcall UnInit();
    TNotifyEvent OnChange;

	__fastcall TStreamCombo(TComponent* Owner);
__published:
};
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#ifndef StreamComboH
#define StreamComboH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "StreamGrid.H"
#include "ToolEdit.hpp"
//---------------------------------------------------------------------------
class PACKAGE TStreamCombo : public TComboEdit
{
private:
protected:
	void __fastcall ButtonClick(TObject * Sender);
    void __fastcall Change(TObject * Sender);
	void __fastcall CreateWnd();
	void __fastcall DestroyWnd();
	TStreamDlg * StreamDialog;
public:
    DataManager * Manager;
    AnsiString Editor;
    AnsiString SyncFieldName;
    AnsiString ShowFieldName;
    StackValue Value;
    EditFieldActions *Actions;
    Params *CurrentRowState;

	__fastcall TStreamCombo(TComponent* Owner);
__published:
};
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#ifndef StreamDialogH
#define StreamDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//#include "RXGrids.hpp"
#include "StreamGrid.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
class TStreamGrid;
//---------------------------------------------------------------------------
class TStreamDlg : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TButton *Button1;
	TButton *Button2;
	TPanel *GridPanel;
 TPanel *TranHolder;
 TPanel *Panel3;
 TSpeedButton *btnTranLeft;
 TSpeedButton *btnTranRight;
 TPanel *Panel6;
 THeaderControl *HeaderControl2;
void __fastcall Button1Click(TObject *Sender);
  
 void __fastcall FormShow(TObject *Sender);
 void __fastcall HeaderControl2SectionClick(THeaderControl *HeaderControl,
          THeaderSection *Section);
 void __fastcall btnTranRightClick(TObject *Sender);
 void __fastcall btnTranLeftClick(TObject *Sender);
 
 void __fastcall HeaderControl2SectionResize(THeaderControl *HeaderControl,
          THeaderSection *Section);
 void __fastcall Panel6Resize(TObject *Sender);
private:	// User declarations
public:		// User declarations
    void *StreamGrid1;

    DataManager * Manager;
    AnsiString Editor;
    AnsiString SyncFieldName;
    AnsiString ShowFieldName;
    StackValue Value;
    EditFieldActions *Actions;
    Params *CurrentRowState;

    void Setup(DataManager *_Manager,AnsiString _Editor){
    	Manager=_Manager;
        Editor=_Editor;
    };

          // Задали способ преобразования редактора
    void SetActions(EditFieldActions *_Actions, Params *_CurrentRowState){
    	Actions=_Actions;
        CurrentRowState=_CurrentRowState;
    };

          //  Задали текущую строку
    void SetValue(AnsiString _SyncFieldName, StackValue *_Value){
          SyncFieldName=_SyncFieldName;
          Value=_Value;
    };

    void ActivateEditor();
    StackValue *Lookup(AnsiString SyncFieldName,
                      AnsiString ShowFieldName,
                      AnsiString Text);
    AnsiString LookupText( AnsiString ShowFieldName,
                           AnsiString Text);

    TModalResult RunDialog(AnsiString *TextToFind=0);

    StackValue GetValue( AnsiString FieldName);

	__fastcall TStreamDlg(TComponent* Owner);

 long TranIdx;
};
//---------------------------------------------------------------------------
extern PACKAGE TStreamDlg *StreamDlg;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "StreamCombo.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TStreamCombo *)
{
	new TStreamCombo(NULL);
}
//---------------------------------------------------------------------------
__fastcall TStreamCombo::TStreamCombo(TComponent* Owner)
	: TComboEdit(Owner)
{
	Manager=0;
    StreamDialog = 0;
    Actions=0;
    OnButtonClick=ButtonClick;
    OnChange=Change;

}

void __fastcall TStreamCombo::ButtonClick(TObject * Sender);
void __fastcall TStreamCombo::CreateWnd(){
	TComboEdit::CreateWnd();
    StreamDialog = new TStreamDlg(this);
    StreamDialog->Parent=this;
};

void __fastcall TStreamCombo::DestroyWnd(){
    delete StreamDialog;
 	TComboEdit::DestroyWnd();
};

void __fastcall TStreamCombo::ButtonClick(TObject * Sender){
	  if(Manager && !Editor.IsEmpty())

          // «адали параметры редактора
	      StreamDialog->Setup(Manager,Editor);

          // «адали способ преобразовани€ редактора
          StreamDialog->SetActions(Actions, CurrentRowState);

          //  «адали текущую строку
		  StreamDialog->SetValue(SyncFieldName, Value);

          // показали редактор
	      if(StreamDialog->RunDialog()==mrOk){

            // получили значение пол€
			Value = StreamDialog->GetValue(SyncFieldName);

            // получили изображение
            Text= StreamDialog->GetValue(ShowFieldName);
          };


};

//---------------------------------------------------------------------------
namespace Streamcobmo
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TStreamCombo)};
		RegisterComponents("System", classes, 0);
	}
}
//---------------------------------------------------------------------------

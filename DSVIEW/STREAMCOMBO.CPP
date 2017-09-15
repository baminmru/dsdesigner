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
	: TPanel(Owner)
{
  	Manager=0;
    StreamDialog = 0;
    Actions=0;
    OnEnter=SCOnEnter;
    OnChange=0;
    Inited=false;
    TabStop = false;
}


void __fastcall TStreamCombo::CreateWnd(){
	  TPanel::CreateWnd();
		if(!StreamDialog){
				StreamDialog = new TStreamDlg(this);
		}
		StreamDialog->Width = 530;
		StreamDialog->Height = 400;
    Height=20;

    Button1= new TSpeedButton(this);
    Button1->Parent=this;
    Button1->Caption="...";
    Button1->Width=18;
    Button1->Top=2;
    Button1->Left=Width-19;
    Button1->Height = Height-4;
    Button1->OnClick =SCButtonClick;
    //Button1->TabStop = true;

    Edit1= new TEdit(this);
    Edit1->Parent=this;
    Edit1->Height = Height;
    Edit1->Width=Width-20;
    Edit1->Top=0;
    Edit1->Left=0;
    Edit1->OnChange =SCChange;
    Edit1->OnDblClick=SCButtonClick;
    Edit1->OnKeyDown=SCOnKeyDown;
    Edit1->OnExit=SCOnExit;
    OnResize=SCOnResize;
};

void __fastcall TStreamCombo::SCOnResize(TObject * Sender){
    if(Button1){
      Button1->Width=18;
      Button1->Top=2;
      Button1->Left=Width-19;
      Button1->Height = Height-4;
    }
    if(Edit1){
      Edit1->Height = Height;
      Edit1->Width=Width-20;
      Edit1->Top=0;
      Edit1->Left=0;
    }
};

void __fastcall TStreamCombo::SCOnKeyDown(
            System::TObject* Sender,
            Word &Key,
            Classes::TShiftState Shift){
  if(Shift.Contains(ssAlt)){
    if(Key==VK_DOWN)
      SCButtonClick(Sender);
  }
  if(!(Shift.Contains(ssAlt) ||
       Shift.Contains(ssCtrl) ||
       Shift.Contains(ssShift) )){
       if(Key==VK_F2)
          SCButtonClick(Sender);
  }
};

void __fastcall TStreamCombo::SCOnEnter(TObject * Sender){
  Edit1->SetFocus();
};


void __fastcall TStreamCombo::DestroyWnd(){
	delete StreamDialog;
  delete Button1;
  delete Edit1;
	TPanel::DestroyWnd();
};

void __fastcall TStreamCombo::UnInit(){
   Inited = false;
};

void __fastcall TStreamCombo::Init(){
	if(Manager && !Editor.IsEmpty()  ){
		if(!Inited){
				if(!StreamDialog){
										StreamDialog = new TStreamDlg(this);
			  }
						// Задали параметры редактора
				StreamDialog->Setup(Manager,Editor);

						// Задали способ преобразования редактора
				StreamDialog->SetActions(Actions, CurrentRowState);

				StreamDialog->ActivateEditor();

				Inited = true;
		}
	}
};

void __fastcall TStreamCombo::TextFromValue(){
	Init();
	if(Inited){
			if(Value.IsNull()){
					Edit1->Text="";
          if(OnChange)
            OnChange(this);
						return;
				}
				StackValue * v;
				v = StreamDialog->Lookup(ShowFieldName,SyncFieldName,Value.ToStr());
				if(v) {
          Edit1->Text=v->ToStr();
          if(OnChange)
            OnChange(this);
        }
		}
};

void __fastcall TStreamCombo::SCOnExit(TObject * Sender){
	 Init();
	 if(Inited){
			if(Edit1->Text.IsEmpty()){
      		Value.Erase();
          if(OnChange)
            OnChange(this);
 					return;
		 	}

		 	StackValue * v;
		 	v = StreamDialog->Lookup(SyncFieldName, ShowFieldName,Edit1->Text);
			if(v) {
        Value=*v;
        Edit1->Text= StreamDialog->GetValue(ShowFieldName).ToStr();
        if(OnChange)
            OnChange(this);

     }else{
       Value.Erase();
       Edit1->Text="";
       if(OnChange)
            OnChange(this);

     }
	 }
};

void __fastcall TStreamCombo::SCChange(TObject * Sender){
		 Init();
		 if(Inited){

			if(Edit1->Text.IsEmpty()){
					Value.Erase();
          if(OnChange)
            OnChange(this);
					return;
				}

				StackValue * v;
				v = StreamDialog->Lookup(SyncFieldName, ShowFieldName,Edit1->Text);
				if(v) {
          Value=*v;
          if(OnChange)
            OnChange(this);

        }
		 }
};

void __fastcall TStreamCombo::SCButtonClick(TObject * Sender){
		Init();

			if(Inited){

				StreamDialog->Lookup(SyncFieldName, ShowFieldName,Edit1->Text);

				// показали редактор
				if(StreamDialog->RunDialog()==mrOk){
							// получили значение поля
		      		Value = StreamDialog->GetValue(SyncFieldName);
							// получили изображение
							Edit1->Text= StreamDialog->GetValue(ShowFieldName).ToStr();
              if(OnChange)
                OnChange(this);

					};
			}
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

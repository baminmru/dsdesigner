//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "StreamGrid.h"
#include "EditForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "RXSplit"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
void __fastcall TfrmEditForm::CreateWnd(){
			TForm::CreateWnd();
};

//---------------------------------------------------------------------------
void __fastcall TfrmEditForm::DestroyWnd(){
	 Controls.Clear();
	 TForm::DestroyWnd();
};

//---------------------------------------------------------------------------
__fastcall TfrmEditForm::TfrmEditForm(TComponent* Owner)
	:TForm(Owner)
{
 SettedUp=false;
 OnLoad = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEditForm::btnCancelClick(TObject *Sender)
{
	ModalResult=mrCancel;
};
//---------------------------------------------------------------------------

void __fastcall TfrmEditForm::btnSaveClick(TObject *Sender)
{
	SaveControls();
	if(CheckNulls()){
   try{
    if(Action.Trim().IsEmpty()){
      ModalResult=mrOk;
      return;
    }

    if(s->MakeAction(Action,NewRow)){
     ModalResult=mrOk;
    }

   }catch(...){
     return;
   }
 }
}
//---------------------------------------------------------------------------


int __fastcall TfrmEditForm::Setup(				// mrCancell or mrOK
			DataManager *_Manager,
			EditFieldsAttr *_FormDef,
			EditFieldsDisables *_rights,
			//DataSnapshot *_Snap,
			DataStream *_s,
			bool _ForInsert,
			Params *NewFields,
     AnsiString _Action
		){

			rights=_rights;
			//Snap=_Snap;
			s=_s;
			ForInsert=_ForInsert;
			Manager=_Manager;
			NewRow=NewFields;
     Action=_Action;
     FormDef=_FormDef;

			if(!FormDef || !s ) //|| !Snap)
				return mrCancel;

			if(!NewRow)
				return mrCancel;


			BuildControls();
            SettedUp=true;
			InitControls();

			if(Controls.Count()){
				return ShowModal();
			}else
				return mrCancel;
};


int __fastcall TfrmEditForm::Run(				// mrCancell or mrOK
			//DataManager *_Manager,
   	//EditFieldsAttr *_FormDef,
			//EditFieldsDisables *_rights,
			//DataSnapshot *_Snap,
			//DataStream *_s,
			bool  _ForInsert,
			Params *NewFields,
     AnsiString _Action
		){
			//Manager=_Manager;
			//rights=_rights;
			//FormDef=_FormDef;
			//Snap=_Snap;
			//s=_s;
			ForInsert=_ForInsert;
			NewRow=NewFields;
     Action=_Action;

			//if(!FormDef || !s ) //|| !Snap)
			//	return mrCancel;

			if(!NewRow)
				return mrCancel;

			InitControls();

			if(Controls.Count()){
				return ShowModal();
			}else
				return mrCancel;
};


void __fastcall TfrmEditForm::BuildControls(){
	Controls.Clear();
	long cLeft=Width/3;
	long lLeft=5;
	TWinControl *wc;
	TMaskEdit *me;
	TLabel *lbl;

	long lTabOrder=0;
	long cTop=5;
	for(long i=0;i<(*FormDef).Count();i++){
		EditableFieldAttr * ea= (*FormDef)[i];
   EditFieldDisables *r =0;
   if(rights)
 	 r  = (*rights)[i];

		StackValue *sv=GetFieldValue(ea->FieldName);
		if(ea){
			wc=0;
			me=0;
			lbl=0;
			switch(ea->ControlType){
				case ectNone:
					break;

				case 	ectInteger:
				{
					TMaskEdit *rce=new TMaskEdit(this);
          rce->EditMask="#9999999999999;1;_";
          rce->OnChange=CtrlChange;
					wc=rce;
					break;
				}

				case 	ectNumeric:
				{
					TMaskEdit *rce=new TMaskEdit(this);
          rce->EditMask="#9999999999999.99;1;_";
          rce->OnChange=CtrlChange;
					wc=rce;
					break;
				}

				case 	ectMultiLineText:
				{
					TMemo *mem= new TMemo(this);
					mem->Parent=this;
					mem->ScrollBars =ssBoth;
					wc=mem;
          mem->OnChange=CtrlChange;
					break;
				}
				case 	ectText:
				{
					TEdit *ed= new TEdit(this);
    			wc=ed;
          ed->OnChange=CtrlChange;
					break;
				}
				case 	ectPassword:
				{
					TEdit *ed= new TEdit(this);
					ed->PasswordChar='*';
					wc=ed;
          ed->OnChange=CtrlChange;
					break;
				}

				case 	ectDate:
				{
					TDateTimePicker *de=new TDateTimePicker(this);
					wc=de;
          //          de->YearDigits=dyFour;
                    de->OnChange=CtrlChange;
					break;
				}


				case 	ectTime:
				{
					me= new TMaskEdit(this);
					me->EditMask="!90:00:99;1;_";
					wc=me;
					me=0;
          me->OnChange=CtrlChange;
					break;
				}

				case 	ectDateTime:
					{
					TDateTimePicker *de=new TDateTimePicker(this);
					wc=de;
          //de->YearDigits=dyFour;
          de->OnChange=CtrlChange;
          me= new TMaskEdit(this);
					me->EditMask="!90:00:99;1;_";
          me->OnChange=CtrlChange;
					break;
					}


				case 	ectYesNo:
					{
						TCheckBox *cb;
						cb= new TCheckBox(this);
    				cb->AllowGrayed=true;
            cb->OnClick=CtrlChange;
						wc=cb;
						break;
					}


			 case 	ectSubStream:
					{
						TStreamCombo *ce;
						ce= new TStreamCombo(this);
						//ce->GlyphKind=gkEllipsis;
            ce->OnChange=CtrlChange;
            ce->Height=20;
						wc=ce;
						break;
					}

				default:
						break;
			}


			if(wc){
				wc->Left =cLeft;
				wc->Top  =cTop;
				wc->Parent=cbox;
        if(ea->ControlType!=ectSubStream){
				  wc->TabStop =true;
				  wc->TabOrder=lTabOrder++;
        }else
          lTabOrder++;
				if(me){
					wc->Width = (Width-20)/3;
					me->Left=cLeft+wc->Width+2;
					me->Width=wc->Width;
					me->Top=cTop;
					me->Height=wc->Height;
					me->Parent=cbox;
					me->TabStop =true;
					me->TabOrder=lTabOrder++;
				}else
					wc->Width = (Width-20)*2/3;


				lbl= new TLabel(this);
				lbl->Parent=cbox;
				lbl->Top=wc->Top;
				lbl->Left=lLeft;
				lbl->Caption=ea->Label;
				lbl->Width= (Width-20)/3;
				lbl->Height=21;
       CtrlStorage *cs;

       if(r)
 				cs=new CtrlStorage(r->InsertAccess,r->EditAccess);
       else
         cs=new CtrlStorage((*FormDef)[i]->DefInsertAccess,(*FormDef)[i]->DefEditAccess);

				cs->ctrl=wc;
				cs->lbl=lbl;
				cs->msk=me;
				cs->ColIdx=i;
				Controls.Add(cs);
				cTop+=cs->GetHeight(ForInsert);
			}

		}
	}
	Height=cTop+60;
	Width=Width+22;
};


StackValue * __fastcall  TfrmEditForm::GetFieldValue(AnsiString Name){
		for(long i=0;i<NewRow->Count();i++)
				if(NewRow->Name(i)==Name)
					return (*NewRow)[i];

/*		for(long i=0;i<Snap->params.Count();i++)
			if(Snap->params.Name(i)==Name)
					return Snap->params[i];
*/
   StackValue * ptr;
   NewRow->Add(ptr=new StackValue(),Name);
		return ptr;
};



void __fastcall TfrmEditForm::InitControls(){

    OnLoad = true;
	TWinControl *wc;
	TMaskEdit *me;
	TLabel *lbl;

	for(long i=0;i<(*FormDef).Count();i++){
		EditableFieldAttr * ea=(*FormDef)[i];

		StackValue *sv=GetFieldValue(ea->FieldName);
		CtrlStorage *cs=Controls[i];
		wc=cs->ctrl;
		lbl=cs->lbl;
		me=cs->msk;

		if(ea){
			switch(ea->ControlType){
				case ectNone:
					break;

				case 	ectInteger:
				{
					TMaskEdit *rce=(TMaskEdit*) wc;
					if(!sv->IsNull())
						rce->Text=StrClearAllChar(sv->ToStr());
         else
           rce->Text="";
					break;
				}

				case 	ectNumeric:
				{
					TMaskEdit *rce=(TMaskEdit*) wc;
					if(!sv->IsNull())
						rce->Text=StrClearAllChar(sv->ToStr());
         else
           rce->Text="";
					break;
				}

				case 	ectMultiLineText:
				{
					TMemo *mem= (TMemo*)wc;
					mem->Lines->Text=sv->ToStr().Trim();
					break;
				}
				case 	ectText:
				{
					TEdit *ed= (TEdit*) wc;
					ed->Text=sv->ToStr().Trim();
					break;
				}
				case 	ectPassword:
				{
					TEdit *ed= (TEdit*) wc;
					ed->Text=sv->ToStr().Trim();
					break;
				}

				case 	ectDate:
				{
					TDateTimePicker *de=(TDateTimePicker*) wc;
					de->Date=sv->ToDate();
					break;
				}


				case 	ectTime:
				{
					me= (TMaskEdit*) wc;
					me->Text=sv->ToDate().TimeString();
					break;
				}

				case 	ectDateTime:
					{
					TDateTimePicker *de=(TDateTimePicker*)wc;
						de->Date=sv->ToDate();

					me->Text=sv->ToDate().TimeString();
					break;
					}


				case 	ectYesNo:
					{
						TCheckBox *cb;
						cb= (TCheckBox*)wc;
						cb->AllowGrayed=true;
						if(sv && sv->IsNull())
							cb->State=cbGrayed;
						else
							if(sv && sv->ToNumeric()!=0.0)
								cb->State=cbChecked;
							else
								cb->State=cbUnchecked;
						break;
					}


			 case 	ectSubStream:
					{
						TStreamCombo *ce;
						ce= (TStreamCombo*)wc;
						ce->Manager=Manager;
						ce->Editor=ea->StreamName;
						ce->SyncFieldName=ea->SyncField;
						ce->ShowFieldName=ea->ShowField;
           ce->Value.Clear();
						ce->Value=*sv;
						ce->Actions=ea->WhenChanged;
						ce->CurrentRowState=NewRow;
           if(ea->FillOnFormShow){
             ce->UnInit();
           }
						ce->TextFromValue();
						break;
					}

				default:
						break;
			}

      switch((ForInsert ? cs->InsertAccess:cs->EditAccess)){
					case faNone:
						cs->ctrl->Visible = false;
						cs->lbl->Visible = false;
						if(cs->msk) cs->msk->Visible = false;
						break;
					case faRead:
						cs->ctrl->Visible = true;
						cs->lbl->Visible = true;
						if(cs->msk) cs->msk->Visible = true;
						cs->ctrl->Enabled = false;
						cs->lbl->Enabled = false;
						if(cs->msk) cs->msk->Enabled = false;
						break;
					case faWrite:
						cs->ctrl->Visible = true;
						cs->lbl->Visible = true;
						if(cs->msk) cs->msk->Visible = true;
						cs->ctrl->Enabled = true;
						cs->lbl->Enabled = true;
						if(cs->msk) cs->msk->Enabled = true;
						break;
			}
    }
  }
  OnLoad = false;
};
//---------------------------------------------------------------------------
void __fastcall TfrmEditForm::SaveControls(){
	for(long i=0;i<Controls.Count();i++){
			StackValue sv;

			switch((*FormDef)[Controls[i]->ColIdx]->ControlType){
				case 	ectNumeric:
				case 	ectInteger:
				{
					TMaskEdit *rce=(TMaskEdit*)(Controls[i]->ctrl);
					sv.Set(stNumeric,StrClearAllChar(rce->Text));

					break;
				}

				case 	ectMultiLineText:
				{
					TMemo *mem= (TMemo*)(Controls[i]->ctrl);
					sv.Set(stString,mem->Lines->Text);
					break;
				}
				case 	ectPassword:
				case 	ectText:
				{
					TEdit *ed= (TEdit*)(Controls[i]->ctrl);
					sv.Set(stString,ed->Text);
					break;
				}

				case 	ectDate:
				{
					TDateTimePicker *de=(TDateTimePicker*)(Controls[i]->ctrl);
					sv.type=stDate;
          TDateTime *tmp =new TDateTime(de->Date);
          unsigned short d,m,y;
          tmp->DecodeDate(&y,&m,&d);
          delete tmp;
          tmp=new TDateTime(y,m,d);
					sv.d=*tmp;
          delete tmp;
					break;
				}

				case 	ectDateTime:
				{

					TMaskEdit *me= (TMaskEdit*)(Controls[i]->msk);
					sv.Set(stDate,"30/12/1899 "+me->Text);
					TDateTimePicker *de=(TDateTimePicker*)(Controls[i]->ctrl);
					sv.type=stDate;
          TDateTime *tmp =new TDateTime(de->Date);
          unsigned short d,m,y;
          tmp->DecodeDate(&y,&m,&d);
          delete tmp;
          tmp=new TDateTime(y,m,d);
					sv.d=sv.d+*tmp;
          delete tmp;
					break;
				}


				case 	ectTime:
				{
					TMaskEdit *me= (TMaskEdit*)(Controls[i]->ctrl);
					sv.Set(stDate,"30/12/1899 " + me->Text);
					me=0;
					break;
				}

				case 	ectYesNo:
					{
						TCheckBox *cb;
						cb=(TCheckBox*)(Controls[i]->ctrl);
						if(cb->State==cbChecked)
							sv.Set(stNumeric,"1");
						else if(cb->State==cbUnchecked)
							sv.Set(stNumeric,"0");
						else
							 sv.Erase();
						break;
					}

			 case 	ectSubStream:
					{
						TStreamCombo *ce;
						ce= (TStreamCombo*)(Controls[i]->ctrl);
						sv=ce->Value;
						break;
					}

			}
            /*if((ForInsert ? Controls[i]->ctrl->InsertAccess:cs->EditAccess)==faWrite)*/
        		SetFieldValue((*FormDef)[Controls[i]->ColIdx]->FieldName, sv );

	}

};


//---------------------------------------------------------------------------
void __fastcall  TfrmEditForm::SetFieldValue(AnsiString Name,StackValue sv){
		StackValue *ptr;
			for(long i=0;i<NewRow->Count();i++)
				if(NewRow->Name(i)==Name){
					ptr=(*NewRow)[i];
					*ptr=sv;
				}
};
//---------------------------------------------------------------------------
bool __fastcall TfrmEditForm::CheckNulls(){
	// пройти по всем полям
	for(long i=0;i<(*FormDef).Count();i++){
		EditableFieldAttr * ea=0;
   ea=(*FormDef)[i];
   StackValue *sv=0;
   if(ea)
		  sv=GetFieldValue(ea->FieldName);
		if( sv && ea )
       if (
            (!ForInsert &&
               ((rights ? (*rights)[i]->EditAccess:(*FormDef)[i]->DefEditAccess)== faWrite)
            )

            ||

           (ForInsert  &&
               ((rights ? (*rights)[i]->InsertAccess:(*FormDef)[i]->DefInsertAccess)== faWrite)
            )
          )
          if( sv->IsNull() && !ea->AllowNull ){
       			AnsiString buf="Необхдимо задать значение для поля '" +  ea->Label + "'.";
       			MessageBox(0,buf.c_str(),"Внимание!!!",MB_OK);
		     			return false;
   		   }
	}

	return true;
};
//---------------------------------------------------------------------------

void __fastcall TfrmEditForm::Panel3Resize(TObject *Sender)
{

	if(Width < 50) Width=50 ;

	long W=Width-22;
	for(long i=0;i<Controls.Count();i++){
			Controls[i]->lbl->Width =W/3-10;
			Controls[i]->lbl->Left=5;
			if(	(*FormDef)[Controls[i]->ColIdx]->ControlType!=ectDateTime){
				Controls[i]->ctrl->Width =W*2/3-5;
				Controls[i]->ctrl->Left=W/3;
			}else{
				Controls[i]->ctrl->Width =W/3-5;
				Controls[i]->ctrl->Left=W/3;
				Controls[i]->msk->Width =W/3-5;
				Controls[i]->msk->Left=W*2/3;
			}
	}

}
//---------------------------------------------------------------------------

void __fastcall TfrmEditForm::FormShow(TObject *Sender)
{
	for(long i=0;i<(*FormDef).Count();i++){
	 CtrlStorage *cs=Controls[i];
      switch((ForInsert ? cs->InsertAccess:cs->EditAccess)){
					case faNone:
						break;
					case faRead:
						break;
					case faWrite:
						cs->ctrl->SetFocus();
           return;
	  }
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmEditForm::CtrlChange(TObject *Sender)
{
    if(OnLoad) return;
    SaveControls();
    InitOnChange();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEditForm::InitOnChange(){


	for(long i=0;i<(*FormDef).Count();i++){
		EditableFieldAttr * ea=(*FormDef)[i];
   	    if(ea && ea->ControlType==ectSubStream && ea->FillOnGetFocus){
        	StackValue *sv=GetFieldValue(ea->FieldName);
		   TStreamCombo *ce;
		   ce= (TStreamCombo*)Controls[i]->ctrl;
		   ce->Manager=Manager;
		   ce->Editor=ea->StreamName;
		   ce->SyncFieldName=ea->SyncField;
		   ce->ShowFieldName=ea->ShowField;
           ce->Value.Clear();
		   ce->Value=*sv;
		   ce->Actions=ea->WhenChanged;
		   ce->CurrentRowState=NewRow;
           ce->UnInit();
		}
   }
};


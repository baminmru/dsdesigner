//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "StreamGrid.h"
#include "Filter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "RXSplit"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
void __fastcall TfrmFilter::CreateWnd(){
			TForm::CreateWnd();
};

//---------------------------------------------------------------------------
void __fastcall TfrmFilter::DestroyWnd(){
	 Controls.Clear();
	 TForm::DestroyWnd();
};

//---------------------------------------------------------------------------
__fastcall TfrmFilter::TfrmFilter(TComponent* Owner)
	:TForm(Owner)
{
 SettedUp=false;
 OnLoad = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmFilter::btnCancelClick(TObject *Sender)
{
	ModalResult=mrCancel;
};
//---------------------------------------------------------------------------

void __fastcall TfrmFilter::btnSaveClick(TObject *Sender)
{
	SaveControls();
	if(CheckNulls()){
     ModalResult=mrOk;
 }
}
//---------------------------------------------------------------------------


int __fastcall TfrmFilter::Setup(				// mrCancell or mrOK
     DataManager *_Manager,
   	FilterFormDef *_F
		){
			Manager=_Manager;
     F=_F;
			if(!Manager || !F  )
				return mrCancel;

     Caption = F->Caption;  
			BuildControls();
     SettedUp=true;
			InitControls();

			if(Controls.Count()){
				return ShowModal();
			}else
				return mrCancel;
};


int __fastcall TfrmFilter::Run(				// mrCancell or mrOK
		){
			InitControls();
			if(Controls.Count()){
				return ShowModal();
			}else
				return mrCancel;
};


void __fastcall TfrmFilter::BuildControls(){
	Controls.Clear();
	long cLeft=Width/3;
	long lLeft=5;
	TWinControl *wc;
	TMaskEdit *me;
	TLabel *lbl;

	long lTabOrder=0;
	long cTop=5;
	for(long i=0;i<F->FormDef.Count();i++){
		EditableFieldAttr * ea= F->FormDef[i];

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
					//rce->DecimalPlaces=0;
         //           rce->ZeroEmpty=true;
         rce->EditMask="#9999999999999;1;_";
	                rce->OnChange=CtrlChange;
    				wc=rce;
					break;
				}

				case 	ectNumeric:
				{
					TMaskEdit *rce=new TMaskEdit(this);
					//rce->DecimalPlaces=4;
          //          rce->ZeroEmpty=true;
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
                    mem->OnChange=CtrlChange;
					wc=mem;
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
                    ed->OnChange=CtrlChange;
					wc=ed;
					break;
				}

				case 	ectDate:
				{
					TDateTimePicker *de=new TDateTimePicker(this);
					wc=de;
          // de->YearDigits=dyFour;
          de->OnChange=CtrlChange;
					break;
				}


				case 	ectTime:
				{
					me= new TMaskEdit(this);
					me->EditMask="!90:00:99;1;_";
          me->OnChange=CtrlChange;
					wc=me;
					me=0;
					break;
				}

				case 	ectDateTime:
					{
					TDateTimePicker *de=new TDateTimePicker(this);
					wc=de;
                    //de->YearDigits=dyFour;
          de->OnChange=CtrlChange;
          me= new TMaskEdit(this);
          me->OnChange=CtrlChange;
					me->EditMask="!90:00:99;1;_";
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
			 case 	ectStreamGrid:
					{
						TStreamGrid *ce;
						ce= new TStreamGrid(this);
            ce->Height = 100;
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
       FCtrlStorage *cs;

       cs=new FCtrlStorage(F->FormDef[i]->DefInsertAccess,F->FormDef[i]->DefInsertAccess);

				cs->ctrl=wc;
				cs->lbl=lbl;
				cs->msk=me;
				cs->ColIdx=i;
				Controls.Add(cs);
				cTop+=cs->GetHeight(true);
			}

		}
	}
	Height=cTop+60;
	Width=Width+22;

};


StackValue * __fastcall  TfrmFilter::GetFieldValue(AnsiString Name){
		for(long i=0;i<NewRow.Count();i++)
				if(NewRow.Name(i)==Name)
					return NewRow[i];

   StackValue * ptr;
   NewRow.Add(ptr=new StackValue(),Name);
		return ptr;
};



void __fastcall TfrmFilter::InitControls(){
    OnLoad = true;
	TWinControl *wc;
	TMaskEdit *me;
	TLabel *lbl;

	for(long i=0;i<F->FormDef.Count();i++){
		FilterFieldDef * ea=F->FormDef[i];

		StackValue *sv=GetFieldValue(ea->FieldName);
		FCtrlStorage *cs=Controls[i];
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
						ce->CurrentRowState=&NewRow;
           if(ea->FillOnFormShow)
             ce->UnInit();
						ce->TextFromValue();
						break;
					}

			 case 	ectStreamGrid:
					{
						TStreamGrid *ce;
						ce= (TStreamGrid*)wc;
            ce->SetEditor(Manager,ea->StreamName);  // задание настроек
            ce->InitSlave(ea->WhenChanged,&NewRow);
          	ce->RunEditor();            // полная перестройка Грида
            //ce->Sync(ea->SyncName,
           if(ea->FillOnFormShow)
            ce->InitSlave(ea->WhenChanged,&NewRow);
            ce->RunEditor();
						break;
					}

				default:
						break;
			}
      switch(cs->InsertAccess){
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
void __fastcall TfrmFilter::SaveControls(){
	for(long i=0;i<Controls.Count();i++){
			StackValue sv;

			switch(F->FormDef[Controls[i]->ColIdx]->ControlType){
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
        /*
				case 	ectDate:
				{
					TDateTimePicker *de=(TDateTimePicker*)(Controls[i]->ctrl);
					sv.type=stDate;
					sv.d=de->Date;
					break;
				}

				case 	ectDateTime:
				{

					TMaskEdit *me= (TMaskEdit*)(Controls[i]->msk);
					sv.Set(stDate,"30/12/1899 "+me->Text);
					TDateTimePicker *de=(TDateTimePicker*)(Controls[i]->ctrl);
					sv.type=stDate;
					sv.d=sv.d+de->Date;
					break;
				}
        */
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
			 case 	ectStreamGrid:
					{
						TStreamGrid *ce;
						ce= (TStreamGrid*)(Controls[i]->ctrl);
						sv=ce->GetValue(F->FormDef[Controls[i]->ColIdx]->SyncField);
						break;
					}                     

			}
            /*
            if((ForInsert ? Controls[i]->ctrl->InsertAccess:cs->EditAccess)==faWrite)*/
        		SetFieldValue(F->FormDef[Controls[i]->ColIdx]->FieldName, sv );
	}

};


//---------------------------------------------------------------------------
void __fastcall  TfrmFilter::SetFieldValue(AnsiString Name,StackValue sv){
		for(long i=0;i<NewRow.Count();i++)
				if(NewRow.Name(i)==Name){
					*(NewRow[i])=sv;
         return;
				}

   NewRow.Add(new StackValue(sv),Name);
};
//---------------------------------------------------------------------------
bool __fastcall TfrmFilter::CheckNulls(){

	// пройти по всем полям
	for(long i=0;i<F->FormDef.Count();i++){
		EditableFieldAttr * ea=F->FormDef[i];
		StackValue *sv=GetFieldValue(ea->FieldName);
   if ((F->FormDef[i]->DefEditAccess)== faWrite)
     if( sv->IsNull() && !ea->AllowNull ){
     	AnsiString buf="Необхдимо задать значение для поля '" +  ea->Label + "'.";
     	MessageBox(0,buf.c_str(),"Внимание!!!",MB_OK);
		  	return false;
     }
	}
	return true;
};
//---------------------------------------------------------------------------

void __fastcall TfrmFilter::Panel3Resize(TObject *Sender)
{

	if(Width < 50) Width=50 ;

	long W=Width-22;
	for(long i=0;i<Controls.Count();i++){
			Controls[i]->lbl->Width =W/3-10;
			Controls[i]->lbl->Left=5;
			if(	F->FormDef[Controls[i]->ColIdx]->ControlType!=ectDateTime){
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

void __fastcall TfrmFilter::FormShow(TObject *Sender)
{
	for(long i=0;i<F->FormDef.Count();i++){
	     FCtrlStorage *cs=Controls[i];
      switch(cs->InsertAccess){
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
AnsiString __fastcall TfrmFilter::MakeSubWhere(){
 AnsiString sw;

 for(long i=0;i<F->FormDef.Count();i++){
   FilterFieldDef *ffd = F->FormDef[i];
   StackValue *sv =NewRow(ffd->FieldName);

   if(	F->FormDef[Controls[i]->ColIdx]->ControlType==ectStreamGrid){
      AnsiString gs;
      TStreamGrid *Grd = (TStreamGrid *)(Controls[i]->ctrl);
      if(Grd->SelectedRowCount()){
        for(long j=0;j<Grd->RowCount;j++){
          if(Grd->IsSelected(j)){
            Params *p=Grd->GetState(j);
            sv=p->Get(F->FormDef[Controls[i]->ColIdx]->SyncField);
            NewRow.Set(NewRow.Where(ffd->FieldName),new StackValue(sv));
            delete p;
            if (!gs.IsEmpty()) gs=gs + " or ";
            gs=gs+Parse(ffd->Condition);
          }
        }
        sw=sw+" "+F->Concatenator+" ("+ gs +") ";
      }
   } else{
     if(!sv->IsNull()){
       sw=sw+" "+F->Concatenator+" "+Parse(ffd->Condition);
     }
   }
 }
 if(!sw.IsEmpty())
 sw = F->SubWherePrephics + " " + sw + " " + F->SubWherePostphics;
 if (NewRow.Where("Filter.SubWhere")==-1){
  NewRow.Add(new StackValue(sw),"Filter.SubWhere");
 }else{
  NewRow.Set(NewRow.Where("Filter.SubWhere"),new StackValue(sw));
 }

 return sw;
};
//---------------------------------------------------------------------------
AnsiString __fastcall TfrmFilter::Parse(AnsiString Expression){
	 AnsiString NameBuffer;
	 bool mode=false;
	 AnsiString ParseBuffer;
	 for(long i=1;i<=Expression.Length();i++){
		if (Expression[i]=='^'){
				mode = !mode;
				 if(mode){
						NameBuffer="";
				 }else{
						ParseBuffer=ParseBuffer+GetValue(NameBuffer);
				 }
			}else{
				if(mode){
					NameBuffer=NameBuffer+Expression[i];
				 }else{
					ParseBuffer=ParseBuffer+Expression[i];
				 }
			}
	 }
	 return ParseBuffer;

};


AnsiString __fastcall TfrmFilter::GetValue(AnsiString Name){
	for(int i=0 ;i<NewRow.Count();i++){
		if(Name==NewRow.Name(i)){
				 return   NewRow[i]->ToSQLStr();
			}
	 }
	return "";
};

void __fastcall TfrmFilter::CtrlChange(TObject *Sender)
{
    if (OnLoad) return;
    SaveControls();
    InitOnChange();
}
//---------------------------------------------------------------------------

void __fastcall TfrmFilter::InitOnChange(){


	for(long i=0;i<F->FormDef.Count();i++){
		EditableFieldAttr * ea=F->FormDef[i];

		StackValue *sv=GetFieldValue(ea->FieldName);
		FCtrlStorage *cs=Controls[i];

		if(ea && ea->ControlType==ectSubStream && ea->FillOnGetFocus){
		   TStreamCombo *ce;
		   ce= (TStreamCombo*)cs->ctrl;
		   ce->Manager=Manager;
		   ce->Editor=ea->StreamName;
		   ce->SyncFieldName=ea->SyncField;
		   ce->ShowFieldName=ea->ShowField;
           ce->Value.Clear();
		   ce->Value=*sv;
		   ce->Actions=ea->WhenChanged;
		   ce->CurrentRowState=&NewRow;
           ce->UnInit();
		}
   }
};


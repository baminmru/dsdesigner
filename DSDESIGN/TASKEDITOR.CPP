//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TaskEditor.h"
#include "Changes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "RXCtrls"
//#pragma link "ToolEdit"
//#pragma link "RXSpin"
#pragma resource "*.dfm"
TfrmTasks *frmTasks;
//---------------------------------------------------------------------------
__fastcall TfrmTasks::TfrmTasks(TComponent* Owner)
 : TForm(Owner)
{
 OnLstMenu = false;
 OnLstTask= false;
 OnLstTran = false;
 OnLstTranField =false;
 OnLstReport = false;
 OnLstFormulas = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmTasks::FormClose(TObject *Sender, TCloseAction &Action)
{
   Action = caFree;
   frmTasks =0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::Init(){
 lstTask->Items->Clear();
 for(long i=0;i< App->Tasks.Count();i++){
   lstTask->Items->Add(App->Tasks[i]->Name+" ("+App->Tasks[i]->Caption+")");
 }
 lstMenu->Items->Clear();
 cmbMenu->Items->Clear();
 cmbMenuEditor->Items->Clear();
 cmbMenuReport->Items->Clear();
 cmbMenuFilter->Items->Clear();
 lstTran->Items->Clear();
 lstReport->Clear();

 for(long i=0;i<App->Manager.Editors.Count();i++){
   cmbMenuEditor->Items->Add(App->Manager.Editors.Name(i));
   cmbTranSubStream->Items->Add(App->Manager.Editors.Name(i));
 }

 for(long i=0;i<App->Reports.Count();i++){
   cmbMenuReport->Items->Add(App->Reports[i]->Caption);
   lstReport->Items->Add(App->Reports[i]->Caption);
 }

 for(long i=0;i<App->Filters.Count();i++){
   cmbMenuFilter->Items->Add(App->Filters[i]->Caption);
   lstTran->Items->Add(App->Filters[i]->Caption);
 }

 cmbRepConnection->Items->Clear();
 for(long i=0;i<App->Manager.Sources.Count();i++){
  cmbRepConnection->Items->Add(App->Manager.Sources.Name(i));
 }
};

void __fastcall TfrmTasks::lstTaskClick(TObject *Sender)
{
 if(lstTask->ItemIndex==-1) return;
 OnLstTask = true;
 Task *t;
 t=App->Tasks[lstTask->ItemIndex];
 txtTaskName->Text = t->Name;
 txtTaskCaption->Text = t->Caption;
 txtTaskVersion->Text = t->Version;
 FillMenuBoxes();

 OnLstTask = false;

}
//---------------------------------------------------------------------------
void __fastcall TfrmTasks::FillMenuBoxes(){
 if(lstTask->ItemIndex==-1) return;
 Task *t;
 t=App->Tasks[lstTask->ItemIndex];
 lstMenu->Items->Clear();
 cmbMenu->Items->Clear();
 MyArray<MenuDef> *s;
 MenuHolder *p;
 for(long i=0;i< t->Menu.Count();i++){
     p = new MenuHolder(i,-1);
     lstMenu->Items->AddObject(t->Menu[i]->Caption,(TObject*)p);
     cmbMenu->Items->Add(t->Menu[i]->Caption);
     s=&t->Menu[i]->Items;
     if(s){
       for(long j=0;j< s->Count();j++){
        p = new MenuHolder(i,j);
        lstMenu->Items->AddObject(t->Menu[i]->Caption +" \ " +s->Get(j)->Caption,(TObject*) p );
       }
     }
 }

}
void __fastcall TfrmTasks::btnTaskAddClick(TObject *Sender)
{
 Task *t;
 App->Tasks.Add(t =new Task());
 t->Caption = "Новая задача";
 lstTask->Items->Add(t->Caption);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::btnDelTaskClick(TObject *Sender)
{
 if(lstTask->ItemIndex==-1) return;
 App->Tasks.Destroy(lstTask->ItemIndex);
 lstTask->Items->Delete(lstTask->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::txtTaskNameChange(TObject *Sender)
{
 if(lstTask->ItemIndex==-1) return;
 if(OnLstTask) return;
 Task *t;
 t=App->Tasks[lstTask->ItemIndex];
 t->Name=txtTaskName->Text;
 t->Caption=txtTaskCaption->Text;
 t->Version=txtTaskVersion->Text;
 lstTask->Items->Strings[lstTask->ItemIndex]=(App->Tasks[lstTask->ItemIndex]->Name)+" ("+(App->Tasks[lstTask->ItemIndex]->Caption)+")";
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::btnNoParentClick(TObject *Sender)
{
 cmbMenu->ItemIndex =-1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::btnMenuAddClick(TObject *Sender)
{
 if(lstTask->ItemIndex==-1) return;
 if(OnLstTask) return;
 Task *t;
 t=App->Tasks[lstTask->ItemIndex];
 t->Menu.Add(new MenuDef("Новый пункт меню"));
 FillMenuBoxes();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::lstMenuClick(TObject *Sender)
{
 if(lstTask->ItemIndex==-1) return;
 if(lstMenu->ItemIndex==-1) return;
 OnLstMenu = true;
 Task *t;
 t=App->Tasks[lstTask->ItemIndex];
 MenuHolder *p=(MenuHolder *)lstMenu->Items->Objects[lstMenu->ItemIndex];
 MenuDef *parent, *child=0,*cur;
 parent = t->Menu[p->p];
 if(p->c >= 0)
   child =t->Menu[p->p]->Items[p->c];

 cur=(child?child:parent);
 cmbMenu->ItemIndex =(p->c != -1 ? p->p : -1);
 txtMenuCaption->Text=cur->Caption;
 cmbMenuFilter->ItemIndex=cmbMenuFilter->Items->IndexOf(cur->Filter);
 cmbMenuEditor->ItemIndex=cmbMenuEditor->Items->IndexOf(cur->Editor);
 cmbMenuReport->ItemIndex=cmbMenuReport->Items->IndexOf(cur->Report);
 OnLstMenu = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::txtMenuCaptionChange(TObject *Sender)
{
 if(lstTask->ItemIndex==-1) return;
 if(lstMenu->ItemIndex==-1) return;
 if(OnLstMenu)return;
 long idx =lstMenu->ItemIndex;
 long pidx;
 Task *t;
 t=App->Tasks[lstTask->ItemIndex];
 MenuHolder *p=(MenuHolder *)lstMenu->Items->Objects[lstMenu->ItemIndex];
 MenuDef *parent, *child=0,*cur;
 parent = t->Menu[p->p];
 if(p->c>=0)
 child =t->Menu[p->p]->Items[p->c];
 cur=(child?child:parent);
 cur->Caption=txtMenuCaption->Text;
 cur->Filter=cmbMenuFilter->Text;
 cur->Editor=cmbMenuEditor->Text;
 cur->Report=cmbMenuReport->Text;

 if( p->c>=0){
   if(cmbMenu->ItemIndex != p->p){
     if(cmbMenu->ItemIndex != -1 ){
       MenuDef* newParent =t->Menu[cmbMenu->ItemIndex];
       parent->Items.DeleteOrder( false);
       parent->Items.Destroy(p->c);
       parent->Items.DeleteOrder ( true);
       newParent->Items.Add(cur);
     }else{
       parent->Items.DeleteOrder ( false);
       parent->Items.Destroy(p->c);
       parent->Items.DeleteOrder ( true);
       t->Menu.Add(cur);
     }
     idx =-1;
   }
 }

 if(p->c==-1){
    if(cmbMenu->ItemIndex==p->p )
       cmbMenu->ItemIndex=-1;
    if(cmbMenu->ItemIndex != -1  ){
        t->Menu.DeleteOrder(false);
        t->Menu.Destroy(p->p);
        t->Menu.DeleteOrder(true);
       (t->Menu[cmbMenu->ItemIndex])->Items.Add(cur);
        idx=-1;
    }
 }


 pidx=cmbMenu->ItemIndex;
 OnLstMenu = true;
 FillMenuBoxes();
 lstMenu->ItemIndex=idx;
 cmbMenu->ItemIndex=pidx;
 OnLstMenu = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::Button3Click(TObject *Sender)
{
 cmbMenuFilter->ItemIndex =-1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::Button2Click(TObject *Sender)
{
 cmbMenuEditor->ItemIndex =-1; 
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::Button1Click(TObject *Sender)
{
 cmbMenuReport->ItemIndex =-1; 
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::btnMenuDelClick(TObject *Sender)
{
 if(lstTask->ItemIndex==-1) return;
 if(lstMenu->ItemIndex==-1) return;
 if(OnLstMenu)return;
 Task *t;
 t=App->Tasks[lstTask->ItemIndex];
 MenuHolder *p=(MenuHolder *)lstMenu->Items->Objects[lstMenu->ItemIndex];
 if(p->c >=0)
   t->Menu[p->p]->Items.Destroy(p->c);
 else
   t->Menu.Destroy(p->p);

 FillMenuBoxes();
}
//---------------------------------------------------------------------------


void __fastcall TfrmTasks::txtTranFieldCaptionChange(TObject *Sender)
{
	if(OnLstTranField) return;
	if(lstTranField->ItemIndex==-1) return;
	if(lstTran->ItemIndex==-1) return;
	FilterFormDef *ed;
	FilterFieldDef *fa;
	ed=App->Filters[lstTran->ItemIndex];
	fa=ed->FormDef[lstTranField->ItemIndex];
	fa->Label=txtTranFieldCaption->Text;
 fa->Condition = txtFieldCondition->Text;

	if(cmbTranControlType->ItemIndex != -1)
		fa->ControlType=(EditControlType)cmbTranControlType->ItemIndex;
	else
		fa->ControlType=ectNone;

	fa->FillOnGetFocus=false;
	fa->FillOnFormShow=false;
	fa->StreamName="";
	fa->ShowField="";

	if(fa->ControlType==ectSubStream || fa->ControlType==ectStreamGrid){
		 fa->FillOnGetFocus=(chkTranReFillOnGetFocus->Checked == cbChecked);
		 fa->FillOnFormShow=(chkTranReFillOnFormShow->Checked == cbChecked);
		 fa->StreamName=cmbTranSubStream->Text;
		 fa->ShowField=cmbTranFieldsToShow->Text;
		 fa->SyncField=cmbTranFieldToSync->Text;
	}


	if(chkTranNoAccess->Checked==cbChecked) fa->DefInsertAccess=faNone;
	if(chkTranReadOnly->Checked==cbChecked) fa->DefInsertAccess=faRead;
	if(chkTranWrite->Checked==cbChecked) fa->DefInsertAccess=faWrite;

	if(chkTranTypeString->Checked==cbChecked) fa->SQLType=stString;
	if(chkTranTypeNumeric->Checked==cbChecked) fa->SQLType=stNumeric;
	if(chkTranTypeDate->Checked==cbChecked) fa->SQLType=stDate;
	fa->AllowNull=(chkTranAllowNull->Checked==cbChecked);

	if(fa->ControlType!=ectSubStream && fa->ControlType!=ectStreamGrid){
		 cmbTranSubStream->ItemIndex=-1;
		 cmbTranFieldsToShow->ItemIndex=-1;
		 cmbTranFieldsToShow->Enabled = false;
		 cmbTranSubStream->Enabled = false;
		 chkTranReFillOnGetFocus->Enabled = false;
		 chkTranReFillOnGetFocus->Checked = cbUnchecked;
		 chkTranReFillOnFormShow->Enabled = false;
		 chkTranReFillOnFormShow->Checked = cbUnchecked;
    cmbTranFieldToSync->Enabled = false;
	}else{
		 chkTranReFillOnGetFocus->Enabled = true;
		 chkTranReFillOnFormShow->Enabled = true;
		 cmbTranFieldsToShow->Enabled = true;
		 cmbTranSubStream->Enabled = true;
    cmbTranFieldToSync->Enabled = true;
	}


}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::lstTranFieldClick(TObject *Sender)
{
	if(lstTranField->ItemIndex==-1) return;
	if(lstTran->ItemIndex==-1) return;
	OnLstTranField =true;
	FilterFormDef *ed;
	FilterFieldDef *fa;
	ed=App->Filters[lstTran->ItemIndex];
	fa=ed->FormDef[lstTranField->ItemIndex];

	switch(fa->SQLType){
		case stNumeric:
			chkTranTypeNumeric->Checked = cbChecked;
			break;
		case stDate:
			chkTranTypeDate->Checked = cbChecked;
			break;
		default:
			chkTranTypeString->Checked = cbChecked;
			break;
	}

	if(fa->AllowNull)
		chkTranAllowNull->Checked = cbChecked;
	else
		chkTranAllowNull->Checked = cbUnchecked;

	txtTranFieldCaption->Text=fa->Label;
 txtFieldCondition->Text=fa->Condition;
	cmbTranControlType->ItemIndex=(int)fa->ControlType;

	if(fa->ControlType!=ectSubStream && fa->ControlType!=ectStreamGrid){
		 cmbTranSubStream->ItemIndex=-1;
		 cmbTranFieldsToShow->ItemIndex=-1;
		 cmbTranFieldsToShow->Enabled = false;
 	 cmbTranFieldToSync->ItemIndex=-1;
		 cmbTranFieldToSync->Enabled = false;
		 cmbTranSubStream->Enabled = false;
		 chkTranReFillOnGetFocus->Enabled = false;
		 chkTranReFillOnGetFocus->Checked = cbUnchecked;
		 chkTranReFillOnFormShow->Enabled = false;
		 chkTranReFillOnFormShow->Checked = cbUnchecked;
    btnTranFieldChanges->Enabled = false;
	}else{
    btnTranFieldChanges->Enabled = true;
		 chkTranReFillOnGetFocus->Enabled = true;
		 chkTranReFillOnFormShow->Enabled = true;

		 if(fa->FillOnGetFocus)
			 chkTranReFillOnGetFocus->Checked = cbChecked;
		 else
			 chkTranReFillOnGetFocus->Checked = cbUnchecked;
		 if(fa->FillOnFormShow)
			 chkTranReFillOnFormShow->Checked = cbChecked;
		 else
			 chkTranReFillOnFormShow->Checked = cbUnchecked;

		 cmbTranFieldsToShow->Enabled = true;
  	 cmbTranFieldToSync->Enabled = true;
		 cmbTranSubStream->Enabled = true;
		 cmbTranSubStream->ItemIndex=cmbTranSubStream->Items->IndexOf(fa->StreamName);
    {
       StreamEditorDef *sub=App->Manager.GetEditor(fa->StreamName);
       cmbTranFieldsToShow->Items->Clear();
       if(sub)
       for(long i=0;i<sub->FormDef.Count();i++){
	        cmbTranFieldsToShow->Items->Add(sub->FormDef[i]->FieldName);
       }

       cmbTranFieldToSync->Items->Clear();
       if(sub)
       for(long i=0;i<sub->FormDef.Count();i++){
         cmbTranFieldToSync->Items->Add(sub->FormDef[i]->FieldName);
       }
    }
		 cmbTranFieldsToShow->ItemIndex=cmbTranFieldsToShow->Items->IndexOf(fa->ShowField);
    cmbTranFieldToSync->ItemIndex=cmbTranFieldToSync->Items->IndexOf(fa->SyncField);
	}

	switch(fa->DefInsertAccess){
		case faNone:
			chkTranNoAccess->Checked=cbChecked;
			break;
	 case faRead:
			chkTranReadOnly->Checked=cbChecked;
			break;
	 case faWrite:
			chkTranWrite->Checked=cbChecked;
			break;
	};
	OnLstTranField =false;

}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::lstTranClick(TObject *Sender)
{
  if(lstTran->ItemIndex==-1) return;
  OnLstTran = true;
  FilterFormDef *ff;
  ff=App->Filters[lstTran->ItemIndex];

  txtTranCaption->Text=ff->Caption;
  txtTranConcatenator->Text=ff->Concatenator;
  txtTranPrephics->Text=ff->SubWherePrephics;
  txtTranPostphics->Text=ff->SubWherePostphics;
  lstTranField->Items->Clear();
  for(long i=0;i<ff->FormDef.Count();i++){
   lstTranField->Items->Add(ff->FormDef[i]->FieldName);
  }
  OnLstTran = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::txtTranCaptionChange(TObject *Sender)
{
 if (OnLstTran) return;
  if(lstTran->ItemIndex==-1) return;
  FilterFormDef *ff;
  long idx =lstTran->ItemIndex;
  ff=App->Filters[lstTran->ItemIndex];
  ff->Caption=txtTranCaption->Text;
  ff->Concatenator=txtTranConcatenator->Text;
  ff->SubWherePrephics=txtTranPrephics->Text;
  ff->SubWherePostphics=txtTranPostphics->Text;
  lstTran->Items->Strings[idx]=ff->Caption;
  lstTran->ItemIndex=idx;
  idx=cmbMenuFilter->ItemIndex;
  cmbMenuFilter->Items->Strings[lstTran->ItemIndex]=ff->Caption;
  cmbMenuFilter->ItemIndex=idx;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::btnDelFilterClick(TObject *Sender)
{
  if(lstTran->ItemIndex==-1) return;
  App->Filters.Destroy(lstTran->ItemIndex);
  cmbMenuFilter->Items->Delete(lstTran->ItemIndex);
  lstTran->Items->Delete(lstTran->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::btnAddTranFieldClick(TObject *Sender)
{
   if(lstTran->ItemIndex==-1) return;
   FilterFormDef *Tran;
   Tran=App->Filters[lstTran->ItemIndex];
   AnsiString f =InputBox("Новое поле","Задайте имя поля","");
   if(f=="") return;
  	FilterFieldDef *fa;
	  Tran->FormDef.Add(fa=new FilterFieldDef());
   fa->FieldName=f;
   lstTranField->Items->Add(f);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::btnDelTranFieldClick(TObject *Sender)
{
   if(lstTran->ItemIndex==-1) return;
   if(lstTranField->ItemIndex==-1) return;
   FilterFormDef *Tran;
   Tran=App->Filters[lstTran->ItemIndex];
	  Tran->FormDef.Destroy(lstTranField->ItemIndex);
   lstTranField->Items->Delete(lstTranField->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::btnAddFilterClick(TObject *Sender)
{
 App->Filters.Add(new FilterFormDef());
 lstTran->Items->Add("Новая операция");
 cmbMenuFilter->Items->Add("Новая операция");

}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::cmbTranSubStreamChange(TObject *Sender)
{
	if(cmbTranSubStream->ItemIndex < 0) return;
 if(lstTranField->ItemIndex==-1) return;
	if(lstTran->ItemIndex==-1) return;
	OnLstTranField =true;
	FilterFormDef *ed;
	FilterFieldDef *fa;

	ed=App->Filters[lstTran->ItemIndex];
	fa=ed->FormDef[lstTranField->ItemIndex];
 StreamEditorDef *sub=App->Manager.GetEditor(cmbTranSubStream->Text);
 cmbTranFieldsToShow->Items->Clear();
 if(sub)
  for(long i=0;i<sub->FormDef.Count();i++){
	   cmbTranFieldsToShow->Items->Add(sub->FormDef[i]->FieldName);
  }

  cmbTranFieldToSync->Items->Clear();
  if(sub)
   for(long i=0;i<sub->FormDef.Count();i++){
     cmbTranFieldToSync->Items->Add(sub->FormDef[i]->FieldName);
	 }
 cmbTranFieldsToShow->ItemIndex=cmbTranFieldsToShow->Items->IndexOf(fa->ShowField);
 cmbTranFieldToSync->ItemIndex=cmbTranFieldToSync->Items->IndexOf(fa->SyncField);

 OnLstTranField =false;
 txtTranFieldCaptionChange(this);

}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::btnTranFieldChangesClick(TObject *Sender)
{
if(lstTranField->ItemIndex==-1 ||lstTran->ItemIndex==-1 || !App )
   return;
 FilterFormDef *Tran = App->Filters[lstTran->ItemIndex];
 FilterFieldDef *fa;
 fa=Tran->FormDef[lstTranField->ItemIndex];
 TfrmChanges *chng;
 chng = new TfrmChanges(this);
 chng->Manager = &App->Manager;
 if(!fa->WhenChanged )
   fa->WhenChanged = new EditFieldActions();
 chng->Actions=fa->WhenChanged;
 chng->Prepare();
 chng->ShowModal();
 if(!fa->WhenChanged->NotEmpty()){
   delete fa->WhenChanged;
   fa->WhenChanged=0;
 }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::bntTranChangesClick(TObject *Sender)
{
 if(lstTran->ItemIndex==-1 || !App )   return;
 FilterFormDef *Tran = App->Filters[lstTran->ItemIndex];
 TfrmChanges *chng;
 chng = new TfrmChanges(this);
 chng->Manager = &App->Manager;
 chng->Actions=&Tran->Changes;
 chng->Prepare();
 chng->txtSubCondition->Text="";
 chng->txtSubCondition->Visible=false;
 chng->ShowModal();
 delete chng;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::lstReportClick(TObject *Sender)
{
  if(lstReport->ItemIndex==-1) return;
  OnLstReport = true;
  ReportDef *ff;
  ff=App->Reports[lstReport->ItemIndex];
  txtRepName->Text=ff->Caption;
  cmbRepConnection->ItemIndex=cmbRepConnection->Items->IndexOf(ff->ConnectionName);
  txtRepFileName->Text=ff->ReportFile.Path;
  txtRepSQL->Text=ff->SQL;
  txtCaptionExpression->Text=ff->CaptionExpression;
  txtBuildProc->Text=ff->BuildProc;
  lstFormula->Items->Clear();
  for(long i=0;i<ff->Formulas.Count();i++){
    lstFormula->Items->Add(ff->Formulas[i]->Name);
  }
  OnLstReport = false;

}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::btnRepAddClick(TObject *Sender)
{
 ReportDef * ptr;
 App->Reports.Add(ptr=new ReportDef());
 ptr->Caption="Новый отчет";
 cmbMenuReport->Items->Add("Новый отчет");
 lstReport->Items->Add("Новый отчет");
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::btnRepDelClick(TObject *Sender)
{
 if(lstReport->ItemIndex==-1) return;
 long idx=lstReport->ItemIndex;
 lstReport->Items->Delete(idx);
 cmbMenuReport->Items->Delete(idx);
 App->Reports.Destroy(idx);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::txtRepNameChange(TObject *Sender)
{
  if(lstReport->ItemIndex==-1) return;
  if(OnLstReport) return;
  ReportDef *ff;
  ff=App->Reports[lstReport->ItemIndex];
  ff->Caption=txtRepName->Text;
  ff->ConnectionName=cmbRepConnection->Text;
  ff->ReportFile.Path=txtRepFileName->Text;
  ff->SQL=txtRepSQL->Text;
  ff->CaptionExpression= txtCaptionExpression->Text;
  ff->BuildProc=txtBuildProc->Text;
  long idx = lstReport->ItemIndex;
  lstReport->Items->Strings[idx]=ff->Caption;
  lstReport->ItemIndex=idx;
  long idx2 = cmbMenuReport->ItemIndex;
  cmbMenuReport->Items->Strings[idx]=ff->Caption;
  cmbMenuReport->ItemIndex=idx2;

}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::btnFormulaAddClick(TObject *Sender)
{
  if(lstReport->ItemIndex==-1) return;
  ReportDef *ff;
  ff=App->Reports[lstReport->ItemIndex];
  FormulaDef *fr;
  ff->Formulas.Add( fr= new FormulaDef());
  fr->Name = "Новая формула";
  lstFormula->Items->Add("Новая формула");
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::btnFormulaDelClick(TObject *Sender)
{
  if(lstReport->ItemIndex==-1) return;
  if(lstFormula->ItemIndex==-1) return;
  ReportDef *ff;
  ff=App->Reports[lstReport->ItemIndex];
  ff->Formulas.Destroy(lstFormula->ItemIndex);
  lstFormula->Items->Delete(lstFormula->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::lstFormulaClick(TObject *Sender)
{
  if(lstReport->ItemIndex==-1) return;
  if(lstFormula->ItemIndex==-1) return;
  OnLstFormulas=true;
  ReportDef *ff;
  FormulaDef *fr;
  ff=App->Reports[lstReport->ItemIndex];
  fr=ff->Formulas[lstFormula->ItemIndex];
  txtFormulaName->Text = fr->Name;
  txtFormulaText->Text = fr->Formula;
  OnLstFormulas=false;

}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::txtFormulaNameChange(TObject *Sender)
{
  if(lstReport->ItemIndex==-1) return;
  if(lstFormula->ItemIndex==-1) return;
  if(OnLstFormulas) return;
  ReportDef *ff;
  FormulaDef *fr;
  ff=App->Reports[lstReport->ItemIndex];
  fr=ff->Formulas[lstFormula->ItemIndex];
  fr->Name=txtFormulaName->Text;
  fr->Formula=txtFormulaText->Text;
  long idx =lstFormula->ItemIndex;
  lstFormula->Items->Strings[idx]=fr->Name;
  lstFormula->ItemIndex=idx;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::btnTranFieldMoveBottomClick(TObject *Sender)
{
  if(lstTran->ItemIndex==-1) return;
  if( lstTranField->ItemIndex==-1 ) return;
  if(lstTranField->ItemIndex==lstTranField->Items->Count-1) return;

  long idx=lstTranField->ItemIndex;
  MyMemory mem1,mem2;
  FilterFormDef *Tran;
  Tran=App->Filters[lstTran->ItemIndex];
  FilterFieldDef *fa,*fa1;
  fa=Tran->FormDef[idx];
  fa1=Tran->FormDef[idx+1];

  // обмениваем
  mem1<<*fa;
  mem2<<*fa1;

  mem1>>*fa1;
  mem2>>*fa;

  lstTranField->Items->Clear();

  for(long i=0;i<Tran->FormDef.Count();i++){
		lstTranField->Items->Add(Tran->FormDef[i]->FieldName);
  }


  lstTranField->ItemIndex=idx+1;

}
//---------------------------------------------------------------------------

void __fastcall TfrmTasks::btnTranFieldMoveTopClick(TObject *Sender)
{
  if(lstTran->ItemIndex==-1) return;
  if( lstTranField->ItemIndex==-1 ) return;
  if(lstTranField->ItemIndex==0) return;

  long idx=lstTranField->ItemIndex;
  MyMemory mem1,mem2;
  FilterFormDef *Tran;
  Tran=App->Filters[lstTran->ItemIndex];
  FilterFieldDef *fa,*fa1;
  fa=Tran->FormDef[idx];
  fa1=Tran->FormDef[idx-1];

  // обмениваем
  mem1<<*fa;
  mem2<<*fa1;

  mem1>>*fa1;
  mem2>>*fa;

  lstTranField->Items->Clear();

  for(long i=0;i<Tran->FormDef.Count();i++){
		lstTranField->Items->Add(Tran->FormDef[i]->FieldName);
  }

  lstTranField->ItemIndex=idx-1;

}
//---------------------------------------------------------------------------




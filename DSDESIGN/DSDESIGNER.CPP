//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DSDesigner.h"
#include "Changes.h"
#include "SlaveGrid.h"
#include "Navigator.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "RXGrids"
//#pragma link "RXSpin"
#pragma link "StreamGrid"
//#pragma link "RXCtrls"
#pragma resource "*.dfm"
TfrmStreamDesigner *frmStreamDesigner;
//---------------------------------------------------------------------------
__fastcall TfrmStreamDesigner::TfrmStreamDesigner(TComponent* Owner)
	: TForm(Owner)
{
	Manager = 0;
	OnLstFields =false;
 OnLstConnections=false;
 OnCmbEditors=false;
 OnLstLinks=false;
 OnLstParams=false;
 OnLstTran=false;
 OnLstTranField=false;
 OnLstIndex=false;
}
//---------------------------------------------------------------------------

__fastcall TfrmStreamDesigner::~TfrmStreamDesigner(){
};
void __fastcall TfrmStreamDesigner::btnNewConnectionClick(TObject *Sender)
{
	ODBCDataSource *ds = new ODBCDataSource(txtDsn->Text);
	Manager->AddSource(ds,txtConnection->Text);
	lstConnections->Items->Add(txtConnection->Text);
	cmbConnections->Items->Add(txtConnection->Text);

}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnConnectClick(TObject *Sender)
{
	if(lstConnections->ItemIndex<0) return;
	ODBCDataSource *ds = (ODBCDataSource *)Manager->GetSource(lstConnections->Items->Strings[lstConnections->ItemIndex]);
	if(!ds) return;
	ds->LogOn(txtUID->Text,txtPWD->Text);
	if( ds->IsLoggedOn())
		lstConnections->State[lstConnections->ItemIndex] =cbChecked;
	else
		lstConnections->State[lstConnections->ItemIndex] =cbUnchecked;

}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::lstConnectionsClickCheck(
			TObject *Sender)
{
	if(lstConnections->ItemIndex<0) return;
	ODBCDataSource *ds =(ODBCDataSource *)Manager->GetSource(lstConnections->Items->Strings[lstConnections->ItemIndex]);
	if(!ds){
		lstConnections->State[lstConnections->ItemIndex] =cbUnchecked;
		return;
	}
	if( ds->IsLoggedOn())
		lstConnections->State[lstConnections->ItemIndex] =cbChecked;
	else
		lstConnections->State[lstConnections->ItemIndex] =cbUnchecked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::lstConnectionsClick(TObject *Sender)
{

 	if(lstConnections->ItemIndex<0) return;
    OnLstConnections = true;
	ODBCDataSource *ds =(ODBCDataSource *)Manager->GetSource(lstConnections->Items->Strings[lstConnections->ItemIndex]);
	if(!ds){
		return;
	}
	txtDsn->Text=ds->GetDSN();
	txtConnection->Text=lstConnections->Items->Strings[lstConnections->ItemIndex];
    OnLstConnections = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnDeleteConnectionClick(
			TObject *Sender)
{
	Manager->DelSource(lstConnections->Items->Strings[lstConnections->ItemIndex]);
	cmbConnections->Items->Delete(lstConnections->ItemIndex);
	lstConnections->Items->Delete(lstConnections->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnDisConnectClick(TObject *Sender)
{
	if(lstConnections->ItemIndex<0) return;
	ODBCDataSource *ds = (ODBCDataSource *)Manager->GetSource(lstConnections->Items->Strings[lstConnections->ItemIndex]);
	if(!ds) return;
	ds->LogOff();
	if( ds->IsLoggedOn())
		lstConnections->State[lstConnections->ItemIndex] =cbChecked;
	else
		lstConnections->State[lstConnections->ItemIndex] =cbUnchecked;

}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnLoadClick(TObject *Sender)
{

	MyFile file("Save.txt",acWrite);
	file << *Manager;

}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::Button1Click(TObject *Sender)
{

 if(MessageBox(0,
  "При загрузке текущие изменения могут быть утеряны. \r\n Загрузить описание из файла ?",
  "ВНИМАНИЕ!",MB_YESNO)!=IDYES) return;
	StreamGrid1->SetEditor(Manager,"");
	MyFile f("save.txt",acRead);
	Manager->Sources.Clear();
	Manager->Editors.Clear();

	f >> *Manager;
	lstConnections->Items->Clear();
 cmbConnections->Items->Clear();
	for(long i=0;i<Manager->Sources.Count();i++){
		lstConnections->Items->Add(Manager->Sources.Name(i));
		cmbConnections->Items->Add(Manager->Sources.Name(i));
	}

	cmbEditors->Items->Clear();
 cmbTranEditor->Items->Clear();
 cmbMaster->Items->Clear();
	cmbSlave->Items->Clear();
	cmbTestEditors->Items->Clear();
	cmbSubStream->Items->Clear();
	cmbTranSubStream->Items->Clear();
	for(long i=0;i<Manager->Editors.Count();i++){
		cmbEditors->Items->Add(Manager->Editors.Name(i));
	  cmbTranEditor->Items->Add(Manager->Editors.Name(i));
		cmbMaster->Items->Add(Manager->Editors.Name(i));
		cmbSlave->Items->Add(Manager->Editors.Name(i));
		cmbSubStream->Items->Add(Manager->Editors.Name(i));
		cmbTranSubStream->Items->Add(Manager->Editors.Name(i));
		cmbTestEditors->Items->Add(Manager->Editors.Name(i));
	}

 lstLinks->Items->Clear();
	for(long i=0;i<Manager->Links.Count();i++){
		lstLinks->Items->AddObject(Manager->Links[i]->MasterName+" -> "+Manager->Links[i]->Caption,(TObject *)i);
	}

 lstTran->Items->Clear();
	for(long i=0;i<Manager->Trans.Count();i++){
		lstTran->Items->AddObject(Manager->Trans[i]->EditorName+" -> "+Manager->Trans[i]->Caption,(TObject*) i);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::txtConnectionChange(TObject *Sender)
{
	if(lstConnections->ItemIndex<0) return;
    if(OnLstConnections) return;
	ODBCDataSource *ds =(ODBCDataSource *)Manager->GetSource(lstConnections->Items->Strings[lstConnections->ItemIndex]);
	if(!ds){
		return;
	}
	if(ds->IsLoggedOn())
		return;

	Manager->DelSource(ds);
	ds=new ODBCDataSource(txtDsn->Text);
	Manager->AddSource(ds,txtConnection->Text);
	lstConnections->Items->Strings[lstConnections->ItemIndex]=txtConnection->Text;
	cmbConnections->Items->Strings[lstConnections->ItemIndex]=txtConnection->Text;

}
//---------------------------------------------------------------------------





void __fastcall TfrmStreamDesigner::btnDelEditorClick(TObject *Sender)
{
	if(cmbEditors->ItemIndex < 0) return;
	StreamEditorDef *ed=Manager->Editors(cmbEditors->Text);
	Manager->Editors.Destroy(Manager->Editors.Where(ed));
	cmbSubStream->Items->Delete(cmbEditors->ItemIndex);
    cmbTranSubStream->Items->Delete(cmbEditors->ItemIndex);
    cmbMaster->Items->Delete(cmbEditors->ItemIndex);
    cmbSlave->Items->Delete(cmbEditors->ItemIndex);
	cmbTestEditors->Items->Delete(cmbEditors->ItemIndex);
	cmbTranEditor->Items->Delete(cmbEditors->ItemIndex);
	cmbEditors->Items->Delete(cmbEditors->ItemIndex);
}
//---------------------------------------------------------------------------


void __fastcall TfrmStreamDesigner::btnNewEditorClick(TObject *Sender)
{
	AnsiString n=InputBox("Новый редактор","Задайте имя редактора","");
  if(n=="") return;
  StreamEditorDef *ed=new StreamEditorDef();
	ed->Caption=n;
	if(n.IsEmpty()) return;
	Manager->AddEditor(ed,n);
	cmbEditors->Items->Add(n);
	cmbSubStream->Items->Add(n);
	cmbTranSubStream->Items->Add(n);
	cmbTestEditors->Items->Add(n);
 cmbMaster->Items->Add(n);
 cmbSlave->Items->Add(n);
	cmbTranEditor->Items->Add(n);
	cmbEditors->Text=n;
}
//---------------------------------------------------------------------------


void __fastcall TfrmStreamDesigner::cmbEditorsClick(TObject *Sender)
{
	if(cmbEditors->ItemIndex < 0) return;
    OnCmbEditors = true;
	StreamEditorDef *ed=Manager->Editors(cmbEditors->Text);
	txtSQL->Text=ed->SQL;
	txtRowSQL->Text=ed->ROWSQL;
	txtInsert->Text=ed->INSERT;
	txtUpdate->Text=ed->UPDATE;
	txtDelete->Text=ed->sDELETE;
 txtSubwhere->Text=ed->SubCondition;
 txtOrderBy->Text=ed->OrderBy;

	cmbConnections->ItemIndex = cmbConnections->Items->IndexOf(ed->ConnectionName);
 txtQryCaption->Text = ed->Caption;
	lstFields->Items->Clear();

	for(long i=0;i<ed->FormDef.Count();i++){
		lstFields->Items->Add(ed->FormDef[i]->FieldName);
	}

  lstParams->Items->Clear();

	for(long i=0;i<ed->ParamNames.Count();i++){
		lstParams->Items->Add(*ed->ParamNames[i]);
	}

 lstIndex->Items->Clear();
	for(long i=0;i<ed->Indexes.Count();i++){
		lstIndex->Items->Add(ed->Indexes[i]->Caption);
	}
 OnCmbEditors=false;
	}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::cmbSubStreamClick(TObject *Sender)
{
	if(cmbSubStream->ItemIndex < 0) return;
  if(lstFields->ItemIndex==-1) return;
	if(cmbEditors->ItemIndex==-1) return;
	OnLstFields =true;
	StreamEditorDef *ed;
	EditableFieldAttr *fa;

	ed=Manager->GetEditor(cmbEditors->Text);
	fa=ed->FormDef[lstFields->ItemIndex];
  StreamEditorDef *sub=Manager->GetEditor(cmbSubStream->Text);
  if(sub){
  cmbFieldsToShow->Items->Clear();
  for(long i=0;i<sub->FormDef.Count();i++){
	    cmbFieldsToShow->Items->Add(sub->FormDef[i]->FieldName);
  }

  cmbFieldToSync->Items->Clear();
  for(long i=0;i<sub->FormDef.Count();i++){
    cmbFieldToSync->Items->Add(sub->FormDef[i]->FieldName);
	}
  }
  cmbFieldsToShow->ItemIndex=cmbFieldsToShow->Items->IndexOf(fa->ShowField);
  cmbFieldToSync->ItemIndex=cmbFieldToSync->Items->IndexOf(fa->SyncField);

  OnLstFields =false;
	SaveFieldsChanges(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmStreamDesigner::SaveQueries(TObject *Sender)
{
			ODBCDataSource * ds;
			StreamEditorDef *ed;
			if(cmbConnections->ItemIndex==-1) return;
			if(cmbEditors->ItemIndex==-1) return;
     if(OnCmbEditors) return;
			ds=Manager->GetSource(cmbConnections->Text);
			ed=Manager->GetEditor(cmbEditors->Text);
			if(!ed) return;

			ed->ConnectionName=cmbConnections->Text;
			ed->SQL=txtSQL->Text;
			ed->ROWSQL=txtRowSQL->Text;
			ed->INSERT=txtInsert->Text;
			ed->UPDATE=txtUpdate->Text;
			ed->sDELETE=txtDelete->Text;
     ed->SubCondition=txtSubwhere->Text;
     ed->OrderBy=txtOrderBy->Text;
     ed->Caption =txtQryCaption->Text;
     if(!(cmbEditors->Text==txtQryCaption->Text)){
       long idx=cmbEditors->ItemIndex;
       long idx2;
       Manager->Editors.SetName(cmbEditors->Text,txtQryCaption->Text);
       cmbEditors->Items->Strings[idx]=txtQryCaption->Text;
       cmbEditors->ItemIndex=idx;

       idx2=cmbSubStream->ItemIndex;
	      cmbSubStream->Items->Strings[idx]=txtQryCaption->Text;
       cmbSubStream->ItemIndex=idx2;

       idx2=cmbTranSubStream->ItemIndex;
	      cmbTranSubStream->Items->Strings[idx]=txtQryCaption->Text;
       cmbTranSubStream->ItemIndex=idx2;

       idx2=cmbTestEditors->ItemIndex;
	      cmbTestEditors->Items->Strings[idx]=txtQryCaption->Text;
       cmbTestEditors->ItemIndex=idx2;

       idx2=cmbMaster->ItemIndex;
       cmbMaster->Items->Strings[idx]=txtQryCaption->Text;
       cmbMaster->ItemIndex=idx2;

       idx2=cmbSlave->ItemIndex;
       cmbSlave->Items->Strings[idx]=txtQryCaption->Text;
       cmbSlave->ItemIndex=idx2;

       idx2=cmbTranEditor->ItemIndex;
   	  cmbTranEditor->Items->Strings[idx]=txtQryCaption->Text;
       cmbTranEditor->ItemIndex=idx2;
     }
};


void __fastcall TfrmStreamDesigner::btnExecClick(TObject *Sender)
{
			ODBCDataSource * ds;
			StreamEditorDef *ed;
			if(cmbConnections->ItemIndex==-1) return;
			if(cmbEditors->ItemIndex==-1) return;
			ds=Manager->GetSource(cmbConnections->Text);
			if(!ds->IsLoggedOn()) return;
			ed=Manager->GetEditor(cmbEditors->Text);
			if(!ed) return;
      ProcCursor cccc;
			ed->ConnectionName=cmbConnections->Text;
			ed->SQL=txtSQL->Text;
			ed->ROWSQL=txtRowSQL->Text;
			ed->INSERT=txtInsert->Text;
			ed->UPDATE=txtUpdate->Text;
			ed->sDELETE=txtDelete->Text;

			if(MessageBox(0,"Все установки для полей будут утеряны, продолжить?","Внимание!!!",MB_YESNO)!=IDYES) return;
			DataStream *s=ds->GetStream(ed,0,0);
			if(!s) return;




       try{
			  s->Refresh();
			 }catch(TODBCException *e){
			  	e->Show();
				  return;
      }


			ed->FormDef.Clear();
			ed->ColDef.Clear();
			ColumnDef * cd;
			EditableFieldAttr *atr;
			for(long i=0;i<s->fields.Count();i++){
				atr = new EditableFieldAttr();
				atr->FieldName=s->fields.Name(i);
        atr->SyncField=s->fields.Name(i);
				atr->Label=s->fields.Name(i);
				ed->FormDef.Add(atr);
				cd = new ColumnDef();
				cd->Value=s->fields.Name(i);
				cd->FieldKind=StreamField;
				cd->Width=80; // ширина ячейки
				cd->Caption=s->fields.Name(i); 	// заголовок и в какой он будет строке
				cd->HeaderVert=0;
				cd->Vert=0;  // позиция по вертикали в выходной секции
				cd->Horiz=i; // позиция по горизонтали (и заголовок и выходная секция)
				ed->ColDef.Add(cd);
			}
			Manager->KillStream(s);
			cmbEditorsClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnTestClick(TObject *Sender)
{
	 StreamGrid1->SetEditor(Manager,cmbTestEditors->Text);
	 StreamGrid1->RunEditor();
}
//---------------------------------------------------------------------------



void __fastcall TfrmStreamDesigner::btnFieldListClick(TObject *Sender)
{
	AnsiString fl="";
	for(long i=0;i<lstFields->Items->Count;i++){
		fl=fl+lstFields->Items->Strings[i]+", ";
	}
	txtSQL->Text=fl+"\r\n" +txtSQL->Text;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnCopyToRowSQLClick(TObject *Sender)
{
	txtRowSQL->Text=txtSQL->Text;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnMakeInsertClick(TObject *Sender)
{
	AnsiString fl="Insert Into \r\n"
	"... ( \r\n";
	for(long i=0;i<lstFields->Items->Count;i++){
		if(i!=0)
			fl=fl+",\r\n";
		fl=fl+lstFields->Items->Strings[i];
	}
	 fl=fl+") Values ( \r\n";

	for(long i=0;i<lstFields->Items->Count;i++){
		if(i!=0)
			fl=fl+",\r\n";
		fl=fl+"^"+lstFields->Items->Strings[i] +"^";
	}

	fl=fl+")";

	txtInsert->Text=fl;

}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnCopyToDeleteClick(TObject *Sender)
{
	txtDelete->Text=txtRowSQL->Text;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnMakeUpdateClick(TObject *Sender)
{
	AnsiString fl="Update \r\n"
	"...  Set  \r\n";
	for(long i=0;i<lstFields->Items->Count;i++){
		if(i!=0)
			fl=fl+",\r\n";
		fl=fl+lstFields->Items->Strings[i]+"=^"+lstFields->Items->Strings[i] +"^";
	}

	fl=fl+" Where ...";

	txtUpdate->Text=fl;

}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::lstFieldsClick(TObject *Sender)
{

	if(lstFields->ItemIndex==-1) return;
	if(cmbEditors->ItemIndex==-1) return;
	OnLstFields =true;
	StreamEditorDef *ed;
	EditableFieldAttr *fa;
	ColumnDef *cd;
	ed=Manager->GetEditor(cmbEditors->Text);
	fa=ed->FormDef[lstFields->ItemIndex];
	cd=ed->ColDef[lstFields->ItemIndex];
	/*
	for(int i=0;i<ed->FormDef.Count();i++){
		if(ed->FormDef[i]->FiledName==lstFields->Text){
			fa=ed->FormDef[i];
			break;
		}
	}
	*/


	switch(fa->SQLType){
		case stNumeric:
			typeNumeric->Checked = cbChecked;
			break;
		case stDate:
			typeDateTime->Checked = cbChecked;
			break;
		default:
			typeString->Checked = cbChecked;
			break;
	}

	if(fa->AllowNull)
		AllowNull->Checked = cbChecked;
	else
		AllowNull->Checked = cbUnchecked;

	txtFieldCaption->Text=fa->Label;
	cmbControlType->ItemIndex=(int)fa->ControlType;

	if(fa->ControlType!=ectSubStream){
		 cmbSubStream->ItemIndex=-1;
		 cmbFieldsToShow->ItemIndex=-1;
		 cmbFieldsToShow->Enabled = false;
 	 cmbFieldToSync->ItemIndex=-1;
		 cmbFieldToSync->Enabled = false;
		 cmbSubStream->Enabled = false;
		 ReFillOnGetFocus->Enabled = false;
		 ReFillOnGetFocus->Checked = cbUnchecked;
		 ReFillOnFormShow->Enabled = false;
		 ReFillOnFormShow->Checked = cbUnchecked;
    btnFieldChanges->Enabled = false;
	}else{
    btnFieldChanges->Enabled = true;
		 ReFillOnGetFocus->Enabled = true;
		 ReFillOnFormShow->Enabled = true;

		 if(fa->FillOnGetFocus)
			 ReFillOnGetFocus->Checked = cbChecked;
		 else
			 ReFillOnGetFocus->Checked = cbUnchecked;
		 if(fa->FillOnFormShow)
			 ReFillOnFormShow->Checked = cbChecked;
		 else
			 ReFillOnFormShow->Checked = cbUnchecked;

		 cmbFieldsToShow->Enabled = true;
  	 cmbFieldToSync->Enabled = true;
		 cmbSubStream->Enabled = true;
		 cmbSubStream->ItemIndex=cmbSubStream->Items->IndexOf(fa->StreamName);
     {
          StreamEditorDef *sub=Manager->GetEditor(fa->StreamName);
          cmbFieldsToShow->Items->Clear();
          cmbFieldToSync->Items->Clear();
          if(sub){
            for(long i=0;i<sub->FormDef.Count();i++){
		          cmbFieldsToShow->Items->Add(sub->FormDef[i]->FieldName);
            }

            for(long i=0;i<sub->FormDef.Count();i++){
              cmbFieldToSync->Items->Add(sub->FormDef[i]->FieldName);
  	        }
          }
     }
		 cmbFieldsToShow->ItemIndex=cmbFieldsToShow->Items->IndexOf(fa->ShowField);
     cmbFieldToSync->ItemIndex=cmbFieldToSync->Items->IndexOf(fa->SyncField);
	}

	if(cd->Visible)
		VisibleOnView->Checked = cbChecked;
	else
		VisibleOnView->Checked = cbUnchecked;

	txtFieldWidth->Text=cd->Width;
    txtHoriz->Text=cd->Horiz;
    txtVert->Text=cd->Vert;
    txtHeaderVert->Text=cd->HeaderVert;

	switch(fa->DefInsertAccess){
		case faNone:
			InsNoAccess->Checked=cbChecked;
			break;
	 case faRead:
			InsReadOnly->Checked=cbChecked;
			break;
	 case faWrite:
			InsReadWrite->Checked=cbChecked;
			break;
	};
	switch(fa->DefEditAccess){
		case faNone:
			UpdNoAccess->Checked=cbChecked;
			break;
	 case faRead:
			UpdReadOnly->Checked=cbChecked;
			break;
	 case faWrite:
			UpdReadWrite->Checked=cbChecked;
			break;
	};
	OnLstFields =false;
}

//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::SaveFieldsChanges(TObject *Sender)
{
	if(OnLstFields) return;
	if(lstFields->ItemIndex==-1) return;
	if(cmbEditors->ItemIndex==-1) return;
	StreamEditorDef *ed;
	EditableFieldAttr *fa;
	ColumnDef *cd;
	ed=Manager->GetEditor(cmbEditors->Text);
	fa=ed->FormDef[lstFields->ItemIndex];
	cd=ed->ColDef[lstFields->ItemIndex];

	fa->Label=txtFieldCaption->Text;
	cd->Caption=txtFieldCaption->Text;
	if(cmbControlType->ItemIndex != -1)
		fa->ControlType=(EditControlType)cmbControlType->ItemIndex;
	else
		fa->ControlType=ectNone;

	fa->FillOnGetFocus=false;
	fa->FillOnFormShow=false;
	fa->StreamName="";
	fa->ShowField="";

	if(fa->ControlType==ectSubStream){
		 fa->FillOnGetFocus=(ReFillOnGetFocus->Checked == cbChecked);
		 fa->FillOnFormShow=(ReFillOnFormShow->Checked == cbChecked);
		 fa->StreamName=cmbSubStream->Text;
		 fa->ShowField=cmbFieldsToShow->Text;
		 fa->SyncField=cmbFieldToSync->Text;
	}

	cd->Visible=(VisibleOnView->Checked == cbChecked);
	cd->Width=80;
    try{
	cd->Width=txtFieldWidth->Text.ToInt();
    }catch(...){
    }
    try{
    if(cd->Visible)
        cd->Horiz=txtHoriz->Text.ToInt();
    else
        cd->Horiz=0;
    }catch(...){
    }
    try{
    cd->Vert=txtVert->Text.ToInt();
    }catch(...){
    }
    try{
    cd->HeaderVert=txtHeaderVert->Text.ToInt();
    }catch(...){
    }


	if(InsNoAccess->Checked==cbChecked) fa->DefInsertAccess=faNone;
	if(InsReadOnly->Checked==cbChecked) fa->DefInsertAccess=faRead;
	if(InsReadWrite->Checked==cbChecked) fa->DefInsertAccess=faWrite;
	if(UpdNoAccess->Checked==cbChecked) fa->DefEditAccess=faNone;
	if(UpdReadOnly->Checked==cbChecked) fa->DefEditAccess=faRead;
	if(UpdReadWrite->Checked==cbChecked) fa->DefEditAccess=faWrite;

	if(typeString->Checked==cbChecked) fa->SQLType=stString;
	if(typeNumeric->Checked==cbChecked) fa->SQLType=stNumeric;
	if(typeDateTime->Checked==cbChecked) fa->SQLType=stDate;
	fa->AllowNull=(AllowNull->Checked==cbChecked);

	if(fa->ControlType!=ectSubStream){
		 cmbSubStream->ItemIndex=-1;
		 cmbFieldsToShow->ItemIndex=-1;
		 cmbFieldsToShow->Enabled = false;
		 cmbSubStream->Enabled = false;
		 ReFillOnGetFocus->Enabled = false;
		 ReFillOnGetFocus->Checked = cbUnchecked;
		 ReFillOnFormShow->Enabled = false;
		 ReFillOnFormShow->Checked = cbUnchecked;
     cmbFieldToSync->Enabled = false;
	}else{
		 ReFillOnGetFocus->Enabled = true;
		 ReFillOnFormShow->Enabled = true;
		 cmbFieldsToShow->Enabled = true;
		 cmbSubStream->Enabled = true;
     cmbFieldToSync->Enabled = true;
	}
	fa->FillOnGetFocus=(ReFillOnGetFocus->Checked==cbChecked);
	fa->FillOnFormShow=(ReFillOnFormShow->Checked==cbChecked);

}







void __fastcall TfrmStreamDesigner::btnFieldChangesClick(TObject *Sender)
{
 if(lstFields->ItemIndex==-1 || !Manager || cmbEditors->ItemIndex==-1 || cmbSubStream->ItemIndex==-1)
   return;

 StreamEditorDef *ed;
 EditableFieldAttr *fa;
 ed=Manager->GetEditor(cmbEditors->Text);
 fa=ed->FormDef[lstFields->ItemIndex];
 TfrmChanges *chng;
 chng = new TfrmChanges(this);
 chng->Manager = Manager;
 if(!fa->WhenChanged )
   fa->WhenChanged = new EditFieldActions();
 chng->Actions=fa->WhenChanged;
 chng->Prepare();
 chng->ShowModal();
 if(!fa->WhenChanged->NotEmpty()){
   delete fa->WhenChanged;
   fa->WhenChanged=0;
 }
 delete chng;

}
//---------------------------------------------------------------------------



void __fastcall TfrmStreamDesigner::btnAddLinkClick(TObject *Sender)
{
 Manager->Links.Add( new StreamEditorLink());
 lstLinks->Items->AddObject("??? -> Новая связь",(TObject*)(Manager->Links.Count()-1));

}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnDelLinkClick(TObject *Sender)
{
 if(lstLinks->ItemIndex==-1) return;
 Manager->Links.Destroy((long)lstLinks->Items->Objects[lstLinks->ItemIndex]);
 lstLinks->Items->Delete(lstLinks->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::cmbMasterChange(TObject *Sender)
{
 if(lstLinks->ItemIndex==-1) return;
 if(OnLstLinks) return;
 StreamEditorLink *Link = Manager->Links[(long)lstLinks->Items->Objects[lstLinks->ItemIndex]];
 Link->MasterName=cmbMaster->Text;
 Link->SlaveName=cmbSlave->Text;
 Link->MasterCondition =txtMasterCondition->Text;
 Link->Caption=txtLinkName->Text;
 Link->SlaveCaption=txtSlaveCaption->Text;
 lstLinks->Items->Strings[lstLinks->ItemIndex]=Link->MasterName +" -> " +txtLinkName->Text;
 lstLinks->ItemIndex= lstLinks->Items->IndexOf(Link->MasterName +" -> " +txtLinkName->Text);
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::lstLinksClick(TObject *Sender)
{
 if(lstLinks->ItemIndex==-1) return;
 OnLstLinks=true;
 StreamEditorLink *Link = Manager->Links[(long)lstLinks->Items->Objects[lstLinks->ItemIndex]];
 cmbMaster->ItemIndex = cmbMaster->Items->IndexOf(Link->MasterName);
 cmbSlave->ItemIndex = cmbSlave->Items->IndexOf(Link->SlaveName);
 txtMasterCondition->Text = Link->MasterCondition;
 txtLinkName->Text=Link->Caption;
 txtSlaveCaption->Text=Link->SlaveCaption;
 OnLstLinks=false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::Button2Click(TObject *Sender)
{
 if(lstLinks->ItemIndex==-1 || !Manager || cmbMaster->ItemIndex==-1 || cmbSlave->ItemIndex==-1)
   return;
 TfrmChanges *chng;
 chng = new TfrmChanges(this);
 StreamEditorLink *Link = Manager->Links[(long)lstLinks->Items->Objects[lstLinks->ItemIndex]];
 chng->Manager = Manager;
 chng->Actions=&Link->WhenChanged;
 chng->Prepare();
 chng->ShowModal();
 delete chng;

}
//---------------------------------------------------------------------------


void __fastcall TfrmStreamDesigner::btnAddParamClick(TObject *Sender)
{
	if(cmbEditors->ItemIndex < 0) return;
	StreamEditorDef *ed=Manager->Editors(cmbEditors->Text);
  ed->ParamNames.Add( new AnsiString("Новый параметр"));
	lstParams->Items->Add("Новый параметр");

}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnDelParamClick(TObject *Sender)
{
	if(cmbEditors->ItemIndex < 0) return;
  if(lstParams->ItemIndex < 0) return;

	StreamEditorDef *ed=Manager->Editors(cmbEditors->Text);
  ed->ParamNames.Destroy(lstParams->ItemIndex);
	lstParams->Items->Delete(lstParams->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::txtParamNameChange(TObject *Sender)
{
  if(OnLstParams) return;
	if(cmbEditors->ItemIndex < 0) return;
  if(lstParams->ItemIndex < 0) return;

	StreamEditorDef *ed=Manager->Editors(cmbEditors->Text);
  *(ed->ParamNames[lstParams->ItemIndex])=txtParamName->Text;
	lstParams->Items->Strings[lstParams->ItemIndex]=txtParamName->Text;

}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::lstParamsClick(TObject *Sender)
{
	if(cmbEditors->ItemIndex < 0) return;
  if(lstParams->ItemIndex < 0) return;
  OnLstParams =true;
	txtParamName->Text=lstParams->Items->Strings[lstParams->ItemIndex];
  OnLstParams =false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnAddFieldClick(TObject *Sender)
{
	if(cmbEditors->ItemIndex==-1) return;
  AnsiString f =InputBox("Новое поле","Задайте имя поля","");
  if(f=="") return;
	StreamEditorDef *ed;
	EditableFieldAttr *fa;
	ColumnDef *cd;
	ed=Manager->GetEditor(cmbEditors->Text);
	ed->FormDef.Add(fa=new EditableFieldAttr());
  fa->FieldName=f;
	ed->ColDef.Add(cd=new ColumnDef());
  cd->Value=f;
  cd->FieldKind=StreamField;
  lstFields->Items->Add(f);
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnDelFieldClick(TObject *Sender)
{
	if(lstFields->ItemIndex==-1) return;
	if(cmbEditors->ItemIndex==-1) return;

	StreamEditorDef *ed;
	ed=Manager->GetEditor(cmbEditors->Text);
  ed->FormDef.Destroy(lstFields->ItemIndex);
  ed->ColDef.Destroy(lstFields->ItemIndex);
  lstFields->Items->Delete(lstFields->ItemIndex);

}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::cmbTestEditorsClick(TObject *Sender)
{
   cmbLinks->Items->Clear();
   if(cmbTestEditors->ItemIndex==-1) return;
   for(long i=0;i<Manager->Links.Count();i++){
      if(cmbTestEditors->Text==Manager->Links[i]->MasterName)
      cmbLinks->Items->Add(Manager->Links[i]->Caption);
   }
   if(cmbLinks->Items->Count>0)
     cmbLinks->ItemIndex=0;

   cmbTrans->Items->Clear();
   if(cmbTestEditors->ItemIndex==-1) return;
   for(long i=0;i<Manager->Trans.Count();i++){
      if(cmbTestEditors->Text==Manager->Trans[i]->EditorName)
      cmbTrans->Items->Add(Manager->Trans[i]->Caption);
   }
   if(cmbTrans->Items->Count>0)
     cmbTrans->ItemIndex=0;

}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnCallLinkedClick(TObject *Sender)
{
if(
   StreamGrid1->GetStream()  &&
   StreamGrid1->GetSnap()->RowCount()  &&
   cmbLinks->ItemIndex !=-1){
   Params *p;
   StreamEditorLink *Link;
   for(long i=0;i<Manager->Links.Count();i++){
         if(Manager->Links[i]->MasterName==StreamGrid1->GetEditorName() &&
            Manager->Links[i]->Caption==cmbLinks->Text){
            Link=Manager->Links[i];
            goto LinkFound;
            }
   }
   return;

   LinkFound:
   p=StreamGrid1->GetState();
      if(StreamGrid1->TestLink(Link->MasterCondition)){
            TfrmSlave * frmSlave =  new TfrmSlave(Application);
            frmSlave->StreamGrid1->SetEditor(Manager,Link->SlaveName);
            frmSlave->StreamGrid1->InitSlave(&Link->WhenChanged,p);
            frmSlave->Caption = Link->Caption;
            frmSlave->Init();
            frmSlave->Show();
      }
   delete p;
 }

}
//---------------------------------------------------------------------------




void __fastcall TfrmStreamDesigner::Button3Click(TObject *Sender)
{
 Manager->Trans.Add(new StreamTransaction());
 lstTran->Items->AddObject("??? -> Новая операция",(TObject*)(Manager->Trans.Count()-1));
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::lstTranClick(TObject *Sender)
{
   OnLstTran = true;
   if(lstTran->ItemIndex==-1) return;
   StreamTransaction *Tran;
   Tran=Manager->Trans[(long)lstTran->Items->Objects[lstTran->ItemIndex]];
   txtTranCaption->Text = Tran->Caption;
   txtTranMessage->Text = Tran->Message;
   txtTranCondition->Text = Tran->TranCondition;
   txtTransaction->Text = Tran->Transaction;
   cmbTransactionSource->ItemIndex=Tran->TransactionSource;
   cmbAfterTransaction->ItemIndex=Tran->AfterTransaction;
   cmbTranEditor->ItemIndex = cmbTranEditor->Items->IndexOf(Tran->EditorName);
   lstTranField->Items->Clear();
   for(long i=0;i<Tran->FormDef.Count();i++){
     lstTranField->Items->Add(Tran->FormDef[i]->FieldName);
   }
   OnLstTran = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::txtTranCaptionChange(TObject *Sender)
{
   if(OnLstTran) return;
   if(lstTran->ItemIndex==-1) return;
   StreamTransaction *Tran;
   Tran=Manager->Trans[(long)lstTran->Items->Objects[lstTran->ItemIndex]];
   Tran->Caption=txtTranCaption->Text;
   Tran->Message=txtTranMessage->Text;
   Tran->TranCondition=txtTranCondition->Text;
   Tran->Transaction=txtTransaction->Text;
   Tran->TransactionSource=cmbTransactionSource->ItemIndex;
   Tran->AfterTransaction=cmbAfterTransaction->ItemIndex;
   Tran->EditorName=cmbTranEditor->Text;
   lstTran->Items->Strings[lstTran->ItemIndex]=Tran->EditorName+" -> "+Tran->Caption;
   lstTran->ItemIndex=lstTran->Items->IndexOf(Tran->EditorName+" -> "+Tran->Caption);
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::Button4Click(TObject *Sender)
{
   if(lstTran->ItemIndex==-1) return;
   Manager->Trans.Destroy(lstTran->ItemIndex);
   lstTran->Items->Delete(lstTran->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnAddTranFieldClick(TObject *Sender)
{
   if(lstTran->ItemIndex==-1) return;
   StreamTransaction *Tran;
   Tran=Manager->Trans[(long)lstTran->Items->Objects[lstTran->ItemIndex]];
   AnsiString f =InputBox("Новое поле","Задайте имя поля","");
   if(f=="") return;
  	EditableFieldAttr *fa;
	  Tran->FormDef.Add(fa=new EditableFieldAttr());
   fa->FieldName=f;
   lstTranField->Items->Add(f);
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnDelTranFieldClick(TObject *Sender)
{
   if(lstTran->ItemIndex==-1) return;
   if(lstTranField->ItemIndex==-1) return;
   StreamTransaction *Tran;
   Tran=Manager->Trans[(long)lstTran->Items->Objects[lstTran->ItemIndex]];
	  Tran->FormDef.Destroy(lstTranField->ItemIndex);
   lstTranField->Items->Delete(lstTranField->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::lstTranFieldClick(TObject *Sender)
{
	if(lstTranField->ItemIndex==-1) return;
	if(lstTran->ItemIndex==-1) return;
	OnLstTranField =true;
	StreamTransaction *ed;
	EditableFieldAttr *fa;
	ed=Manager->Trans[(long)lstTran->Items->Objects[lstTran->ItemIndex]];
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
	cmbTranControlType->ItemIndex=(int)fa->ControlType;

	if(fa->ControlType!=ectSubStream){
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
       StreamEditorDef *sub=Manager->GetEditor(fa->StreamName);
       cmbTranFieldsToShow->Items->Clear();
       cmbTranFieldToSync->Items->Clear();
       if(sub){
         for(long i=0;i<sub->FormDef.Count();i++){
	          cmbTranFieldsToShow->Items->Add(sub->FormDef[i]->FieldName);
         }

         for(long i=0;i<sub->FormDef.Count();i++){
           cmbTranFieldToSync->Items->Add(sub->FormDef[i]->FieldName);
         }
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

void __fastcall TfrmStreamDesigner::txtTranFieldCaptionChange(
      TObject *Sender)
{
	if(OnLstTranField) return;
	if(lstTranField->ItemIndex==-1) return;
	if(lstTran->ItemIndex==-1) return;
	StreamTransaction *ed;
	EditableFieldAttr *fa;
	ed=Manager->Trans[(long)lstTran->Items->Objects[lstTran->ItemIndex]];
	fa=ed->FormDef[lstTranField->ItemIndex];
	fa->Label=txtTranFieldCaption->Text;

	if(cmbTranControlType->ItemIndex != -1)
		fa->ControlType=(EditControlType)cmbTranControlType->ItemIndex;
	else
		fa->ControlType=ectNone;

	fa->FillOnGetFocus=false;
	fa->FillOnFormShow=false;
	fa->StreamName="";
	fa->ShowField="";

	if(fa->ControlType==ectSubStream){
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

	if(fa->ControlType!=ectSubStream){
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

void __fastcall TfrmStreamDesigner::cmbTranSubStreamClick(TObject *Sender)
{
	if(cmbTranSubStream->ItemIndex < 0) return;
 if(lstTranField->ItemIndex==-1) return;
	if(lstTran->ItemIndex==-1) return;
	OnLstTranField =true;
	StreamTransaction *ed;
	EditableFieldAttr *fa;

	ed=Manager->Trans[(long)lstTran->Items->Objects[lstTran->ItemIndex]];
	fa=ed->FormDef[lstTranField->ItemIndex];
 StreamEditorDef *sub=Manager->GetEditor(cmbTranSubStream->Text);
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

void __fastcall TfrmStreamDesigner::btnTranFieldChangesClick(
      TObject *Sender)
{
 if(lstTranField->ItemIndex==-1 ||lstTran->ItemIndex==-1 || !Manager )
   return;
 StreamTransaction *Tran = Manager->Trans[(long)lstTran->Items->Objects[lstTran->ItemIndex]];
 EditableFieldAttr *fa;
 fa=Tran->FormDef[lstTranField->ItemIndex];
 TfrmChanges *chng;
 chng = new TfrmChanges(this);
 chng->Manager = Manager;
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


void __fastcall TfrmStreamDesigner::Button5Click(TObject *Sender)
{
   if(OnLstTran) return;
   if(lstTran->ItemIndex==-1) return;
   if(cmbTranEditor->ItemIndex==-1) return;
   StreamTransaction *Tran;
   Tran=Manager->Trans[(long)lstTran->Items->Objects[lstTran->ItemIndex]];
   if(Tran->FormDef.Count() && MessageBox(0,"Список полей формы ввода будет заменен. Заменить ?","Внимание",MB_YESNO)!=IDYES) return;
   MyMemory Mem;
   Mem << Manager->Editors.Get(cmbTranEditor->Text)->FormDef;
   Mem >> Tran->FormDef;
   lstTranClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnTranClick(TObject *Sender)
{
  if(
   StreamGrid1->GetStream()  &&
   cmbTrans->ItemIndex !=-1){
               StreamGrid1->RunTransaction(cmbTrans->Text);
  }
}
//---------------------------------------------------------------------------


void __fastcall TfrmStreamDesigner::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 if(Action!=caFree){
    Action=caFree;
    frmStreamDesigner =0;
 }

}
//---------------------------------------------------------------------------
void __fastcall TfrmStreamDesigner::Init(){
	StreamGrid1->SetEditor(Manager,"");
	lstConnections->Items->Clear();
 cmbConnections->Items->Clear();
	for(long i=0;i<Manager->Sources.Count();i++){
		lstConnections->Items->Add(Manager->Sources.Name(i));
		cmbConnections->Items->Add(Manager->Sources.Name(i));
	  if( Manager->Sources[i]->IsLoggedOn())
 		lstConnections->State[i] =cbChecked;
	  else
	  	lstConnections->State[i] =cbUnchecked;

	}

	cmbEditors->Items->Clear();
 cmbTranEditor->Items->Clear();
 cmbMaster->Items->Clear();
	cmbSlave->Items->Clear();
	cmbTestEditors->Items->Clear();
	cmbSubStream->Items->Clear();
	cmbTranSubStream->Items->Clear();
	for(long i=0;i<Manager->Editors.Count();i++){
		cmbEditors->Items->Add(Manager->Editors.Name(i));
	  cmbTranEditor->Items->Add(Manager->Editors.Name(i));
		cmbMaster->Items->Add(Manager->Editors.Name(i));
		cmbSlave->Items->Add(Manager->Editors.Name(i));
		cmbSubStream->Items->Add(Manager->Editors.Name(i));
		cmbTranSubStream->Items->Add(Manager->Editors.Name(i));
		cmbTestEditors->Items->Add(Manager->Editors.Name(i));
	}

 lstLinks->Items->Clear();
	for(long i=0;i<Manager->Links.Count();i++){
		lstLinks->Items->AddObject(Manager->Links[i]->MasterName+" -> "+Manager->Links[i]->Caption,(TObject*)i);
	}

 lstTran->Items->Clear();
	for(long i=0;i<Manager->Trans.Count();i++){
		lstTran->Items->AddObject(Manager->Trans[i]->EditorName+" -> "+Manager->Trans[i]->Caption,(TObject*)i);
	}

};



void __fastcall TfrmStreamDesigner::btnAddIndexClick(TObject *Sender)
{
	if(cmbEditors->ItemIndex < 0) return;
	StreamEditorDef *ed=Manager->Editors(cmbEditors->Text);
 StreamIndex * ptr;
 ed->Indexes.Add( ptr=new StreamIndex());
	lstIndex->Items->Add("Новый индекс");
 ptr->Caption ="Новый индекс";

}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnDelIndexClick(TObject *Sender)
{
	if(cmbEditors->ItemIndex < 0) return;
 if(lstIndex->ItemIndex < 0) return;
	StreamEditorDef *ed=Manager->Editors(cmbEditors->Text);
 ed->Indexes.Destroy(lstIndex->ItemIndex);
	lstIndex->Items->Delete(lstIndex->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::txtIndexNameChange(TObject *Sender)
{
  if(cmbEditors->ItemIndex < 0) return;
  if(lstIndex->ItemIndex < 0) return;
  if(OnLstIndex) return;
	 StreamEditorDef *ed=Manager->Editors(cmbEditors->Text);
  StreamIndex * ptr=ed->Indexes[lstIndex->ItemIndex];
  ptr->Caption=txtIndexName->Text;
  ptr->OrderBy=txtIndexOrderBy->Text;
  long idx=lstIndex->ItemIndex;
  lstIndex->Items->Strings[idx]=txtIndexName->Text;
  lstIndex->ItemIndex=idx;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::lstIndexClick(TObject *Sender)
{
  if(cmbEditors->ItemIndex < 0) return;
  if(lstIndex->ItemIndex < 0) return;

	 StreamEditorDef *ed=Manager->Editors(cmbEditors->Text);
  StreamIndex * ptr=ed->Indexes[lstIndex->ItemIndex];
  OnLstIndex =true;
  txtIndexName->Text=ptr->Caption;
  txtIndexOrderBy->Text=ptr->OrderBy;
  OnLstIndex =false;
}
//---------------------------------------------------------------------------




void __fastcall TfrmStreamDesigner::btnFieldMoveBottomClick(
      TObject *Sender)
{
  if(cmbEditors->ItemIndex==-1) return;
  if( lstFields->ItemIndex==-1 ) return;
  if(lstFields->ItemIndex==lstFields->Items->Count-1) return;

  long idx=lstFields->ItemIndex;
  MyMemory mem1,mem2,mem3,mem4;

  StreamEditorDef *ed;
  EditableFieldAttr *fa,*fa1;
  ColumnDef *cd,*cd1;
  ed=Manager->GetEditor(cmbEditors->Text);
  fa=ed->FormDef[idx];
  cd=ed->ColDef[idx];
  fa1=ed->FormDef[idx+1];
  cd1=ed->ColDef[idx+1];

  // обмениваем
  mem1<<*fa;
  mem2<<*cd;
  mem3<<*fa1;
  mem4<<*cd1;

  mem1>>*fa1;
  mem2>>*cd1;
  mem3>>*fa;
  mem4>>*cd;

  lstFields->Items->Clear();

  for(long i=0;i<ed->FormDef.Count();i++){
		lstFields->Items->Add(ed->FormDef[i]->FieldName);
  }


  lstFields->ItemIndex=idx+1;

}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnFieldMoveTopClick(TObject *Sender)
{
  if(cmbEditors->ItemIndex==-1) return;
  if( lstFields->ItemIndex==-1 ) return;
  if(lstFields->ItemIndex==0) return;

  long idx=lstFields->ItemIndex;
  MyMemory mem1,mem2,mem3,mem4;

  StreamEditorDef *ed;
  EditableFieldAttr *fa,*fa1;
  ColumnDef *cd,*cd1;
  ed=Manager->GetEditor(cmbEditors->Text);
  fa=ed->FormDef[idx];
  cd=ed->ColDef[idx];
  fa1=ed->FormDef[idx-1];
  cd1=ed->ColDef[idx-1];

  // обмениваем
  mem1<<*fa;
  mem2<<*cd;
  mem3<<*fa1;
  mem4<<*cd1;

  mem1>>*fa1;
  mem2>>*cd1;
  mem3>>*fa;
  mem4>>*cd;

  lstFields->Items->Clear();

  for(long i=0;i<ed->FormDef.Count();i++){
		lstFields->Items->Add(ed->FormDef[i]->FieldName);
  }


  lstFields->ItemIndex=idx-1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnTranFieldMoveBottomClick(
      TObject *Sender)
{
  if(lstTran->ItemIndex==-1) return;
  if( lstTranField->ItemIndex==-1 ) return;
  if(lstTranField->ItemIndex==lstTranField->Items->Count-1) return;

  long idx=lstTranField->ItemIndex;
  MyMemory mem1,mem2;

  StreamTransaction *Tran;
  EditableFieldAttr *fa,*fa1;

  Tran=Manager->Trans[(long)lstTran->Items->Objects[lstTran->ItemIndex]];
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

void __fastcall TfrmStreamDesigner::btnTranFieldMoveTopClick(
      TObject *Sender)
{
  if(lstTran->ItemIndex==-1) return;
  if(lstTranField->ItemIndex<=0) return;

  long idx=lstTranField->ItemIndex;
  MyMemory mem1,mem2;

  StreamTransaction *Tran;
  EditableFieldAttr *fa,*fa1;

  Tran=Manager->Trans[(long)lstTran->Items->Objects[lstTran->ItemIndex]];
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

void __fastcall TfrmStreamDesigner::btnRenumberFieldsClick(TObject *Sender)
{
  if(cmbEditors->ItemIndex==-1) return;
  lstFields->ItemIndex=-1;

  StreamEditorDef *ed;
  ColumnDef *cd;
  ed=Manager->GetEditor(cmbEditors->Text);
  long idx=0;
  for(long i=0;i<ed->ColDef.Count();i++){
      cd=ed->ColDef[i];
      if(cd->Visible)
         cd->Horiz=idx++;
      else
         cd->Horiz=0;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmStreamDesigner::btnCopyTranClick(TObject *Sender)
{
 if(lstTran->ItemIndex==-1) return;
 MyMemory mem;
 StreamTransaction *Tran;
 Tran=Manager->Trans[(long)lstTran->Items->Objects[lstTran->ItemIndex]];
 mem <<*Tran;
 Manager->Trans.Add(Tran=new StreamTransaction());
 mem >>*Tran;
 lstTran->Items->AddObject(Tran->EditorName+" "+Tran->Caption,(TObject*)(Manager->Trans.Count()-1));
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SG_Garbage.h"
#include "EditForm.h"
//#include "_SortDlg.h"
//#include "_FilterDlg.h"
#include "Index.h"
#include "ToHTML.h"
#include "ToEXCEL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//---------------------------------------------------------------------------
__fastcall TSG_Garbage::TSG_Garbage(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TSG_Garbage::mnuUpdateClick(TObject *Sender)
{

 if(StreamGrid1->GetStream() &&
    StreamGrid1->GetStream()->AllowUpdate() &&
    StreamGrid1->GetSnap() &&
    StreamGrid1->GetSnap()->RowCount()>0){

   StreamGrid1->EditForm()->Caption=StreamGrid1->GetStreamEditor()->Caption +" (изменение)";
   StreamGrid1->SetEditMark();

  // сохраняем состояние строки до изменения
  Params *state=StreamGrid1->GetState();
  Params *state2=StreamGrid1->GetState();

	 // меняем
  if(!StreamGrid1->EditForm()->SettedUp){

  	if(mrOk==StreamGrid1->EditForm()->Setup(
     StreamGrid1->GetManager(),
     &StreamGrid1->GetStreamEditor()->FormDef,
			&StreamGrid1->LocalFormDef,
			StreamGrid1->GetStream(),
			false,
			state,
     StreamGrid1->GetStreamEditor()->UPDATE )
    )
       StreamGrid1->RefreshRow();
   else
       StreamGrid1->SetState(state2);
  } else {
    if(mrOk==StreamGrid1->EditForm()->Run(
			false,
			state,
     StreamGrid1->GetStreamEditor()->UPDATE
     ))
      StreamGrid1->RefreshRow();
    else
       // если не удалось, восстанавливаем предыдущее состояние
       StreamGrid1->SetState(state2);
  }

    // забываем буфер(а)
    delete state;
    delete state2;
    StreamGrid1->ClearEditMark();
 }
}
//---------------------------------------------------------------------------

void __fastcall TSG_Garbage::mnuInsertClick(TObject *Sender)
{

	Params * f = new Params;
	StreamGrid1->MakeFieldsForInsert(f);
 StreamGrid1->EditForm()->Caption=StreamGrid1->GetStreamEditor()->Caption +" (создание новой записи)";
 if(!StreamGrid1->EditForm()->SettedUp){
	  if(mrOk==StreamGrid1->EditForm()->Setup(
     StreamGrid1->GetManager(),
     &StreamGrid1->GetStreamEditor()->FormDef,
			&StreamGrid1->LocalFormDef,
			StreamGrid1->GetStream(),
			true,
			f,
     StreamGrid1->GetStreamEditor()->INSERT
     )
		){
   StreamGrid1->RefreshData();
   StreamGrid1->Refresh();
   }
  }else{
  	if(mrOk==StreamGrid1->EditForm()->Run(
			true,
			f,
     StreamGrid1->GetStreamEditor()->INSERT )
		){
   StreamGrid1->RefreshData();
   StreamGrid1->Refresh();
   }
  }
	delete f;
}
//---------------------------------------------------------------------------

void __fastcall TSG_Garbage::mnuDeleteClick(TObject *Sender)
{

 if(StreamGrid1->GetStream() &&
    StreamGrid1->GetSnap() &&
    StreamGrid1->GetSnap()->RowCount()>0){

    StreamGrid1->SetEditMark();
	  if(MessageBox(0,"Удалить текущую запись","Подтверждение удаления",MB_YESNO)==IDYES){
		   StreamGrid1->DeleteSRow();
    }
    StreamGrid1->ClearEditMark();
  }  
}
//---------------------------------------------------------------------------

void __fastcall TSG_Garbage::mnuRefreshClick(TObject *Sender)
{
	StreamGrid1->RefreshData();
	StreamGrid1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TSG_Garbage::mnuSortClick(TObject *Sender)
{
		 /*
		 EditFieldsAttr * fa=StreamGrid1->GetFieldsAttr();

		 if(!fa->Count()){
				if(StreamGrid1->GetSnap()->CurrentSort) delete StreamGrid1->GetSnap()->CurrentSort;
				StreamGrid1->Sort(0);
				return;
		 }

		 TsortDlg * dlg = new TsortDlg(this);
		 dlg->SortList->Items->Clear();

		 //Сначала загружаем то, что участвует в сортировке

				if(StreamGrid1->GetSnap()->CurrentSort){
					for(int j=0;j<StreamGrid1->GetSnap()->CurrentSort->Count();j++){
						for(long i=0;i<fa->Count();i++){
							if(fa->Get(i)->FieldName==
								 StreamGrid1->GetSnap()->CurrentSort->Get(j)->Name){
								dlg->SortList->Items->Add(fa->Get(i)->Label);
								if(StreamGrid1->GetSnap()->CurrentSort->Get(j)->Asc)
									dlg->SortList->State[dlg->SortList->Items->Count-1]=cbChecked;
								else
									dlg->SortList->State[dlg->SortList->Items->Count-1]=cbUnchecked;
								break;
							}
						}
					}

					// Потом все остальное
					for(long i=0;i<fa->Count();i++){
						for(int j=0;j<StreamGrid1->GetSnap()->CurrentSort->Count();j++){
							if(fa->Get(i)->FieldName==StreamGrid1->GetSnap()->CurrentSort->Get(j)->Name)
								 goto nxtField;

						}
						dlg->SortList->Items->Add(fa->Get(i)->Label);
						dlg->SortList->State[dlg->SortList->Items->Count-1]=cbGrayed;
		nxtField:
					}

				}else{
					for(long i=0;i<fa->Count();i++){
						dlg->SortList->Items->Add(fa->Get(i)->Label);
						dlg->SortList->State[dlg->SortList->Items->Count-1]=cbGrayed;
					}
				}



		 if(dlg->ShowModal()!=mrOk) { delete dlg; return; }

		 SortDef *sort=new SortDef();
		 SortLineDef *sld;

		 for(long i=0;i<dlg->SortList->Items->Count;i++){
			 if(dlg->SortList->State[i]!=cbGrayed){
				 sld= new SortLineDef;

				 for(long j=0;j<fa->Count();j++){
					if((fa->Get(j))->Label == (dlg->SortList->Items->Strings[i]) ){
						sld->Name=fa->Get(j)->FieldName;
						sld->Asc=(dlg->SortList->State[i]==cbChecked);
						sort->Add(sld);
						break;
					}
				 }

			 }
		 }

		 StreamGrid1->Sort(sort);
		 */

}
//---------------------------------------------------------------------------




void __fastcall TSG_Garbage::mnuFilterClick(TObject *Sender)
{
	/*
	FilterDef *filter;
	FilterLineDef *fl;

	EditFieldsAttr * fa=StreamGrid1->GetFieldsAttr();

	if(!fa->Count()){
				if(StreamGrid1->GetSnap()->CurrentFilter) delete StreamGrid1->GetSnap()->CurrentFilter;
				StreamGrid1->Filter(0);
				return;
	}


	TFilterDlg *dlg=new TFilterDlg(this);

	dlg->FieldList->Clear();
	dlg->Storage.Clear();

	for(long i=0;i<fa->Count();i++){
		dlg->FieldList->Items->Add(fa->Get(i)->Label);
		dlg->FieldList->State[dlg->FieldList->Items->Count-1]=cbUnchecked;
		//dlg->FieldList->EnabledItem[dlg->FieldList->Items->Count-1]=false;
		fl = new FilterLineDef();
		fl->Condition=fcNone;
		fl->Name=fa->Get(i)->FieldName;
		dlg->Storage.Add(fl);
	}
	if(dlg->ShowModal()!=mrOk){
		delete dlg;
		return;
	}

	if(StreamGrid1->GetSnap()->CurrentFilter) delete StreamGrid1->GetSnap()->CurrentFilter;

	filter = new FilterDef;
	for( long i=0;i<dlg->Storage.Count();i++){
		if(dlg->Storage[i]->Condition!=fcNone){
				fl=new FilterLineDef;
				fl->Name=dlg->Storage[i]->Name;
				fl->Condition=dlg->Storage[i]->Condition;
				fl->From=dlg->Storage[i]->From;
				fl->To=dlg->Storage[i]->To;
				filter->Add(fl);
		}
	}

	delete dlg;

	if(filter->Count())
		StreamGrid1->Filter(filter);
	else{
		delete filter;
		StreamGrid1->Filter(0);
	}
	*/

}
//---------------------------------------------------------------------------


void __fastcall TSG_Garbage::StreamGrid1DblClick(TObject *Sender)
{
		mnuUpdateClick(Sender);
}
//---------------------------------------------------------------------------









void __fastcall TSG_Garbage::MarkRowClick(TObject *Sender)
{
	if(StreamGrid1->IsSelected(StreamGrid1->GetRow())){
    	StreamGrid1->UnSelectRow(StreamGrid1->GetRow());
    }else{
      	StreamGrid1->SelectRow(StreamGrid1->GetRow());
    }
}
//---------------------------------------------------------------------------

void __fastcall TSG_Garbage::MarkAllClick(TObject *Sender)
{
	if(StreamGrid1->SelectedRowCount()){
    	StreamGrid1->UnSelectAllRow();
    }else{
      	StreamGrid1->SelectAllRow();
    }
}
//---------------------------------------------------------------------------

void __fastcall TSG_Garbage::FindColClick(TObject *Sender)
{
     StreamGrid1->FindOnColumn();
}
//---------------------------------------------------------------------------

void __fastcall TSG_Garbage::FindAllClick(TObject *Sender)
{
  StreamGrid1->Find();
}
//---------------------------------------------------------------------------

void __fastcall TSG_Garbage::FindNextClick(TObject *Sender)
{
 if(!StreamGrid1->FindNext()){
   MessageBox(0,"Вхождение строки не найдено","Поиск завершен",MB_OK);
 };
}
//---------------------------------------------------------------------------

void __fastcall TSG_Garbage::FindPrevClick(TObject *Sender)
{
 if(!StreamGrid1->FindPrev()){
   MessageBox(0,"Вхождение строки не найдено","Поиск завершен",MB_OK);
 };

}
//---------------------------------------------------------------------------


void __fastcall TSG_Garbage::mnuIndexClick(TObject *Sender)
{
  if(StreamGrid1->GetStreamEditor()->Indexes.Count()){
    TfrmIndex * frm=new TfrmIndex(Application);
    frm->lstIndex->Items->Clear();
    frm->lstIndex->Items->Add("(по умолчанию)");
    for(long i=0;i<StreamGrid1->GetStreamEditor()->Indexes.Count();i++)
      frm->lstIndex->Items->Add(StreamGrid1->GetStreamEditor()->Indexes[i]->Caption);
    frm->lstIndex->ItemIndex=0;
    if(frm->ShowModal()==mrOk){
       if(frm->lstIndex->ItemIndex>=0){
         if(frm->lstIndex->ItemIndex==0)
           StreamGrid1->OrderBy=StreamGrid1->GetStreamEditor()->OrderBy;
         else
           StreamGrid1->OrderBy=
           StreamGrid1->GetStreamEditor()->Indexes[frm->lstIndex->ItemIndex-1]->OrderBy;
	        StreamGrid1->RefreshData();
       	StreamGrid1->Refresh();
       }
    }
    delete frm;
  }
}
//---------------------------------------------------------------------------

void __fastcall TSG_Garbage::mnuToHTMLClick(TObject *Sender)
{
  //MessageBox(0,"Процедура экспорта в процессе разработки","Извините!",MB_OK);
  frmToHTML = new TfrmToHTML(Application);
  if(frmToHTML->ShowModal()==mrOk){
    if(!frmToHTML->htmlName->Text.IsEmpty()){
      AnsiString h = StreamGrid1->BuildHTML();
      TStringList * sl = new TStringList();
      sl->Text=h;
      sl->SaveToFile(frmToHTML->htmlName->Text);
      delete sl;
    }

  }
  delete frmToHTML;

}
//---------------------------------------------------------------------------

void __fastcall TSG_Garbage::mnuToXLClick(TObject *Sender)
{
  //MessageBox(0,"Процедура экспорта в процессе разработки","Извините!",MB_OK);
  frmToEXCEL = new TfrmToEXCEL(Application);
  if(frmToEXCEL->ShowModal()==mrOk){
    if(!frmToEXCEL->excelName->Text.IsEmpty()){
      StreamGrid1->BuildXL(frmToEXCEL->excelName->Text);
    }

  }
  delete frmToEXCEL;

}
//---------------------------------------------------------------------------


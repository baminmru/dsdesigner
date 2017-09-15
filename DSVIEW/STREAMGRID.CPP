
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "StreamGrid.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

#define MAXCOL 254
#define MAXROW 16380


bool TStreamGrid::HintState=false;

// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TStreamGrid *)
{
	new TStreamGrid(NULL);
}


//---------------------------------------------------------------------------
__fastcall TStreamGrid::~TStreamGrid(){
};

//---------------------------------------------------------------------------
__fastcall TStreamGrid::TStreamGrid(TComponent* Owner)
	: TCustomGrid(Owner)
{
 s=0;
 sed=0;
 Snap=0;
 FixedRows=0;
 FixedCols=0;
 ColCount=0;
 RowCount=0;
 MenuForm=0;
 Manager=0;
 PrevDblClick=0;
 FindUp=false;
 FindAllColumns=true;
 ExclusiveSnapshot = false;
 DefaultRowHeight=18;
 Options.Clear();
 Options << goFixedVertLine << goFixedHorzLine << goVertLine << goHorzLine
 << goDrawFocusSelected <</* goRowSelect <<*/ goColSizing <<  goThumbTracking;
 ScrollBars = ssBoth	;
 PrevRow=-1;
 ShowHint = HintState;
 Hint="";
};

void __fastcall TStreamGrid::InvalidateStreamRow(int Row){
	long i;
	for(i=Row;i<RowMap.Count();i++){
		if(*RowMap[Row]==*RowMap[i])
			InvalidateRow(i);
		else
			break;
	}
	for(i=Row-1;i>=0;i--){
		if(*RowMap[Row]==*RowMap[i])
			InvalidateRow(i);
		else
			break;
	}

};

bool __fastcall TStreamGrid::SelectCell(int ACol, int ARow){
	if(ScrollFullRow){
		if(RowMap.Count()){
			long iii=ARow;
			long inc=1;
			if(Row != ARow){
            	if(abs(Row-ARow)>1){
        		   if(*RowMap[Row]==*RowMap[ARow])
                	 return true;
                }

				if(Row > ARow) inc=-1;

				while( iii< RowMap.Count() &&
                       iii>=0 &&
                       *RowMap[iii]>=0 &&
                	   *RowMap[Row]==*RowMap[iii] )
					iii+=inc;

                if(iii<0)
									return false;

				if(iii>= RowMap.Count()){
	                if ( ARow != RowCount-1) {
    	            	Row=RowCount-1;
						return false;
            	    }else
                    	return true;
                }

					if( *RowMap[iii]< 0){
                	if( ARow !=FixedRows){
    	             	Row=FixedRows;
						return false;
	                }else
                    	return true;
                }

    			if(iii!=ARow ){
					Row=iii;
					return false;
				}
			}
		}
	}

	if(RowMap.Count() && ARow < RowMap.Count() && Row < RowMap.Count()){
		if(*RowMap[Row]!=*RowMap[ARow]){
     Invalidate();
			//InvalidateStreamRow(Row);
			//InvalidateStreamRow(ARow);
		}
	}

	return TCustomGrid::SelectCell( ACol,  ARow);
};


void __fastcall TStreamGrid::DrawCell(int ACol, int ARow, const Windows::TRect &ARect, TGridDrawState AState){

/*
 if(PrevRow!=Row){
   InvalidateRow(Row);
   PrevRow=Row;
   if(ColCount)
    InvalidateCol(ColCount-1);
 }
*/


	if(!ScrollFullRow && AState.Contains(gdSelected)){
              if(IsSelected(ARow)){
        				Canvas->Brush->Color = clRed;
        				Canvas->Font->Color  = clBlack;                
              }else{
                Canvas->Brush->Color = (TColor) RGB(0,0,255);
         				Canvas->Font->Color = clWhite;
              }

	}else{
		if(AState.Contains(gdFixed)){
			Canvas->Brush->Color = FixedColor;
			Canvas->Font->Color = clBlack;
		}else{
		  if(RowMap.Count() && ARow < RowMap.Count()){
			if(*RowMap[Row]==*RowMap[ARow] && *RowMap[ARow]>=0 ){
            	if(!IsSelected(ARow)){
					Canvas->Brush->Color =(TColor) RGB(0,128,128);
					Canvas->Font->Color = clWhite;
                }else{
					Canvas->Brush->Color = (TColor)RGB(0,0,128);
					Canvas->Font->Color = clWhite;
                }
			}else{
              if(!IsSelected(ARow)){
        				Canvas->Brush->Color = clWhite;
				        Canvas->Font->Color = clBlack;
              }else{
                Canvas->Brush->Color = clYellow;
        				Canvas->Font->Color = clBlue;
              }

		   	}
		 }else{
		 	Canvas->Brush->Color = clWhite;
		 	Canvas->Font->Color = clBlack;
		 }
		}
	}
	Canvas->FillRect(ARect);

	 RECT R;
	 R.left   = ARect.Left;
	 R.top    = ARect.Top;
	 R.right  = ARect.Right - 2;
	 R.bottom = ARect.Bottom - 2;
	 AnsiString ss=CellValue(ARow,ACol);
	 DrawText(Canvas->Handle,ss.c_str(),
	 ss.Length(), &R, CellAlign(ARow,ACol));
};


AnsiString __fastcall  TStreamGrid::CellValue(long ARow, long ACol){
		if(Snap){
       	if(Refreshed != Snap->RefreshTime() || TotalCount!=Snap->TotalRowCount()){
         	Refresh();
        }
        //ShowHint=HintState;
   	   if(RC.Count()>ACol && RC[ACol]->Get(ARow) && RC[ACol]->Get(ARow)->ptr )
					return RC[ACol]->Get(ARow)->ptr->ToStr();
				else
					return "";
		}
		return "";
};


long __fastcall  TStreamGrid::CellAlign(long ARow, long ACol){
		if(Snap){
   	    if(RC.Count()>ACol && RC[ACol]->Get(ARow) && RC[ACol]->Get(ARow)->ptr ){
					  if( RC[ACol]->Get(ARow)->ptr->type==stNumeric || RC[ACol]->Get(ARow)->ptr->type==stDate)
             return DT_RIGHT | DT_VCENTER;
           else
            return DT_LEFT  | DT_VCENTER | DT_SINGLELINE;

				}else
					return DT_RIGHT | DT_VCENTER;
		}
		return DT_RIGHT | DT_VCENTER;
};


void __fastcall TStreamGrid::RunEditor(){

  if(s && Snap) s->KillSnapshot(Snap);
    Snap=0;
  ProcCursor cccc;

	if(!s) return;

  try
  {
		Snap=s->GetSnapshot(ExclusiveSnapshot);
	}catch(TODBCException *e){
		e->Show();
	  return;
   }

	if(!Parser)
		Parser = new StreamGridParser(this);

    if(!Parser)
        return;

	Groups = &(sed->Groups);

	if(Snap){
		ResolveRC();
		FixedCols=0;
	}else{
		FixedRows=0;
		FixedCols=0;
		ColCount=0;
		RowCount=0;
	}
    Refreshed=Snap->RefreshTime();
    TotalCount=Snap->TotalRowCount();

}


void __fastcall TStreamGrid::RunTransaction(AnsiString TranName){
 StreamTransaction * Tran;
 TranInfo *info;

 for(long i=0;i<Trans.Count();i++){
   if(Trans[i]->Tran->Caption==TranName){
     Tran=Trans[i]->Tran;
     info=Trans[i];
   }
 }
 // можно ли что - то делать
 if(!TestLink(Tran->TranCondition)) return;


 // выводим сообщение, если надо
 if(!Tran->Message.Trim().IsEmpty())
   if(MessageBox(0,Tran->Message.c_str(),TranName.c_str(),MB_YESNO)!=IDYES) return;

 Params * f;

 if(!Snap->RowCount() && Tran->TransactionSource==1)
   return ;

 if(!SelectedRowCount() && Tran->TransactionSource==2)
   return;

 if(!Snap->RowCount() && Tran->TransactionSource==3)
   return;

 if(Tran->TransactionSource==1 || (Tran->TransactionSource==3 && !SelectedRowCount()))
  f = GetState();
 else{
  f = new Params;
  MakeFieldsForInsert(f);
 }

 long cnt;
 bool Run;

TryChange:
 cnt=0;
 // добавляем все поля из формы редактирования
 if(Tran->FormDef.Count()){
   StackValue * ptr;
   for(long i=0;i<Tran->FormDef.Count();i++){

     // добавляем недостающие поля в кучку
     ptr=0;
     ptr=f->Get(Tran->FormDef[i]->FieldName);
     if(!ptr)
       f->Add(new StackValue(),Tran->FormDef[i]->FieldName);

     // считаем количество информационных полей доступных пользователю
     if(Tran->FormDef[i]->DefInsertAccess==faWrite) cnt++;
   }

   // действительно есть поля которые надо задавать
   if(cnt){
     // создаем форму
     if(!info->form){
       info->form = new TfrmEditForm(this);
     }
   }

 }


 // выводим форму ввода, если надо, заполняем, но ничего не делаем
 Run = true;
 if(cnt){
  Run = false;
  info->form->Caption=Tran->Caption;
  if(!info->form->SettedUp){
	  if(mrOk==info->form->Setup(
     Manager,
     &Tran->FormDef,
			0,//&LocalFormDef,
			s,
			true,
			f,
     ""
     )
		){
     Run = true;
   }

  }else{
  	if(mrOk==info->form->Run(
			true,
			f,
     "" )
		)
	  Run=true;
  }
 }

 if(Run){
  bool OK;
   // перебираем источники информции
  switch(Tran->TransactionSource){
   case 0: //New Row (Nothing)
     OK=s->MakeAction(Tran->Transaction,f);
     if(!OK){
           if(MessageBox(0,"Изменить параметры операции ?","Ошибка операции",MB_YESNO)==ID_YES)
            goto TryChange;
     }
   break;

   case 1: //Current Row
   asCurrent:
     OK=s->MakeAction(Tran->Transaction,f);
     if(!OK){
           if(MessageBox(0,"Изменить параметры операции ?","Ошибка операции",MB_YESNO)==ID_YES)
            goto TryChange;
     }

   break;

   case 2: //AllSelected Row
   asSel:
     {
      ProcCursor ccccc;
      for(long r=0;r<Snap->RowCount();r++){
        if(*SelectedRow[r]){

          // вписываем значения полей выбранной записи
          for(long fc=0;fc<f->Count();fc++){
            for(long c=0;c< s->fields.Count();c++){
              if(s->fields.Name(c)==f->Name(fc)){
                *(f->GetI(fc))=*(Snap->GetRow(r)->Get(c));
              }
            }
          }
          OK=s->MakeAction(Tran->Transaction,f);
          if(!OK){
           if(MessageBox(0,"Изменить параметры операции ?","Ошибка операции",MB_YESNO)==ID_YES)
            goto TryChange;
           if(MessageBox(0,"Прервать операцию ?","Ошибка операции",MB_YESNO)==ID_YES)
            goto bye;

          }else{
           *SelectedRow[r]=false;
          }
        }
      }

     }
   break;

   case 3: //Selected Or Current
        if(!SelectedRowCount())
           goto asCurrent;
        else
           goto asSel;

  }

  // что делать после изменения
  switch(Tran->AfterTransaction){
   case 0: // Nothing
   break;
   case 1: //Refresh Current Row
     RefreshRow();
   break;

   case 2: //Refresh All
     RefreshData();
     Refresh();
   break;

   case 3: //Delete Row
     RefreshData();
     Refresh();
   break;
  }

 }
 bye:

	delete f;

 ///////////////////
 Invalidate();


};

void __fastcall TStreamGrid::SetEditor(DataManager* _Manager,AnsiString Name){
	sed=0;
 if(s && Snap) s->KillSnapshot(Snap);
	Snap=0;
 if(Manager && s) Manager->KillStream(s);
 s=0;
 EditorName=Name;
 Manager=_Manager;
 sed=Manager->GetEditor(EditorName);

 Trans.Clear();

 for(long i=0;i<Manager->Trans.Count();i++){
   if(Manager->Trans[i]->EditorName==EditorName){
     Trans.Add(new TranInfo(Manager->Trans[i]));
   }
 }

	AnsiString *sc=0,*ob=0;
	if(!SubCondition.IsEmpty()) sc=&SubCondition;
	//if(!OrderBy.IsEmpty()) ob=&OrderBy;
	s= Manager->GetNewStream(sed,sc,0);

  if(EditForm())
    EditForm()->SettedUp = false;
	   LocalFormDef.Clear();
    if(sed){
      for(long i=0;i<sed->FormDef.Count();i++){
    	   LocalFormDef.Add(
        		new EditFieldDisables(
                		sed->FormDef[i]->FieldName,
                        sed->FormDef[i]->DefInsertAccess,
                        sed->FormDef[i]->DefEditAccess
               )
        );
      }
    }

	FixedRows=0;
	FixedCols=0;
	ColCount=0;
	RowCount=0;

    if(s && MenuForm){
	    MenuForm->mnuInsert->Enabled = s->AllowInsert();
    	MenuForm->mnuUpdate->Enabled = s->AllowUpdate();
		  MenuForm->mnuDelete->Enabled = s->AllowDelete();
     MenuForm->mnuIndex->Enabled = (sed->Indexes.Count()!=0);
    }else{
      if(MenuForm){
		     MenuForm->mnuInsert->Enabled = false;
    	   MenuForm->mnuUpdate->Enabled = false;
		     MenuForm->mnuDelete->Enabled = false;
         MenuForm->mnuDelete->Enabled = false;
      }
    }

};

void __fastcall   TStreamGrid::Refresh(){

	long PrevRow=Row;
	if(!sed)  return;
	if(!s) 	   return;
	if(!Snap) 	return;
	if(!Parser) return;
  ProcCursor cccccc;

	ResolveRC( true );

	if(RowCount>PrevRow)
			Row=PrevRow;

    Refreshed=Snap->RefreshTime();
    TotalCount=Snap->TotalRowCount();

	Invalidate();
  Hint="Всего строк: " +AnsiString(Snap->RowCount())+". Количество выделенных строк: "+AnsiString(SelectedRowCount());
  ShowHint = HintState;
};


//---------------------------------------------------------------------------
namespace Streamgrid
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TStreamGrid)};
		RegisterComponents("System", classes, 0);
	}
}

//---------------------------------------------------------------------------
void __fastcall  TStreamGrid::ResolveRC( bool ReFillOnly){
 ProcCursor ccc;

	RC.Clear();
	GroupResults.Clear();
	RowMap.Clear();
 SelectedRow.Clear();

	// отводим место под массив для расчета групп
	if(Groups->Count()){
		for(long i=0;i<Snap->RowCount();i++){
			DoubleBools *bbb=new DoubleBools;
			GroupResults.Add (bbb);
			for(long j=0;j<Groups->Count();j++)
				bbb->Add(new DoubleBool);
		}
	}

	// расчитываем группы
	MakeGroupResults();


	if(!sed){
  	return;
  }

	// анализируем количество колонок
	long cols=-1;
	long V=-1;
	long hdr=-1;

	for(long i=0;i<sed->ColDef.Count();i++){
		if(sed->ColDef.Get(i)->Visible){
			if(cols==-1 || cols<sed->ColDef.Get(i)->Horiz)
				cols=sed->ColDef.Get(i)->Horiz;

			if(V==-1 || V<sed->ColDef.Get(i)->Vert)
				V=sed->ColDef.Get(i)->Vert;

			if(hdr==-1 || hdr<sed->ColDef.Get(i)->HeaderVert)
				hdr=sed->ColDef.Get(i)->HeaderVert;
		}
	}

	if(cols>0)
		ColCount=cols+1;
	else{
		ColCount=0;
		return;
	}

	if(V<0 ){
		FixedRows=0;
		RowCount=0;
		return;
	}

	// отводим RC колонки
	for(long i=0;i<=cols;i++){
		RC.Add(new DynaStackValues());
		for(long j=0;j<=hdr;j++){
			if(!i) RowMap.Add(new long(-1));
			RC[i]->Add(0);
		}
	}

	// рассчитываем строки
	long curV=hdr;
	StackValue * sv=0;
	bool AllowDelete=false;

	DynaStackValues tmp(cols,false);

	for(long i=0;i<Snap->RowCount();i++){

        SelectedRow.Add(new bool(false));
		// считаем поля для конкретной секции
		tmp.Clear();
		Parser->SetCurrentRow(i);
		for(long j=0;j<sed->ColDef.Count();j++){
		 if(sed->ColDef.Get(j)->Visible){
			// вся проверка на условный вывод!!!
			if(!sed->ColDef.Get(j)->GroupName.IsEmpty()){

				long id;
				if(!i) sed->ColDef[j]->SetGroupID(Groups->Where(sed->ColDef[j]->GroupName.c_str()));
				id=sed->ColDef[j]->GetGroupID();
				if(id<0){
						AllowDelete=false;
						sv=0;
						goto nxtField;
				}
				if(sed->ColDef[j]->BeforChanged){
					if( GroupResults.Get(i)->GetBefor(id) == false){
						AllowDelete=false;
						sv=0;
						goto nxtField;
					}
				}else{
					 if( GroupResults.Get(i)->GetAfter(id) == false){
						AllowDelete=false;
						sv=0;
						goto nxtField;
					}
				}
			}


			if (!sed->ColDef[j]->Value.IsEmpty()){
				switch(sed->ColDef[j]->FieldKind){
					case StaticField:
						AllowDelete = true;
						sv= sed->ColDef[j]->InitStatic();
						break;
					 case FormulaField:
						 AllowDelete =true;
						 ((StreamGridParser*)Parser)->SetCurrentRow(i);
						 sv=new StackValue(sed->ColDef[j]->ExecExpression(Parser));
						 break;
					 case StreamField:
						AllowDelete=false;
						{
							long k;
							if(!i) sed->ColDef[j]->SetGroupID(s->fields.Where(sed->ColDef[j]->Value.c_str()));
							k=sed->ColDef[j]->GetGroupID();
							if(k==-1)
								sv=0;
							else
								sv= Snap->GetRow(i)->Get(k);
						}
						break;
					default:
						 AllowDelete=false;
						 sv= 0;
					}
				}else{
					AllowDelete=false;
					sv = 0;
				}
			nxtField:
			tmp.Add(new DynaStackValue(sv,AllowDelete));
		 }// end if Visible
		} // next j

		// цикл между найденными номерами чтобы исключить невидимые строки из нутри
		bool ttt;
		long kk;
		for(long l=0;l<=V;l++){
		 ttt=false;
		 kk=0;
		 for(long k=0;k<sed->ColDef.Count();k++){
			if(sed->ColDef.Get(k)->Visible){
				if(tmp[kk]->ptr && l==sed->ColDef.Get(k)->Vert ){
					if(!ttt){
						ttt=true;
						curV++;

						// отвели еще одну строку
						for(long m=0;m<=cols;m++)
								RC[m]->Add(0);
						RowMap.Add(new long(i));
					}
					RC[sed->ColDef.Get(k)->Horiz]->Set(RC[0]->Count()-1,tmp[kk]);

				}
				kk++;
			}
		 } // next k
		} // next l вертикальная секция
	} // next i (Row)

	if(!RC[0])
		RowCount=hdr+1;
	else
		RowCount=RC[0]->Count();

	if(RowCount<=hdr+1)
			RowCount++;
	FixedRows=hdr+1;

	{
		// заполняем заголовки колонок
		for(long i=0;i<=cols;i++){
			for(long j=0;j<sed->ColDef.Count();j++){
				if(sed->ColDef.Get(j)->Visible && !sed->ColDef.Get(j)->Caption.IsEmpty() && sed->ColDef.Get(j)->Horiz==i ){
					RC[i]->Set(sed->ColDef.Get(j)->HeaderVert,new DynaStackValue(new StackValue(sed->ColDef.Get(j)->Caption),true));
				}
				if(!ReFillOnly)
					if(sed->ColDef.Get(j)->Visible && sed->ColDef.Get(j)->Width && ColWidths[sed->ColDef.Get(j)->Horiz] < sed->ColDef.Get(j)->Width){
						ColWidths[sed->ColDef.Get(j)->Horiz] = sed->ColDef.Get(j)->Width;
					}
			}
		}
	}
};

//---------------------------------------------------------------------------
void __fastcall  TStreamGrid::MakeGroupResults(void){

	if(!Groups->Count() && s) return;
  ProcCursor pc;
		long SecRowID;
		DoubleBools *res=0,*res1=0;
		MyArray < MyArray<AnsiString> > rbuf;
		MyArray<AnsiString> *buf;

		// строим значения выражений для всех строк
		for(SecRowID=0;SecRowID<Snap->RowCount();SecRowID++){
			Parser->SetCurrentRow(SecRowID);
			rbuf.Add (buf = new MyArray <AnsiString> );

			for(long i=0;i<Groups->Count();i++){
					buf->Add(new AnsiString(Groups->GetI(i)->Evaluate(Parser)));
			}
		}

		// строим массив флажков для управления группами
		for(long i=0;i<Groups->Count();i++){
			for(SecRowID=0;SecRowID<Snap->RowCount();SecRowID++){
						res=GroupResults[SecRowID];
						if(!SecRowID){
							res->SetBefor(i,true);
						}

						if( Snap->RowCount()-1==SecRowID){
							res->SetAfter(i,true);
						}

						if( Snap->RowCount()-1==SecRowID)
							goto nxtGrp;
						res1=GroupResults[SecRowID+1];
						if(*(rbuf.Get(SecRowID)->Get(i))!=*(rbuf.Get(SecRowID+1)->Get(i))){
								res1->SetBefor(i,true);
								res->SetAfter(i,true);
						}
			}
		nxtGrp:
		}
};


void __fastcall TStreamGrid::MakeFieldsForInsert(Params *f){
	f->Clear();
	if(s )
		s->MakeInsertFields(f);

	for(int j=0;j<InsertDefaults.Count();j++){
		for (int i=0;i<f->Count();i++){
			 if(InsertDefaults.Name(j)==f->Name(i)){
           if(f->GetI(i) && InsertDefaults[j])
						*f->GetI(i)=*InsertDefaults[j];
						break;
			}
		 }
	}


};

void __fastcall TStreamGrid::InsertRow(Params *f){
	if(s && Snap ){
      ProcCursor cccccc;
			try{
			 s->Insert(Snap,f);
			 Refresh();
			}catch(TODBCException *e){
				e->Show();
			}
		}
};

bool __fastcall TStreamGrid::UpdateRow(){
	if(s && Snap && RowMap.Count()>Row && *RowMap[Row]>=0){
      ProcCursor cccccc;
			try{
			 s->Update(Snap,*RowMap[Row],ChangedWhileEdit());
			 Refresh();
			}catch(TODBCException *e){
				e->Show();
        return false;
			}
		}
    return true;
};

void __fastcall TStreamGrid::DeleteSRow(void){
	if(s && Snap && RowMap.Count()>Row && *RowMap[Row]>=0){
      ProcCursor cccccc;
			 try {
			 s->Delete(Snap,*RowMap[Row],ChangedWhileEdit());
			 Refresh();
			}catch(TODBCException *e){
				e->Show();
			}
		}
};


void __fastcall TStreamGrid::RefreshData(){  // только обновление Snap
  ProcCursor cccccc;
  try{
       Snap->OrderBy = OrderBy;
       //Snap->SubCondition=SubCondition;
			  s->RefreshSnapshot(Snap);
  }catch(TODBCException *e){
			  	e->Show();
				  return;
  }

};



void __fastcall TStreamGrid::Sort(SortDef *sort){
	if(!s || !Snap)
		return;
	Refresh();
};

void __fastcall TStreamGrid::Filter(FilterDef *filter){
	if(!s || !Snap)
		return;
	//Snap->SetFilter(filter,&s->fields);
	Refresh();
};


void __fastcall TStreamGrid::CreateWnd(){
			TCustomGrid::CreateWnd();
			try {
					MenuForm = new TSG_Garbage(this);
					MenuForm->StreamGrid1=this;
					PopupMenu=MenuForm->SGMenu;
				if(!OnDblClick)
					OnDblClick=MenuForm->StreamGrid1DblClick;

        OnKeyUp = SGKeyUp;

			}catch(...){
					MenuForm=0;
			}

			try {
					_EditForm = new TfrmEditForm(this);
			}catch(...){
					_EditForm=0;
			}
       if(s && MenuForm){
			    MenuForm->mnuInsert->Enabled = s->AllowInsert();
    			MenuForm->mnuUpdate->Enabled = s->AllowUpdate();
				  MenuForm->mnuDelete->Enabled = s->AllowDelete();
         if(sed)
            MenuForm->mnuIndex->Enabled = (sed->Indexes.Count()!=0);
		    }

};

void __fastcall TStreamGrid::DestroyWnd(){
			delete MenuForm;
			delete _EditForm;
			TCustomGrid::DestroyWnd();
};


void  __fastcall TStreamGrid::SelectRow(long RowID){
	if(RowID>0 && RowMap.Count()>RowID &&*RowMap[RowID]>=0){
		*SelectedRow[*RowMap[RowID]]=true;
        Invalidate();
    }
 Hint="Всего строк: " +AnsiString(Snap->RowCount())+". Количество выделенных строк: "+AnsiString(SelectedRowCount());
 ShowHint = HintState;

};

bool __fastcall TStreamGrid::IsSelected(long RowID){
	if(RowID>=0 && RowMap.Count()>RowID &&*RowMap[RowID]>=0){
		return *SelectedRow[*RowMap[RowID]];
    }
    return false;
};

void  __fastcall TStreamGrid::UnSelectRow(long RowID){
	if(RowID>0 && RowMap.Count()>RowID &&*RowMap[RowID]>=0 ){
		*SelectedRow[*RowMap[RowID]]=false;
        Invalidate();
    }
 Hint="Всего строк: " +AnsiString(Snap->RowCount())+". Количество выделенных строк: "+AnsiString(SelectedRowCount());
 ShowHint = HintState;

};

void  __fastcall TStreamGrid::SelectAllRow(){
	for(long i=0;i<SelectedRow.Count();i++){
		*SelectedRow[i]=true;
        Invalidate();
    }
 Hint="Всего строк: " +AnsiString(Snap->RowCount())+". Количество выделенных строк: "+AnsiString(SelectedRowCount());
 ShowHint = HintState;

};


void  __fastcall TStreamGrid::UnSelectAllRow(){
	for(long i=0;i<SelectedRow.Count();i++){
		*SelectedRow[i]=false;
        Invalidate();
    }
 Hint="Всего строк: " +AnsiString(Snap->RowCount())+". Количество выделенных строк: "+AnsiString(SelectedRowCount());
 ShowHint = HintState;

};

long __fastcall  TStreamGrid::SelectedRowCount(){
	long Res=0;
	for(long i=0;i<SelectedRow.Count();i++){
		if(*SelectedRow[i]) Res++;
    }
    return Res;
}

// получить текущее состояяние строки редактора
Params * __fastcall TStreamGrid::GetState(){
		Params * p;
		if(!s || !Snap) return 0;
		p=new Params;

		RowBuffer *rb =Snap->GetRow(GetStreamRow());

		if(rb)
			// запаковываем поля
			for(long i=0;i<Snap->FieldCount();i++){
				p->Add(new StackValue(rb->Get(i)),s->fields.Name(i));
			}

		for (long i=0;i<Snap->params.Count();i++)
		{
			p->Add( new StackValue(Snap->params[i]),Snap->params.Name(i));
		}

	return p;
};

Params * __fastcall TStreamGrid::GetState(long ROWID){
		Params * p;
		if(!s || !Snap) return 0;
		p=new Params;

		RowBuffer *rb =Snap->GetRow(*RowMap[ROWID]);

		if(rb)
			// запаковываем поля
			for(long i=0;i<Snap->FieldCount();i++){
				p->Add(new StackValue(rb->Get(i)),s->fields.Name(i));
			}

		for (long i=0;i<Snap->params.Count();i++)
		{
			p->Add( new StackValue(Snap->params[i]),Snap->params.Name(i));
		}

	return p;
};


// вернуть текущее состояние строки редактора
void  __fastcall TStreamGrid::SetState(Params *NewState){
		RowBuffer *rb =Snap->GetRow(GetStreamRow());

		if(rb)
		// восстанавливаем поля
			for(long i=0;i<Snap->FieldCount();i++){
				 (*(rb->Get(i)))=*(NewState->GetI(i));
		}

};

bool __fastcall TStreamGrid::Sync(AnsiString Field, StackValue *Value){	 // синхронизация
	if(!Snap) return false;
  ProcCursor cccccc;
	long FldID=-1,SnapRowID=-1;
	for(long i=0;i<Snap->FieldCount();i++){
				 if(Field==s->fields.Name(i)){
					FldID=i;
						break;
				 }
		}
		if(FldID==-1)
			return false;

		for(long i=0;i< Snap->RowCount();i++){
			RowBuffer *rb;
				rb=Snap->GetRow(i);

     //if( *(rb->Get(FldID))==*Value){
			if( (rb->Get(FldID))->Filter(fcLikeLeft, *Value, *Value)){
					SnapRowID=i;
						break;
				}
		}

		if(SnapRowID==-1)
			return false;

		for (long i=0; i<=RowMap.Count() ;i++)
		{
		if(*RowMap[i]==SnapRowID){
					Row=i;
         TopRow=i;
         LeftCol=0;
					return true;
				}
		}
		return false;
};

StackValue * __fastcall TStreamGrid::GetValue(AnsiString Field){		 // получение значения поля для текущей строки
	if(!Snap) 	return 0;

		long FldID=-1;
	for(long i=0;i<Snap->FieldCount();i++){
				 if(Field==s->fields.Name(i)){
					FldID=i;
						break;
				 }
		}
		if(FldID==-1)
			return 0;

		RowBuffer *rb=Snap->GetRow(GetStreamRow());
		return rb->Get(FldID);

};

void __fastcall TStreamGrid::InitSlave(EditFieldActions *Actions, Params *MasterRow){ // ининциализация редактора для работы в подчиненном режиме

		if(!Actions) return;

		// установка состояния параметров
		for(long i=0;i<Actions->Changes.Count();i++){
			EditFieldChanges *efc = Actions->Changes.Get(i);
				StackValue *sv,*sv1 ;
				if(efc->SetAsValue)
						sv=( new StackValue(efc->ValueOrName));
				else
					if(MasterRow){
            sv1=MasterRow->Get(efc->ValueOrName);
            if(sv1)
						  sv=(new StackValue(sv1));
            else
							sv= new StackValue();
					}else
							sv= new StackValue();

       if(s->params(efc->ParamName))
 				*(s->params(efc->ParamName))=*sv;
				delete sv;
		}

       LocalFormDef.Clear();
		// установка доступа к полям
		for(long i=0;i<sed->FormDef.Count();i++){
			LocalFormDef.Add(
					new EditFieldDisables(
						sed->FormDef[i]->FieldName,
						sed->FormDef[i]->DefInsertAccess,
						sed->FormDef[i]->DefEditAccess
					)
			 );
		}

    for(long i=0;i<Actions->Disables.Count();i++){
    	EditFieldDisables *efd=Actions->Disables[i];
        for(int j=0;j < LocalFormDef.Count();j++){
	      if( LocalFormDef[j]->FieldName==efd->FieldName){
           	LocalFormDef[j]->InsertAccess=efd->InsertAccess;
            LocalFormDef[j]->EditAccess=efd->EditAccess;
          }
        }
    }

	   s->SubCondition=Actions->SubCondition;

    // полная замена умолчаний
    InsertDefaults.Clear();

    for(long i=0;i<Actions->DefaultChanges.Count();i++){
    	EditFieldChanges *efc = Actions->DefaultChanges.Get(i);
        StackValue *sv,*sv1 ;
        if(efc->SetAsValue)
            sv=( new StackValue(efc->ValueOrName));
        else
					if(MasterRow){
            sv1=MasterRow->Get(efc->ValueOrName);
            if(sv1)
						  sv=(new StackValue(sv1));
            else
							sv= new StackValue();
					}else
							sv= new StackValue();

        if(s->params(efc->ParamName))
         *(s->params(efc->ParamName))=*sv;

        InsertDefaults.Add(
        	sv,
        	efc->ParamName
        );

    }
};


bool __fastcall TStreamGrid::ChangedWhileEdit(){
	return LockForEdit && ((TmpRefreshed!=Snap->RefreshTime()) || (TmpTotalCount!=Snap->TotalRowCount()));
};
void __fastcall TStreamGrid::SetEditMark(){
	LockForEdit=true;
    TmpRefreshed=Snap->RefreshTime();
    TmpTotalCount=Snap->TotalRowCount();
};
void __fastcall TStreamGrid::ClearEditMark(){
	LockForEdit=false;
    TmpRefreshed=Snap->RefreshTime();
    TmpTotalCount=Snap->TotalRowCount();
};

bool __fastcall TStreamGrid::Find(){
     FindAllColumns = true;
     if(! InputQuery("Поиск по вхождению", "Задайте строку для поиска", FindData)) return true;
     if(FindData=="") return true;
     if(!FindUp)
          return FindNext();
     else
          return FindPrev();
};

bool __fastcall TStreamGrid::FindOnColumn(){
     FindAllColumns = false;
     if(! InputQuery("Поиск по колонке", "Задайте строку для поиска", FindData)) return true;
     if(FindData=="")  return true;
     if(!FindUp)
          return FindNext();
     else
          return FindPrev();

};

bool __fastcall TStreamGrid::FindNext(){
     FindUp =false;
     if(FindData=="") return Find();
     StackValue Data;
     Data.Set(stString,FindData);

     for(long ARow=Row+1;ARow<RowCount;ARow++)
     if(FindAllColumns){
       for(long ACol=0;ACol<ColCount;ACol++)
   	    if(RC.Count()>ACol && RC[ACol]->Get(ARow) && RC[ACol]->Get(ARow)->ptr &&
					RC[ACol]->Get(ARow)->ptr->Filter(fcLikeCenter,Data,Data)){
             Row=ARow;
             Col=ACol;
             return  true;
         }
     }else{
   	    if(RC.Count()>Col && RC[Col]->Get(ARow) && RC[Col]->Get(ARow)->ptr &&
					RC[Col]->Get(ARow)->ptr->Filter(fcLikeCenter,Data,Data)){
             Row=ARow;
             return  true;
         }
     }
     //FindData="";
     return false;
}

bool __fastcall TStreamGrid::FindPrev(){
     FindUp =true;
     if(FindData=="") return Find();
     StackValue Data;
     Data.Set(stString,FindData);

     for(long ARow=Row-1;ARow>=FixedRows;ARow--)
     if(FindAllColumns){
       for(long ACol=0;ACol<ColCount;ACol++)
   	    if(RC.Count()>ACol && RC[ACol]->Get(ARow) && RC[ACol]->Get(ARow)->ptr &&
					RC[ACol]->Get(ARow)->ptr->Filter(fcLikeCenter,Data,Data)){
             Row=ARow;
             Col=ACol;
             return  true;
         }
     }else{
   	    if(RC.Count()>Col && RC[Col]->Get(ARow) && RC[Col]->Get(ARow)->ptr &&
					RC[Col]->Get(ARow)->ptr->Filter(fcLikeCenter,Data,Data)){
             Row=ARow;
             return  true;
         }
     }
     //FindData="";
     return  false;
}


void __fastcall TStreamGrid::SGKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{


        if(Key==VK_F1)
             MessageBox(0,
             "Insert - ввод\r\n"
             "Delete - удаление\r\n"
             "Пробел - пометить/снять пометку строки\r\n"
             "F1 - подсказка\r\n"
             "F2 - редактирование\r\n"
             "F3 - поиск\r\n"
             "F4 - поиск следующей\r\n"
             "F5 - обновить\r\n"
             "F6 - порядок вывода\r\n"
             "F7 - включить/выключить счетчики\r\n"
             "F8 - пометить строку\r\n"
             "F9 - пометить все строки\r\n"
             "Shift+F3 - поиск по колонке\r\n"
             "Shift+F4 - поиск предыдущей\r\n"
             "Shift+F5 - обновить данные\r\n"
             "Shift+F6 - смена режима двойного щелчка \r\n"
             "Shift+F8 - снять пометку строки\r\n"
             "Shift+F9 - снять все пометки",
             "Командные клавиши",MB_OK);



  if(Shift.Contains(ssShift)){
    switch(Key){
        case VK_F3:
             FindOnColumn();
        break;
        case VK_F4:
            MenuForm->FindPrevClick(this);
        break;
        case VK_F5:
             RefreshData();
             Refresh();
        break;
        case VK_F6:
            if(OnDblClick==FlipMark)
              OnDblClick=PrevDblClick;
            else{
              PrevDblClick=OnDblClick;
              OnDblClick=FlipMark;
            }

        case VK_F8:
             UnSelectRow(Row);
        break;
        case VK_F9:
             UnSelectAllRow();
        break;
    }
  }else{
      switch(Key){
        case VK_SPACE  :
            FlipMark(this);
        break;

        case VK_INSERT:
             if(s && s->AllowInsert()){
                  MenuForm->mnuInsertClick(this);
             }

        break;

        case VK_DELETE:
             if(s && s->AllowDelete()){
                  MenuForm->mnuDeleteClick(this);
             }
        break;

        case VK_F2:
             if(s && s->AllowUpdate()){
                  MenuForm->mnuUpdateClick(this);
             }
        break;
        case VK_F3:
             Find();
         break;
        case VK_F4:
                MenuForm->FindNextClick(this);
        break;
        case VK_F5:
             Refresh();

        break;

        case VK_F6:
              MenuForm->mnuIndexClick(this);
        break;

        case VK_F7:
              HintState=!HintState;
              Hint="Всего строк: " +AnsiString(Snap->RowCount())+". Количество выделенных строк: "+AnsiString(SelectedRowCount());
              ShowHint=HintState;
        break;

        case VK_F8:
             SelectRow(Row);
        break;
        case VK_F9:
             SelectAllRow();
        break;
    }

  }
}

bool   __fastcall TStreamGrid::TestLink(AnsiString Condition){
       if(Condition.Trim().IsEmpty()) return true;
       Parser->SetCurrentRow(GetStreamRow());
       AnsiString pe=Parser->ParseExpression(Condition);
       if(pe=="0")  return false;
       return true;
};


void __fastcall TStreamGrid::TopLeftChanged(void){
 TCustomGrid::TopLeftChanged();
 //Invalidate();
};

void __fastcall TStreamGrid::FlipMark(TObject * Obj){
  if(IsSelected(Row))
   UnSelectRow(Row);
  else
   SelectRow(Row);
};



AnsiString __fastcall TStreamGrid::BuildHTML(){
    AnsiString sss,sss1,sss2;
    HTMLProto ppp;
    if(RowCount>0){
     sss= sss+ ppp.H(sed->Caption,1);
     sss1="";
     long ARow=0;
     while(ARow<FixedRows){
      sss2="";
      for(long c=0;c<ColCount;c++)
       sss2=sss2+ppp.HCELL(CellValue(ARow,c));
      sss1 =sss1+ ppp.ROW(sss2);
      ARow++;
    }
    while(ARow<RowCount){
      sss2="";
      for(long c=0;c<ColCount;c++)
       sss2=sss2+ppp.CELL(CellValue(ARow,c));
      sss1 =sss1 +ppp.ROW(sss2);
      ARow++;

    }
    sss = sss + ppp.TABLE(sss1, " border=1 ");

    return ppp.HTML(sed->Caption,sss);
    }
    return ppp.HTML(sed->Caption,"");
};

void __fastcall TStreamGrid::BuildXL(AnsiString FileName){

  TCustomExcel *AdvExcel1 = new TCustomExcel(Application);
  try
    { AdvExcel1->Connect(); }
  catch (...) { }


  if (AdvExcel1->FConnected)
  {
    ProcCursor ccc;
    if (!AdvExcel1->FLoadExcel)
    AdvExcel1->Exec("[NEW(1)]");

    AdvExcel1->Exec("[DISABLE.INPUT(TRUE)]");
    long ARow=0;
    while(ARow<RowCount && ARow<MAXROW ){
       for(long c=0;c<ColCount && c < MAXCOL;c++){
       try
         {
          AnsiString ss= "[FORMULA(\"";
          AnsiString s;
          StackValue *ptr = RC[c]->Get(ARow)->ptr;
          if(ptr){
					  if( ptr->type==stNumeric || ptr->type==stDate)
               s=ptr->ToStr();
            else
               s="'" + ptr->ToStr();
          }else
            s="' ";
            ss=ss+s+ "\", \"R"+(ARow+1)+"C"+ (c+1) +"\")]";
            AdvExcel1->Exec(ss);
         } catch(...){}
         Application->ProcessMessages();
       }
       ARow++;
    }

    AnsiString Buf;
    if(FileName[1]=='\"')
      Buf = FileName.SubString(2,FileName.Length()-2);
    else
      Buf = FileName;
    unlink(Buf.c_str());
    AdvExcel1->Exec("[SAVE.AS(\""+Buf+"\")]");
    Application->ProcessMessages();
    AdvExcel1->Exec("[CLOSE( FALSE)]");
    Application->ProcessMessages();
    AdvExcel1->Exec("[DISABLE.INPUT( FALSE)]");
    Application->ProcessMessages();
    if (AdvExcel1->FLoadExcel)
      AdvExcel1->CloseExcel();
    MessageBox(0,FileName.c_str(),"Экспорт завершен",0);
    AdvExcel1->Disconnect();
  }
  delete AdvExcel1;
  AdvExcel1=0;

};


//////////////////////////////////////////////////////////////////
bool StreamGridParser::RunFunction (const AnsiString &Name,  MyArray<StackValue>* FArgs){
		if(Name=="ROW"){
			PushValue(new StackValue(CurrentRow ));
				return true;
		}
	if(Name=="ROWCOUNT"){
			PushValue(new StackValue(sg->GetSnap()->RowCount()));
				return true;
		};
	if(Name=="GETFIELD"){ // name,Row
		 for(long i=0;i< sg->GetStream()->fields.Count();i++){
		if(sg->GetStream()->fields.Name(i)==(*FArgs)[1]->ToStr()){

			PushValue(
						 (*sg->GetSnap()->GetRow((*FArgs)[2]->ToNumeric())->Get(i))
						);
			return true;
		}
		}
		};
	return false;
};

bool StreamGridParser::SetVariable (const AnsiString &Name, const StackValue &Value){

	for(long i=0;i< sg->GetSnap()->params.Count();i++){
		if(sg->GetSnap()->params.Name(i)==Name){
			delete sg->GetSnap()->params.Get(i);
			sg->GetSnap()->params.Set(i,new StackValue(Value));
			return true;
		}
	}
	return false;

};

bool StreamGridParser::GetVariable (const AnsiString &Name, StackValue &Storage){
	for(long i=0;i< sg->GetSnap()->params.Count();i++){
		if(sg->GetSnap()->params.Name(i)==Name){
			Storage=*sg->GetSnap()->params.Get(i);
			return true;
		}
	}

	 for(long i=0;i< sg->GetStream()->fields.Count();i++){
		if(sg->GetStream()->fields.Name(i)==Name){
				if(CurrentRow>=0 && CurrentRow < sg->GetSnap()->RowCount()){
							Storage=*(sg->GetSnap()->GetRow(CurrentRow)->Get(i));
				}
				return true;
		}
	}

	return false;
};

void StreamGridParser::PostInit(){
	AddUserFunction("ROW",0);
	AddUserFunction("ROWCOUNT",0);
	AddUserFunction("GETFIELD",2);
};




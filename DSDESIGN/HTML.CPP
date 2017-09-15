#include "HTML.h"

AnsiString Documentator::CTLNAME[13];

AnsiString Documentator::DescribeEditor(LMEAppDef & App,AnsiString EditorName,bool RefOnly){

  long i0=-1;
  for(i0=0;i0<App.Manager.Editors.Count();i0++){
    if(App.Manager.Editors[i0]->Caption==EditorName)
      break;
  }

  if(!RefOnly){

  AnsiString sss,sss1,s1,s2,s3;
  sss= H("Редактор: " + EditorName,1);
  sss= sss+ H("Общие данные",2);

  StreamEditorDef *d=App.Manager.GetEditor(EditorName);
  	//sss=sss+ P(d->Caption) ;
		sss=sss+ P("Соединение: ")+P(i(d->ConnectionName));
		sss=sss+ P("Запрос: ")+P(i(d->SQL));
		sss=sss+ P("Запрос строки: ")+P(i(d->ROWSQL));
		sss=sss+ P("Вставка: ")+P(i(d->INSERT));
		sss=sss+ P("Изменение: ")+P(i(d->UPDATE));
		sss=sss+ P("Удаление: ")+P(i(d->sDELETE));
   	sss=sss+ P("Дополнительное условие: ")+P(i(d->SubCondition));
		sss=sss+ P("Индекс по умолчанию: ")+P(i(d->OrderBy));

   if(d->ParamNames.Count())
     sss= sss+ H("Параметры",2);
   for(long i1=0;i1<d->ParamNames.Count();i1++){
        sss= sss+P(*(d->ParamNames[i1]));
   }

   if(d->Indexes.Count())
     sss= sss+ H("Индексы",2);
   for(long i1=0;i1<d->Indexes.Count();i1++){
        sss= sss+P(d->Indexes[i1]->Caption +": ")+ P(i(d->Indexes[i1]->OrderBy));
   }

   if(d->ColDef.Count())
     sss= sss+ H("Колонки просмотра",2);
      sss1 = ROW(
        HCELL("Заголовок") +
        HCELL("Поле") +
        HCELL("Расположение по горизонтали") +
        HCELL("Ширина") );



   for(long i1=0;i1<d->ColDef.Count();i1++){
      if(d->ColDef[i1]->Visible){
        sss1 = sss1 + ROW(
        CELL(d->ColDef[i1]->Caption)+
        CELL(i(d->ColDef[i1]->Value))+
        CELL(i(d->ColDef[i1]->Horiz))+
        CELL(i(d->ColDef[i1]->Width)));
      }
   }
  sss = sss + TABLE(sss1, " border=2 ");

   if(d->FormDef.Count() && (!d->UPDATE.IsEmpty() || !d->INSERT.IsEmpty()) ){
     sss= sss+ H("Поля формы ввода и редактирования",2);

    sss1 = ROW(
        HCELL("Заголовок") +
        HCELL("Поле") +
        HCELL("Способ представления") +
        HCELL("Допускает пустое значение") +
        HCELL("Доступ при вставке")+
        HCELL("Доступ при изменении")+
        HCELL("Редактор")+
        HCELL("Поле для синхронизации")+
        HCELL("Поле для отображения")
        );

    for(long i1=0;i1<d->FormDef.Count();i1++){
    if(d->FormDef[i1]->ControlType &&
      (d->FormDef[i1]->DefInsertAccess!=faNone ||
       d->FormDef[i1]->DefEditAccess!=faNone)
      ){
        switch(d->FormDef[i1]->DefInsertAccess){
      case 	faNone:
         s1=i("нет");
        break;
      case 	faRead:
         s1= i("просмотр");
        break;
      case	faWrite:
         s1= i("запись");
        break;
     }

     switch(d->FormDef[i1]->DefEditAccess){
      case 	faNone:
         s2= i("нет");
        break;
      case 	faRead:
         s2= ("просмотр");
        break;
      case	faWrite:
         s2=i("запись");
        break;
     }
     s3="";
     if(d->FormDef[i1]->ControlType==10){
     s3=CELL(DescribeEditor(App,d->FormDef[i1]->StreamName))+
      CELL(d->FormDef[i1]->SyncField)+
      CELL(d->FormDef[i1]->ShowField);
     }
     sss1 = sss1 + ROW(
      CELL(d->FormDef[i1]->Label)+
      CELL(d->FormDef[i1]->FieldName)+
      CELL(CTLNAME[d->FormDef[i1]->ControlType])+
      CELL(d->FormDef[i1]->AllowNull ? "Да":"Нет")+
      CELL(s1)+
      CELL(s2)+
      s3
      );

     /*
	EditFieldActions *WhenChanged;              // правило задания параметров подпотока при инициализации
	// актуально только для подпотока ???
	bool FillOnGetFocus;                        // инициализировать при получении фокуса
	bool FillOnFormShow;                        // инициализировать при показе формы
     */
   }
  }
  sss = sss + TABLE(sss1, " border=2 ");
  }
  // Операции
  sss=sss+DescribeTrans(App,EditorName);


  // Связи
  sss=sss+DescribeLinks(App,EditorName);



  TStrings  *ccc = new TStringList;
  ccc->Text=HTML(EditorName,sss);


  ccc->SaveToFile("HTML\\"+Path+FilePrephics+i0+"_SED.HTML");
  delete ccc;
  }
  return HREF(Path+FilePrephics+i0+"_SED.HTML",EditorName);

};
AnsiString Documentator::DescribeFilter(LMEAppDef & App,AnsiString FilterName,bool RefOnly){
  long i0=-1;
  for(i0=0;i0<App.Filters.Count();i0++){
    if(App.Filters[i0]->Caption==FilterName)
      break;
  }
  if(!RefOnly){
    AnsiString sss,sss1,s1,s3;
    sss=H("Фильтр: "+FilterName,1);
    if(!App.Filters[i0]->SubWherePrephics.IsEmpty())
    sss=sss+P("Префикс запроса")+P(i(App.Filters[i0]->SubWherePrephics));
    if(!App.Filters[i0]->SubWherePostphics.IsEmpty())
    sss=sss+P("Постфикс запроса")+P(i(App.Filters[i0]->SubWherePostphics));
    if(!App.Filters[i0]->Concatenator.IsEmpty())
    sss=sss+P("Обединяющая конструкция")+P(i(App.Filters[i0]->Concatenator));

    if(App.Filters[i0]->FormDef.Count()){
      sss=sss+H("Описание формы фильтра",2);
    sss1 = ROW(
        HCELL("Заголовок") +
        HCELL("Поле") +
        HCELL("Способ представления") +
        HCELL("Допускает пустое значение") +
        HCELL("Доступ при вставке")+
        HCELL("Условие на поле")+
        HCELL("Редактор")+
        HCELL("Поле для синхронизации")+
        HCELL("Поле для отображения")
        );
    FilterFormDef *d=App.Filters[i0];
    for(long i1=0;i1<d->FormDef.Count();i1++){
    if(d->FormDef[i1]->ControlType &&
      d->FormDef[i1]->DefInsertAccess!=faNone
      ){
        switch(d->FormDef[i1]->DefInsertAccess){
      case 	faNone:
         s1=i("нет");
        break;
      case 	faRead:
         s1= i("просмотр");
        break;
      case	faWrite:
         s1= i("запись");
        break;
     }

     s3="";
     if(d->FormDef[i1]->ControlType==10){
     s3=CELL(DescribeEditor(App,d->FormDef[i1]->StreamName))+
      CELL(d->FormDef[i1]->SyncField)+
      CELL(d->FormDef[i1]->ShowField);
     }
     sss1 = sss1 + ROW(
      CELL(d->FormDef[i1]->Label)+
      CELL(d->FormDef[i1]->FieldName)+
      CELL(CTLNAME[d->FormDef[i1]->ControlType])+
      CELL(d->FormDef[i1]->AllowNull ? "Да":"Нет")+
      CELL(s1)+
      CELL(d->FormDef[i1]->Condition)+
      s3
      );

   }
  }
  sss = sss + TABLE(sss1, " border=2 ");


    }

  /*
  EditFieldActions Changes;
  */


  TStrings  *ccc = new TStringList;
  ccc->Text=HTML(FilterName,sss);
  ccc->SaveToFile("HTML\\"+Path+FilePrephics+i0+"_FLT.HTML""");
  delete ccc;
  }

  return HREF(Path+FilePrephics+i0+"_FLT.HTML""",FilterName);
};

AnsiString Documentator::DescribeReport(LMEAppDef & App,AnsiString ReportName,bool RefOnly){

  long i0=-1;
  for(i0=0;i0<App.Reports.Count();i0++){
   if(App.Reports[i0]->Caption==ReportName)
      break;
  }

  if(!RefOnly){

  AnsiString sss,sss1;
  sss=H("Отчет: "+ReportName,1);
  sss=sss+P("Соединение")+P(i(App.Reports[i0]->ConnectionName));
  sss=sss+P("Файл отчета")+P(i(App.Reports[i0]->ReportFile.Path));
  if(!App.Reports[i0]->CaptionExpression.IsEmpty())
    sss=sss+P("Заголовок")+P(i(App.Reports[i0]->CaptionExpression));
  if(!App.Reports[i0]->SQL.IsEmpty())
    sss=sss+P("Запрос")+P(i(App.Reports[i0]->SQL));
  if(!App.Reports[i0]->BuildProc.IsEmpty())
  sss=sss+P("Процедура подготовки данных")+P(i(App.Reports[i0]->BuildProc));

  if(App.Reports[i0]->Formulas.Count()){
    sss=sss+H("Формулы",2);
    sss1 = ROW(
      HCELL("Название")+
      HCELL("Значение"));
    for(long i1=0;i1<App.Reports[i0]->Formulas.Count();i1++){
      sss1=sss1+ROW(
      CELL(App.Reports[i0]->Formulas[i1]->Name)+
      CELL(App.Reports[i0]->Formulas[i1]->Formula));
    }

    sss=sss+TABLE(sss1," border=2 ");
  }

  TStrings  *ccc = new TStringList;
  ccc->Text=HTML(ReportName,sss);
  ccc->SaveToFile("HTML\\"+Path+FilePrephics+i0+"_RPT.HTML");
  delete ccc;
  }
  return HREF(Path+FilePrephics+i0+"_RPT.HTML",ReportName);
};

void Documentator::MakeHTML( LMEAppDef & App){
  // создаем документ - описание программы
  AnsiString sss;
  AnsiString sss1;


  sss = H("Описание проекта ", 1);
  for (long i0=0;i0<App.Tasks.Count();i0++){

  Task *A= App.Tasks[i0];
  // Заголовок
  sss = sss + H("Описание задачи " + A->Name, 2);
  sss = sss + P("Заголовок : " + A->Caption);
  sss = sss + P("Версия задачи : " + A->Version);


  // Меню
  sss = sss + H("Структура меню", 3);
  for(long i1=0; i1<A->Menu.Count();i1++){
    MenuDef * m =A->Menu[i1];
    sss = sss + P(b(i(" " + m->Caption)));
    if ( m->Items.Count() != 0  ) {
      for(long i2=0; i2<m->Items.Count();i2++){
        MenuDef * sm = m->Items[i2];
        sss = sss + UL(LI(i(sm->Caption)));
        if(!sm->Filter.IsEmpty()) {
          sss = sss +P("Форма фильтрации:")+DescribeFilter(App,sm->Filter);
        }
        if(!sm->Editor.IsEmpty()) {
          sss = sss + P("Редактор:")+DescribeEditor(App,sm->Editor);
        }
        if(!sm->Report.IsEmpty() && sm->Editor.IsEmpty()) {
          sss = sss +P("Отчет:")+DescribeReport(App,sm->Report);
        }
      } /* end for  i2 */
    } else {
        if(!m->Filter.IsEmpty()) {
          sss = sss +P("Форма фильтрации:")+DescribeFilter(App,m->Filter);
        }
        if(!m->Editor.IsEmpty()) {
            sss = sss + P("Редактор:")+DescribeEditor(App,m->Editor);
        }
        if(!m->Report.IsEmpty() && m->Editor.IsEmpty()){
         sss = sss +P("Отчет:")+DescribeReport(App,m->Report);
        }
    }
  } /* end for i1 */
} /* end for i0 */

  sss = sss + H("Пообъектное описание ", 2);
  //Редакторы
  sss = sss + H("Редакторы", 3);
  for(long i1=0; i1<App.Manager.Editors.Count();i1++){
    sss = sss + P(DescribeEditor(App,App.Manager.Editors.Name(i1),false));
  }

  // Филтра
  sss = sss + H("Формы фильтрации", 3);
  for(long i1=0; i1<App.Filters.Count();i1++){
    sss = sss + P(DescribeFilter(App,(App.Filters[i1])->Caption,false));
  }

  // Отчеты
  sss = sss + H("Отчеты", 3);
  for(long i1=0; i1<App.Reports.Count();i1++){
    sss = sss +P(DescribeReport(App,(App.Reports[i1])->Caption,false));
  }


  TStrings  *ccc = new TStringList;
  ccc->Text=HTML("Описание приложения ", sss);
  ccc->SaveToFile("HTML\\"+Path+FilePrephics+"INDEX.HTML");
  delete ccc;
};


AnsiString Documentator::DescribeTrans(LMEAppDef & App,AnsiString EditorName){
  AnsiString sss,sss1,out,s1,s3;
  bool HasLinks = false;
  sss=H("Операции",2);
  StreamTransaction *d;

  for(int i0=0;i0<App.Manager.Trans.Count();i0++){
    if(App.Manager.Trans[i0]->EditorName==EditorName){
      HasLinks= true;
      sss =sss + H(App.Manager.Trans[i0]->Caption,3);

      if(!App.Manager.Trans[i0]->Message.IsEmpty())
        sss =sss + P("Сообщение")+P(i(App.Manager.Trans[i0]->Message));

      if(!App.Manager.Trans[i0]->TranCondition.IsEmpty())
        sss =sss + P("Условие")+P(i(App.Manager.Trans[i0]->TranCondition));

      sss =sss + P("Операция")+P(i(App.Manager.Trans[i0]->Transaction));
      sss =sss + P("Применяется для");
      switch(App.Manager.Trans[i0]->TransactionSource){
      case 0:
      sss =sss +P(i("новой строки"));
      break;
      case 1:
      sss =sss +P(i("текущей строки"));
      break;
      case 2:
      sss =sss +P(i("выделенных строк"));
      break;
      case 3:
      sss =sss +P(i("выделенных строк или текущей строки"));
      break;
      };
      sss =sss + P("После операции");

      switch(App.Manager.Trans[i0]->AfterTransaction){
      case 0:
      sss =sss +P(i("ничего не делать"));
      break;
      case 1:
      sss =sss +P(i("обновить текущую строку"));
      break;
      case 2:
      sss =sss +P(i("обновить все строки"));
      break;
      case 3:
      sss =sss +P(i("удалить текущую строку"));
      break;
      };
      d=App.Manager.Trans[i0];
      if(d->FormDef.Count()){
      sss =sss +  H("Описание формы для операции",4);
      sss1 = ROW(
        HCELL("Заголовок") +
        HCELL("Поле") +
        HCELL("Способ представления") +
        HCELL("Допускает пустое значение") +
        HCELL("Доступ")+
        HCELL("Редактор")+
        HCELL("Поле для синхронизации")+
        HCELL("Поле для отображения")
        );

    for(long i1=0;i1<d->FormDef.Count();i1++){
    if(d->FormDef[i1]->ControlType &&
      d->FormDef[i1]->DefInsertAccess!=faNone
        ){
        switch(d->FormDef[i1]->DefInsertAccess){
      case 	faNone:
         s1=i("нет");
        break;
      case 	faRead:
         s1= i("просмотр");
        break;
      case	faWrite:
         s1= i("запись");
        break;
     }

     s3="";
     if(d->FormDef[i1]->ControlType==10){
     s3=CELL(DescribeEditor(App,d->FormDef[i1]->StreamName))+
      CELL(d->FormDef[i1]->SyncField)+
      CELL(d->FormDef[i1]->ShowField);
     }
     sss1 = sss1 + ROW(
      CELL(d->FormDef[i1]->Label)+
      CELL(d->FormDef[i1]->FieldName)+
      CELL(CTLNAME[d->FormDef[i1]->ControlType])+
      CELL(d->FormDef[i1]->AllowNull ? "Да":"Нет")+
      CELL(s1)+
      s3
      );

     }
     }
     sss = sss + TABLE(sss1, " border=2 ");
     }
    }
  }

  if(HasLinks)
    return sss;
  else
    return "";

};


AnsiString Documentator::DescribeLinks(LMEAppDef & App,AnsiString EditorName){
/*
	EditFieldActions WhenChanged;               // правило задания параметров подпотока при инициализации
*/

AnsiString sss,sss1,out;
  bool HasLinks = false;
  sss=H("Связи редактора",2);
  sss1=ROW(
  HCELL("Название")+
  HCELL("Подчиненный редактор")+
  HCELL("Условие перехода")+
  HCELL("Заголовок подчиненного редактора"));

  for(int i0=0;i0<App.Manager.Links.Count();i0++){
    if(App.Manager.Links[i0]->MasterName==EditorName){
      HasLinks= true;
      sss1=sss1+ROW(
      CELL(App.Manager.Links[i0]->Caption)+
      CELL(DescribeEditor(App,App.Manager.Links[i0]->SlaveName))+
      CELL(App.Manager.Links[i0]->MasterCondition)+
      CELL(App.Manager.Links[i0]->SlaveCaption));
    }

  }
  sss=sss+TABLE(sss1," border=2 ");
  if(HasLinks)
    return sss;
  else
    return "";

};


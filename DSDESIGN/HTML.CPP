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
  sss= H("��������: " + EditorName,1);
  sss= sss+ H("����� ������",2);

  StreamEditorDef *d=App.Manager.GetEditor(EditorName);
  	//sss=sss+ P(d->Caption) ;
		sss=sss+ P("����������: ")+P(i(d->ConnectionName));
		sss=sss+ P("������: ")+P(i(d->SQL));
		sss=sss+ P("������ ������: ")+P(i(d->ROWSQL));
		sss=sss+ P("�������: ")+P(i(d->INSERT));
		sss=sss+ P("���������: ")+P(i(d->UPDATE));
		sss=sss+ P("��������: ")+P(i(d->sDELETE));
   	sss=sss+ P("�������������� �������: ")+P(i(d->SubCondition));
		sss=sss+ P("������ �� ���������: ")+P(i(d->OrderBy));

   if(d->ParamNames.Count())
     sss= sss+ H("���������",2);
   for(long i1=0;i1<d->ParamNames.Count();i1++){
        sss= sss+P(*(d->ParamNames[i1]));
   }

   if(d->Indexes.Count())
     sss= sss+ H("�������",2);
   for(long i1=0;i1<d->Indexes.Count();i1++){
        sss= sss+P(d->Indexes[i1]->Caption +": ")+ P(i(d->Indexes[i1]->OrderBy));
   }

   if(d->ColDef.Count())
     sss= sss+ H("������� ���������",2);
      sss1 = ROW(
        HCELL("���������") +
        HCELL("����") +
        HCELL("������������ �� �����������") +
        HCELL("������") );



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
     sss= sss+ H("���� ����� ����� � ��������������",2);

    sss1 = ROW(
        HCELL("���������") +
        HCELL("����") +
        HCELL("������ �������������") +
        HCELL("��������� ������ ��������") +
        HCELL("������ ��� �������")+
        HCELL("������ ��� ���������")+
        HCELL("��������")+
        HCELL("���� ��� �������������")+
        HCELL("���� ��� �����������")
        );

    for(long i1=0;i1<d->FormDef.Count();i1++){
    if(d->FormDef[i1]->ControlType &&
      (d->FormDef[i1]->DefInsertAccess!=faNone ||
       d->FormDef[i1]->DefEditAccess!=faNone)
      ){
        switch(d->FormDef[i1]->DefInsertAccess){
      case 	faNone:
         s1=i("���");
        break;
      case 	faRead:
         s1= i("��������");
        break;
      case	faWrite:
         s1= i("������");
        break;
     }

     switch(d->FormDef[i1]->DefEditAccess){
      case 	faNone:
         s2= i("���");
        break;
      case 	faRead:
         s2= ("��������");
        break;
      case	faWrite:
         s2=i("������");
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
      CELL(d->FormDef[i1]->AllowNull ? "��":"���")+
      CELL(s1)+
      CELL(s2)+
      s3
      );

     /*
	EditFieldActions *WhenChanged;              // ������� ������� ���������� ��������� ��� �������������
	// ��������� ������ ��� ��������� ???
	bool FillOnGetFocus;                        // ���������������� ��� ��������� ������
	bool FillOnFormShow;                        // ���������������� ��� ������ �����
     */
   }
  }
  sss = sss + TABLE(sss1, " border=2 ");
  }
  // ��������
  sss=sss+DescribeTrans(App,EditorName);


  // �����
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
    sss=H("������: "+FilterName,1);
    if(!App.Filters[i0]->SubWherePrephics.IsEmpty())
    sss=sss+P("������� �������")+P(i(App.Filters[i0]->SubWherePrephics));
    if(!App.Filters[i0]->SubWherePostphics.IsEmpty())
    sss=sss+P("�������� �������")+P(i(App.Filters[i0]->SubWherePostphics));
    if(!App.Filters[i0]->Concatenator.IsEmpty())
    sss=sss+P("����������� �����������")+P(i(App.Filters[i0]->Concatenator));

    if(App.Filters[i0]->FormDef.Count()){
      sss=sss+H("�������� ����� �������",2);
    sss1 = ROW(
        HCELL("���������") +
        HCELL("����") +
        HCELL("������ �������������") +
        HCELL("��������� ������ ��������") +
        HCELL("������ ��� �������")+
        HCELL("������� �� ����")+
        HCELL("��������")+
        HCELL("���� ��� �������������")+
        HCELL("���� ��� �����������")
        );
    FilterFormDef *d=App.Filters[i0];
    for(long i1=0;i1<d->FormDef.Count();i1++){
    if(d->FormDef[i1]->ControlType &&
      d->FormDef[i1]->DefInsertAccess!=faNone
      ){
        switch(d->FormDef[i1]->DefInsertAccess){
      case 	faNone:
         s1=i("���");
        break;
      case 	faRead:
         s1= i("��������");
        break;
      case	faWrite:
         s1= i("������");
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
      CELL(d->FormDef[i1]->AllowNull ? "��":"���")+
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
  sss=H("�����: "+ReportName,1);
  sss=sss+P("����������")+P(i(App.Reports[i0]->ConnectionName));
  sss=sss+P("���� ������")+P(i(App.Reports[i0]->ReportFile.Path));
  if(!App.Reports[i0]->CaptionExpression.IsEmpty())
    sss=sss+P("���������")+P(i(App.Reports[i0]->CaptionExpression));
  if(!App.Reports[i0]->SQL.IsEmpty())
    sss=sss+P("������")+P(i(App.Reports[i0]->SQL));
  if(!App.Reports[i0]->BuildProc.IsEmpty())
  sss=sss+P("��������� ���������� ������")+P(i(App.Reports[i0]->BuildProc));

  if(App.Reports[i0]->Formulas.Count()){
    sss=sss+H("�������",2);
    sss1 = ROW(
      HCELL("��������")+
      HCELL("��������"));
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
  // ������� �������� - �������� ���������
  AnsiString sss;
  AnsiString sss1;


  sss = H("�������� ������� ", 1);
  for (long i0=0;i0<App.Tasks.Count();i0++){

  Task *A= App.Tasks[i0];
  // ���������
  sss = sss + H("�������� ������ " + A->Name, 2);
  sss = sss + P("��������� : " + A->Caption);
  sss = sss + P("������ ������ : " + A->Version);


  // ����
  sss = sss + H("��������� ����", 3);
  for(long i1=0; i1<A->Menu.Count();i1++){
    MenuDef * m =A->Menu[i1];
    sss = sss + P(b(i(" " + m->Caption)));
    if ( m->Items.Count() != 0  ) {
      for(long i2=0; i2<m->Items.Count();i2++){
        MenuDef * sm = m->Items[i2];
        sss = sss + UL(LI(i(sm->Caption)));
        if(!sm->Filter.IsEmpty()) {
          sss = sss +P("����� ����������:")+DescribeFilter(App,sm->Filter);
        }
        if(!sm->Editor.IsEmpty()) {
          sss = sss + P("��������:")+DescribeEditor(App,sm->Editor);
        }
        if(!sm->Report.IsEmpty() && sm->Editor.IsEmpty()) {
          sss = sss +P("�����:")+DescribeReport(App,sm->Report);
        }
      } /* end for  i2 */
    } else {
        if(!m->Filter.IsEmpty()) {
          sss = sss +P("����� ����������:")+DescribeFilter(App,m->Filter);
        }
        if(!m->Editor.IsEmpty()) {
            sss = sss + P("��������:")+DescribeEditor(App,m->Editor);
        }
        if(!m->Report.IsEmpty() && m->Editor.IsEmpty()){
         sss = sss +P("�����:")+DescribeReport(App,m->Report);
        }
    }
  } /* end for i1 */
} /* end for i0 */

  sss = sss + H("����������� �������� ", 2);
  //���������
  sss = sss + H("���������", 3);
  for(long i1=0; i1<App.Manager.Editors.Count();i1++){
    sss = sss + P(DescribeEditor(App,App.Manager.Editors.Name(i1),false));
  }

  // ������
  sss = sss + H("����� ����������", 3);
  for(long i1=0; i1<App.Filters.Count();i1++){
    sss = sss + P(DescribeFilter(App,(App.Filters[i1])->Caption,false));
  }

  // ������
  sss = sss + H("������", 3);
  for(long i1=0; i1<App.Reports.Count();i1++){
    sss = sss +P(DescribeReport(App,(App.Reports[i1])->Caption,false));
  }


  TStrings  *ccc = new TStringList;
  ccc->Text=HTML("�������� ���������� ", sss);
  ccc->SaveToFile("HTML\\"+Path+FilePrephics+"INDEX.HTML");
  delete ccc;
};


AnsiString Documentator::DescribeTrans(LMEAppDef & App,AnsiString EditorName){
  AnsiString sss,sss1,out,s1,s3;
  bool HasLinks = false;
  sss=H("��������",2);
  StreamTransaction *d;

  for(int i0=0;i0<App.Manager.Trans.Count();i0++){
    if(App.Manager.Trans[i0]->EditorName==EditorName){
      HasLinks= true;
      sss =sss + H(App.Manager.Trans[i0]->Caption,3);

      if(!App.Manager.Trans[i0]->Message.IsEmpty())
        sss =sss + P("���������")+P(i(App.Manager.Trans[i0]->Message));

      if(!App.Manager.Trans[i0]->TranCondition.IsEmpty())
        sss =sss + P("�������")+P(i(App.Manager.Trans[i0]->TranCondition));

      sss =sss + P("��������")+P(i(App.Manager.Trans[i0]->Transaction));
      sss =sss + P("����������� ���");
      switch(App.Manager.Trans[i0]->TransactionSource){
      case 0:
      sss =sss +P(i("����� ������"));
      break;
      case 1:
      sss =sss +P(i("������� ������"));
      break;
      case 2:
      sss =sss +P(i("���������� �����"));
      break;
      case 3:
      sss =sss +P(i("���������� ����� ��� ������� ������"));
      break;
      };
      sss =sss + P("����� ��������");

      switch(App.Manager.Trans[i0]->AfterTransaction){
      case 0:
      sss =sss +P(i("������ �� ������"));
      break;
      case 1:
      sss =sss +P(i("�������� ������� ������"));
      break;
      case 2:
      sss =sss +P(i("�������� ��� ������"));
      break;
      case 3:
      sss =sss +P(i("������� ������� ������"));
      break;
      };
      d=App.Manager.Trans[i0];
      if(d->FormDef.Count()){
      sss =sss +  H("�������� ����� ��� ��������",4);
      sss1 = ROW(
        HCELL("���������") +
        HCELL("����") +
        HCELL("������ �������������") +
        HCELL("��������� ������ ��������") +
        HCELL("������")+
        HCELL("��������")+
        HCELL("���� ��� �������������")+
        HCELL("���� ��� �����������")
        );

    for(long i1=0;i1<d->FormDef.Count();i1++){
    if(d->FormDef[i1]->ControlType &&
      d->FormDef[i1]->DefInsertAccess!=faNone
        ){
        switch(d->FormDef[i1]->DefInsertAccess){
      case 	faNone:
         s1=i("���");
        break;
      case 	faRead:
         s1= i("��������");
        break;
      case	faWrite:
         s1= i("������");
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
      CELL(d->FormDef[i1]->AllowNull ? "��":"���")+
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
	EditFieldActions WhenChanged;               // ������� ������� ���������� ��������� ��� �������������
*/

AnsiString sss,sss1,out;
  bool HasLinks = false;
  sss=H("����� ���������",2);
  sss1=ROW(
  HCELL("��������")+
  HCELL("����������� ��������")+
  HCELL("������� ��������")+
  HCELL("��������� ������������ ���������"));

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


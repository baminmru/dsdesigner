//---------------------------------------------------------------------------
#ifndef DW_ORC
#define DW_ORC
#pragma hdrstop
//---------------------------------------------------------------------------
#include "bamilib.h"
#include "New_dw.h"
#pragma package(smart_init)
//OutRectContainer
////////////////////////////////////////////////////////////////////////////////
StackValue* OutRectContainer::InitField(int RowID, MiniParser *Parser, bool &AllowDelete)
{
 return 0;
};
OutRectContainer::OutRectContainer(OutRect * _parent ):OutRect(StaticField,NULL,_parent)
{
 FieldKind=StaticField;
 SetupString("",FieldName);
 Visible=true;
 Rects.Add( this );
 GrowVertical=true;
 MultiRow=false;
 Style=&DefOutRectContainerStyle;
 Group=0;
};
OutRectContainer *OutRectContainer::Clone()
{
 return new OutRectContainer(*this);
};
OutRectContainer::~OutRectContainer()
{
 if(Rects[0]) Rects.Set(0,NULL);
};
StackValue *OutRectContainer::FieldData(int SectionRowID, char *Name)
{
 if (parent) return ((DataWindow*)parent)->FieldData(SectionRowID, Name);
 else return NULL;
};
void OutRectContainer::Paint(int RowID,HDC dc,TNRect *Client, TNRect *Portion)
{
 int RowCount=1;
 if (parent && ((DataWindow*)parent)->GetStream() && MultiRow) RowCount=((DataWindow*)parent)->SectionRowCount(RowID);
 for(int i=0;i<=RowCount;i++) for(int j=0;j<Rects.Count();j++)
 {
  if(Rects[j]->IsResolved(i) && Rects[j]->GetRect(i).Touches(*Portion))
  {
   if(j)
   {
    Rects[j]->Paint(i,dc,Client,Portion);
   }
   else
   {
    // чтобы не зацикливаось !!!
    OutRect *tmp=new OutRect(*(Rects[0]),this);
    tmp->Paint(i,dc,Client,Portion);
    delete tmp;
   }
  }
 }
};
void OutRectContainer::Resolve(int rID,HDC dc,OutRectContainer * p, DataStream * is)
{
 //MDebug("befor OutRectContainer::Resolve" );
 int RC,prevRC;
 int RowCount;
 int curW,curH;
 DataStream *curStream;
 Map(0).W=false;
 Map(0).H=false;
 // Вычисляем LT Секции в целом
 if (!ResolvedLT(0)) OutRect::Resolve(0,dc,p,is);
 if (!ResolvedLT(0)) return;
 // Выясняем сколько строк в этой секции
 if (parent && ((DataWindow*)parent)->GetStream()) curStream =((DataWindow*)parent)->GetStream();
 else curStream=0;
 if (curStream && MultiRow) RowCount=((DataWindow*)parent)->SectionRowCount(rID);
 else RowCount =1;
 for(int i=0; i<Rects.Count();i++)
 {
  // места под прямоугольнички
  Rects[i]->SetRowCount(RowCount);
 }
 // Для каждой из строк
 for(int RowID=1;RowID<=RowCount;RowID++)
 {
  for(int i=0; i<Rects.Count();i++) Rects[i]->UnResolve(RowID);
  //инициализируем LT для нулевого прямоугольника
  if(RowID==1)
  {
   Rects[0]->GetRect(RowID).Left=GetLeft(0);
   Rects[0]->GetRect(RowID).Top =GetTop(0);
  }
  else
  {
   if(GrowVertical)
   {
    Rects[0]->GetRect(RowID).Left=GetLeft(0);
    Rects[0]->GetRect(RowID).Top =Rects[0]->GetRect(RowID-1).Bottom+5;
   }
   else
   {
    Rects[0]->GetRect(RowID).Left=Rects[0]->GetRect(RowID-1).Right+5;
    Rects[0]->GetRect(RowID).Top =GetTop(0);
   }
  }
  //Rects[0]->Map(rID).LT=true;
  Rects[0]->Map(RowID).LT=true;
  //if(curStream && MultiRow)
  //  	curStream->GetRow(RowID);
  // Пытаемся разрешить
  prevRC=-1;
  RC=0;
  while(RC!=prevRC)
  {
   prevRC=RC;
   RC=0;
   for(int i=1; i<Rects.Count();i++)
   {
    if (!Rects[i]->IsResolved(RowID))
    {
     Rects[i]->Resolve(RowID,dc,this,curStream);
    }
    RC+=Rects[i]->ResolverCount(RowID);
   }
  }
  curW=0;
  curH=0;
  //пересчитываем ширину и длину для нулевого прямоугольника
  for(int i=1; i<Rects.Count();i++)
  {
   if (Rects[i]->IsResolved(RowID) && Rects[i]->Visible)
   {
    // исключаем пустышки из рассчета размера секции
    if(Rects[i]->GetRect(RowID).Width() && Rects[i]->GetRect(RowID).Height() )
    {
     if(curW<Rects[i]->GetRight(RowID))	curW=Rects[i]->GetRight(RowID);
     if(curH<Rects[i]->GetBottom(RowID)) curH=Rects[i]->GetBottom(RowID);
    }
   }
  }
  Rects[0]->GetRect(RowID).Right=curW+3;
  Rects[0]->GetRect(RowID).Bottom=curH+3;
  Rects[0]->Map(RowID).W=true;
  Rects[0]->Map(RowID).H=true;
 }
 curW=0;
 curH=0;
 for(int RowID=1; RowID<=RowCount; RowID++)
 {
  if(curW<Rects[0]->GetRight(RowID)) curW =Rects[0]->GetRight(RowID);
  if(curH <Rects[0]->GetBottom(RowID)) curH =Rects[0]->GetBottom(RowID);
 }
 Map(0).Rect.Bottom=curH;
 Map(0).Rect.Right=curW;
 Map(0).Rect.Left=Rects[0]->GetRect(0).Left;
 Map(0).Rect.Top=Rects[0]->GetRect(0).Top;
 Map(0).LT=true;
 Map(0).W=true;
 Map(0).H=true;
};
OutRectContainer::OutRectContainer(OutRectContainer &d):OutRect(d)
{
 Rects.Add(this);
 for(int i=1;i<d.Rects.Count();i++)
 {
  Rects.Add(d.Rects.Get(i)->Clone());
 }
 GrowVertical=d.GrowVertical;
 MultiRow=d.MultiRow;
};
//DataWindow
////////////////////////////////////////////////////////////////////////////////
void DataWindow::MakeGroupResults(int RowID)
{
 if(!Groups.Count() && s) return;
 int SecRowID;
 MyArray<bool> *res=0, *res1=0;
 MyArray < MyArray<AnsiString> > rbuf;
 MyArray<AnsiString> *buf;
 // строим значения выражений для всех строк
 for(SecRowID=0;SecRowID<SectionRowCount(RowID);SecRowID++)
 {
  Parser->SetCurrentRow(SecRowID);
  Parser->SetCurrentRect(0);
  rbuf.Add (buf = new MyArray <AnsiString>);
  for(int i=0;i<Groups.Count();i++)
  {
   buf->Add(new AnsiString(Groups[i]->Evaluate(Parser)));
  }
 }
 Debug("\nGrouping Map");
 for(int i=0;i<Groups.Count();i++)
 {
  bool befor =Groups[i]->IsBefor();
  for(SecRowID=0;SecRowID<SectionRowCount(RowID);SecRowID++)
  {
   res=(*GroupResults[RowID])[SecRowID];
   Debug("\nGroupValue for row %d is '%s'",SecRowID,(rbuf.Get(SecRowID)->Get(i))->c_str());
   if(!SecRowID && befor)
   {
    *res->Get(i)=true;
    Debug("\nGrouping for group %d  at row %d",i,SecRowID);
   }
   if( SectionRowCount(RowID)-1==SecRowID)
   {
    if(!befor)
    {
     *res->Get(i)=true;
     Debug("\nGrouping for group %d  at row %d",i,SecRowID);
    }
    goto nxtGrp;
   }
   res1=(*GroupResults[RowID])[SecRowID+1];
   if(*(rbuf.Get(SecRowID)->Get(i))!=*(rbuf.Get(SecRowID+1)->Get(i)))
   {
    if(befor)
    {
     *res1->Get(i)=true;
     Debug("\nGrouping for group %d  at row %d",i,SecRowID+1);
    }
    else
    {
     *res->Get(i)=true;
     Debug("\nGrouping for group %d  at row %d",i,SecRowID);
    }
    //}else{
    //	delete res->Get(i);
    //	res->Set(i,new bool(false));
   }
  }
  nxtGrp:
 }
};
DataStream *DataWindow::GetStream(){return s;};
DataSnapshot* DataWindow::GetSnapshot(int RowID)
{
 return (*RowInfo[RowID]);
};
StackValue *DataWindow::FieldData(int SectionRowID, char *Name)
{
 return FieldData(CurrentRow,SectionRowID,Name);
};
void DataWindow::SetRowCount(int RowCount)
{
 //	Debug("RowCount for DataWindow is %ld\n",RowCount);
 for(int i=State.Count();i<=RowCount;i++)
 {
  State.Add(new OutRectState);
  Title.Add(new OutRectContainer) ;
  Detail.Add(new OutRectContainer);
  Summary.Add(new OutRectContainer);
  RowParam.Add(new Params);
  RowInfo.Add(new DataSnapshot*);
  //NeedRefresh.Add(new bool(true));
  // готовим массив результатов
  GroupResults.Add( new MyArray< MyArray< bool > >);
 }
};
DataWindow::~DataWindow()
{
 Title.Clear();
 Detail.Clear();
 Summary.Clear();
 RowParam.Clear();
 RowInfo.Clear();
 TitleDef.Rects.Clear();
 DetailDef.Rects.Clear();
 SummaryDef.Rects.Clear();
 ParamDef.Clear();
 Groups.Clear();
 GroupResults.Clear();
 if(Parser && DeleteParser) delete Parser;
 if(s && DeleteStream) delete s;
};
/*
void DataWindow::Clear()
{
 Title.Clear();
 Detail.Clear();
 Summary.Clear();
 //RowParam.Clear();
 //RowInfo.Clear();
 //TitleDef.Rects.Clear();
 //DetailDef.Rects.Clear();
 //SummaryDef.Rects.Clear();
 //ParamDef.Clear();
 //Groups.Clear();
 //GroupResults.Clear();
 for(int i=0;i<NeedRefresh.Count();i++)
 {
  *NeedRefresh[i]=true;
  UnResolve(i);
 }
};
*/
// RowID - Строка в потоке, которому принадлежит документ
// ps - поток, которому принадлежит документ
void DataWindow::Prepare(int RowID,DataStream  *ps)
{
 //MDebug(" Prepare DataWindow");
 // переписать из Def секций полезную информацию
 if (Title[RowID])
 {
  Title[RowID]->parent=this;
  for(int i=1;i<TitleDef.Rects.Count();i++)
  {
   Title[RowID]->Rects.Add(TitleDef.Rects[i]->Clone());
   Title[RowID]->Rects[i]->SetParent(Title[RowID]);
  }
 }
 if (Detail[RowID])
 {
  Detail[RowID]->parent=this;
  Detail[RowID]->MultiRow = true;
  Detail[RowID]->GrowVertical =DetailDef.GrowVertical ;
  for(int i=1;i<DetailDef.Rects.Count();i++)
  {
   Detail[RowID]->Rects.Add(DetailDef.Rects.Get(i)->Clone());
   Detail[RowID]->Rects[i]->SetParent(Detail[RowID]);
  }
 }
 if (Summary[RowID])
 {
  Summary[RowID]->parent=this;
  for(int i=1;i<SummaryDef.Rects.Count();i++)
  {
   Summary[RowID]->Rects.Add(SummaryDef.Rects[i]->Clone());
   Summary[RowID]->Rects[i]->SetParent(Summary[RowID]);
  }
 }
};
void DataWindow::Resolve(int RowID,HDC dc,OutRectContainer * p,DataStream  *is)
{
 CurrentRow = RowID;
 SetRowCount(RowID);
 if(!Parser)
 {
  Parser=new DataWindowParser(this);
  DeleteParser=true;
 }
 /*if(NeedRefresh.Count()>RowID && *NeedRefresh[RowID]){*/
 // создать Snapshot
 if(s && RowInfo[RowID])
 {
  *RowInfo[RowID]=s->GetSnapshot(true);
  for(int i=0;i<SectionRowCount(RowID);i++)
  {
   MyArray<bool> *ptr;
   GroupResults[RowID]->Add(ptr =new MyArray<bool>(5,true));
   for(int j=0;j<Groups.Count();j++)
   {
    ptr->Add(new bool(false));
   }
  }
 }
 Prepare(RowID,is);
 MakeGroupResults(RowID);
 /*}else
 {
  Prepare(RowID,is);
 }*/
 // Вычисления LT !
 if (!ResolvedLT(RowID)) OutRect::Resolve(RowID,dc,p,is);
 if (!ResolvedLT(RowID)) return;
 TSize section;
 //Debug("\nResolve Title");
 section.cx = GetLeft(RowID);
 section.cy = GetTop(RowID);
 Title[RowID]->SetRowCount(0);
 Title[RowID]->SetLT(0,section);
 Title[RowID]->Resolve(RowID,dc,NULL,is);
 //Debug("\nResolve Detail");
 section.cy=Title[RowID]->GetBottom(0)+3;
 Detail[RowID]->SetRowCount(0);
 Detail[RowID]->SetLT(0,section);
 Detail[RowID]->Resolve(RowID,dc,NULL,is);
 //Debug("\nResolve Summary");
 section.cy=Detail[RowID]->GetBottom(0)+3;
 Summary[RowID]->SetRowCount(0);
 Summary[RowID]->SetLT(0,section);
 Summary[RowID]->Resolve(RowID,dc,NULL,is);
 GetRect(RowID).Bottom=Summary[RowID]->GetBottom(0);
 GetRect(RowID).Right=Title[RowID]->GetRight(0);
 if (GetRight(RowID)<Detail[RowID]->GetRight(0)) GetRect(RowID).Right=Detail[RowID]->GetRight(0);
 if (GetRight(RowID)<Summary[RowID]->GetRight(0)) GetRect(RowID).Right=Summary[RowID]->GetRight(0);
 Map(RowID).W=true;
 Map(RowID).H=true;
 //*NeedRefresh[RowID] = false;
};
void DataWindow::Paint(int RowID,HDC dc,TNRect *Client,TNRect *Portion)
{
 /*if(NeedRefresh.Count()>RowID && *NeedRefresh[RowID]) return;*/
 CurrentRow = RowID;
 if(Group && !State[RowID]->Info) return;
 if (RowID==0)
 {
  TCanvas *Canvas=new TCanvas();
  Canvas->Handle=dc;
  TNRect r2=*Client;
  r2.MoveTo(0,0);
  Canvas->Brush->Color=clWhite;
  Canvas->FillRect(r2);
  if (Title[RowID]->GetRect().Touches(*Portion)) Title[RowID]->Paint(RowID,dc,&r2,Portion);
  if (Detail[RowID]->GetRect().Touches(*Portion)) Detail[RowID]->Paint(RowID,dc,&r2,Portion);
  if (Summary[RowID]->GetRect().Touches(*Portion)) Summary[RowID]->Paint(RowID,dc,&r2,Portion);
  delete Canvas;
 }
 else
 {
  if (Title[RowID]->GetRect().Touches(*Portion)) Title[RowID]->Paint(RowID,dc,Client,Portion);
  if (Detail[RowID]->GetRect().Touches(*Portion)) Detail[RowID]->Paint(RowID,dc,Client,Portion);
  if (Summary[RowID]->GetRect().Touches(*Portion)) Summary[RowID]->Paint(RowID,dc,Client,Portion);
 }
};
DataWindow::DataWindow(DataStream *_s,bool _DeleteStream,OutRect * _parent):OutRect(StaticField,NULL,_parent)
{
 s=_s;
 DeleteStream=_DeleteStream;
 CurrentRow=0;
 FieldKind=SubReportField;
 SetupString("DataWindowRect",FieldName);
 Visible=true;
 H.NoSizing=false;
 W.NoSizing=false;
 Parser=0;
 DeleteParser=false;
 CurrentRow=0;
};
DataWindow * DataWindow::Clone()
{
 return new DataWindow(*this);
};
DataWindow::DataWindow(DataWindow & d):
 OutRect(d),
 ParamDef(d.ParamDef),
 TitleDef(d.TitleDef),
 DetailDef(d.DetailDef),
 SummaryDef(d.SummaryDef),
 s(d.s),
 Parser(d.Parser),
 InitExpression(d.InitExpression)
{
 DeleteParser = false;		// поток и интерпретатор остается у хозяина
 DeleteStream = false;
 CurrentRow=0;
 for(int i=0;i<d.Groups.Count();i++)
 {
  _OutRectGroup *ptr=new _OutRectGroup(d.Groups.GetI(i)->Condition, d.Groups.GetI(i)->BeforGroup);
  if(ptr) Groups.Add (ptr, d.Groups.Name(i));
 }
};
int DataWindow::SectionRowCount()
{
 return SectionRowCount(CurrentRow);
};
int DataWindow::SectionRowCount(int RowID)
{
 if(RowInfo[RowID] && *RowInfo[RowID] )
 {
  return (*RowInfo[RowID])->RowCount();
 }
 else return 0;
};
StackValue* DataWindow::FieldData(int DocRowID,int SectionRowID, char *Name)
{
 if (s)
 {
  for (int i=0;i<s->fields.Count();i++)
  {
   if(s->fields.Name(i)==Name)
   {
    if(RowInfo[DocRowID] && (*(RowInfo[DocRowID])) && (*(RowInfo[DocRowID]))->GetRow(SectionRowID)) return (*(*RowInfo[DocRowID])->GetRow(SectionRowID))[i];
    else return NULL;
   }
  }
  return NULL;
 }
 else return NULL;
};
void DataWindow::SetParameter( const char * Name, const StackValue *Value)
{
 for(int i=0;i< RowParam[CurrentRow]->Count();i++)
 {
  if(RowParam[CurrentRow]->Name(i)==Name)
  {
   delete RowParam[CurrentRow]->Get(i);
   RowParam[CurrentRow]->Set(i,new StackValue(Value));
   return ;
  }
 }
};
DataWindowParser *DataWindow::GetParser()
{
 return Parser;
};
void DataWindow::SetParser(DataWindowParser * _Parser, bool _DeleteParser)
{
 if(Parser && DeleteParser) delete Parser;
 Parser=_Parser;
 DeleteParser = _DeleteParser;
};
void DataWindow::SetStream(DataStream *_s, bool _DeleteStream)
{
 if(DeleteStream && s) delete s;
 s=_s;
 DeleteStream=_DeleteStream;
};
StackValue * DataWindow::InitField(int RowID, MiniParser *parser,bool & AllowDelete)
{
 StackValue *sv=0;
 MiniParser *p=parser;
 if(!p) p=Parser;
 if(p && !InitExpression.IsEmpty())
 {
  ((DataWindowParser*)p)->SetCurrentRect(this);
  ((DataWindowParser*)p)->SetCurrentRow(0);
  p->ParseExpression(InitExpression);
 }
 sv=OutRect::InitField(RowID,p,AllowDelete);
 return sv;
};
bool DataWindow::GetGroupResult(int SectionRowID, int GroupID)
{
 return *(*(*GroupResults[CurrentRow])[SectionRowID-1])[GroupID];
};
// DataWindowParser
/////////////////////////////////////////////////////////////////////////////////////////
bool DataWindowParser::RunFunction (const AnsiString &Name,  MyArray<StackValue>* FArgs)
{
 if(Name=="SETRECTPARAMETER")
 {
  if(CurrentRect) CurrentRect->SetParameter((*FArgs)[1]->ToStr().c_str(),(*FArgs)[2]);
  return true;
 }
 if(Name=="ROW")
 {
  PushValue(new StackValue(CurrentRow ));
  return true;
 }
 if(Name=="ROWCOUNT")
 {
  PushValue(new StackValue(dw->SectionRowCount()));
  return true;
 };
 if(Name=="GETFIELD")
 {
  // name,Row
  for(int i=0;i< dw->GetStream()->fields.Count();i++)
  {
   if(dw->GetStream()->fields.Name(i)==(*FArgs)[1]->ToStr())
   {
    PushValue((*dw->RowInfo[dw->CurrentRow])->GetRow((*FArgs)[2]->ToNumeric())->Get(i));
    return true;
   }
  }
 };
 return false;
};
bool DataWindowParser::SetVariable (const AnsiString &Name, const StackValue &Value)
{
 // просматриваем только!!! параметры dw и dw->s
 for(int i=0;i< dw->RowParam[dw->CurrentRow]->Count();i++)
 {
  if(dw->RowParam[dw->CurrentRow]->Name(i)==Name)
  {
   delete dw->RowParam[dw->CurrentRow]->Get(i);
   dw->RowParam[dw->CurrentRow]->Set(i,new StackValue(Value));
   return true;
  }
 }
 for(int i=0;i< dw->GetStream()->params.Count();i++)
 {
  if(dw->GetStream()->params.Name(i)==Name)
  {
   delete dw->GetStream()->params.Get(i);
   dw->GetStream()->params.Set(i,new StackValue(Value));
   return true;
  }
 }
 return false;
};
bool DataWindowParser::GetVariable (const AnsiString &Name, StackValue &Storage)
{
 // сначала просматриваем параметры
 for(int i=0;i< dw->RowParam[dw->CurrentRow]->Count();i++)
 {
  if(dw->RowParam[dw->CurrentRow]->Name(i)==Name)
  {
   Storage=*dw->RowParam[dw->CurrentRow]->Get(i);
   return true;
  }
 }
 for(int i=0;i< dw->GetStream()->params.Count();i++)
 {
  if(dw->GetStream()->params.Name(i)==Name)
  {
   Storage=*dw->GetStream()->params.Get(i);
   return true;
  }
 }
 for(int i=0;i< dw->GetStream()->fields.Count();i++)
 {
  if(dw->GetStream()->fields.Name(i)==Name)
  {
   if(dw->CurrentRow>=0 && dw->CurrentRow<dw->RowInfo.Count())
   {
    if(CurrentRow>=0 && CurrentRow<dw->SectionRowCount())
    {
     if( dw->RowInfo.Get(dw->CurrentRow) &&	*(dw->RowInfo.Get(dw->CurrentRow))
                         && (*(dw->RowInfo.Get(dw->CurrentRow)))->GetRow(CurrentRow)
                         && (*(dw->RowInfo.Get(dw->CurrentRow)))->GetRow(CurrentRow)->Get(i))
      Storage=*(*(dw->RowInfo.Get(dw->CurrentRow)))->GetRow(CurrentRow)->Get(i);
    }
   }
   return true;
  }
 }
 return false;
};
void DataWindowParser::PostInit()
{
 AddUserFunction("SETRECTPARAMETER",2);  // параметры для OutRect
 AddUserFunction("ROW",0);
 AddUserFunction("ROWCOUNT",0);
 AddUserFunction("GETFIELD",2);
};
//---------------------------------------------------------------------------
#endif


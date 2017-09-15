//---------------------------------------------------------------------------
#ifndef DW_OR
#define DW_OR
#pragma hdrstop
//---------------------------------------------------------------------------
#include "bamilib.h"
#include "New_dw.h"
#pragma package(smart_init)
//OutRect
////////////////////////////////////////////////////////////////////////////////
TNRect OutRect::ErrRect;
bool OutRect::ErrBool;
ResolverMap OutRect::ErrMap;
// virtal function reserved for subreports
void OutRect::SetParameter( const char * Name, const StackValue *Value){};
void OutRect::SetGroup(const char *Name)
{
 DataWindow *p = (DataWindow *)parent->parent;
 Group=0;
 if(!p) return;
 for(int i=0;i<p->Groups.Count();i++)
 {
	if(p->Groups.Name(i)==Name)
  {
   Group=i+1;
   return;
  }
 }
}
void OutRect::SetParent(OutRect * _parent){parent=_parent;};
ResolverMap& OutRect::Map(int RowID)
{
 if(State[RowID]) return State[RowID]->Pos;
 else return ErrMap;
};
// –ассчитана позици€ или нет
bool OutRect::IsResolved(int RowID)
{
 if(State[RowID]) return State[RowID]->Pos.IsResolved();
 else return false;
};
//  оличество расчитанных координат
int OutRect::ResolverCount(int RowID)
{
 if(State[RowID]) return State[RowID]->Pos.ResolverCount();
 else return 0;
};
// расположение дл€ конкретной строки
TNRect& OutRect::GetRect(int  RowID)
{
 if(State[RowID]) return State[RowID]->Pos.Rect;
 else
 {
  ErrRect.SetNull();
  return ErrRect;
 }
};
// рассчитан ли левый верхний угол
bool & OutRect::ResolvedLT(int RowID)
{
 if(State[RowID]) return State[RowID]->Pos.LT;
 else
 {
	ErrBool = false;
  return ErrBool;
 }
};
// –ассчитана ли ширина
bool & OutRect::ResolvedW(int RowID)
{
 if(State[RowID]) return State[RowID]->Pos.W;
 else
 {
  ErrBool = false;
  return ErrBool;
 }
};
// –ассчитана ли высота
bool & OutRect::ResolvedH(int RowID)
{
 if(State[RowID]) return State[RowID]->Pos.H;
 else
 {
  ErrBool = false;
	return ErrBool;
 }
};
//  оординаты расчитнного пр€моугольника
int OutRect::GetLeft(int RowID)
{
 if(State[RowID]) return State[RowID]->Pos.Rect.Left;
 else return 0;
};
int OutRect::GetTop(int RowID)
{
 if(State[RowID]) return State[RowID]->Pos.Rect.Top;
 else return 0;
};
int OutRect::GetRight(int RowID)
{
 if(State[RowID]) return State[RowID]->Pos.Rect.Right;
 else return 0;
};
int OutRect::GetBottom(int RowID)
{
 if(State[RowID]) return State[RowID]->Pos.Rect.Bottom;
 else return 0;
};
// ¬ысота рассчитанного пр€моугольника
int OutRect::GetH(int RowID)
{
 if(State[RowID]) return State[RowID]->Pos.Rect.Height();
 else return 0;
};
// Ўирина рассчитанного пр€моугольника
int OutRect::GetW(int RowID)
{
 if(State[RowID]) return State[RowID]->Pos.Rect.Width();
 else return 0;
};
// –абота с текстом пол€
void OutRect::SetText(int RowID,char * _Text)
{
 if(State[RowID] && State[RowID]->Info)
 {
  State[RowID]->Info->type=stString;
	State[RowID]->Info->s=_Text;
 }
};
const char * OutRect::GetText(int RowID)
{
 if(State[RowID] && State[RowID]->Info) return State[RowID]->Info->ToStr().c_str();
 else return 0;
};
// ќтметить как нерасчитанный
void OutRect::UnResolve(int RowID)
{
 if(State[RowID]) State[RowID]->Pos.Clear();
};
//  оличество строк
int OutRect::RowCount()
{
 return State.Count();
};
// ѕометить конкретную строку
void OutRect::Select(int RowID)
{
 if(State[RowID]) State[RowID]->Selected=true;
};
// —н€ть пометку
void OutRect::UnSelect(int RowID)
{
 if(State[RowID]) State[RowID]->Selected=false;
};
bool OutRect::IsSelected(int RowID)
{
 if (Selectable && Visible && State[RowID]) return State[RowID]->Selected;
 else return false;
};
void OutRect::SetLT(int RowID,int Left,int Top)
{
 if(State[RowID])
 {
  State[RowID]->Pos.Rect.Left=Left;
  State[RowID]->Pos.Rect.Top=Top;
  State[RowID]->Pos.LT=true;
 }
};
void OutRect::SetLT(int RowID,TSize startPosition)
{
 if(State[RowID])
 {
  State[RowID]->Pos.Rect.Left=startPosition.cx;
  State[RowID]->Pos.Rect.Top=startPosition.cy;
  State[RowID]->Pos.LT=true;
 }
};
OutRect::~OutRect()
{
 State.Clear();
 if(FieldName) delete FieldName;
};
OutRect::OutRect(FieldKinds _FieldKind,char * _FieldName, OutRect *_parent )
{
 Visible=true;
 Style=&DefOutRectStyle;
 Selectable=true;
 SetParent(_parent);
 FieldKind=_FieldKind;
 SetupString(_FieldName,FieldName);
 Group=0;
};
OutRect::OutRect(OutRect & d,OutRect *NewParent)
{
 FieldKind=d.FieldKind;
 SetupString(d.FieldName,FieldName);
 LT=d.LT;
 W=d.W;
 H=d.H;
 Visible=d.Visible;
 Selectable=d.Selectable;
 Style=d.Style;
 if (NewParent) SetParent(NewParent);
 else SetParent(d.parent);
 for(int i=0;i<d.State.Count();i++)
 {
  State.Add(new OutRectState(*d.State.Get(i)));
 }
 Group=d.Group;
 GroupName=d.GroupName;
};
OutRect * OutRect::Clone()
{
 return new OutRect(*this);
};
void OutRect::SetRowCount(int RowCount)
{
 //Debug("RowCount for OutRect is %ld\n",RowCount);
 for(int i=State.Count();i<=RowCount;i++)
 {
  State.Add(new OutRectState);
  State[i]->Selected=false;
 }
};
void OutRect::Paint(int RowID,HDC dc,TNRect *Client, TNRect *Portion )
{
 TCanvas *Canvas=new TCanvas();
 Canvas->Handle=dc;
 if (IsResolved(RowID) && Visible )
 {
	TColor bg;
	TColor fg;
	OutRectStyle * st;
	TNRect Rect=GetRect(RowID);
	if(!Rect.Width() ||!Rect.Height()) return;
	Rect.MoveTo(Rect.Left-Portion->Left+Client->Left, Rect.Top-Portion->Top+Client->Top);
	st = Style;
	if (!Style)
		 Debug("\nDanger!!! Style is null");
	if(! IsSelected(RowID))
	{
	 bg = st->Background;
	 fg = st->Foreground;
	}
	else
	{
	 bg = st->FocusBackground;
	 fg = st->FocusForeground;
	}
	Canvas->Brush->Color=bg;
	Canvas->FillRect(Rect);
	// длитеьный процесс рисовани€ рамочек
	{
	 switch (st->LineLeft)
	 {
	 case lsSingle:
		Canvas->Pen->Color=clBlack;
		Canvas->Pen->Width=1;
		Canvas->MoveTo(Rect.Left,Rect.Top);
		Canvas->LineTo(Rect.Left,Rect.Bottom);
		break;
	 case lsDouble:
		Canvas->Pen->Color=clBlack;
		Canvas->Pen->Width=2;
		Canvas->MoveTo(Rect.Left,Rect.Top);
		Canvas->LineTo(Rect.Left,Rect.Bottom);
		break;
	 case ls3d:
		Canvas->Pen->Color=clWhite;
		Canvas->Pen->Width=1;
		Canvas->MoveTo(Rect.Left,Rect.Top);
		Canvas->LineTo(Rect.Left,Rect.Bottom);
		Canvas->MoveTo(Rect.Left+1,Rect.Top);
		Canvas->LineTo(Rect.Left+1,Rect.Bottom-1);
    break;
   };
   switch (st->LineRight)
   {
   case lsSingle:
    Canvas->Pen->Color=clBlack;
    Canvas->Pen->Width=1;
    Canvas->MoveTo(Rect.Right,Rect.Top);
		Canvas->LineTo(Rect.Right,Rect.Bottom);
    break;
   case lsDouble:
    Canvas->Pen->Color=clBlack;
		Canvas->Pen->Width=2;
		Canvas->MoveTo(Rect.Right,Rect.Top);
    Canvas->LineTo(Rect.Right,Rect.Bottom);
    break;
	 case ls3d:
    Canvas->Pen->Color=clBlack;
    Canvas->Pen->Width=1;
    Canvas->MoveTo(Rect.Right-1,Rect.Top+2);
		Canvas->LineTo(Rect.Right-1,Rect.Bottom);
    Canvas->MoveTo(Rect.Right,Rect.Top+1);
    Canvas->LineTo(Rect.Right,Rect.Bottom);
    break;
   };
   switch (st->LineTop)
   {
   case lsSingle:
    Canvas->Pen->Color=clBlack;
		Canvas->Pen->Width=1;
    Canvas->MoveTo(Rect.Left,Rect.Top);
    Canvas->LineTo(Rect.Right,Rect.Top);
    break;
	 case lsDouble:
		Canvas->Pen->Color=clBlack;
		Canvas->Pen->Width=1;
    Canvas->MoveTo(Rect.Left,Rect.Top);
		Canvas->LineTo(Rect.Right,Rect.Top);
    break;
   case ls3d:
    Canvas->Pen->Color=clWhite;
		Canvas->Pen->Width=1;
    Canvas->MoveTo(Rect.Right-1,Rect.Top+1);
    Canvas->LineTo(Rect.Left,Rect.Top+1);
    Canvas->MoveTo(Rect.Left,Rect.Top);
    Canvas->LineTo(Rect.Right,Rect.Top);
    break;
   };
   switch (st->LineBottom)
   {
	 case lsSingle:
    Canvas->Pen->Color=clBlack;
    Canvas->Pen->Width=1;
    Canvas->MoveTo(Rect.Left,Rect.Bottom);
		Canvas->LineTo(Rect.Right,Rect.Bottom);
		break;
	 case lsDouble:
    Canvas->Pen->Color=clBlack;
		Canvas->Pen->Width=2;
    Canvas->MoveTo(Rect.Left,Rect.Bottom);
    Canvas->LineTo(Rect.Right,Rect.Bottom);
    break;
	 case ls3d:
    Canvas->Pen->Color=clBlack;
    Canvas->Pen->Width=1;
		Canvas->MoveTo(Rect.Right,Rect.Bottom);
		Canvas->LineTo(Rect.Left,Rect.Bottom);
		Canvas->MoveTo(Rect.Left+1,Rect.Bottom-1);
		Canvas->LineTo(Rect.Right,Rect.Bottom-1);
		break;
	 };
	}
	short Align;
	switch (st->TextAlignment)
	{
	case saTopLeft:
	 Align =(DT_TOP|DT_LEFT); break;
	case saTopCenter:
	 Align =(DT_TOP|DT_CENTER); break;
	case saTopRight:
	 Align = (DT_TOP|DT_RIGHT); break;
	case saCenterLeft:
	 Align =(DT_VCENTER|DT_LEFT); break;
	case saCenterCenter:
	 Align = (DT_VCENTER|DT_CENTER); break;
	case saCenterRight:
	 Align =(DT_VCENTER|DT_RIGHT); break;
	case saBottomLeft:
	 Align = (DT_BOTTOM|DT_LEFT); break;
	case saBottomCenter:
	 Align =  (DT_BOTTOM|DT_CENTER); break;
	case saBottomRight:
	 Align = (DT_BOTTOM|DT_RIGHT); break;
	};
	if (st->WordBreak)	Align|=DT_WORDBREAK;
	if(GetText(RowID)!=NULL)
	{
	 if(st->GetFont()){
		 Canvas->Font->Assign(st->GetFont());
		 Canvas->Font->Color=fg;
	 }
	 DrawText(Canvas->Handle, GetText(RowID), -1, &RECT(Rect.Inflate(-2, -2)), Align);
	}
 }
 delete Canvas;
};
StackValue * OutRect::InitField(int RowID, MiniParser *parser,bool & AllowDelete)
{
 // вс€ проверка на условный вывод!!!
 if(Group && ((DataWindow *)parent->parent)->Groups.Count()>=Group)
 {
  if(!((DataWindow *)parent->parent)->GetGroupResult(RowID,Group-1))
  {
   AllowDelete=false;
   return 0;
	}
 }
 if (FieldName)
 {
  switch(FieldKind)
  {
	case StaticField:
   AllowDelete = true;
   return new StackValue(FieldName);
  case FormulaField:
  if(parser)
  {
   AllowDelete=true;
   ((DataWindowParser*)parser)->SetCurrentRect(this);
   ((DataWindowParser*)parser)->SetCurrentRow(RowID-1);
   return new StackValue(parser->ParseExpression(FieldName));
  }
  else
  {
   AllowDelete=false;
   return 0;
	}
  case StreamField:
	 if(parent && parent->parent)
   {
    AllowDelete=false;
    return ((OutRectContainer*)parent)->FieldData(RowID-1, FieldName);
	 }
   else
   {
    AllowDelete=false;
		return 0;
   }
  case SubReportField:
   AllowDelete=true;
   return new StackValue();
  default:
   AllowDelete=false;
   return 0;
  }
 }
 else
 {
  AllowDelete=false;
	return 0;
 }
}
void OutRect::Resolve(int RowID,HDC dc,OutRectContainer * p, DataStream * s)
{
 TCanvas *Canvas=new TCanvas();
 Canvas->Handle=dc;
 //Debug("\nResolving Rect");
 //MDebug("befor OutRect::Resolve" );
 if(!GroupName.IsEmpty() && !Group) SetGroup(GroupName.c_str());
 int pLeft=0,pTop=0;
 ResolverMap &RM=Map(RowID);
 RM.Clear();
 if(p && p->ResolvedLT(RowID))
 {
  pLeft=p->GetLeft(RowID)+2;
  pTop=p->GetTop(RowID)+2;
 }
 if(!p)
 {
  RM.Rect.Left=LT.Offset.cx;
	RM.Rect.Top=LT.Offset.cy;
  RM.LT = true;
 }
 else
 {
  switch (LT.FromPoint)
	{
  case NoLink:
	 RM.LT = true;
   RM.Rect.Left= pLeft + LT.Offset.cx+2;
   RM.Rect.Top = pTop + LT.Offset.cy+2;
   break;
  case LeftTop:
   if(p->Rects[LT.FromObject]->ResolvedLT(RowID))
   {
    RM.LT = true;
    RM.Rect.Left = p->Rects[LT.FromObject]->GetLeft(RowID) + LT.Offset.cx;
    RM.Rect.Top = p->Rects[LT.FromObject]->GetTop(RowID) + LT.Offset.cy;
   }
	 break;
  case CenterTop:
	 if(p->Rects[LT.FromObject]->IsResolved(RowID))
   {
    RM.LT = true;
    RM.Rect.Left = p->Rects[LT.FromObject]->GetLeft(RowID)+p->Rects[LT.FromObject]->GetW(RowID)/2+LT.Offset.cx;
    RM.Rect.Top =  p->Rects[LT.FromObject]->GetTop(RowID) + LT.Offset.cy;
   }
	 break;
  case RightTop:
	 if(p->Rects[LT.FromObject]->IsResolved(RowID))
   {
    RM.LT = true;
    RM.Rect.Left= p->Rects[LT.FromObject]->GetLeft(RowID)+p->Rects[LT.FromObject]->GetW(RowID)+LT.Offset.cx;
    RM.Rect.Top = p->Rects[LT.FromObject]->GetTop(RowID) + LT.Offset.cy;
   }
   break;
  case LeftCenter:
   if(p->Rects[LT.FromObject]->IsResolved(RowID))
   {
    RM.LT = true;
		RM.Rect.Left= p->Rects[LT.FromObject]->GetLeft(RowID) + LT.Offset.cx;
    RM.Rect.Top= p->Rects[LT.FromObject]->GetTop(RowID)+p->Rects[LT.FromObject]->GetH(RowID)/2+LT.Offset.cy;
	 }
   break;
  case CenterCenter:
   if(p->Rects[LT.FromObject]->IsResolved(RowID))
   {
    RM.LT = true;
		RM.Rect.Left= p->Rects[LT.FromObject]->GetLeft(RowID)+p->Rects[LT.FromObject]->GetW(RowID)/2+LT.Offset.cx;
    RM.Rect.Top= p->Rects[LT.FromObject]->GetTop(RowID)+p->Rects[LT.FromObject]->GetH(RowID)/2+LT.Offset.cy;
	 }
   break;
  case RightCenter:
   if(p->Rects[LT.FromObject]->IsResolved(RowID))
   {
    RM.LT = true;
    RM.Rect.Left= p->Rects[LT.FromObject]->GetLeft(RowID)+p->Rects[LT.FromObject]->GetW(RowID)+LT.Offset.cx;
    RM.Rect.Top= p->Rects[LT.FromObject]->GetTop(RowID)+p->Rects[LT.FromObject]->GetH(RowID)/2+LT.Offset.cy;
   }
   break;
  case LeftBottom:
	 if(p->Rects[LT.FromObject]->IsResolved(RowID))
   {
		RM.LT = true;
    RM.Rect.Left= p->Rects[LT.FromObject]->GetLeft(RowID)+LT.Offset.cx;
    RM.Rect.Top= p->Rects[LT.FromObject]->GetTop(RowID)+p->Rects[LT.FromObject]->GetH(RowID)+LT.Offset.cy;
   }
   break;
  case CenterBottom:
	 if(p->Rects[LT.FromObject]->IsResolved(RowID))
   {
		RM.LT = true;
    RM.Rect.Left= p->Rects[LT.FromObject]->GetLeft(RowID)+p->Rects[LT.FromObject]->GetW(RowID)/2+LT.Offset.cx;
    RM.Rect.Top= p->Rects[LT.FromObject]->GetTop(RowID)+p->Rects[LT.FromObject]->GetH(RowID)+LT.Offset.cy;
   }
   break;
  case RightBottom:
   if(p->Rects[LT.FromObject]->IsResolved(RowID))
   {
    RM.LT = true;
    RM.Rect.Left= p->Rects[LT.FromObject]->GetLeft(RowID)+p->Rects[LT.FromObject]->GetW(RowID)+LT.Offset.cx;
    RM.Rect.Top= p->Rects[LT.FromObject]->GetTop(RowID)+p->Rects[LT.FromObject]->GetH(RowID)+LT.Offset.cy;
	 }
   break;
	}
 }
 // ¬ычисл€ем значение пол€
 {
  bool AllowDelete;
  MiniParser*p=0;
	if(parent && parent->parent)
  p=((DataWindow*)parent->parent)->GetParser();
	State[RowID]->Info=InitField(RowID,p,AllowDelete);
  State[RowID]->DeleteInfo=AllowDelete;
 }
 // ¬ы€сн€ем что же содержит наше поле
/*
 if (FieldName)
 switch(FieldKind)
 {
 case StaticField:
 case FormulaField:
 case SpecalField:
	if (!State[RowID]->Info && FieldName)
  {
	 State[RowID]->Info =new StackValue(FieldName);
   State[RowID]->DeleteInfo= true;
  }
  State[RowID]->Info->type=stString;
  State[RowID]->Info->s=FieldName;
  break;
 case StreamField:
  if(p && p->parent)
	{
	 State[RowID]->Info=p->FieldData(RowID-1, FieldName);
	 State[RowID]->DeleteInfo= false;
	}
	break;
 }
*/
 if(!Visible || (Group && !State[RowID]->Info))
 {
	RM.Rect.Bottom=RM.Rect.Top;
	RM.H = true;
	RM.Rect.Right=RM.Rect.Left;
	RM.W = true;
	return;
 }
 // ¬ычисление реальной ширины и высоты пол€ !!!
 if (H.NoSizing)
 {
	RM.Rect.Bottom=RM.Rect.Top+H.minValue;
	RM.H = true;
 }
 else
 {
	RM.Rect.Bottom=RM.Rect.Top+H.minValue;
	RM.H = false;
 }
 if(W.NoSizing)
 {
	RM.Rect.Right=RM.Rect.Left+W.minValue;
	RM.W = true;
 }
 else
 {
	RM.Rect.Right=RM.Rect.Left+W.minValue;
	RM.W = false;
 }
 if (GetText(RowID))
 {
	int Align=0;
	switch (Style->TextAlignment)
	{
	case saTopLeft:
	 Align =(DT_TOP|DT_LEFT);
	 break;
	case saTopCenter:
	 Align =(DT_TOP|DT_CENTER);
	 break;
	case saTopRight:
	 Align = (DT_TOP|DT_RIGHT);
	 break;
	case saCenterLeft:
	 Align =(DT_VCENTER|DT_LEFT);
	 break;
	case saCenterCenter:
	 Align = (DT_VCENTER|DT_CENTER);
	 break;
	case saCenterRight:
	 Align =(DT_VCENTER|DT_RIGHT);
	 break;
	case saBottomLeft:
	 Align = (DT_BOTTOM|DT_LEFT);
	 break;
	case saBottomCenter:
	 Align =  (DT_BOTTOM|DT_CENTER);
	 break;
	case saBottomRight:
	 Align = (DT_BOTTOM|DT_RIGHT);
	 break;
	};
	if (Style->WordBreak)	Align=Align|DT_WORDBREAK;
	int Left = RM.Rect.Left;
	int Top = RM.Rect.Top;
	if (!H.NoSizing  && W.NoSizing)
	{
	 if(Style->GetFont())
		Canvas->Font->Assign(Style->GetFont());
	 RM.Rect.MoveTo(0,0);
	 TNRect r(RM.Rect.Left,RM.Rect.Top,RM.Rect.Right,RM.Rect.Bottom);
	 DrawText(Canvas->Handle, GetText(RowID), -1,  (RECT*)&r, DT_CALCRECT |  DT_EXTERNALLEADING |Align );
	 RM.Rect.Set(r.Left,r.Top,r.Right,r.Bottom);
	 RM.H = true;
	 RM.W = true;
	 RM.Rect.Bottom+=4;
	 RM.Rect.Right+=4;
	 RM.Rect.MoveTo(Left,Top);
	 if (H.maxValue && RM.Rect.Height()>H.maxValue)
			RM.Rect.Bottom =RM.Rect.Top+H.maxValue;
	 // восстанавливаем заданное значение высоты
	 if (RM.Rect.Height()<H.minValue) RM.Rect.Bottom =RM.Rect.Top+H.minValue;
	 // восстанавливаем заданное значение ширины
	 if (RM.Rect.Width()<W.minValue) RM.Rect.Right =RM.Rect.Left+W.minValue;
	}
	if (!W.NoSizing && H.NoSizing)
	{
	 if(Style->GetFont()){
		Canvas->Font->Assign(Style->GetFont());
	 }
	 RM.Rect.MoveTo(0,0);
	 TNRect r(RM.Rect.Left,RM.Rect.Top,RM.Rect.Right,RM.Rect.Bottom);
	 DrawText(Canvas->Handle, GetText(RowID), -1,  (RECT*)&r, DT_CALCRECT |  DT_EXTERNALLEADING |Align );
	 RM.Rect.Set(r.Left,r.Top,r.Right,r.Bottom);
	 RM.H = true;
	 RM.W = true;
	 RM.Rect.Bottom+=4;
	 RM.Rect.Right+=4;
	 RM.Rect.MoveTo(Left,Top);
	 // восстанавливаем заданное значение высоты
	 if (RM.Rect.Height()<H.minValue) RM.Rect.Bottom =RM.Rect.Top+H.minValue;
	 // восстанавливаем заданное значение ширины
	 if (RM.Rect.Width()<W.minValue) RM.Rect.Right =RM.Rect.Left+W.minValue;
	}
	if (!H.NoSizing  && !W.NoSizing)
	{
	 if(Style->GetFont())
		Canvas->Font->Assign(Style->GetFont());
	 RM.Rect.MoveTo(0,0);
	 TNRect r(RM.Rect.Left,RM.Rect.Top,RM.Rect.Right,RM.Rect.Bottom);
	 DrawText(Canvas->Handle, GetText(RowID), -1,  (RECT*)&r, DT_CALCRECT |  DT_EXTERNALLEADING |Align);
	 RM.Rect.Set(r.Left,r.Top,r.Right,r.Bottom);
	 RM.H = true;
	 RM.W = true;
	 RM.Rect.Bottom+=4;
	 RM.Rect.Right+=4;
	 RM.Rect.MoveTo(Left,Top);
	 if (H.maxValue && RM.Rect.Height()>H.maxValue) RM.Rect.Bottom =RM.Rect.Top+H.maxValue;
	 if (W.maxValue && RM.Rect.Width()>W.maxValue) RM.Rect.Right =RM.Rect.Left+W.maxValue;
	 // восстанавливаем заданное значение высоты
	 if (RM.Rect.Height()<H.minValue) RM.Rect.Bottom =RM.Rect.Top+H.minValue;
	 // восстанавливаем заданное значение ширины
	 if (RM.Rect.Width()<W.minValue) RM.Rect.Right =RM.Rect.Left+W.minValue;
	}
 }
 //Debug("\n'%s' Resolved At (%ld,%ld,%ld,%ld)\n",GetText(RowID)
 //,RM.Rect.Left,RM.Rect.Top,RM.Rect.Right,RM.Rect.Bottom);
 //MDebug("after OutRect::Resolve" );
};
//---------------------------------------------------------------------------
#endif


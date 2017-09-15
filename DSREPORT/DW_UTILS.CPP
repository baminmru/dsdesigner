//---------------------------------------------------------------------------
#ifndef DW_UTILS
#define DW_UTILS
#pragma hdrstop
//---------------------------------------------------------------------------
#include "bamilib.h"
#include "New_dw.h"
#pragma package(smart_init)
void MDebug(char * str)
{
#if 0 //DebugVersion
 MEMORYSTATUS ms;
 GlobalMemoryStatus(&ms);
 Debug("\n%s MemoryUsed Phys %ld PageFile %ld Total %ld\n",str
  ,(ms.dwTotalPhys-ms.dwAvailPhys) / 1024,(ms.dwTotalPageFile-ms.dwAvailPageFile) / 1024
  ,(ms.dwTotalPhys-ms.dwAvailPhys) / 1024+(ms.dwTotalPageFile-ms.dwAvailPageFile) / 1024
 );
#endif
};
//RowBuffer StaticFields;
OutRectStyle DefOutRectStyle(
 (TColor) 0x00FFFFFF,
 (TColor) 0x00000000,
 (TColor) 0x000000FF,
 (TColor) 0x00FFFF00,
 lsSingle,
 lsSingle,
 lsSingle,
 lsSingle,
 saCenterCenter,
 AnsiString("Arial Cyr"),
 10,
 true
);
OutRectStyle DefOutRectContainerStyle(
 (TColor) 0x00FFFFFF,
 (TColor) 0x00000000,
 (TColor) 0x000000FF,
 (TColor) 0x00FFFF00,
 lsSingle,
 lsSingle,
 lsSingle,
 lsSingle,
 saCenterLeft,
 AnsiString("Arial Cyr"),
 12,
 true
);
OutRectStyle DefOutRectTitle(
 (TColor) 0x00FFFFFF,
 (TColor) 0x00000000,
 (TColor) 0x000000FF,
 (TColor) 0x00FFFF00,
 lsSingle,
 lsSingle,
 lsSingle,
 lsSingle,
 saCenterLeft,
 AnsiString("Arial Cyr"),
 16,
 true
);
OutRectStyle DefOutRectSummary(
 (TColor) 0x00FFFFFF,
 (TColor) 0x00000000,
 (TColor) 0x000000FF,
 (TColor) 0x00FFFF00,
 lsSingle,
 lsSingle,
 lsSingle,
 lsSingle,
 saCenterLeft,
 AnsiString("Arial Cyr"),
 16,
 false
);
//ResolverMap
////////////////////////////////////////////////////////////////////////////
ResolverMap::ResolverMap()
{
 Clear();
};
bool ResolverMap::IsResolved()
{
 return LT && W && H;
};
void ResolverMap::Clear()
{
 LT=W=H=false;
 Rect.SetNull();
};
void ResolverMap::UnResolve()
{
 LT=W=H=false;
};
int ResolverMap::ResolverCount()
{
 return (LT?1:0) + (W?1:0) + (H?1:0);
};
//WHLink
////////////////////////////////////////////////////////////////////////////////
void WHLink::Set(int min, int max)
{
 if (min>=0) minValue=min;
 else minValue=0;
 if (max>=0) maxValue=max;
 else maxValue=0;
 if (maxValue && minValue && minValue>maxValue)
 {
  int l=maxValue;
  maxValue=minValue;
  minValue=l;
 }
};
WHLink::WHLink(bool _NoSizing, int _minValue, int _maxValue)
{
 NoSizing=_NoSizing;
 Set(_minValue,_maxValue);
};
WHLink::WHLink(const WHLink & w)
{
 NoSizing=w.NoSizing;
 minValue=w.minValue;
 maxValue=w.maxValue;
};
//XYLink
////////////////////////////////////////////////////////////////////////////////
XYLink::XYLink(LinkPoint _FromPoint , int _FromObject,int _OffsetX,int _OffsetY)
{
 FromPoint=_FromPoint;
 FromObject=_FromObject;
 Offset.cx=_OffsetX;
 Offset.cy=_OffsetY;
};
XYLink::XYLink(const XYLink &xy)
{
 FromPoint=xy.FromPoint;  FromObject=xy.FromObject;
 Offset=xy.Offset;
};
//OutRectStyle
//////////////////////////////////////////////////////////////////////////////
OutRectStyle::OutRectStyle():
 Background((TColor) 0x00FFFFFF),
 Foreground((TColor) 0x00000000),
 FocusBackground((TColor) 0x000000FF),
 FocusForeground((TColor) 0x00FFFF00),
 TextAlignment(saCenterLeft),
 LineLeft(lsSingle),
 LineRight(lsSingle),
 LineTop(lsSingle),
 LineBottom(lsSingle),
 Font(new TFont()),
 WordBreak(false)
{
 Font->Name="MS Sans Serif";
 Font->Size=9;
};
OutRectStyle::OutRectStyle
(TColor _Background,
 TColor _Foreground,
 TColor _FocusBackground,
 TColor _FocusForeground,
 OutRectLineStyle _LineLeft,
 OutRectLineStyle _LineRight,
 OutRectLineStyle _LineTop,
 OutRectLineStyle _LineBottom,
 OutRectAlignmentStyle _TextAlignment,
 AnsiString _FontName,
 int _FontSize,
 bool _WordBreak)
{
 Background=_Background;
 Foreground=_Foreground;
 FocusBackground=_FocusBackground;
 FocusForeground=_FocusForeground;
 TextAlignment=_TextAlignment;
 LineLeft=_LineLeft;
 LineRight=_LineRight;
 LineTop=_LineTop;
 LineBottom=_LineBottom;
 Font=new TFont();
 Font->Name=_FontName;
 Font->Size=_FontSize;
 WordBreak=_WordBreak;
};
OutRectStyle::OutRectStyle(const OutRectStyle & s)
{
 Background=s.Background;
 Foreground=s.Foreground;
 FocusBackground=s.FocusBackground;
 FocusForeground =s.FocusForeground;
 TextAlignment=s.TextAlignment;
 LineLeft=s.LineLeft;
 LineRight=s.LineRight;
 LineTop=s.LineTop;
 LineBottom=s.LineBottom;
 if (s.Font)
 {
  Font=new TFont();
  Font->Assign(s.Font);
 }
 WordBreak=s.WordBreak;
};
OutRectStyle::~OutRectStyle()
{
 if (Font) delete Font;
};
void OutRectStyle::SetFont (TFont *f)
{
 if (Font) delete Font;
 Font=new TFont();
 Font->Assign(f);
};
TFont *OutRectStyle::GetFont()
{
 return Font;
};
//OutRectState
////////////////////////////////////////////////////////////////////////////
OutRectState::OutRectState()
{
 Info=NULL;
 DeleteInfo=false;
};
OutRectState::OutRectState(const OutRectState & s )
{
 Pos=s.Pos;
 Selected=s.Selected;
 DeleteInfo=s.DeleteInfo;
 Info=s.Info;
 if(s.DeleteInfo && s.Info) Info=new StackValue(*s.Info);
 else Info=s.Info;
};
OutRectState::~OutRectState()
{
 if(DeleteInfo && Info) delete Info;
};
// OutRectGroup
////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#endif


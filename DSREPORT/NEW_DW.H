//---------------------------------------------------------------------------
#ifndef NEW_DW
#define NEW_DW
//---------------------------------------------------------------------------
#include <bamilib.h>
//---------------------------------------------------------------------------
class OutRect;
class OutRectContainer;
class DataWindow;
class DataSnapshot;
class DataWindowParser;
// Internal classes
class TNRect : public TRect
{
private:
protected:
public:
 TNRect();
 TNRect(int _Left, int _Top, int _Right, int _Bottom);
 TNRect(TRect Rect);
 void SetNull();
 void Set(int _Left, int _Top, int _Right, int _Bottom);
 int Width();
 int Height();
 bool Touches(TNRect& other);
 TNRect& MoveTo(int x, int y);
 TNRect& Inflate(int dx, int dy);
};
// ��������������� � ���������� ��������� ���������
struct ResolverMap
{
 TNRect Rect;
 bool LT;
 bool W;
 bool H;
 ResolverMap();
 bool IsResolved();
 void Clear();
 void UnResolve();
 int ResolverCount();
};
typedef  MyArray<ResolverMap> Position ;
// ������������ ������� � ���������� �� ���������
struct WHLink
{
 bool NoSizing;
 int minValue;
 int maxValue;
 void Set(int min, int max=0);
 WHLink(bool _NoSizing=true, int _minValue=0, int _maxValue=0);
 WHLink(const WHLink & w);
};
enum LinkPoint
{
 NoLink, LeftTop, CenterTop, RightTop,
 LeftCenter, CenterCenter, RightCenter,
 LeftBottom, CenterBottom, RightBottom
};
// �������� � ������ (�� ����� ������) ����������� ����
struct XYLink
{
 LinkPoint FromPoint;
 int FromObject;
 TSize Offset;
 XYLink(LinkPoint _FromPoint=NoLink, int _FromObject=0, int _OffsetX=0, int _OffsetY=0);
 XYLink(const XYLink &xy);
};
// OutRectStyle
////////////////////////////////////////////////////////////////////////////////
// ����� ������ ������
class OutRectStyle
{
private:
 TFont *Font;
public:
 bool WordBreak;
 OutRectLineStyle LineLeft;
 OutRectLineStyle LineRight;
 OutRectLineStyle LineTop;
 OutRectLineStyle LineBottom;
 OutRectAlignmentStyle TextAlignment;
 TColor Background;
 TColor Foreground;
 TColor FocusBackground;
 TColor FocusForeground;
 OutRectStyle();
 OutRectStyle
 (
  TColor _Background,
  TColor _Foreground,
  TColor _FocusBackground,
  TColor _FocusForeground,
  OutRectLineStyle _LineLeft,
  OutRectLineStyle _LineRight,
  OutRectLineStyle _LineTop,
  OutRectLineStyle _LineBottom,
  OutRectAlignmentStyle _TextAlignment,
  AnsiString FontName,
  int FontSize,
  bool _WordBreak
 );
 OutRectStyle(const OutRectStyle & s );
 ~OutRectStyle();
 void SetFont (TFont * f) ;
 TFont * GetFont();
};
// ��������� ���������������� ����� ������
extern OutRectStyle DefOutRectStyle;
extern OutRectStyle DefOutRectContainerStyle;
extern OutRectStyle DefOutRectTitle;
extern OutRectStyle DefOutRectSummary;
// OutRectState - ��������� ��������������
////////////////////////////////////////////////////////////////////////////////
struct OutRectState
{
 ResolverMap Pos;
 unsigned Selected:1;
 unsigned DeleteInfo:1;
 StackValue *Info;
 OutRectState();
 OutRectState(const OutRectState & s );
 ~OutRectState();
};
typedef class MyArray<OutRectState> RectStates;
// OutRectGroup
////////////////////////////////////////////////////////////////////////////////
class _OutRectGroup
{
public:
 bool BeforGroup;
 AnsiString Condition;
 bool IsBefor(){ return BeforGroup; };
 AnsiString Evaluate(MiniParser *p){return p->ParseExpression(Condition);};
 _OutRectGroup(AnsiString _GroupExpression, bool _BeforGroup=true)
 {
  Condition=_GroupExpression;
  BeforGroup=_BeforGroup;
 };
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// OutRect
////////////////////////////////////////////////////////////////////////////////
class OutRect
{
public:
 // ����� ������
 OutRectStyle *Style;
 AnsiString GroupName;
 // ������ � ������� ������ ������
 int Group;
 // ��������� ��� ���������
 bool Visible;
 bool Selectable;
 //  ��������
 XYLink LT;
 // ������
 WHLink W;
 // ������
 WHLink H;
 // ���� / �������� / ���������
 char *FieldName;
 void SetGroup(const char *Name);
 FieldKinds FieldKind; //-- �� ���� ???
 OutRect * parent;
 void SetParent(OutRect * _parent);
 //��������� ���� ��� ���� �����
 RectStates State;
 // ������������ � ������ ������������ ���������
 static TNRect ErrRect;
 static ResolverMap ErrMap;
 static bool ErrBool;
 // ��������� ������� ������������ ���� ��� ���������� ������
 ResolverMap& Map(int RowID);
 // ���������� ������� ��� ���
 bool IsResolved(int RowID);
 // ���������� ����������� ���������
 int ResolverCount(int RowID);
 // ������������ ��� ���������� ������
 TNRect& GetRect(int  RowID =0);
 // ��������� �� ����� ������� ����
 bool & ResolvedLT(int RowID);
 // ���������� �� ������
 bool & ResolvedW(int RowID);
 // ���������� �� ������
 bool & ResolvedH(int RowID);
 // ���������� ����������� ��������������
 int GetLeft(int RowID);
 int GetTop(int RowID);
 int GetRight(int RowID);
 int GetBottom(int RowID);
 // ������ ������������� ��������������
 int GetH(int RowID);
 // ������ ������������� ��������������
 int GetW(int RowID);
 // ������ � ������� ����
 void SetText(int RowID,char * _Text);
 const char * GetText(int RowID);
 // �������� ��� �������������
 void UnResolve(int RowID);
 // ���������� �����
 int RowCount();
 // �������� ���������� ������
 void Select(int RowID);
 // ����� �������
 void UnSelect(int RowID);
 // �������� ��� ���
 bool IsSelected(int RowID);
 // ������ ��������� ������ �������� ����
 void SetLT(int RowID,int Left,int Top);
 void SetLT(int RowID,TSize startPosition);
 virtual ~OutRect();
 // ������������
 OutRect(FieldKinds _FieldKind= StaticField,char * _FieldName=NULL, OutRect *_parent = NULL);
 OutRect(OutRect & d,OutRect *NewParent=NULL);
 virtual OutRect * Clone();
// ___ virtual functions ____________________________________________________________
 // �������� ���������
 virtual void Resolve(int RowID, HDC dc, OutRectContainer * p, DataStream *s);
 virtual void SetRowCount(int RowCount);
 virtual void Paint(int RowID, HDC dc, TNRect *Client,TNRect *Portion);
 virtual void SetParameter( const char * Name, const StackValue *Value);
 // ���������� �������� ��� ���������� ������ � ����������� �� �������� ������
 virtual StackValue* InitField(int RowID, MiniParser *Parser, bool &AllowDelete);
private:
};
// OutRectContainer
////////////////////////////////////////////////////////////////////////////////
class OutRectContainer : public OutRect
{
private:
 int CurrentRect;
public:
 int RectNo(){return CurrentRect;}
 bool GrowVertical;
 bool MultiRow;
 MyArray<OutRect> Rects;
 OutRectContainer(OutRect * _parent = NULL);
 OutRectContainer(OutRectContainer &d);
 ~OutRectContainer();
 // RowID - ���  ����� ������ � ������ ������������� ��������� !!!
 // ��������� ��� ������ � ��������������� Snapshot-��
 void Resolve(int RowID,HDC dc, OutRectContainer * p, DataStream *s);
 void Paint  (int RowID,HDC dc, TNRect *Client, TNRect*Portion);
 StackValue *FieldData(int SectionRowID, char *Name);
 virtual OutRectContainer * Clone();
private:
 // ���������� �������� ����� ��� ���������� ������
 virtual StackValue* InitField(int RowID, MiniParser *Parser, bool &AllowDelete);
};
// DataWindowParser
////////////////////////////////////////////////////////////////////////////////
enum SectionType
{
 secPageHeader=0,
 secTitle,
 secDetail,
 secSummary,
 secPageFooter
};
class DataWindowParser : public MiniParser
{
private:
 OutRect *CurrentRect;
 int CurrentRow;
 DataWindow *dw;
 virtual bool RunFunction (const AnsiString &Name,  MyArray<StackValue>* FArgs);
 virtual bool SetVariable (const AnsiString &Name, const StackValue &Value);
 virtual bool GetVariable (const AnsiString &Name, StackValue &Storage);
public:
 void SetCurrentRect(OutRect*r){CurrentRect=r;};
 void SetCurrentRow(int r){CurrentRow=r;};
 virtual void PostInit();
 DataWindowParser(DataWindow *_dw):dw(_dw){};
};
// DataWindow
////////////////////////////////////////////////////////////////////////////////
class DataWindow: public OutRect
{
private:
 //MyArray <bool> NeedRefresh;
 DataStream *s;
 DataWindowParser *Parser;
 //  ��������� ��� ������� �������� ����
 bool DeleteParser;
 bool DeleteStream;
 void MakeGroupResults(int RowID); // ������� ����������� ������ ��� ������
public:
 // Runtime data
 // for each row + 1 !!!
 MyArray<Params> RowParam;
 MyArray<DataSnapshot*> RowInfo;
 MyList<_OutRectGroup> Groups;   // ������
 MyArray< MyArray< MyArray< bool > >  >GroupResults;     // ��������� T/F ��� ������ ������ � ������
 bool GetGroupResult(int SectionRowID, int GroupID);
 MyArray<OutRectContainer> Title;
 MyArray<OutRectContainer> Detail;
 MyArray<OutRectContainer> Summary;
 int CurrentRow;
 // ��������� ������
 AnsiString InitExpression;
 DataWindowParser *GetParser();
 void SetParser(DataWindowParser * _Parser, bool _DeleteParser = false);
 DataStream *GetStream();
 void SetStream(DataStream *s, bool _DeleteStream = false);
 Params ParamDef;
 OutRectContainer TitleDef;
 OutRectContainer DetailDef;
 OutRectContainer SummaryDef;
 DataWindow(DataStream *_s,bool _DeleteStream = false, OutRect * _parent = NULL);
 DataWindow(DataWindow & d);
 virtual DataWindow * Clone();
 // RowID - ��� ����� ������ � ������, �������� ����������� ��������
 // ��� 0 ��� ���������������� ���������
 //void Refresh(int RowID,TDC* dc,OutRectContainer * p=NULL,DataStream  *ps=NULL);
 virtual void Resolve(int RowID, HDC dc, OutRectContainer * p, DataStream  *s);
 virtual void Paint(int RowID, HDC dc, TNRect *Client, TNRect *Portion);
 virtual void SetParameter( const char * Name, const StackValue *Value);
 virtual StackValue * InitField(int RowID, MiniParser *parser,bool & AllowDelete);
 void Prepare(int RowID, DataStream * s=NULL);
 DataSnapshot* GetSnapshot(int RowID);
 StackValue *FieldData(int DocRowID,int SectionRowID, char *Name);
 StackValue *FieldData(int SectionRowID, char *Name);
 int SectionRowCount(int RowID);
 int SectionRowCount();
 virtual void SetRowCount(int RowCount);
 ~DataWindow();
 //void Clear();
};
//---------------------------------------------------------------------------
#endif

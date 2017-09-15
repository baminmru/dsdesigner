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
// прямоугольничек с признаками установки координат
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
// максимальные размеры и разрешение на изменение
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
// привязка к уголку (не помню какому) предыдущего поля
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
// Стиль вывода ячейки
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
// несколько предопределенных типов вывода
extern OutRectStyle DefOutRectStyle;
extern OutRectStyle DefOutRectContainerStyle;
extern OutRectStyle DefOutRectTitle;
extern OutRectStyle DefOutRectSummary;
// OutRectState - состояние прямоугольника
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
 // стиль вывода
 OutRectStyle *Style;
 AnsiString GroupName;
 // Группа в которую входит ячейка
 int Group;
 // Видимость при просмотре
 bool Visible;
 bool Selectable;
 //  Привязка
 XYLink LT;
 // Ширина
 WHLink W;
 // Высота
 WHLink H;
 // Поле / Значение / Выражение
 char *FieldName;
 void SetGroup(const char *Name);
 FieldKinds FieldKind; //-- не надо ???
 OutRect * parent;
 void SetParent(OutRect * _parent);
 //состояния поля для всех строк
 RectStates State;
 // возвращаются в случае неправильной адресации
 static TNRect ErrRect;
 static ResolverMap ErrMap;
 static bool ErrBool;
 // Результат расчета расположения поля для конкретной строки
 ResolverMap& Map(int RowID);
 // Рассчитана позиция или нет
 bool IsResolved(int RowID);
 // Количество расчитанных координат
 int ResolverCount(int RowID);
 // расположение для конкретной строки
 TNRect& GetRect(int  RowID =0);
 // рассчитан ли левый верхний угол
 bool & ResolvedLT(int RowID);
 // Рассчитана ли ширина
 bool & ResolvedW(int RowID);
 // Рассчитана ли высота
 bool & ResolvedH(int RowID);
 // Координаты расчитнного прямоугольника
 int GetLeft(int RowID);
 int GetTop(int RowID);
 int GetRight(int RowID);
 int GetBottom(int RowID);
 // Высота рассчитанного прямоугольника
 int GetH(int RowID);
 // Ширина рассчитанного прямоугольника
 int GetW(int RowID);
 // Работа с текстом поля
 void SetText(int RowID,char * _Text);
 const char * GetText(int RowID);
 // Отметить как нерасчитанный
 void UnResolve(int RowID);
 // Количество строк
 int RowCount();
 // Пометить конкретную строку
 void Select(int RowID);
 // Снять пометку
 void UnSelect(int RowID);
 // Выделена или нет
 bool IsSelected(int RowID);
 // Задать положение левого верхнего угла
 void SetLT(int RowID,int Left,int Top);
 void SetLT(int RowID,TSize startPosition);
 virtual ~OutRect();
 // конструктора
 OutRect(FieldKinds _FieldKind= StaticField,char * _FieldName=NULL, OutRect *_parent = NULL);
 OutRect(OutRect & d,OutRect *NewParent=NULL);
 virtual OutRect * Clone();
// ___ virtual functions ____________________________________________________________
 // Пересчет координат
 virtual void Resolve(int RowID, HDC dc, OutRectContainer * p, DataStream *s);
 virtual void SetRowCount(int RowCount);
 virtual void Paint(int RowID, HDC dc, TNRect *Client,TNRect *Portion);
 virtual void SetParameter( const char * Name, const StackValue *Value);
 // вычисление значения для конкретной строки в зависимость от значения группы
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
 // RowID - это  номер строки в потоке родительского документа !!!
 // необходим для работы с соответствующим Snapshot-ом
 void Resolve(int RowID,HDC dc, OutRectContainer * p, DataStream *s);
 void Paint  (int RowID,HDC dc, TNRect *Client, TNRect*Portion);
 StackValue *FieldData(int SectionRowID, char *Name);
 virtual OutRectContainer * Clone();
private:
 // вычисление значений групп для конкретной строки
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
 //  состояние для расчета внешнего вида
 bool DeleteParser;
 bool DeleteStream;
 void MakeGroupResults(int RowID); // рассчет результатов вывода для группы
public:
 // Runtime data
 // for each row + 1 !!!
 MyArray<Params> RowParam;
 MyArray<DataSnapshot*> RowInfo;
 MyList<_OutRectGroup> Groups;   // Группы
 MyArray< MyArray< MyArray< bool > >  >GroupResults;     // результат T/F для каждой строки и группы
 bool GetGroupResult(int SectionRowID, int GroupID);
 MyArray<OutRectContainer> Title;
 MyArray<OutRectContainer> Detail;
 MyArray<OutRectContainer> Summary;
 int CurrentRow;
 // состояние секции
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
 // RowID - Это номер строки в потоке, которому принадлежит документ
 // или 0 для самостоятельного документа
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

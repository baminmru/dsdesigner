//---------------------------------------------------------------------------
#ifndef TaskH
#define TaskH
#include "bamilib.h"

//---------------------------------------------------------------------------
struct FilterFieldDef : public EditableFieldAttr{
  AnsiString Condition;

  friend SimpleStream & operator >> (SimpleStream &,FilterFieldDef & F);
  friend SimpleStream & operator << (SimpleStream &,FilterFieldDef & F);

};
//---------------------------------------------------------------------------
struct FilterFormDef{
  AnsiString Caption;
  MyArray<FilterFieldDef> FormDef;
  AnsiString SubWherePrephics;
  AnsiString Concatenator;
  AnsiString SubWherePostphics;
  EditFieldActions Changes;

  friend SimpleStream & operator >>(SimpleStream &,FilterFormDef & F);
  friend SimpleStream & operator <<(SimpleStream &,FilterFormDef & F);
};

//---------------------------------------------------------------------------
struct FormulaDef{
 AnsiString Name;
 AnsiString Formula;

 friend SimpleStream & operator >>(SimpleStream &,FormulaDef & F);
 friend SimpleStream & operator<<(SimpleStream &,FormulaDef & F);

};
//---------------------------------------------------------------------------
struct ReportDef{
 AnsiString Caption;
 AnsiString CaptionExpression;
 AnsiString ConnectionName;
 MyArray<FormulaDef> Formulas;
 StreamSavedFile ReportFile;
 AnsiString SQL;
 AnsiString BuildProc;

 friend SimpleStream & operator >>(SimpleStream &,ReportDef & F);
 friend SimpleStream & operator<<(SimpleStream &,ReportDef & F);
};

//---------------------------------------------------------------------------
struct MenuDef{
 AnsiString Caption;
 AnsiString Filter;
 AnsiString Editor;
 AnsiString Report;

 MyArray<MenuDef> Items;
 MenuDef(AnsiString _Caption="", AnsiString _Editor=""){
   Caption = _Caption;
   Editor=_Editor;
 };

 
 friend SimpleStream & operator >>(SimpleStream &,MenuDef & F);

 friend SimpleStream & operator<<(SimpleStream &,MenuDef & F);
};

//---------------------------------------------------------------------------
struct Task{
 AnsiString Name;
 AnsiString Caption;
 AnsiString Version;
 MyArray<MenuDef> Menu;

 friend SimpleStream & operator >>(SimpleStream &,Task & F);
 friend SimpleStream & operator<<(SimpleStream &,Task & F);
};

struct LMEAppDef{
 MyArray<Task> Tasks;
 DataManager Manager;
 MyArray<ReportDef> Reports;
 MyArray<FilterFormDef> Filters;

 friend SimpleStream & operator >>(SimpleStream & s,LMEAppDef & F);
 friend SimpleStream & operator<<(SimpleStream & s,LMEAppDef & F);
};


//---------------------------------------------------------------------------
#endif

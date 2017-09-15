//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Task.h"

//---------------------------------------------------------------------------
SimpleStream & operator >>(SimpleStream & s,FilterFieldDef & F){
    s >> (EditableFieldAttr&)F;
    s >> F.Condition;
    return s;
};

SimpleStream & operator<<(SimpleStream & s,FilterFieldDef & F){
    s << (EditableFieldAttr&)F;
    s << F.Condition;
    return s;
};

//---------------------------------------------------------------------------


SimpleStream & operator >>(SimpleStream &s,FilterFormDef & F){
  s >> F.Caption;
  s >> F.FormDef;
  s >> F.SubWherePrephics;
  s >> F.Concatenator;
  s >> F.SubWherePostphics;
  s >> F.Changes;  
  return s;
};

SimpleStream & operator <<(SimpleStream & s,FilterFormDef & F){
  s << F.Caption;
  s << F.FormDef;
  s << F.SubWherePrephics;
  s << F.Concatenator;
  s << F.SubWherePostphics;
  s << F.Changes;
  return s;
};


//---------------------------------------------------------------------------

SimpleStream & operator >>(SimpleStream & s,FormulaDef & F){
  s >> F.Name;
  s >> F.Formula;
  return s;
};

SimpleStream & operator<<(SimpleStream & s,FormulaDef & F){
  s << F.Name;
  s << F.Formula;
  return s;
};
//---------------------------------------------------------------------------

SimpleStream & operator >>(SimpleStream & s,ReportDef & F){
 s >> F.Caption;
 s >> F.Formulas;
 s >> F.ReportFile;
 s >> F.SQL;
 s >> F.ConnectionName;
 s >> F.CaptionExpression;
 s >> F.BuildProc;

 return s;
};
SimpleStream & operator<<(SimpleStream & s,ReportDef & F){
 s << F.Caption;
 s << F.Formulas;
 s << F.ReportFile;
 s << F.SQL;
 s << F.ConnectionName;
 s << F.CaptionExpression;
 s << F.BuildProc;
 return s;
};

//---------------------------------------------------------------------------
SimpleStream & operator >>(SimpleStream & s,MenuDef & F){
 s >> F.Caption;
 s >> F.Filter;
 s >> F.Editor;
 s >> F.Report;
 s >> F.Items;
 return s;
};

SimpleStream & operator<<(SimpleStream & s,MenuDef & F){
 s << F.Caption;
 s << F.Filter;
 s << F.Editor;
 s << F.Report;
 s << F.Items;
 return s;
};

//---------------------------------------------------------------------------

SimpleStream & operator >>(SimpleStream & s,Task & F){
 s >> F.Name;
 s >> F.Caption;
 s >> F.Version;
 s >> F.Menu;
 return s;
};

SimpleStream & operator<<(SimpleStream & s,Task & F){
 s << F.Name;
 s << F.Caption;
 s << F.Version;
 s << F.Menu;
 return s;
};

//---------------------------------------------------------------------------
SimpleStream & operator >>(SimpleStream & s,LMEAppDef & F){
  s >> F.Manager;
  s >> F.Tasks;
  s >> F.Reports;
  s >> F.Filters;

  return s;
};

SimpleStream & operator<<(SimpleStream & s,LMEAppDef & F){
  s << F.Manager;
  s << F.Tasks;
  s << F.Reports;
  s << F.Filters;
  return s;
};

//---------------------------------------------------------------------------
#pragma package(smart_init)

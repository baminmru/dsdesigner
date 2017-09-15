//---------------------------------------------------------------------------
#ifndef MenuDefH
#define MenuDefH
#include "StackValue.h"

struct CallMode{
 bool RunModal;
 AnsiString ObjectName;
 friend SimpleStream & operator >>(SimpleStream &,CallMode & F);
 friend SimpleStream & operator<<(SimpleStream &,CallMode & F);
};


struct MenuDef{
 AnsiString Caption;
 long Level;
 MyArray<CallMode> Calls;
 MenuDef(AnsiString _Caption="",long _Level=0){
   Caption = _Caption;
   Level=_Level;
};

 TMenuItem *mnuptr;

 friend SimpleStream & operator >>(SimpleStream &,MenuDef & F);
 friend SimpleStream & operator<<(SimpleStream &,MenuDef & F);
};


//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MenuDef.h"

//---------------------------------------------------------------------------

SimpleStream & operator >>(SimpleStream &s,CallMode & F){
    s >> F.RunModal;
    s >> F.ObjectName;
    return s;
};
SimpleStream & operator<<(SimpleStream &s,CallMode & F){
    s << F.RunModal;
    s << F.ObjectName;
    return s;
};
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
SimpleStream & operator >>(SimpleStream & s,MenuDef & F){
 s >> F.Caption;
 s >> F.Calls;
 s >> F.Level;
 return s;
};

SimpleStream & operator<<(SimpleStream & s,MenuDef & F){
 s << F.Caption;
 s << F.Calls;
 s << F.Level;
 return s;
};

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#pragma package(smart_init)

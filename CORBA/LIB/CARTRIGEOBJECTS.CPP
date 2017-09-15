//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CartrigeObjects.h"


SimpleStream & operator >> (SimpleStream & s,CartrigeParam &ft){
 s >> ft.Name;
 s >> ft.Value;
 return s;
};

SimpleStream & operator << (SimpleStream & s,CartrigeParam &ft){
 s << ft.Name;
 s << ft.Value;
 return s;
};

SimpleStream & operator >> (SimpleStream & s,FieldHeader &ft){
  s >> ft.Name;
  s >> ft.AllowNull;
  s >> ft.Type;
  s >> ft.MaxSize;
 return s;
};

SimpleStream & operator << (SimpleStream & s,FieldHeader &ft){
  s << ft.Name;
  s << ft.AllowNull;
  s << ft.Type;
  s << ft.MaxSize;
  return s;
};


SimpleStream & operator >> (SimpleStream & s,RowSet &ft){
  s >> ft.Header;
  s >> ft.Rows;
  return s;

};
SimpleStream & operator << (SimpleStream & s,RowSet &ft){
  s << ft.Header;
  s << ft.Rows;
  return s;
};




//---------------------------------------------------------------------------
#pragma package(smart_init)

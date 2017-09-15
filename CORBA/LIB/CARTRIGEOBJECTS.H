//---------------------------------------------------------------------------
#ifndef CartrigeObjectsH
#define CartrigeObjectsH

#include "stackValue.h"

struct CartrigeParam{
  AnsiString Name;
  StackValue Value;
	friend SimpleStream & operator >> (SimpleStream & s,CartrigeParam &ft);
	friend SimpleStream & operator << (SimpleStream & s,CartrigeParam &ft);
};

struct FieldHeader{
  AnsiString Name;
  boolean AllowNull;
  long Type;
  long MaxSize;
	friend SimpleStream & operator >> (SimpleStream & s,FieldHeader &ft);
	friend SimpleStream & operator << (SimpleStream & s,FieldHeader &ft);
};


struct RowData : public   MyArray<StackValue>{
};

template struct RowData ;

struct CParam : public   MyArray<CartrigeParam>{
};

template struct CParam ;


struct RowSet{
  MyArray<FieldHeader> Header;
  MyArray<RowData> Rows;
	friend SimpleStream & operator >> (SimpleStream & s,RowSet &ft);
	friend SimpleStream & operator << (SimpleStream & s,RowSet &ft);

};




//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MemoryBase.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#include "StackValue.h"


Streamable2(MemoryRecord,RecordID,fields);
Streamable3(FieldDef,Name,Type,AllowNull);
Streamable3(KeyDef,Name,Unique,Fields);
Streamable2(MemoryKeyItem,RowID,Fields);
Streamable3(MemoryKey,Name,FieldPosition,Items);
Streamable6(MemoryTable,Name,MaxID,Header,Rows,KeyDefs,Keys);
Streamable2(FieldRef,FromField,ToField);
Streamable4(FKeyDef,Name,FromTable,ToTable,Fields);
Streamable2(MemoryBase,Tables,FKeys);

bool operator == ( const MemoryRecord &a, const MemoryRecord &b){
  return a.RecordID==b.RecordID;
};
bool operator < ( const MemoryRecord &a, const MemoryRecord &b){
  return a.RecordID<b.RecordID;
};


bool operator == ( const MemoryKeyItem &a, const MemoryKeyItem &b){
  if(a.Fields.Count()!=b.Fields.Count()) return false;
  for(long i=0;i<a.Fields.Count();i++){
    StackValue *aa,*bb;
    aa=a.Fields.Get(i);
    bb=b.Fields.Get(i);
    if(!(*aa==*bb))
      return false;

  }
  return true;

};
bool operator < (const MemoryKeyItem &a, const MemoryKeyItem &b){
  long m=a.Fields.Count();
  if(m>b.Fields.Count()) m=b.Fields.Count();
  for(long i=0;i<m;i++){
    if(!((*a.Fields.Get(i))<(*b.Fields.Get(i))))
      return false;
    if((*a.Fields.Get(i))<(*b.Fields.Get(i)))
      return true;
  }
  if(m < b.Fields.Count())
    return true;
  return false;
};

StackValue MemoryTable::GetField(long RowID,AnsiString Name){ return new StackValue();};
void MemoryTable::SetField(long RowID,AnsiString Name,StackValue Val){};

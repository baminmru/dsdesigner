//---------------------------------------------------------------------------
#ifndef MemoryBaseH
#define MemoryBaseH

#include "StackValue.h"

class MemoryRecord{

     long RecordID;
     MyArray<StackValue> fields;
public:

     Streamable(MemoryRecord)
     MyArray<StackValue> *GetFields(){return &fields;};
     void SetFields(MyArray<StackValue> *f){
      if(f){
        MyMemory m;
        m << *f;
        m >>fields;
      }
     };

     long GetRecordID(){return RecordID;};

     MemoryRecord(long RowID=0,MyArray<StackValue> *f=0){
      RecordID=RowID;
      if(f){
        MyMemory m;
        m << *f;
        m >>fields;
      }
     };
     Sortable(MemoryRecord)

};

struct FieldDef{
  AnsiString Name;
  StackType Type;
  bool AllowNull;
  Streamable(FieldDef)
};


struct KeyDef{
  AnsiString Name;
  bool Unique;
  MyArray<AnsiString> Fields;
  Streamable(KeyDef)

};



class MemoryKeyItem{
public:
  MyArray<StackValue> Fields;
  long RowID;
  Streamable(MemoryKeyItem)
  Sortable(MemoryKeyItem)
};

class MemoryKey{
  MyArray<long> FieldPosition;
  bool Refreshed;
  long CurrentItem;
public:
  AnsiString Name;
  MySortedArray<MemoryKeyItem> Items;
  Streamable(MemoryKey)

};


class MemoryTable{
protected:
  long MaxID;
  AnsiString Name;
  MyArray<FieldDef> Header;
  MySortedArray<MemoryRecord> Rows;
  MyArray<KeyDef> KeyDefs;
  MyArray<MemoryKey> Keys;
  //void RefreshKeys();
  //AnsiString CurrentKey;
  //void ChangeAllKeys(long RowID);


public:
  Streamable(MemoryTable)
  AnsiString GetName(){ return Name;};

  void Trancate(){
    Rows.Clear();
    for(long i=0;i<Keys.Count();i++)
      Keys[i]->Items.Clear();
    MaxID=0;
  };

  long AddRow(MyArray<StackValue> *fields){
    MemoryRecord *mr = new MemoryRecord (++MaxID,fields);
    Rows.Add(mr);
  };

  void UpdateRow(long RowID, MyArray<StackValue> *fields){
    MemoryRecord mr(RowID,0);
    long id=Rows.Find(&mr);
    if(id==-1) return ;
    Rows[id]->SetFields(fields);
  };

  void DeleteRow(long RowID){
    MemoryRecord mr(RowID,0);
    long id=Rows.Find(&mr);
    if(id==-1) return ;
    Rows.Destroy(id);

  };
  MyArray<StackValue>* GetRow(long RowID){
    MemoryRecord mr(RowID,0);
    long id=Rows.Find(&mr);
    if(id==-1) return 0;
    return Rows[id]->GetFields();
  };

  long RowCount(){
    return Rows.Count();
  };
  const MyArray<FieldDef> * GetHeader(){ return &Header;};


  StackValue GetField(long RowID,AnsiString Name);
  void SetField(long RowID,AnsiString Name,StackValue Val);

  //void SetKey(AnsiString NewKey);
  //AnsiSting GetKey();
  MemoryTable(){MaxID=0;};

  MemoryTable(AnsiString _Name,MyArray<FieldDef> *TableDef){
    if(TableDef){
      MyMemory m;
      m << *TableDef;
      m >>Header;
    }
    Name=_Name;
    MaxID=0;
  };

  ~MemoryTable(){
  };
};



struct FieldRef{
  AnsiString FromField;
  AnsiString ToField;
  Streamable(FieldRef)
};

struct FKeyDef{
  AnsiString Name;
  AnsiString FromTable;
  AnsiString ToTable;
  MyArray<FieldRef> Fields;
  Streamable(FKeyDef)
};

class MemoryBase{
public:
  MyArray<MemoryTable> Tables;
  MyArray<FKeyDef> FKeys;
  Streamable(MemoryBase)

};


//---------------------------------------------------------------------------
#endif

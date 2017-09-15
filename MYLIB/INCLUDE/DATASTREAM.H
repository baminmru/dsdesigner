#ifndef DATASTREAM_H
#define DATASTREAM_H

// Data stream support
////////////////////////////////////////////////////////////////////////////////

// dataField
////////////////////////////////////////////////////////////////////////////////

class prolog dataField {
	long sqlType;
	long ID;
	short Nullable;                    // допускается ли пустое значение
	short AutoInc;
	fieldCont value;                   // буфер для сохранения значения пол
	fieldCont * defValue;              // значение по умолчанию
	long Prec;
	long Scale;
	long defLen;
	long realLen;                      // реальная длинна для сторки
public:
	fieldCont * GetCont(){ return &value;};
	AnsiString Name;

	 // информациооно - справочные
	long GetType(){return value.ctype;};
	long GetSQLType(){return sqlType;};
	long GetDefaultSize(){return defLen;}
	long GetScale(){return Scale;};
	long GetPrecision(){return Prec;};
	bool IsNullable(){return Nullable;}
	bool IsAutoInc(){return AutoInc;}
	long GetID(long _ID){return ID;};
	void Clear();

	 void SetDefValue(fieldCont * _defValue){
		 defValue=_defValue;
	 };

	 const fieldCont *  GetDefValue(){
		 return defValue;
	 };

	 // вывод
	 void *GetBinaryPrt();
	 long GetLastSize();

	 dataField(long _ID,
				 AnsiString _Name,
				 long _cType   = SQL_C_CHAR,
				 long _sqlType = SQL_CHAR,
				 long _realLen=0,
				 long _Scale=0,
				 long _Prec=0,
				 SWORD _Nullable=0,
				 SWORD _AutoInc=0
		);

		dataField(const dataField & d);
		~dataField();
};


// Params
////////////////////////////////////////////////////////////////////////////////
class Params : public MyList<StackValue>{
public:
	Params(){};
	Params(const Params &d);
	Params * Clone(){
		return new Params(*this);
	}
};



// DataStream
////////////////////////////////////////////////////////////////////////////////
typedef  MyArray<StackValue> RowBuffer;
typedef  MyArray<RowBuffer> StreamRows;
//extern RowBuffer StaticFields;

struct DoubleLong{
	long From;
	long To;
};



struct FilterLineDef{
	AnsiString Name;          // имя поля из DataStream !!!
	FilterCondition Condition;
	StackValue From;
	StackValue To;
};


typedef	MyArray<FilterLineDef> FilterDef;
typedef MyList<FilterDef> Filters;


struct SortLineDef{
	AnsiString Name;  // имя поля из из DataStream !!!
	bool Asc;
};

typedef MyArray<SortLineDef> SortDef;
typedef MyList<SortDef> Sorts;


/*
struct SnapFieldDef{
	bool AllowNull;
	long MaxSize;
	SnapFieldDef(bool _AllowNull,long _MaxSize){
		AllowNull=_AllowNull;
		MaxSize=_MaxSize;
	}
};
*/



// DataStream
////////////////////////////////////////////////////////////////////////////////
class DataSnapshot{
	 StreamRows Info;
	 //MyArray<long> FilterMap;
	 //MyArray<long> SortMap;
	 bool locked;
	 //MyList<SnapFieldDef> Names;
	 long UsageCount;
	 TDateTime LastRefresh;
	 TDateTime LastRowRefresh;

	 void Clear(){
		 Info.Clear();
		 //CurrentSort=0;
		 //CurrentFilter=0;
		 //SortMap.Clear();
		 //FilterMap.Clear();
	 };
	 void AddRow( RowBuffer * r){
		// FilterMap.Add(new long(FilterMap.Count()));
		// SortMap.Add(new long(SortMap.Count()));
		Info.Add(r);
	 };

	 void AddRowField(long Row, StackValue * f){
		RowBuffer * r= Info[Row];
		if(!r) return;
		r->Add(f);
	 };

	 void DeleteRow(long RowID);
		 long IncUsage(){return ++UsageCount; };
		 long DecUsage(){return --UsageCount; };

 public:
	 AnsiString OrderBy;
	 //SortDef *CurrentSort;
	 //FilterDef *CurrentFilter;
	 Params params;
	 AnsiString SubCondition;

	 long Usage(){return UsageCount;};
		 TDateTime RefreshTime(){return LastRefresh;};
		 TDateTime RowRefreshTime(){return LastRowRefresh;};

	 /*
	 // работа с именами и атрибутами полей
	 void ClearFieldDef(){
			Names.Clear();
	 };

	 void AddFieldDef(AnsiString Name,SnapFieldDef*ptr){
			Names.Add(ptr,Name);
	 };

	 SnapFieldDef* GetFieldDef(AnsiString Name){
			return Names(Name);
	 };

	 SnapFieldDef* GetFieldDef(long i){
			return Names[i];
	 };

	 AnsiString FieldName(long i){
		return Names.Name(i);
	 };
	 */
		 long FieldCount(){
			 if(Info.Count()) return Info[0]->Count(); else return 0;//Names.Count();
		 };

	 DataSnapshot(){
		 //CurrentSort=0;
		 //CurrentFilter=0;
	 };

	 long RowCount(){
		 return TotalRowCount(); //FilterMap.Count();
	 };

	 long TotalRowCount(){
		 return Info.Count();
	 };

	 RowBuffer *GetRow(long Index){
				if(Index>=0 && Index<Info.Count())
				 return Info[Index];
			 else
				 return 0;
			 /*
			 if(Index>=0 && Index<FilterMap.Count())
				 return Info[*SortMap[*FilterMap[Index]]];
			 else
				 return 0;
			 */

	 };

	 /*
	 RowBuffer *GetSortedRow(long Index){
			 if(Index>=0 && Index<SortMap.Count())
				 return Info[*SortMap[Index]];
			 else
				 return 0;
	 };
	 */
	 bool IsLocked(){
		 return locked;
	 };

	 void SetLock(bool Lock) {
		 locked=Lock;
	 };
	 /*
	 void ClearFilter(){
		FilterMap.Clear();
		for(long i=0;i<Info.Count();i++){
			FilterMap.Add(new long(i));
		}
		CurrentFilter=0;
	 };

	 void ClearSort(){
		for(long i=0;i<FilterMap.Count();i++){
			long *lll =FilterMap[i];
			*lll=*SortMap[*FilterMap[i]];
		}

		SortMap.Clear();
		for(long i=0;i<Info.Count();i++){
			SortMap.Add(new long(i));
		}
		CurrentSort=0;
	 };


	 void SetSort(SortDef *Sort,  Params * fields);
	 void SetFilter(FilterDef *Filter,  Params * fields);

	 bool AllowNull(AnsiString Name);
	 */
	 friend class DataStream;

};

// DataStream
////////////////////////////////////////////////////////////////////////////////

class DataManager;
class DataSourceBase;

class DataStream {

	 // источник данных
		 DataSourceBase *Owner;

		 long UsageCount;
		 DataSnapshot* CreateSnapshot(bool Lock);
public:

	 DataStream(){UsageCount=0;}

	 long IncUsage(){return ++UsageCount; };
	 long DecUsage(){return --UsageCount; };
	 long Usage(){return UsageCount;};

	 virtual ~DataStream(){};
	 long FieldCount(){return fields.Count();};

		DataSnapshot* GetSnapshot(bool Lock=false);
	 void KillSnapshot(DataSnapshot* ds);

	 void RefreshSnapshot(DataSnapshot* ds);
	 void SnapRowToFields(DataSnapshot* ds,long RowID);
	 void FieldsToSnapRow(DataSnapshot* ds,long RowID);
  void RefreshSnapRow(DataSnapshot* ds,long RowID){
  	  RestoreParams(ds);
     SnapRowToFields(ds, RowID);
     RefreshRow();
     FieldsToSnapRow(ds,RowID);
  };

	 void RestoreParams(DataSnapshot* ds);

	 void BeforInsert();
	 void MakeInsertFields(Params * _fieldsToInsert);
	 void Insert(DataSnapshot* ds,Params *_fields=NULL, bool FullRefresh = true);
	 void Delete(DataSnapshot* ds,long RowID, bool FullRefresh = false);
	 void Update(DataSnapshot* ds,long RowID, bool FullRefresh = false);

  virtual bool MakeAction(AnsiString Action, Params *Data){return false;};

	 virtual bool AllowInsert(){ return false;};
	 virtual bool AllowUpdate(){ return false;};
	 virtual bool AllowDelete(){ return false;};
	 virtual void SnapInit(DataSnapshot* ds);

	 virtual void Refresh();
	 virtual void RefreshRow();
	 virtual bool Updateing();
	 virtual bool Deleteing();
	 virtual bool Inserting();
	 virtual bool FetchNext();

	 Params fields;
	 Params params;
	 //Params defaults;
	 AnsiString SubCondition;
	 AnsiString OrderBy;

 private:


	 // Копии запросов, порожденные потоком
	 MyArray<DataSnapshot>  Snapshots;

	 friend class DataManager;
	 friend class DataSourceBase;
};


#endif
#ifndef ODBCENG_H
#define ODBCENG_H

#pragma pack(push,1)

#define MAXMEMO (1024L*16L)
#define HALFMEMO (MAXMEMOIZE/2L)


#define odbcEngineVersion "1.00"

long prolog SQLtoC(long);

class  prolog odbcEnvironment;
class prolog odbcConnection;
class prolog odbcPrepared;
class prolog odbcStatement;

#define StdRowSetSize 1

struct ErrorMessageData{
	AnsiString SQLSate;
	AnsiString NativeError;
	AnsiString ErrorMessage;
	ErrorMessageData(AnsiString S1,AnsiString S2,AnsiString S3){
	SQLSate=S1;
	NativeError=S2;
	ErrorMessage=S3;
	};

};

class TODBCException : public Exception{
	public:
	TODBCException(): Exception("TODBCException"){};
		AnsiString Message1;
		AnsiString Message2;
		MyArray<ErrorMessageData> Data;
		void Show();
};



class prolog odbcField {
	friend class odbcStatement;
	friend class odbcPrepared;
	odbcStatement * parent;
	long sqlType;
	long ID;
	short Nullable;                    // допускается ли пустое значение
	short AutoInc;
	short fieldUpdateable;
	fieldCont value;                   // буфер для сохранения значения поля
	long Prec;
	long Scale;
	long defLen;
	long realLen;                      // реальная длинна для сторки

public:

	 // информациооно - справочные
	long GetType(){return value.ctype;};
	long GetSQLType(){return sqlType;};
	long GetDefaultSize(){return defLen;}
	long GetScale(){return Scale;};
	long GetPrecision(){return Prec;};
	bool IsNullable(){return Nullable;}
	bool IsAutoInc(){return AutoInc;}
	long IsUpdateable(){return fieldUpdateable;} // 0- read, 1 - write, 2 -???
	long IsNull(){return realLen==SQL_NULL_DATA;}
	long GetID(long _ID){return ID;};

 fieldCont * GetCont(){value.Null=IsNull(); return &value;};
	StackValue GetData(void){ value.Null=IsNull(); return value._StackValue();};
 long CurrentSize(){return realLen;};


	 // вывод
	void *GetBinaryPrt();
	long  GetLastSize();

	// контроль текущего UPDATE
	void UnUpdatable(){realLen=SQL_IGNORE;};
	void Updatable(){if(IsUpdateable()!=0) realLen = defLen; else realLen=SQL_IGNORE; };



	 // выборка информации для "длинных" полей
	 // возвращает число реально считанных байт
	 long GetChunk(void * buffer, long bufSize);


	odbcField(odbcStatement* parent,
				 long _ID,
				 long _cType   = SQL_C_CHAR,
				 long _sqlType = SQL_CHAR,
				 long _realLen=0,
				 long _Scale=0,
				 long _Prec=0,
				 SWORD _Nullable=0,
				 SWORD _AutoInc=0,
				 SWORD _Updatable = 0
				 );

	~odbcField();
};

class odbcFieldsList: public 	MyList<odbcField>{
public:
	StackValue Data(long index){
		 odbcField * ptr =GetI(index);
		 if(ptr) return ptr->GetData();
		 return  *new StackValue();
	};

	StackValue Data(AnsiString Name){
		 odbcField * ptr =Get(Name);
		 if(ptr) return ptr->GetData();
		 return  *new StackValue();
	};

	double AsNumeric(long index){
		StackValue & sv =  Data(index);
		double d=sv.ToNumeric();
		delete & sv;
		return d;
	};
	double AsNumeric(AnsiString Name){
		StackValue & sv =  Data(Name);
		double d=sv.ToNumeric();
		delete & sv;
		return d;
	};

	AnsiString  AsString(long index){
		StackValue & sv =  Data(index);
		AnsiString d=sv.ToStr();
		delete & sv;
		return d;
	};

	AnsiString  AsString(AnsiString Name){
		StackValue & sv =  Data(Name);
		AnsiString d=sv.ToStr();
		delete & sv;
		return d;
	};

	TDateTime  AsDate(long index){
		StackValue & sv =  Data(index);
		TDateTime d=sv.ToDate();
		delete & sv;
		return d;
	};

	TDateTime  AsDate(AnsiString Name){
		StackValue & sv =  Data(Name);
		TDateTime d=sv.ToDate();
		delete & sv;
		return d;
	};

};


// Resultset view class
class prolog odbcStatement{
protected:
	friend class odbcEnvironment;
	friend class odbcConnection;
	HSTMT  stID;
	AnsiString SQL;
	int usageCount;
	short cols;

	RETCODE Check(RETCODE, AnsiString Message1="", AnsiString Message2="");

	virtual void SetOption(void);// устанавливает опции по умолчанию
	virtual void Init(void);     // исполняет запрос
	virtual void Setup(void);    // задает параметры полей (BIND)

	RETCODE Mover(UWORD Type,SDWORD irow=1);

public:
	// исполнение SQL предложений
	long Execute(AnsiString SQL);


	 int Usage(){ return usageCount;};
	 int IncUsage(){return ++usageCount;};
	 int DecUsage(){if(usageCount) usageCount--; return usageCount;};

	odbcConnection * parent;
	odbcFieldsList fields;

	void GetOptions(UWORD option, UDWORD* Value);
  void SetOptions(UWORD option, UDWORD Value){
  								Check(
														SQLSetStmtOption(stID,option,Value )
													 ,"SetOptions"
												);

  };
	void DebugPrint();


	// тонкая настройка
	void AsyncOn() {
									Check(
														SQLSetStmtOption(stID,SQL_ASYNC_ENABLE,SQL_ASYNC_ENABLE_ON )
													 ,"AcyncON"
												);
						};
	void AsyncOff() { Check(SQLSetStmtOption(stID,SQL_ASYNC_ENABLE,SQL_ASYNC_ENABLE_OFF),"AcyncOFF");};

	enum LockingType {
		Read    = SQL_CONCUR_READ_ONLY,
		Locks   = SQL_CONCUR_LOCK,
		Value   = SQL_CONCUR_VALUES,
		Version = SQL_CONCUR_ROWVER
	};

	void LockType(LockingType type) {Check(SQLSetStmtOption(stID,SQL_CONCURRENCY,type),"LockType");};

	enum CursorTypes {
		Forward = SQL_CURSOR_FORWARD_ONLY,
		Static  = SQL_CURSOR_STATIC,
		Keyset  = SQL_CURSOR_KEYSET_DRIVEN,
		Dynamic = SQL_CURSOR_DYNAMIC
	};

	void CursorType(CursorTypes type = Static, long keySetSize=0) {
		Check(SQLSetStmtOption(stID,SQL_CURSOR_TYPE,type),"CursorType");
		if(type == Keyset) Check(SQLSetStmtOption(stID,SQL_KEYSET_SIZE,keySetSize),"KeySetSize");
	};

	enum SimulationTypes {
			sNo  = SQL_SC_NON_UNIQUE,
			sTry = SQL_SC_TRY_UNIQUE,
			sYes = SQL_SC_UNIQUE
	};

	void SimulationType(SimulationTypes type){Check(SQLSetStmtOption(stID,SQL_SIMULATE_CURSOR,type),"SimulationType");};
	void BookmarksOn(){Check(SQLSetStmtOption(stID,SQL_USE_BOOKMARKS,SQL_UB_ON),"BookmarksOn");};
	void BookmarksOff(){Check(SQLSetStmtOption(stID,SQL_USE_BOOKMARKS,SQL_UB_OFF),"BookmarksOff");};

	 //
	const AnsiString GetSQL(){return SQL;};
		long SetCursorName(AnsiString name){
		long r =Check(SQLSetCursorName(stID,(UCHAR*)name.c_str(),SQL_NTS),"SetCursorName");
		return r;
	 };
	 long GetCursorName(AnsiString  name,short* maxsize){
		long r =Check(SQLGetCursorName(stID, (UCHAR*)name.c_str(),*maxsize,maxsize),"GetCursorName");
		return r;
	 };

	// перемещение курсора
	RETCODE MoveFirst()   			{return Mover(SQL_FETCH_FIRST);};
	RETCODE MoveLast()           	{return Mover(SQL_FETCH_LAST);};
	RETCODE Move(int step)       	{return Mover(SQL_FETCH_RELATIVE,step);};
	RETCODE MoveNext()           	{return Mover(SQL_FETCH_NEXT);};
	RETCODE MovePrev()           	{return Mover(SQL_FETCH_PRIOR);};
	RETCODE MoveAbs(long pos)    	{return Mover(SQL_FETCH_ABSOLUTE,pos);};
	RETCODE MoveBookmark(long bm)	{return Mover(SQL_FETCH_BOOKMARK);};

	// разрешение или запрещение изменения полей
	void IgnoreAllFields();
	void EnableAllFields();

	// работа с конкретной позицией  SetPos !!!
	void Lock  (){Check(SQLSetPos(stID,1,SQL_POSITION,SQL_LOCK_EXCLUSIVE),"Lock");};
	void UnLock(){Check(SQLSetPos(stID,1,SQL_POSITION,SQL_LOCK_UNLOCK),"UnLock");};
	void Update(){Check(SQLSetPos(stID,1,SQL_UPDATE,SQL_LOCK_NO_CHANGE),"Update");};
	void Delete(){Check(SQLSetPos(stID,1,SQL_DELETE,SQL_LOCK_NO_CHANGE),"Delete");};
	void Add   (){Check(SQLSetPos(stID,1,SQL_ADD,SQL_LOCK_NO_CHANGE),"Add");};
	void Refresh(){Check(SQLSetPos(stID,0,SQL_REFRESH,SQL_LOCK_NO_CHANGE),"Refresh");};

	 bool MoreResults();
	void Cancel() {Check(SQLCancel(stID),"Cancel");};

	 // Transaction Control
	void ManualTrans();
	void AutoTrans();
	 void CommitTrans();
	void RollbackTrans();

	 // для повторного использования
	 void NewQuery(AnsiString  _SQL, long TimeOut=0);

	odbcStatement( odbcConnection * _parent=NULL,AnsiString  _SQL="", long TimeOut=0);
	~odbcStatement();
};

class prolog odbcPrepared:public odbcStatement {
	 // узнает что там такое, создает параметры
	 virtual void Init(void);

public:
	 enum ParameterType {Input =SQL_PARAM_INPUT , Output = SQL_PARAM_OUTPUT};

	 // контроль параметров запроса
	 MyList<odbcField> param;
	 long NumParameters(){
		SWORD npar;
			Check( SQLNumParams(stID,&npar),"NumParams");
			return npar;
	 };
	 void SetParamType(long ParamID, ParameterType Type); // делает BIND для параметра

	 // исполняет запрос
	 long Call();
	 odbcPrepared(odbcConnection * c, AnsiString  SQL,long _TimeOut);
	 ~odbcPrepared();
};

class prolog odbcConnection {
	AnsiString DSN;
	AnsiString UID;
	AnsiString PWD;
	void SetOptions(UWORD option, UDWORD Value){
    try{
		Check(SQLSetConnectOption(hDBC,option,Value),"SetOption");
    }catch(TODBCException *e){
    }

	};
	RETCODE Check(RETCODE, AnsiString Message1="", AnsiString Message2="");
public:
	odbcEnvironment * parent;
	MyArray<odbcStatement> StatementItems;
	MyArray<odbcPrepared> PreparedItems;
	HDBC hDBC;

	void GetOptions(UWORD option, UDWORD *Value){
    try{
		Check(SQLGetConnectOption(hDBC,option,Value),"GetOption");
    }catch(TODBCException *e){
    }

	 };

	// закрыть коннекцию и убить себя как объект
	void Close();

	// Установки всяческих параметров
	void ManualTrans(){
		SetOptions(SQL_AUTOCOMMIT,SQL_AUTOCOMMIT_OFF);

	 };
	void AutoTrans(){
		SetOptions(SQL_AUTOCOMMIT,SQL_AUTOCOMMIT_ON);

	 };
	void ReadOnly() {
		SetOptions(SQL_ACCESS_MODE,SQL_MODE_READ_ONLY);

	 };
	void ReadWrite() {
		SetOptions(SQL_ACCESS_MODE,SQL_MODE_READ_WRITE);
	 };

	// завершение транзакций в режиме ManualTrans
	void CommitTrans();
	void RollbackTrans();

	odbcConnection(odbcEnvironment * _parent, AnsiString DSN, AnsiString UID, AnsiString PWD, long LoginTimeOut);
	~odbcConnection();

	// создание курсоров
	odbcPrepared  * CreatePrepared(AnsiString SQL,long qTimeOut=0);

	odbcStatement * CreateStatement(AnsiString SQL,long qTimeOut=0);
	 void FreeStatement(odbcStatement *p);

	// исполнение SQL предложений
	long Execute(AnsiString SQL);

};

class prolog odbcEnvironment {
	protected:
		AnsiString Password;
		AnsiString UserName;
		RETCODE Check(RETCODE, AnsiString Message1="", AnsiString Message2="");
	public:
		MyList<odbcConnection> Connections;

		HENV hEnv;
		odbcEnvironment(AnsiString PWD="", AnsiString UID="" );
		~odbcEnvironment();
		odbcConnection * OpenConnection(AnsiString Name,AnsiString DSN,AnsiString UID="",
													AnsiString PWD="", long TimeOut=-1);
   void Kill(odbcConnection *c){Connections.Destroy(c);};
   void Kill(AnsiString Name){Connections.Destroy(Connections.Where(Name));};
};


extern odbcEnvironment ODBC;

#pragma pack(pop)
#endif





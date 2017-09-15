#include "bamilib.h"
#include "ErrorFrom.h"

void prolog Debug(char *format,...){
#if DebugVersion
		 FILE* h=fopen((Application->ExeName+".dbg").c_str(),"at");
    long l = filelength( _fileno(h));
    if(l>262144L){
     fclose(h);
		  h=fopen("trace.txt","wt");
    }
		 va_list vl; va_start(vl,format); vfprintf(h,format,vl); va_end(vl);
		 fclose(h);
#endif
};

void TODBCException::Show(){
	AnsiString msg;
	msg=Message1;
	if(!msg.IsEmpty())
		msg=msg +"\r\n";
	msg=msg + Message2 ;
	for(long i=0;i<Data.Count(); i++){
		if(!msg.IsEmpty())
			msg=msg +"\r\n";
			msg=msg + Data[i]->SQLSate+" "+ Data[i]->NativeError+" "+Data[i]->ErrorMessage;
	}

	TfrmError *err = new TfrmError(Application);
	err->Caption="Ошибка работы с базой данных";
	err->msg->Text=msg;
	err->ShowModal();
	delete err;
};



// odbcEnvironment *****************************************************************
RETCODE odbcEnvironment::Check(RETCODE i, AnsiString Message1, AnsiString Message2){
	if (i!=SQL_SUCCESS){
			UCHAR buf[1024];
		UCHAR FAR *szSqlState=buf;
		SDWORD fNativError;
		UCHAR szErrorMsg[SQL_MAX_MESSAGE_LENGTH+1];
		SWORD cbErrorMsgMax=SQL_MAX_MESSAGE_LENGTH;
		SWORD cbErrorMsg;
			buf[0]=0;

		ErrorMessageData *ed;
		TODBCException *t=new TODBCException();
		t->Message1=Message1;
		t->Message2=Message2;

		while	(SQLError(hEnv, SQL_NULL_HDBC, SQL_NULL_HSTMT,szSqlState,
				 &fNativError, szErrorMsg,
				 cbErrorMsgMax, &cbErrorMsg)==SQL_SUCCESS){

			ed=new ErrorMessageData((char*)szSqlState,fNativError,(char*)szErrorMsg);
			Debug("[%s] %d %s \r\n",szSqlState,fNativError,szErrorMsg);
			t->Data.Add(ed);


				 cbErrorMsgMax=SQL_MAX_MESSAGE_LENGTH;
				 szSqlState=buf;
				 buf[0]=0;
			}


		if(i==SQL_ERROR){
			//t->Show();
			throw(t);
		}

		delete t;
	}
	return i;
};


odbcEnvironment::odbcEnvironment(
							 AnsiString   PWD,
							 AnsiString   UID){
	Password=PWD;
	UserName=UID;
  Check(SQLAllocEnv(&hEnv),"AllocEnv");
};

 odbcEnvironment::~odbcEnvironment(){
	Connections.Clear();
	Check(SQLFreeEnv(hEnv),"FreeEnv");
};

odbcConnection *   odbcEnvironment::OpenConnection(AnsiString Name,AnsiString  DSN,AnsiString  UID, AnsiString  PWD, long TimeOut){
		odbcConnection * c=0;
		c = new   odbcConnection(this, DSN,
			(!UID.IsEmpty()?UID:UserName),
			(!PWD.IsEmpty()?PWD:Password),
			TimeOut
		);
   if(c)
 		Connections.Add(c,Name);
		return c;
};

// odbcConnection ******************************************************************
void odbcConnection::Close(){
		if(parent)
			parent->Connections.Destroy(this);
};




RETCODE odbcConnection::Check(RETCODE i, AnsiString Message1, AnsiString Message2){
	//Debug(" (RETCODE = %i)\r\n",i);
	if (i!=SQL_SUCCESS){

		SDWORD fNativError;
		UCHAR szErrorMsg[SQL_MAX_MESSAGE_LENGTH+1];
		SWORD cbErrorMsgMax=SQL_MAX_MESSAGE_LENGTH;
		SWORD cbErrorMsg;
				UCHAR buf[1024];
			UCHAR FAR *szSqlState=buf;
				buf[0]=0;

		ErrorMessageData *ed;
		TODBCException *t=new TODBCException();
		t->Message1=Message1;
		t->Message2=Message2;

		while	(SQLError(parent->hEnv, hDBC, SQL_NULL_HSTMT,szSqlState,
									 &fNativError, szErrorMsg,
									 cbErrorMsgMax, &cbErrorMsg)==SQL_SUCCESS){

				ed=new ErrorMessageData((char*)szSqlState,fNativError,(char*)szErrorMsg);
				Debug("[%s] %d %s \r\n",szSqlState,fNativError,szErrorMsg);
				t->Data.Add(ed);

				cbErrorMsgMax=SQL_MAX_MESSAGE_LENGTH;
				szSqlState=buf;
				buf[0]=0;
		}


		if(i==SQL_ERROR){
			//t->Show();
			throw(t);
		}

		delete t;
	}
	return i;
};

odbcConnection::odbcConnection(odbcEnvironment * _parent, AnsiString  _DSN,
	AnsiString  _UID, AnsiString  _PWD, long LT){
	DSN=_DSN;
	UID=_UID;
	PWD=_PWD;
	parent=_parent;
	hDBC=SQL_NULL_HDBC;
  try{
 	 Check(SQLAllocConnect(parent->hEnv, &hDBC),"AllocConnect");
  }catch(...){
  	hDBC=SQL_NULL_HDBC;
    throw;
  }
  try{
	 Check(SQLSetConnectOption(hDBC, SQL_LOGIN_TIMEOUT, LT),"ConnectOptions");
  }catch(...){
 	 //SQLFreeConnect(hDBC);
   //throw;
  }

  try{
	 Check(SQLConnect(hDBC, (UCHAR*)DSN.c_str(), SQL_NTS,(UCHAR*)UID.c_str(),SQL_NTS ,(UCHAR*)PWD.c_str(),SQL_NTS),"Connect",DSN+" "+UID);
  }catch(...){
 	 SQLFreeConnect(hDBC);
   throw;
  }

	ReadWrite();
};

odbcConnection::~odbcConnection(){
	 StatementItems.Clear();
   PreparedItems.Clear();
	Check(SQLDisconnect(hDBC),"DisConnect");
	Check(SQLFreeConnect(hDBC),"FreeConnect");
};

long odbcConnection::Execute(AnsiString SQL){
	long retcode;
	odbcStatement *p;
	p= new odbcStatement(this,"",0);
	p->IncUsage();
  try{
  p->SetOptions(SQL_RETRIEVE_DATA,SQL_RD_OFF);
  }catch(...){}
 try {
 	retcode=Check(p->Execute(SQL),"ExecDirect",SQL);
 } catch(TODBCException *e){
 	p->DecUsage();
 	delete p;
	  throw;
 }
	p->DecUsage();
	delete p;
	return retcode;
};


odbcPrepared  * odbcConnection::CreatePrepared(AnsiString   SQL,long qt){
	odbcPrepared *p;
	p = new odbcPrepared(this,SQL,qt);
	PreparedItems.Add(p);
	return p;
};

odbcStatement * odbcConnection::CreateStatement(AnsiString   SQL,long qt){
  odbcStatement *p=0;

  for(int i=0;i<StatementItems.Count();i++){
  	if(!StatementItems[i]->Usage())
   	  p=StatementItems[i];
      p->NewQuery(SQL,qt);
      p->IncUsage();
      return p;
  }

  p = new odbcStatement(this,SQL,qt);
	 p->IncUsage();
  StatementItems.Add(p);
  return p;
};

void odbcConnection::FreeStatement(odbcStatement *p){
	if(StatementItems.Where(p)>=0)
   	p->DecUsage();
   //StatementItems.Destroy(p);
};

void odbcConnection::CommitTrans(){  Check(SQLTransact(parent->hEnv, hDBC, SQL_COMMIT),"CommitTrans");};
void odbcConnection::RollbackTrans(){  Check(SQLTransact(parent->hEnv, hDBC, SQL_ROLLBACK),"RollBackTrans");};


// odbcStatement *******************************************************************
long odbcStatement::Execute(AnsiString   SQL){
	RETCODE retcode;
	//Debug("\nSQL>> %s\n",SQL.c_str());
	retcode=Check(SQLExecDirect(stID,(UCHAR*)SQL.c_str(),SQL_NTS),"ExecDirect",SQL);
	return retcode;
};

RETCODE odbcStatement::Check(RETCODE i, AnsiString Message1, AnsiString Message2){
	//Debug(" RETCODE = %i\r\n",i);

	if (i!=SQL_SUCCESS){
		UCHAR buf[1024];
		UCHAR FAR *szSqlState=buf;
		SDWORD fNativError;
		UCHAR szErrorMsg[SQL_MAX_MESSAGE_LENGTH+1];
		SWORD cbErrorMsgMax=SQL_MAX_MESSAGE_LENGTH;
		SWORD cbErrorMsg;
			buf[0]=0;

		AnsiString msg;

		ErrorMessageData *ed;
		TODBCException *t=new TODBCException();
		t->Message1=Message1;
		t->Message2=Message2;

		while	(SQLError(parent->parent->hEnv, parent->hDBC, stID,szSqlState,
									 &fNativError, szErrorMsg,
									 cbErrorMsgMax, &cbErrorMsg)==SQL_SUCCESS){

				ed=new ErrorMessageData((char*)szSqlState,fNativError,(char*)szErrorMsg);
				Debug("[%s] %d %s \r\n",szSqlState,fNativError,szErrorMsg);
				t->Data.Add(ed);

				cbErrorMsgMax=SQL_MAX_MESSAGE_LENGTH;
				szSqlState=buf;
				buf[0]=0;
		}


		if(i==SQL_ERROR){
			//t->Show();
			throw(t);
		}

		delete t;
	}
	return i;
};

odbcStatement::odbcStatement(odbcConnection * p, AnsiString   _SQL, long TimeOut){
	 //Debug("odbcStatement::odbcStatement\n");
	 stID=SQL_NULL_HSTMT;
	 parent=p;
	 usageCount=0;
	 NewQuery(_SQL,TimeOut);
	 //Debug("stID=%ld\n",stID);
};


void odbcStatement::NewQuery(AnsiString   _SQL, long TimeOut){
	SQL=_SQL;
	if(stID==SQL_NULL_HSTMT){
		Check(SQLAllocStmt(parent->hDBC, &stID),"AllocStatement");
        try{
		    Check(SQLSetStmtOption(stID,SQL_QUERY_TIMEOUT,TimeOut),"SetOption TIMEOUT");

        }catch(TODBCException *e){
            //e->Show();
        }
    	    SetOption();
	 }else{
		 Check(SQLFreeStmt(stID,SQL_UNBIND),"UnbindStatement");
		 Check(SQLFreeStmt(stID,SQL_CLOSE),"CloseStatement");
	 }
	 Init();
	 Setup();
};


void odbcStatement::SetOption(){
    try {
	Check(SQLSetStmtOption(stID,SQL_MAX_LENGTH,MAXMEMO),"SetOption MAXLEN");
    }catch(TODBCException *e){
            //e->Show();
        }
    try{
	Check(SQLSetStmtOption(stID,SQL_BIND_TYPE,SQL_BIND_BY_COLUMN),"SetOption BINDBYCOLUMN");
    }catch(TODBCException *e){
            e->Show();
        }
    try{
	Check(SQLSetStmtOption(stID,SQL_NOSCAN,SQL_NOSCAN_ON),"SetOption NOSCAN");
    }catch(TODBCException *e){
            //e->Show();
        }
    try{
	CursorType(Forward);
    }catch(TODBCException *e){
            e->Show();
        }
    try{
	LockType(Read);
    }catch(TODBCException *e){
            e->Show();
        }
    try{
	BookmarksOff();
    }catch(TODBCException *e){
            //e->Show();
        }
};

void odbcStatement::Init(){
	 //Debug("odbcStatement::Init\n");
	if(!SQL.IsEmpty()){
		//Debug("\nSQL>> %s\n",SQL.c_str());
		Check(SQLExecDirect(stID,(UCHAR*)SQL.c_str(),SQL_NTS),"ExecDirect",SQL);
	}
};

void odbcStatement::Setup(){
	 //Debug("odbcStatement::Setup\n");
	odbcField* f;
	UCHAR szColName[81];
	UCHAR szOwnerName[81];
	SWORD cbColNameMax = 80;
	SWORD cbColName;
	SWORD fSqlType;
	UDWORD cbColDef;
	SWORD ibScale;
	SWORD fNullable;
	SDWORD colLen;
	SDWORD colPrec;
	SDWORD Updateable;
	SDWORD AutoInc;
	fields.Clear();
	cols =0;
	if(SQL.IsEmpty())
		return;

	Check(SQLNumResultCols(stID, &cols),"NumCols");
	for( int i =1; i<=cols;i++){
		Check(SQLDescribeCol(stID, i, szColName, cbColNameMax, &cbColName,
								&fSqlType, &cbColDef, &ibScale, &fNullable),"DescribeCol");

			szColName[cbColName]=0;
			szOwnerName[0]=0;

		 /*
			Check(SQLColAttributes(stID, i, SQL_COLUMN_TABLE_NAME
			, szOwnerName, 80, &cbColName , 0)),"ColAttributes TABLENAME");
			szOwnerName[cbColName]=0;
		 */

		Check(SQLColAttributes(stID, i, SQL_COLUMN_AUTO_INCREMENT
			, NULL, 0, &cbColName, &AutoInc),"ColAttributes AUTOINC");

		Check(SQLColAttributes(stID, i, SQL_COLUMN_UPDATABLE
			, NULL, 0, &cbColName, &Updateable),"ColAttributes UPDATABLE");

		 Check(SQLColAttributes(stID, i, SQL_COLUMN_LENGTH
			, NULL, 0, &cbColName, &colLen),"ColAttributes COLLENGTH");

		 Check(SQLColAttributes(stID, i, SQL_COLUMN_PRECISION
			, NULL, 0, &cbColName, &colPrec),"ColAttributes COLPRECISION");

		f = new odbcField(this,i,SQLtoC(fSqlType),fSqlType,colLen,ibScale,colPrec,fNullable,AutoInc,Updateable); //,szOwnerName);

		Check(SQLBindCol(stID, i, f->value.ctype, f->value.F.fPtr, f->realLen, &f->realLen),"BindCol");
		fields.Add(f,i,(char*)szColName);
	}
};

bool odbcStatement::MoreResults(){
	 RETCODE ret;
	 ret = SQLMoreResults(stID);
	 if (ret==SQL_STILL_EXECUTING)
		while(ret==SQL_STILL_EXECUTING)
				ret=SQLMoreResults(stID);
	 if (ret ==SQL_NO_DATA_FOUND) return false;
	 if (ret==SQL_ERROR || ret==SQL_INVALID_HANDLE) return false;

	 // освобождаем все это дело от прошлого Setup
	 Check(SQLFreeStmt(stID,SQL_UNBIND),"FreeStatement","UNBIND");
	 //SQLFreeStmt(stID,SQL_CLOSE),"FreeStatement","CLOSE");
	Setup();
	return true;
};

odbcStatement::~odbcStatement(){
	if (stID!=SQL_NULL_HSTMT){
		 Check(SQLFreeStmt(stID, SQL_DROP),"FreeStatement","DROP");
	}
};

void odbcStatement::DebugPrint(){
/*
#if DebugVersion
		char buf[MAXMEMO+2];

		for (int i =1 ;i<=cols;i++){
			CharToOem(fields.Get(i)->value.printValue(buf),buf);
			cout << buf;
			cout <<" | ";
		}
		cout << endl;
#endif
*/
};

void odbcStatement::IgnoreAllFields(){
	for (int i = 0; i<fields.Count();i++){
		fields.GetI(i)->UnUpdatable();
	}
};

void odbcStatement::EnableAllFields(){
	for (int i = 0; i<fields.Count();i++){
		fields.GetI(i)->Updatable();
	}
};

RETCODE  odbcStatement::Mover(UWORD Type,SDWORD irow) {
		//Debug("Move");
		RETCODE r;
		UWORD array[StdRowSetSize+1];
		UDWORD readed;
		EnableAllFields();
		r = Check(SQLExtendedFetch(stID,Type,irow,&readed,array),"Move");
		return r;
};

void odbcStatement::ManualTrans(){
	parent->ManualTrans();
};

void odbcStatement::AutoTrans(){
	parent->AutoTrans();
};

void odbcStatement::CommitTrans(){
	parent->CommitTrans();
};

void odbcStatement::RollbackTrans(){
	parent->RollbackTrans();
};

// odbcPrepared ********************************************************************
void odbcPrepared::Init(void){
     Check(SQLPrepare(stID, (UCHAR*)SQL.c_str(), SQL_NTS));
     long np = NumParameters(),i;
     SWORD  fSqlType,  ibScale, fNullable;
     unsigned long cbColDef;
     odbcField *f;
	  for(i=1; i++;i<=np){
    	  SQLDescribeParam(stID, i, &fSqlType, &cbColDef, &ibScale, &fNullable);
	 	  f = new odbcField(this,i,SQLtoC(fSqlType),fSqlType,cbColDef,0,0,fNullable,ibScale,1);
		  param.Add(f,i,"");
     }
};

void odbcPrepared::SetParamType(long ParamID, ParameterType Type){
	odbcField *f;
   f = param(ParamID);
   if (f){
  	  //Debug("BindParam %i",ParamID);
     Check(
     	SQLBindParameter(
     		stID, ParamID, Type, f->value.ctype, f->sqlType,
         f->defLen, f->AutoInc, f->value.F.fBinary, f->defLen, &(f->realLen)
      )
     );
	  //Debug("realLen = %d\r\n" ,f->realLen);
  }
};

long odbcPrepared::Call(){
	long r=  Check(SQLExecute(stID));
 	Setup();
  return r;
};

odbcPrepared::odbcPrepared(odbcConnection * p, AnsiString   _SQL,long TimeOut){
  SQL=_SQL;
	parent=p;
	stID=SQL_NULL_HSTMT;
	Check(SQLAllocStmt(parent->hDBC, &stID));
	//Debug("TimeOut ");
	Check(SQLSetStmtOption(stID,SQL_QUERY_TIMEOUT,TimeOut));
   SetOption();
   Init();
};

odbcPrepared::~odbcPrepared(){
   param.Clear();
};



// odbcField ***********************************************************************
long SQLtoC(long type){
	switch(type){
		case SQL_CHAR:       	return 		SQL_C_CHAR;     // char
		case SQL_VARCHAR: 		return 		SQL_C_CHAR;
		case SQL_LONGVARCHAR: return 		SQL_C_CHAR;

		case SQL_BIT: 				return 		SQL_C_DOUBLE;
		case SQL_BIGINT: 			return 		SQL_C_DOUBLE;
		case SQL_DECIMAL: 		return 		SQL_C_DOUBLE;   // double
		case SQL_NUMERIC: 		return 		SQL_C_DOUBLE;
		case SQL_REAL: 			  return 		SQL_C_DOUBLE;
		case SQL_FLOAT: 			return 		SQL_C_DOUBLE;
		case SQL_DOUBLE: 			return 		SQL_C_DOUBLE;
		case SQL_TINYINT: 		return 		SQL_C_DOUBLE;
		case SQL_SMALLINT: 		return 		SQL_C_DOUBLE;
		case SQL_INTEGER: 		return 		SQL_C_DOUBLE;

		case SQL_BINARY: 			  return 		SQL_C_BINARY;   // binary
		case SQL_VARBINARY: 		return 		SQL_C_BINARY;
		case SQL_LONGVARBINARY: return 		SQL_C_BINARY;

		case SQL_DATE: 			  return 		SQL_C_TIMESTAMP;     // time info
		case SQL_TIME: 			  return 		SQL_C_TIMESTAMP;
		case SQL_TIMESTAMP:   return 		SQL_C_TIMESTAMP;
	}
	return 0;
};


odbcField::odbcField(odbcStatement* _parent,
				 long _ID,
				 long _cType,
				 long _sqlType,
				 long _realLen,
				 long _Scale,
				 long _Prec,
				 SWORD _Nullable,
				 SWORD _AutoInc,
				 SWORD _Updateable
	 ){
		 parent = _parent;
		 sqlType=_sqlType;
		 realLen=_realLen;
		 Prec=_Prec;
		 Scale=_Scale;
		 Nullable=_Nullable;
		 AutoInc = _AutoInc;
		 ID=_ID;
		 fieldUpdateable = _Updateable;

		 switch(_cType){
			 case SQL_C_CHAR:
							 realLen *=2;
							 if (realLen<=0 || realLen>MAXMEMO) realLen = MAXMEMO;
							 realLen++;
							 value.Allocate(_cType,realLen);
					break;
					default:
							 value.Allocate(_cType);
			 };
			 defLen=value.len;
			 value.Null=false;
};

odbcField::~odbcField(){
};


long odbcField::GetChunk(void * buffer, long bufSize){ // возвращает число реально считанных байт
	RETCODE ret;
	 long tmpSize=bufSize;
	if (ID>0 && (value.ctype ==SQL_C_CHAR ||value.ctype ==SQL_C_BINARY)){
		 do {
				ret = SQLGetData(parent,ID,value.ctype,buffer,bufSize,&tmpSize);
			 } while (ret ==SQL_STILL_EXECUTING);
			 if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO){
				return tmpSize;
			 }else{
				return 0;
			 }

   }else
   	return 0;
};

void *odbcField::GetBinaryPrt(){
	if (value.ctype == SQL_C_BINARY)
    return value.F.fPtr;
   else
   	return NULL;
};

long odbcField::GetLastSize(){
	if (realLen > 0)
   	return realLen;
   else
   	return 0;
};



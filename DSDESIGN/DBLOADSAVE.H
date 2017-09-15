//---------------------------------------------------------------------------
#ifndef dbLoadSaveH
#define dbLoadSaveH
#include "bamilib.h"
#include "task.h"



enum DbAccess{
	 daRead =  0,
	 daWrite = 1
  //,daAppend =2
};

class DbStream: public SimpleStream{
 AnsiString TaskName;
 ODBCDataSource *ds;
 odbcConnection *c;
 odbcStatement *Stmt;

 StackValue TaskID;
 DbAccess CurAccess;
	long RecordPtr;
 AnsiString MakeHex(unsigned char Len, void * buf);

public:
	DbStream( AnsiString _TaskName,       AnsiString _ConnectionName,
           DataManager *_Manager,      DbAccess _CurAccess);

 ~DbStream();
	virtual unsigned char Write(unsigned char Len, void * buf);
 virtual unsigned char Read(unsigned char Len, void * buf);

};


//---------------------------------------------------------------------------
#endif

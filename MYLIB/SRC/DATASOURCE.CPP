//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "bamilib.h"
#include "login.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

DataStream * DataSourceBase::GetStream(StreamEditorDef* Editor,AnsiString *NewSubCondition,AnsiString *OrderBy){
 for(long i=0;i<Streams.Count();i++)
	if( CheckDataStream( Streams[i], Editor, NewSubCondition,OrderBy)){
				Streams[i]->IncUsage();
			return Streams[i];
		}
 return MakeNewStream(Editor, NewSubCondition,OrderBy);
};

void DataSourceBase::KillStream(DataStream * DS){
	if(Streams.Where(DS)>=0){
			if(!DS->DecUsage())
					Streams.Destroy(DS);
		}
};



DataStream * DataManager::GetNewStream( AnsiString EditorName, AnsiString *NewSubCondition,AnsiString *OrderBy ){
		return GetNewStream( Editors(EditorName) ,NewSubCondition,OrderBy);
};
DataStream * DataManager::GetNewStream( StreamEditorDef * SE, AnsiString *NewSubCondition,AnsiString *OrderBy){
	if(!SE) return 0;
		ODBCDataSource *sb;
		sb=(ODBCDataSource *)Sources(SE->ConnectionName);

   if(sb && !sb->FullLogOn(SE->ConnectionName)){
         return 0;
   }

	 if(!sb) return 0;
	 return sb->GetStream(SE, NewSubCondition,OrderBy);
};


SimpleStream & operator >> (SimpleStream & s,DataManager &m){
            AnsiString Terminator;
			s >> m.Sources >> m.Editors >> m.Styles >> m.Links >> m.Trans >> Terminator ;
            //if(Terminator!=VER)
            //   MessageBox(0,"Ошибка загрузки","ВНИМАНИЕ",MB_OK);
      return s;
		};

SimpleStream & operator << (SimpleStream & s,DataManager &m){
					return s << m.Sources << m.Editors << m.Styles << m.Links << m.Trans << VER;
		};



odbcEnvironment *ODBCDataSource::ODBC=0;

ODBCDataSource::~ODBCDataSource(){
	if(c)
			c->Close();
	if(ODBC && !ODBC->Connections.Count()){
			delete ODBC;
				ODBC=0;
		}
};


void ODBCDataSource::LogOff(){
	if(c){
		c->Close();
		c=0;
	}
	if(ODBC && !ODBC->Connections.Count()){
			delete ODBC;
			ODBC=0;
	}
	LoggedOn=false;
};



DataStream * ODBCDataSource::MakeNewStream(StreamEditorDef* e,  AnsiString *NewSubCondition,AnsiString *OrderBy){
	DataStream* ds;

 if(LoggedOn && c){
		ds=new ODBCDataStream(c,e->SQL,e->ROWSQL,e->INSERT,e->UPDATE,e->sDELETE,
		(NewSubCondition)?*NewSubCondition:e->SubCondition,
		(OrderBy)?*OrderBy:e->OrderBy
		);

		// записали параметры
		for(int j=0;j<e->ParamNames.Count();j++){
			ds->params.Add( new StackValue,e->ParamNames[j]->c_str());
		}

				/*
		// записали значения по умолчанию
		for(int j=0;j<e->Defaults.Count();j++){
			ds->defaults.Add( new StackValue(e->Defaults[j]),e->Defaults.Name(j));
		}

				if(NewDefaults){
						// записали значения по умолчанию
			for(int j=0;j<NewDefaults.Count();j++){
							for(int i=0;i< ds->defaults.Count();i++)
									if(ds->defaults.Name(i)==NewDefaults.Name(j)){
												*(ds->defaults[i])=*(NewDefaults[j]);
												goto nxtDef;

										}
								}
						ds->defaults.Add( new StackValue(NewDefaults[j]),NewDefaults.Name(j));
							 nxtDef:
			}
				}
				*/


	    AddDataStream(ds);
		return ds;
	}
	return 0;
};

bool ODBCDataSource::CheckDataStream( DataStream *o, StreamEditorDef *e,  AnsiString *NewSubCondition,AnsiString *OrderBy){
 ODBCDataStream*s=  (ODBCDataStream*)o;
 if(
		(NewSubCondition?s->SubCondition==*NewSubCondition:s->SubCondition==e->SubCondition) &&
		(OrderBy?s->OrderBy==*OrderBy:s->OrderBy==e->OrderBy) &&
		(e->SQL==s->SQL) &&
		(e->ROWSQL==s->ROWSQL) &&
		(e->INSERT==s->INSERT) &&
		(e->UPDATE==s->UPDATE) &&
		(e->sDELETE==s->sDELETE)
	 ){

	// проверяем наличие параметров
	for(int j=0;j<e->ParamNames.Count();j++){
		for(int i=0;i<s->params.Count();i++){
			if(s->params.Name(i)==*e->ParamNames[i])
				goto nxtfld;
		}
		return false;
	nxtfld:
	}
		/*
	// проверяем наличие УМОЛЧАНИЙ
	for(int j=0;j<e->Defaults.Count();j++){
		for(int i=0;i<s->defaults.Count();i++){
			if(s->defaults.Name(i)==e->Defaults.Name(i))
				goto nxtflddef;
			}
			return false;
		nxtflddef:
		}
    */
		return true;
	//}

    }
	return false;
};

bool ODBCDataSource::FullLogOn( AnsiString Name){
   if(!IsLoggedOn()){
     AnsiString UID;
     AnsiString PWD;
     {
       MyFile ff ((Application->ExeName +".usr").c_str(),acRead);
       ff>>UID;
     }

     TfrmLogin *frmLogin = new TfrmLogin(Application);
     frmLogin->SourceName=Name;
     while(!IsLoggedOn()){
       frmLogin->txtUID->Text=UID;
       frmLogin->txtPWD->Text=PWD;
       if(frmLogin->ShowModal()==mrOk){
         UID=frmLogin->txtUID->Text;
         PWD=frmLogin->txtPWD->Text;
         LogOn(UID,PWD);
       }else{
         delete frmLogin;
         return IsLoggedOn();
       }
     }
     delete frmLogin;

     {
       MyFile ff ((Application->ExeName +".usr").c_str(),acWrite);
       ff << UID;
     }


 }
 return IsLoggedOn();
}




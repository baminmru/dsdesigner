//---------------------------------------------------------------------------
#ifndef DataSourceH
#define DataSourceH

class DataManager;

STREAM_LIST_DEF(StreamEditorDef, StreamEditors);
STREAM_ARRAY_DEF(DataStream, DataStreams);

class DataSourceBase{
	protected:
		DataManager *Owner;
		DataStreams Streams;
		bool LoggedOn;
		void AddDataStream(DataStream * DS){
			DS->Owner=this;
				DS->IncUsage();
				Streams.Add(DS);
		};

		virtual DataStream * MakeNewStream(StreamEditorDef* Editor,AnsiString *SubCondition,AnsiString *OrderBy){
			 if(!LoggedOn) return 0;
				DataStream * DS=new DataStream();
				AddDataStream(DS);
			return DS;
		};

		virtual bool CheckDataStream( DataStream *o, StreamEditorDef *s,AnsiString *NewSubCondition,AnsiString *OrderBy){
			return true;
		};

	public:
		DataSourceBase(){
			LoggedOn=false;
		}
		bool IsLoggedOn(){
			return LoggedOn;
		};
		void SetOwner(DataManager *_Owner){Owner=_Owner;};

		DataStream * GetStream(StreamEditorDef* Editor,AnsiString *NewSubCondition,AnsiString *OrderBy);
		void KillStream(DataStream * DS);
		virtual bool LogOn(AnsiString UID, AnsiString PWD){LoggedOn=true; return LoggedOn;};
		virtual void LogOff(){LoggedOn=false;};

		friend SimpleStream & operator >> (SimpleStream & s,DataSourceBase &b){
			return s ;
		};
		friend SimpleStream & operator << (SimpleStream & s,DataSourceBase &b){
					return s ;
		};

};


class ODBCDataSource: public DataSourceBase{

		static odbcEnvironment *ODBC;
		virtual DataStream * MakeNewStream(StreamEditorDef* Editor,AnsiString *NewSubCondition,AnsiString *OrderBy);
		virtual bool CheckDataStream( DataStream *o, StreamEditorDef *s,AnsiString *NewSubCondition,AnsiString *OrderBy);

		AnsiString DSN;
   AnsiString CurUID;
   AnsiString CurPWD;
		odbcConnection *c;
	public:

		friend SimpleStream & operator >> (SimpleStream & s,ODBCDataSource &o){
			return s >> o.DSN ;
		};
		friend  SimpleStream & operator << (SimpleStream & s,ODBCDataSource &o){
			return s << o.DSN ;
		};


		ODBCDataSource(AnsiString _DSN=""):DataSourceBase(){
			DSN=_DSN;
			c=0;
		};

		~ODBCDataSource();

   bool FullLogOn(AnsiString Name);

		virtual bool LogOn(AnsiString UID, AnsiString PWD){
			if(!c){
			 try{
					if(!ODBC)
						ODBC= new odbcEnvironment();
			 }catch(...){
					LoggedOn=false;
					return false;
			 }
			 try{
         c=0;
					c=ODBC->OpenConnection(DSN,DSN,UID,PWD,30);
					LoggedOn=true;
         CurUID=UID;
         CurPWD=PWD;
			 }catch(TODBCException *e){
             if(c) ODBC->Kill(c);
							c=0;
							LoggedOn=false;
							e->Show();
			 }
			}
			return LoggedOn;
		};

		virtual void LogOff();

		AnsiString GetDSN(){ return DSN;};
	  AnsiString GetUID(){ return CurUID;};
   AnsiString GetPWD(){ return CurPWD;};

		odbcConnection *GetConnection(){
			if(LoggedOn) return c;
			return 0;
		};

};


STREAM_LIST_DEF(ODBCDataSource,DataSourceBag);

class DataManager{
 public:

		DataSourceBag Sources;
		StreamEditors Editors;
		MyList<GridCellStyle> Styles;
   MyArray<StreamEditorLink> Links;
   StreamTransactions Trans;

		friend SimpleStream & operator >> (SimpleStream & s,DataManager &m);

		friend SimpleStream & operator << (SimpleStream & s,DataManager &m);

		void AddSource(ODBCDataSource * DS,	AnsiString Name){
			DS->SetOwner(this);
				Sources.Add(DS,Name);
		};

		void AddEditor(StreamEditorDef * SE,AnsiString Name){
			SE->Owner=this;
				Editors.Add(SE,Name);
		};

		void DelSource(AnsiString Name){
				Sources.Destroy(Sources.Where(Name));
		};

		void DelEditor(AnsiString Name){
				Editors.Destroy(Editors.Where(Name));
		};

		void DelSource(ODBCDataSource * DS){
				Sources.Destroy(Sources.Where(DS));
		};

		void DelEditor(StreamEditorDef * SE){
				Editors.Destroy(Editors.Where(SE));
		};


		// проверить не совпадает ли определение потока с имеющимся потоком
		DataStream * GetNewStream( AnsiString EditorName,AnsiString *NewSubCondition,AnsiString *OrderBy );
		DataStream * GetNewStream( StreamEditorDef * SE ,AnsiString *NewSubCondition,AnsiString *OrderBy);

		void KillStream(DataStream * DS){
			DS->Owner->KillStream(DS);
		};

		StreamEditorDef * GetEditor(AnsiString Name){
				return Editors(Name);
		};

		ODBCDataSource * GetSource(AnsiString Name){
				return Sources(Name);
		};
};

#define VER "DataManager V1.0 b"

//---------------------------------------------------------------------------
#endif

#ifndef ODBCDATASTREAM_H
#define  ODBCDATASTREAM_H



// ODBCDataStream
////////////////////////////////////////////////////////////////////////////////
class ODBCDataStream : public DataStream {
		odbcConnection *Connect;
		AnsiString Parse(AnsiString Expression);
		AnsiString GetValue(AnsiString Name);

		AnsiString Parse(AnsiString Expression,Params *Data);
		AnsiString GetValue(AnsiString Name,Params *Data);

		AnsiString   ParseBuffer;
		AnsiString   ValueBuffer;
		odbcStatement *s;

	public:

		odbcConnection *GetConnection(){ return Connect; };

		AnsiString   SQL;
		AnsiString   ROWSQL;
		AnsiString   INSERT;
		AnsiString   UPDATE;
		AnsiString   sDELETE;

		virtual bool AllowInsert(){ return !(SQL.IsEmpty() || INSERT.IsEmpty()); };
		virtual bool AllowUpdate(){ return !(SQL.IsEmpty() || ROWSQL.IsEmpty() || UPDATE.IsEmpty()); };
		virtual bool AllowDelete(){ return !(SQL.IsEmpty() || ROWSQL.IsEmpty() || sDELETE.IsEmpty()); };
		virtual void SnapInit(DataSnapshot* ds);
   virtual bool MakeAction(AnsiString Action, Params *Data);

		void Refresh();
		void RefreshRow();
		bool Updateing();
		bool Deleteing();
		bool Inserting();
		bool FetchNext();

		ODBCDataStream( odbcConnection * c,
										 const AnsiString  _SQL,
										 const AnsiString  _ROWSQL="",
										 const AnsiString  _INSERT="",
										 const AnsiString  _UPDATE="",
										 const AnsiString  _DELETESTR="",
										 const AnsiString  _SubCondition="",
										 const AnsiString  _OrderBy="" );

		~ODBCDataStream();
};

#endif
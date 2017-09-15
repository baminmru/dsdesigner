#include "bamilib.h"

//ODBCDataStream
////////////////////////////////////////////////////////////////////////////////
ODBCDataStream::ODBCDataStream( odbcConnection * c,   const AnsiString  _SQL,  const AnsiString  _ROWSQL,
							const AnsiString  _INSERT,   const AnsiString  _UPDATE, const AnsiString  _DELETESTR,
							const AnsiString  _SubCondition,const AnsiString  _OrderBy	){
						Connect=c;
						ParseBuffer="";
						ValueBuffer="";
						SQL=_SQL;
						SubCondition=_SubCondition;
						OrderBy=_OrderBy;
						ROWSQL=_ROWSQL;
						INSERT=_INSERT;
						UPDATE=_UPDATE;
						sDELETE=_DELETESTR;
						s=NULL;

				 };


ODBCDataStream::~ODBCDataStream(){
				 if(Connect && s){
						Connect->StatementItems.Destroy(s);
						s=NULL;
				 }
			};


void ODBCDataStream::Refresh(){
	DataStream::Refresh();

	if (SQL.IsEmpty() || !Connect) return;

	Parse(SQL+" "+SubCondition+" " +OrderBy);


	 if(s){
		Connect->FreeStatement(s);
			s=NULL;
	 }

	 s = Connect->CreateStatement(ParseBuffer);

	 if (s){
		 fields.Clear();
		 for(int j=0;j<s->fields.Count();j++){
			// Форируем новое поле
			fields.Add(new StackValue(s->fields[j]->GetCont()->_StackValue()),0,s->fields.Name(j));

		 }
	 }
};

void ODBCDataStream::RefreshRow(){
	if (ROWSQL.IsEmpty() || !Connect) return;
	DataStream::RefreshRow();
	Parse(ROWSQL);
 try {
	 if(s){
		Connect->FreeStatement(s);
			s=NULL;
	 }

	 s = Connect->CreateStatement(ParseBuffer);

  }catch(TODBCException *e){
     e->Show();
     throw;
  }

	  if(s){
			while(FetchNext());
		}

};

bool ODBCDataStream::Updateing(){
	if (UPDATE.IsEmpty() || !Connect) return false;
	Parse(UPDATE);
	RETCODE r= Connect->Execute(ParseBuffer);
	return (r ==SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO);
};

bool ODBCDataStream::Deleteing(){
	if (sDELETE.IsEmpty() || !Connect) return false;
	Parse(sDELETE);
	RETCODE r= Connect->Execute(ParseBuffer);
	return (r ==SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO);
};

bool ODBCDataStream::Inserting(){
	if (INSERT.IsEmpty()  || !Connect) return false;

	Parse(INSERT);
	RETCODE r= Connect->Execute(ParseBuffer);
	return (r ==SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO);

};

bool ODBCDataStream::FetchNext(){
	 RETCODE r;
	if(Connect && s){
		r=s->MoveNext();
			if(r==SQL_NO_DATA_FOUND){
			Connect->FreeStatement(s);
				s=NULL;
				return false;
			}
			if(r ==SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO){
			for(int j=0;j<s->fields.Count();j++){
					if( s->fields[j] && s->fields[j]->GetCont()){
						delete fields[j];

						if(s->fields[j]->IsNull())
							fields.Set(j,new StackValue());
						else
							fields.Set(j,new StackValue(s->fields[j]->GetCont()->_StackValue()));
					}
			 }
					return true;
			}
	 }
	 return false;
};

AnsiString   ODBCDataStream::Parse(AnsiString  Expression){

	 AnsiString NameBuffer;
	 bool mode=false;
	 ParseBuffer = "";
	 for(long i=1;i<=Expression.Length();i++){
		if (Expression[i]=='^'){
				mode = !mode;
				 if(mode){
						NameBuffer="";
				 }else{
						ParseBuffer=ParseBuffer+GetValue(NameBuffer);
				 }
			}else{
				if(mode){
					NameBuffer=NameBuffer+Expression[i];
				 }else{
					ParseBuffer=ParseBuffer+Expression[i];
				 }
			}
	 }
	 return ParseBuffer;
};


AnsiString   ODBCDataStream::Parse(AnsiString  Expression,Params *Data){

	 AnsiString NameBuffer;
	 bool mode=false;
	 ParseBuffer = "";
	 for(long i=1;i<=Expression.Length();i++){
		if (Expression[i]=='^'){
				mode = !mode;
				 if(mode){
						NameBuffer="";
				 }else{
						ParseBuffer=ParseBuffer+GetValue(NameBuffer,Data);
				 }
			}else{
				if(mode){
					NameBuffer=NameBuffer+Expression[i];
				 }else{
					ParseBuffer=ParseBuffer+Expression[i];
				 }
			}
	 }
	 return ParseBuffer;
};


AnsiString ODBCDataStream::GetValue(AnsiString Name){
	for(int i=0 ;i<fields.Count();i++){
		if(Name==fields.Name(i)){
				 //Debug("\nGetValue %s-->%s",Name.c_str(), (fields[i]->ToSQLStr()).c_str());
				 return   fields[i]->ToSQLStr();
			}
	 }
	for(int i=0 ;i<params.Count();i++){
		if(Name==params.Name(i)){
				 //Debug("\nGetValue %s-->%s",Name.c_str(), (params[i]->ToSQLStr()).c_str());
				 return params[i]->ToSQLStr();
			}
	 }
	return "";
};

AnsiString ODBCDataStream::GetValue(AnsiString Name,Params *Data){
	for(int i=0 ;i<Data->Count();i++){
		if(Name==Data->Name(i)){
				 return   (*Data)[i]->ToSQLStr();
			}
	 }
	return "";
};



void ODBCDataStream::SnapInit(DataSnapshot* ds){
/*
	ds->ClearFieldDef();
	for (int i=0;i<s->fields.Count();i++)
		ds->AddFieldDef(
			s->fields.Name(i),
			new SnapFieldDef((s->fields[i])->IsNullable(),(s->fields[i])->GetPrecision())

		);
*/
};

bool ODBCDataStream::MakeAction(AnsiString Action,Params *Data){
	if (Action.Trim().IsEmpty() || !Connect) return false;
	Parse(Action,Data);
 RETCODE r;
 try{
 	r= Connect->Execute(ParseBuffer);
 }catch (TODBCException *e){
   e->Show();
   return false;
 }
	return (r ==SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO);
};


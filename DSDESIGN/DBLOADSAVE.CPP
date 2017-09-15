//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "dbLoadSave.h"


AnsiString DbStream::MakeHex(unsigned char Len, void * buf){
   unsigned char *ptr=(unsigned char *) buf;
   char buff[10];
   AnsiString out;
   for(long i=0;i<Len;i++){
     itoa((int)ptr[i],buff,16);
     if(!buff[1])
       out=out+'0'+buff;
     else
       out=out+buff;
   }
   return out;
};


DbStream::DbStream(
           AnsiString _AppName,
           AnsiString _ConnectionName,
           DataManager *Manager,
           DbAccess _CurAccess
 ){
      ds=0;
      ds = Manager->Sources.Get(_ConnectionName);
      if(!ds){
       MessageBox(0,_ConnectionName.c_str(),"ќписание источника данных не найдено!",MB_OK);
       return;
      }

      if(_CurAccess==daRead)
       ds->LogOn("lme_appserver","lme_appserver");

       if(!ds->FullLogOn(_ConnectionName)){
          MessageBox(0,_ConnectionName.c_str(),"—оединение с источником данных не установлено!",MB_OK);
          return;
       }
      CurAccess=_CurAccess;
      c =ds->GetConnection();
      if(!c){
        MessageBox(0,_ConnectionName.c_str(),"—оединение с источником данных не установлено!",MB_OK);
        return;
      }
      StackValue sv(_AppName),id;
      AnsiString Q;
      long r;
      Q="select max(LME_APPLICATIONSID) LME_APPLICATIONSID  from LME_APPLICATIONS where NAME=" + sv.ToSQLStr();

      try{
      if(CurAccess==daWrite){
         // создать описание задачи
         AnsiString I="insert into LME_APPLICATIONS(NAME) values(" + sv.ToSQLStr() +")";
         Stmt=0;
         try{
         Stmt=c->CreateStatement(Q);
         }catch(TODBCException *e){
          if(Stmt) c->FreeStatement(Stmt);
          c=0;
          e->Show();
          return;
         }

         r=Stmt->MoveNext();
         id=Stmt->fields.Data("LME_APPLICATIONSID");
         if(!(r ==SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO) ||   id.IsNull()){
              c->Execute(I);
              c->FreeStatement(Stmt);
              try{
              Stmt=c->CreateStatement(Q);
              }catch(TODBCException *e){
                 if(Stmt) c->FreeStatement(Stmt);
                  c=0;
                  e->Show();
                 return;
             }

              r=Stmt->MoveNext();
              id=Stmt->fields.Data("LME_APPLICATIONSID");
         }

         if(!(r ==SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO) ||  id.IsNull()){
          c->FreeStatement(Stmt);
          c=0;
          ds=0;
          MessageBox(0,_AppName.c_str(),"Ќе удалось обнаружить (создать) дескриптор приложени€!",MB_OK);
          return;
         }

         // очистить строки описани€
         AnsiString D="delete from LME_APP_RECORD where LME_APPLICATIONSID=" + id.ToSQLStr();
         try{
           c->Execute(D);
         }catch(TODBCException *e){
          c=0;
          e->Show();
          return;
         }
         TaskID=id;
         if(Stmt) c->FreeStatement(Stmt);
         Stmt=0;
      }else{
         try{
         Stmt=c->CreateStatement(Q);
         }catch(TODBCException *e){
          if(Stmt) c->FreeStatement(Stmt);
          c=0;
          e->Show();
          return;
         }

         r=Stmt->MoveNext();
         id=Stmt->fields.Data("LME_APPLICATIONSID");
         if(!(r ==SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO) ||  id.IsNull()){
          c->FreeStatement(Stmt);
          c=0;
          ds=0;
          MessageBox(0,_AppName.c_str(),"Ќе удалось обнаружить дескриптор приложени€!",MB_OK);
          return;
         }
         TaskID=id;

         c->FreeStatement(Stmt);
         Stmt=0;
         try{
             Stmt=c->CreateStatement("select SIZE,VALUE from LME_APP_RECORD where LME_APPLICATIONSID=" + id.ToSQLStr() + " order by RECORDID");
         }catch(TODBCException *e){
             if(Stmt) c->FreeStatement(Stmt);
             c=0;
             e->Show();
             return;
         }

      }
      }catch(TODBCException *e){
       e->Show();
       c=0;
       return;
      }

      RecordPtr=0;
 };

DbStream::~DbStream(){
 if(c && Stmt)
    c->FreeStatement(Stmt);
 c=0;ds=0;
 RecordPtr=0;
};

unsigned char DbStream::Write(unsigned char Len, void * buf){
 if(CurAccess!=daWrite ) return 0;
 if(!c || TaskID.IsNull()) return 0;
 AnsiString h=MakeHex(Len,buf);
 RecordPtr++;
 AnsiString IR="Insert into LME_APP_RECORD(LME_APPLICATIONSID,RECORDID,SIZE,VALUE) values(" + TaskID.ToSQLStr() +","+ RecordPtr +","+Len+ ",0x" +h +"AA55 )";
 try{
   c->Execute(IR);
 } catch(TODBCException* e){
   e->Show();
 }
 return Len;
};
unsigned char DbStream::Read(unsigned char Len, void * buf){
 if(CurAccess==daWrite) return 0;
 if(!c || TaskID.IsNull() && Stmt) return 0;
 memset(buf,0,Len);
 long r=Stmt->MoveNext();
 if(r ==SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO){
   unsigned char *val=Stmt->fields.Get("VALUE")->GetCont()->F.fBinary;
   long l=(long)Stmt->fields.AsNumeric("SIZE");
   if(l<0) return 0;
   if(l>Len) l=Len;
   memcpy(buf,val,l);
   return l;
 }else{
   c->FreeStatement(Stmt);
   Stmt=0;
   c=0;
 }
 return 0;
};

//---------------------------------------------------------------------------
#pragma package(smart_init)

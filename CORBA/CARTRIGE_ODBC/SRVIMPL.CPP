//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SRVImpl.h"
#include "Convertions.h"
#include "syncobjs.hpp"
#include "LZSS.h"

void srvCartrige::CloseInstance(
      CORBA::Long Instance
      ){
    InstanceStore * iptr = (InstanceStore*) Instance;
    long id=Instances.Where(iptr);
    if(id>=0){
      if(iptr->con) DisConnect(Instance);
      Instances.Destroy(id);
      cout << "Instance closed I=" << Instance << endl;
    }

};



void srvCartrige::DisConnect(
      CORBA::Long Instance
      ){

    while(Wait) ;
    Wait = true;
    InstanceStore * iptr = (InstanceStore*) Instance;
    if(Instances.Where(iptr)>=0){
        if(iptr->con){
          for(long i =0;i< Connects.Count();i++){
            if(Connects[i]->con==iptr->con){


              Connects[i]->Usage--;
              if(Connects[i]->Usage==0){
                env.Kill(iptr->con);
                Connects.Destroy(i);
              }
            }
          }
          iptr->con=0;
        }
        cout << "Disconnect I=" << Instance << endl;
      }

   Wait = false;
};



CORBA::Long srvCartrige::OpenCursor(
      CORBA::Long Instance,
      const char* Query,
      const Octets& Parameters
      ){
      odbcStatement * st=0;
    InstanceStore * iptr = (InstanceStore*) Instance;
    if(Instances.Where(iptr)>=0){
        if(iptr->con){
          try{
            st=iptr->con->CreateStatement(Query);
          }catch(TODBCException & e){
          cout << "Open cursor I=" << Instance << " fail! Q=" << Query << endl;
          };

        }
      }
      cout << "Open cursor I=" << Instance << " C=" << ((long) st) << endl;
      //delete & Parameters;
      return (long)st;
};

CORBA::Boolean srvCartrige::IsConnected(
      CORBA::Long Instance
      ){
    InstanceStore * iptr = (InstanceStore*) Instance;
    if(Instances.Where(iptr)>=0)
        if(iptr->con)
          return 1;
      return 0;
};


Octets* srvCartrige::GetRowSet(
      CORBA::Long Instance,
      const char* Query,
      const Octets& Parameters
      ){




    MyArray<FieldHeader> h;
    FieldHeader * ptr;

    odbcStatement * st=0;
    InstanceStore * iptr = (InstanceStore*) Instance;
    if(Instances.Where(iptr)>=0){

      if(iptr->con){
        try{
          st=iptr->con->CreateStatement(Query);
        }catch(TODBCException & e){
          cout << "GetRowSet I=" << Instance << " fail! Q=" << Query << endl;
          //delete & Parameters;
          return new Octets;
        };
      }
    }else{
       //delete & Parameters;
       return new Octets;
    }


    for(long i=0;i<st->fields.Count();i++){
       ptr = new FieldHeader;
       ptr->Name=st->fields.Name(i);
       ptr->AllowNull=st->fields[i]->IsNullable();
       ptr->Type=st->fields[i]->GetType();
       ptr->MaxSize=st->fields[i]->GetDefaultSize();
       h.Add(ptr);
    }

     MyArray<RowData> Rows ;
     RETCODE r;
     while(1){
       r=st->MoveNext();
       if(r==SQL_SUCCESS || r==SQL_SUCCESS_WITH_INFO){

         RowData *v = new RowData;
         for(long i=0;i<st->fields.Count();i++){
           v->Add(st->fields[i]->GetCont()->_StackValue());
         }
         Rows.Add(v);
       }else
        break;
      }
      long id =iptr->con->StatementItems.Where(st);
      if (id>=0)
        iptr->con->StatementItems.Destroy(id);

      cout << "RowSet I=" << Instance << endl;

    MyMemory m;
    m << Rows;
    //delete & Parameters;
      LZSS lz;

      DataBuffer *db=lz.Encode(m.GetBuffer(),m.BufferSize());
      Octets * oc =MakeOctets(*db);
      delete db;      
      return oc;

};



void srvCartrige::Connect(
      CORBA::Long Instance,
      const Octets& Info
      ){


    while(Wait) ;
    Wait = true;
    InstanceStore * iptr = (InstanceStore*) Instance;
    if(Instances.Where(iptr)>=0){
        if(iptr->con){
          DisConnect(Instance);
          iptr->Parameters.Clear();
        }
    }else{
     Wait = false;
     //delete & Info;
     return;
    }

      AnsiString DSN,UID,PWD,NAME;
      double T=0.0;
      MyArray<CartrigeParam> s;
      MyMemory * m = MakeMemory(Info);
      *m >> s;
      for(long i=0;i<s.Count();i++){
        if(s[i]->Name == "DSN")
          DSN=s[i]->Value.ToStr();
        if(s[i]->Name == "UID")
          UID=s[i]->Value.ToStr();
        if(s[i]->Name == "PWD")
          PWD=s[i]->Value.ToStr();
        if(s[i]->Name == "TIMEOUT")
          T=s[i]->Value.ToNumeric();
      }

      m->Restart();
      *m >> iptr->Parameters;
      delete m;
      NAME=DSN+"_|||_"+UID;
      try{
      /*
      for (long i=0;i<env.Connections.Count();i++){
        if (env.Connections.Name(i)==NAME){
          iptr->con=env.Connections[i];
          for(long j =0;j<Connects.Count();j++){
            if(Connects[j]->con==iptr->con)
              Connects[j]->Usage++;
          }
          cout << "Reuse connection N=" << NAME.c_str() << " I=" << Instance << endl;
          Wait = false;
          return;
        }
      }
      */
      iptr->con=env.OpenConnection(NAME,DSN,UID,PWD,T);
      ConnectStore *cptr = new ConnectStore;
      cptr->Usage=1;
      cptr->con=iptr->con;
      Connects.Add(cptr);
      cout << "New Connect I=" << Instance << endl;

      }catch(TODBCException & e){
        cout << "Connect I=" << Instance << " fail! " << endl;
      };

      Wait = false;
      //delete & Info;
};


Octets* srvCartrige::GetConnectInfo(){
  MyArray<AnsiString> s;
  s.Add( new AnsiString("DSN"));
  s.Add( new AnsiString("UID"));
  s.Add( new AnsiString("PWD"));
  s.Add( new AnsiString("TIMEOUT"));
  MyMemory m;
  m << s;
  cout << endl << "ConnectInfo sended" << endl;
  return MakeOctets(m);

};

CORBA::Long srvCartrige::NewInstance(){
  InstanceStore * ptr =new InstanceStore;
  ptr->con=0;
  Instances.Add( ptr);
  cout << endl << "NewInstance I=" << ((long) ptr)  << endl;
  return   (long)ptr;
};


Octets* srvCartrige::GetCursorRow(
      CORBA::Long Instance,
      CORBA::Long Cursor
      ){
    MyArray<StackValue> v;

    odbcStatement * st = (odbcStatement *) Cursor;
    InstanceStore * iptr = (InstanceStore*) Instance;
    if(Instances.Where(iptr)>=0){
        if(iptr->con){
          long id =iptr->con->StatementItems.Where(st);
          if (id>=0){
             for(long i=0;i<st->fields.Count();i++){
                  v.Add(st->fields[i]->GetCont()->_StackValue());
             }

          }
        }
      }

      MyMemory m;
      m << v;
      LZSS lz;

      DataBuffer *db=lz.Encode(m.GetBuffer(),m.BufferSize());
      Octets * oc =MakeOctets(*db);
      delete db;
      return oc;

};


CORBA::Boolean srvCartrige::NoMoreRows(
      CORBA::Long Instance,
      CORBA::Long Cursor
      ){
    odbcStatement * st = (odbcStatement *) Cursor;
    InstanceStore * iptr = (InstanceStore*) Instance;
    if(Instances.Where(iptr)>=0){
        if(iptr->con){
          long id =iptr->con->StatementItems.Where(st);
          if (id>=0){
            try {
            RETCODE r=st->MoveNext();
            if (!(r==SQL_SUCCESS || r==SQL_SUCCESS_WITH_INFO)){
              cout << "No More Rows for cursor I=" << Instance  << " C=" << Cursor << endl;
            }
            return !(r==SQL_SUCCESS || r==SQL_SUCCESS_WITH_INFO);
            }catch(...){
                return 1;
            }
          }
        }
     }
    return 1;
};

void srvCartrige::CloseCursor(
      CORBA::Long Instance,
      CORBA::Long Cursor
      ){
    odbcStatement * st = (odbcStatement *) Cursor;
    InstanceStore * iptr = (InstanceStore*) Instance;
    if(Instances.Where(iptr)>=0){
        if(iptr->con){
          long id =iptr->con->StatementItems.Where(st);
          if (id>=0)
           iptr->con->StatementItems.Destroy(id);
           cout << "CloseCursor I=" << Instance  << " C=" << Cursor<< endl;
        }
    }
};
Octets* srvCartrige::GetCursorHeared(
      CORBA::Long Instance,
      CORBA::Long Cursor
      ){
    MyArray<FieldHeader> h;
    FieldHeader * ptr;
    odbcStatement * st = (odbcStatement *) Cursor;
    InstanceStore * iptr = (InstanceStore*) Instance;
    if(Instances.Where(iptr)>=0){
        if(iptr->con){
        long id =iptr->con->StatementItems.Where(st);
        if (id>=0){
         for(long i=0;i<st->fields.Count();i++){
           ptr = new FieldHeader;
           ptr->Name=st->fields.Name(i);
           ptr->AllowNull=st->fields[i]->IsNullable();
           ptr->Type=st->fields[i]->GetType();
           ptr->MaxSize=st->fields[i]->GetDefaultSize();
           h.Add(ptr);
         }
         cout << "GetCursorHeared I=" << Instance  << " C=" << Cursor<< endl;

        }
      }
    }

    MyMemory m;
    m << h;
      LZSS lz;

      DataBuffer *db=lz.Encode(m.GetBuffer(),m.BufferSize());
      Octets * oc =MakeOctets(*db);
      delete db;
      return oc;

};


void srvCartrige::Exec(
      CORBA::Long Instance,
      const char* Query,
      const Octets& Parameters
      ){

    InstanceStore * iptr = (InstanceStore*) Instance;
    if(Instances.Where(iptr)>=0){

        if(iptr->con){
          try{
            iptr->con->Execute(Query);
          }catch(TODBCException & e){
         cout << "Exec I=" << Instance <<  " failed! Q="<< Query << endl;
          };

        }
    }
    delete & Parameters;

};




//---------------------------------------------------------------------------
#pragma package(smart_init)

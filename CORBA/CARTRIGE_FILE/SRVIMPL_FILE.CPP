//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop


#include "SRVImpl_file.h"
#include "Convertions.h"
#include "syncobjs.hpp"
#include "LZSS.h"

#include <fcntl.h>
#include <io.h>

////////////////////////////////////////////////
SimpleStream & operator >> (SimpleStream & s,Storages & i){
  s >> i.Name;
  s >> i.FullPath;
  return s;
};

SimpleStream & operator << (SimpleStream & s,Storages & i){
  s << i.Name;
  s << i.FullPath;
  return s;
};

/////////////////////////////////////////////////////////////////
void srvCartrige::CloseInstance(
      CORBA::Long Instance
      ){
    InstanceStore * iptr = (InstanceStore*) Instance;
    long id=Instances.Where(iptr);
    if(id>=0){
      if(iptr->Connected) DisConnect(Instance);
      Instances.Destroy(id);
      cout << "Instance closed I=" << Instance << endl;
    }
};


void srvCartrige::DisConnect(
      CORBA::Long Instance
      ){
    InstanceStore * iptr = (InstanceStore*) Instance;
    if(Instances.Where(iptr)>=0){
        if(iptr->Connected){
          long i =0;
          for(;i< iptr->Files.Count();){
            iptr->Files.Destroy(i);
          }
          iptr->Connected = false;
        }
        cout << "Disconnect I=" << Instance << endl;
      }
};


CORBA::Long srvCartrige::OpenCursor(
      CORBA::Long Instance,
      const char* Query,
      const Octets& Parameters
      ){

    InstanceStore * iptr = (InstanceStore*) Instance;
    if(Instances.Where(iptr)>=0){
        if(iptr->Connected){
          FileInfo * fi= new FileInfo;
          if(ParseSelect(Query,fi->pss)){
            fi->FileHandle=open(fi->pss.Name.c_str(),O_RDONLY|O_BINARY,S_IREAD);
            if(fi->FileHandle >= 0){
              fi->Length = filelength(fi->FileHandle);
              cout << "Open cursor I=" << Instance << " C=" << ((long) fi) << endl;
              iptr->Files.Add(fi);
              return (long)fi;
            }else{
              delete fi;
            }
          }
        }
      }
      cout << "Open cursor I=" << Instance << " QRY=" << Query << " failed."<< endl;
      return 0;
};

CORBA::Boolean srvCartrige::IsConnected(
      CORBA::Long Instance
      ){
    InstanceStore * iptr = (InstanceStore*) Instance;
    if(Instances.Where(iptr)>=0)
        if(iptr->Connected)
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

    FileInfo * st=0;
    InstanceStore * iptr = (InstanceStore*) Instance;
    if(Instances.Where(iptr)>=0){
      if(iptr->Connected){
          st= new FileInfo;
          if(ParseSelect(Query, st->pss)){
            if(!IsPseudoFile(st->pss.Name))
              st->FileHandle=open(st->pss.Name.c_str(),O_RDONLY|O_BINARY,S_IREAD);
            if(st->FileHandle>= 0)
              st->Length = filelength(st->FileHandle);
           }else{
            delete st;
            delete & Parameters;
            return new Octets;
          }
      }
    }else{
       delete & Parameters;
       return new Octets;
    }

    if(st->pss.GetSize){
       ptr = new FieldHeader;
       ptr->Name="LENGTH";
       ptr->AllowNull=false;
       ptr->Type=stNumeric;
       ptr->MaxSize=4;
       h.Add(ptr);
    }
    if(st->pss->GetPosition){
       ptr = new FieldHeader;
       ptr->Name="POSITION";
       ptr->AllowNull=false;
       ptr->Type=stNumeric;
       ptr->MaxSize=4;
       h.Add(ptr);
    }
    if(st->pss->GetData){
       ptr = new FieldHeader;
       ptr->Name="DATA";
       ptr->AllowNull=false;
       ptr->Type=stBinary;
       ptr->MaxSize=4096;
       h.Add(ptr);
    }
    if(st->pss->GetInfo){
       ptr = new FieldHeader;
       ptr->Name="INFO";
       ptr->AllowNull=false;
       ptr->Type=stString;
       ptr->MaxSize=4096;
       h.Add(ptr);
    }

    /*
  long  Position;
  long  Length;
  long  Size;
  long  BlockSize;
  AnsiString Name;
    */



     MyArray<RowData> Rows ;

     if(st->pss->GetData)



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
    InstanceStore * iptr = (InstanceStore*) Instance;
    if(Instances.Where(iptr)>=0){
        if(iptr->Connected){
          DisConnect(Instance);
          iptr->Parameters.Clear();
        }
    }else{
     delete & Info;
     return;
    }

      MyArray<CartrigeParam> s;
      MyMemory * m = MakeMemory(Info);
      *m >> iptr->Parameters;
      delete m;
      iptr->COnnected=true;
      cout << "New Connect I=" << Instance << endl;

      }catch(...){
        cout << "Connect I=" << Instance << " fail! " << endl;
      };

};


Octets* srvCartrige::GetConnectInfo(){
  MyArray<AnsiString> s;
  s.Add( new AnsiString("STORAGE"));
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
/////////////////////////////////////////
bool srvCartrige::ParseSelect(AnsiString QRY, ParseSelectStruct & Result  ){
  bool BeforFrom=true;
  bool BeforWhere = true;
  bool BeforSelect = true;
  // memset(&Result,0, sizeof(ParseSelectStruct));

  return false;
};

bool srvCartrige::ParseExec(AnsiString QRY, ParseExecStruct & Result  ){
  return false;
};

bool srvCartrige::IsPseudoFile(AnsiString Name){
  for(long i =0;pfl[i];i++){
    if(Name==pfl[i]) return true;
  }
  return false;
};







//---------------------------------------------------------------------------
#pragma package(smart_init)

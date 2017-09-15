//---------------------------------------------------------------------------
#ifndef SRVImplH
#define SRVImplH
#include "cartrige_s.hh"
#include "CartrigeObjects.h"
#include "tokens.h"

static const char * sdel[] = SELECTDELIMITERS;
static const char * edel[] = EXECDELIMITERS;
static const char * stok[] = SELECTTOKENS;
static const char * etok[] = EXECTOKENS;
static const char * pfl[] = PSEUDOFILE;

struct Storages{
  AnsiString Name;
  AnsiString FullPath;
  friend SimpleStream & operator >> (SimpleStream & s,Storages & i);
  friend SimpleStream & operator << (SimpleStream & s,Storages & i);
};


struct ParseSelectStruct{
  bool  GetSize;
  bool  GetPosition;
  bool  GetData;
  bool  GetInfo;
  long  Position;
  long  Length;
  long  Size;
  long  BlockSize;
  AnsiString Name;
};

struct ParseExecStruct{
  AnsiString Command;
  AnsiString name;
  long Pos;
  long Len;
  DataBuffer Data;
};



struct FileInfo{
  long StorageIndex;
  long FileHandle;
  long Position;
  long Length;
  FileInfo(){
    StorageIndex = 0;
    FileHandle = -1;
    Position = 0;
  };
  ParseSelectStruct pss;
};

struct InstanceStore{
  MyArray<FileInfo> Files;
  MyArray<CartrigeParam> Parameters;
  bool Connected;
  InstanceStore(){Connected = false;};
};


class srvCartrige: public _sk_Cartrige{
 private:

 MyArray<Storages> FileStorages;
 MyArray<InstanceStore> Instances;

 bool ParseSelect(AnsiString QRY, ParseSelectStruct & Result  );
 bool ParseExec(AnsiString QRY, ParseExecStruct & Result  );
 bool IsPseudoFile(AnsiString Name);

 void PreLoad(){
   MyMemory m;
   m.LoadFromFileDecode("Storages.dat");
   m >> FileStorages;
   if(FileStorages.Count()==0){
     Storages *s =  new Storages();
     s->FullPath = ExtractFilePath(Application->ExeName) + "STORAGE\\";
     s->Name = "STORAGE";
     FileStorages.Add(s);
   }
 };

public:

  ~srvCartrige(){
      MyMemory m;
        m <<FileStorages;
        m.SaveToFileEncode("Storages.dat");
  };

  srvCartrige(const char *_obj_name = (const char *)NULL):
  _sk_Cartrige(_obj_name){
    PreLoad();
  };

  srvCartrige(
      const char *_service_name,
      const CORBA::ReferenceData& _data):
  _sk_Cartrige(
      _service_name,
       _data){PreLoad(); };

  virtual void DisConnect(
      CORBA::Long Instance
      );
  virtual void CloseInstance(
      CORBA::Long Instance
      );

  virtual CORBA::Long OpenCursor(
      CORBA::Long Instance,
      const char* Query,
      const Octets& Parameters
      );
  virtual CORBA::Boolean IsConnected(
      CORBA::Long Instance
      );
  virtual Octets* GetRowSet(
      CORBA::Long Instance,
      const char* Query,
      const Octets& Parameters
      );
  virtual void Connect(
      CORBA::Long Instance,
      const Octets& Info
      );
  virtual Octets* GetConnectInfo();
  virtual CORBA::Long NewInstance();
  virtual Octets* GetCursorRow(
      CORBA::Long Instance,
      CORBA::Long Cursor
      );
  virtual CORBA::Boolean NoMoreRows(
      CORBA::Long Instance,
      CORBA::Long Cursor
      );
  virtual void CloseCursor(
      CORBA::Long Instance,
      CORBA::Long Cursor
      );
  virtual Octets* GetCursorHeared(
      CORBA::Long Instance,
      CORBA::Long Cursor
      );
  virtual void Exec(
      CORBA::Long Instance,
      const char* Query,
      const Octets& Parameters
      );
};
//---------------------------------------------------------------------------
#endif

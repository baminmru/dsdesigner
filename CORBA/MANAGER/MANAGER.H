//---------------------------------------------------------------------------
#ifndef ManagerH
#define ManagerH

#include "ManagerStube.h"


class ObjectItem;
class ObjectManager;

class ObjectComponent{
private:
   long (*_NewInstance) (ObjectManager *);
   void (*_CloseInstance) (long);
   void (*_Setup) (long, DataBuffer *, DataBuffer *,DataBuffer *);
   void (*_SetupSlave) (long, DataBuffer *, CParam *);
   void (*_Refresh) (long,  CParam *);
   void (*_Run) (long);
   bool (*_RunModal) (long);        // Ok / Cacell
   CParam * (*_GetParams)(long);    // Params
   RowSet * (*_GetSelection)(long); // RowSet
   RowSet * (*_Filteration)(long, RowSet *); // RowSet --> RowSet

   DataBuffer * (*_EditPI) (ObjectManager *,DataBuffer * PI,AnsiString System);
   DataBuffer * (*_EditPD) (ObjectManager *,DataBuffer * PD,AnsiString System);
   DataBuffer * (*_EditUL) (ObjectManager *,DataBuffer * UL,AnsiString System);
   DataBuffer * (*_EditSL) (ObjectManager *,DataBuffer * UL,AnsiString System);

   AnsiString DLLName;
   AnsiString CName;
   HINSTANCE hinstLib;
   MyArray<ObjectItem> Items;
   ObjectManager * MAN;

public:
   AnsiString DllName(){
        return DLLName;
   };
   AnsiString Name(){
        return CName;
   };

   ObjectComponent(ObjectManager * Man, AnsiString DLLName,AnsiString Name);
   ~ObjectComponent();
   ObjectItemStub * CreateObject(AnsiString _Name);
   DataBuffer * EditPI (DataBuffer * PI,AnsiString System);
   DataBuffer * EditPD (DataBuffer * PD,AnsiString System);
   DataBuffer * EditUL (DataBuffer * UL,AnsiString System);
   DataBuffer * EditSL (DataBuffer * UL,AnsiString System);

   friend class ObjectItem;
};



class ObjectItem: public ObjectItemStub{
private:
   ObjectComponent* Parent;
   long Instance;
   AnsiString Name;
public:

   AnsiString GetName() { return Name;};
   long GetInstance() { return Instance;};
   ObjectItem(ObjectComponent* Parent, long Instance, AnsiString Name);

   void Setup (DataBuffer * PI, DataBuffer *PD,DataBuffer *UL);
   void SetupSlave (DataBuffer * SI, CParam *Par);
   void Refresh (CParam * Par);
   void Run ();
   bool RunModal ();
   CParam * GetParams();
   RowSet * GetSelection();
   RowSet * Filteration(RowSet * IRS);
};




class DataSource: public DataSourceStub{
  Cartrige_var Fcartrige;
  AnsiString CorbaName;
  AnsiString Name;
  long Instance;
  long NewInstance();
  void CloseInstance();
  Cartrige_ptr GetCartrige();
public:

  DataSource(AnsiString CorbaName="",AnsiString DSName="");
  ~DataSource();
  const AnsiString * GetName(){ return & Name;};
  const AnsiString * GetCorbaName(){ return & CorbaName;};

  long OpenCursor(const char* _Query,CParam & _Parameters );
  bool IsConnected();
  RowSet* GetRowSet(const char* _Query,CParam & _Parameters );
  void Connect(DataBuffer& _Info );
  void DisConnect();
  MyArray<AnsiString>* GetConnectInfo();
  RowData* GetCursorRow( long _Cursor  );
  bool NoMoreRows( long _Cursor  );
  void CloseCursor( long _Cursor   );
  MyArray<FieldHeader>* GetCursorHeared(long _Cursor );
  void Exec( const char* _Query,CParam & _Parameters);
};



struct ManagerForms{
  TForm * form;
  ManagerWindowType mwt;
};


#define PLATFORM "TEST"
#define SERVERNAME "BAMI_APPSERVER"


class ObjectManager: public ObjectManagerStub{
private:
  MyArray<ObjectComponent> Components;
  MyArray<ObjectItem> Items;
  MyArray<DataSource> Sources;
  MyArray<ManagerForms> MyForms;
  long ConnectID;
  ObjDefStorage_var FobjDefStorage;

  Build CurrentBuild;
  AnsiString UID;
  AnsiString PWD;

public:
  ObjDefStorage_ptr GetDS();
  long GetConnect(){
    return ConnectID;
  };

  AnsiString GetUID(){ return UID;};
  bool IsConnected(){return ConnectID && FobjDefStorage;};


  ObjectManager();
  ~ObjectManager();

  MyArray<Build> * GetBuilds();
  bool LogOn(char * UID, char * PWD);
  void LogOff();

  void RunBuild(Build *);
  DataSourceStub *GetSource(AnsiString Name,AnsiString SystemName,AnsiString BuildName);
  TForm  *GetForm(ManagerWindowType mwt);
  ObjectItemStub * FindItem(AnsiString ObjectName,AnsiString SystemName);
  ObjectItemStub * NewItem(AnsiString ObjectName,AnsiString SystemName);

  // Получить список объектов данных в задаче
  MyArray<AnsiString> * ObjectList(AnsiString System);

  // Получить список источников данных в задаче
  MyArray<AnsiString> * SourceList(AnsiString System);

  virtual DataBuffer * GetObjectSL(AnsiString ObjectName,AnsiString SystemName);

  // Получить локальную настройку
  virtual DataBuffer * GetObjectUL(AnsiString ObjectName,AnsiString SystemName);

  // Сохранить локальную настройку объекта
  virtual void SaveObjectUL(AnsiString ObjectName,DataBuffer *UL,AnsiString SystemName);

  ObjectComponent * GetComponent(AnsiString ObjectName, AnsiString SystemName);
  virtual AnsiString CurSystem(){ return CurrentBuild.SystemName;};
  virtual AnsiString CurBuild(){ return CurrentBuild.BuildName;};
};


extern ObjectManager *man;
//---------------------------------------------------------------------------
#endif

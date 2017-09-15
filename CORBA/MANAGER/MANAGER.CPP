//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop


#include "Manager.h"

#include "fmdimain.h"
#include "fmdichild.h"
#include "LZSS.h"

/////////////////////////////////////////////
//ObjectComponent

ObjectComponent::ObjectComponent(ObjectManager * Man,AnsiString _DLLName,AnsiString _CName){
    CName = _CName;
    DLLName=_DLLName;
    MAN=Man;
     _NewInstance = 0;
     _CloseInstance = 0;
     _Setup = 0;
     _SetupSlave = 0;
     _Refresh = 0;
     _Run = 0;
     _RunModal = 0;
     _GetParams = 0;
     _GetSelection = 0;
     _Filteration = 0;
     _EditPI = 0;
     _EditPD = 0;
     _EditUL = 0;
     _EditSL = 0;

    hinstLib = LoadLibrary(DLLName.c_str());

    if (hinstLib != NULL)
    {

     _NewInstance  =(long (*) (ObjectManager *)) GetProcAddress(hinstLib, "_NewInstance");
     _CloseInstance=(void (*) (long)) GetProcAddress(hinstLib, "_CloseInstance");
     _Setup        =(void (*) (long, DataBuffer *, DataBuffer *,DataBuffer *)) GetProcAddress(hinstLib, "_Setup");
     _SetupSlave   =(void (*) (long, DataBuffer *, CParam *)) GetProcAddress(hinstLib, "_SetupSlave");
     _Refresh      =(void (*) (long,  CParam *))GetProcAddress(hinstLib, "_Refresh");
     _Run          =(void (*) (long)) GetProcAddress(hinstLib, "_Run");
     _RunModal     =(bool (*) (long)) GetProcAddress(hinstLib, "_RunModal");
     _GetParams    =(CParam * (*)(long)) GetProcAddress(hinstLib, "_GetParams");
     _GetSelection =(RowSet * (*)(long)) GetProcAddress(hinstLib, "_GetSelection");
     _Filteration  =(RowSet * (*)(long, RowSet *))GetProcAddress(hinstLib, "_Filteration");
     _EditPI       =(DataBuffer * (*) (ObjectManager *,DataBuffer * ,AnsiString))GetProcAddress(hinstLib, "_EditPI");
     _EditPD       =(DataBuffer * (*) (ObjectManager *,DataBuffer * ,AnsiString))GetProcAddress(hinstLib, "_EditPD");
     _EditUL       =(DataBuffer * (*) (ObjectManager *,DataBuffer * ,AnsiString))GetProcAddress(hinstLib, "_EditUL");
     _EditSL       =(DataBuffer * (*) (ObjectManager *,DataBuffer * ,AnsiString))GetProcAddress(hinstLib, "_EditSL");
    }
};
ObjectItemStub * ObjectComponent::CreateObject(AnsiString _Name){
        if(MAN && _NewInstance){
           long NI = _NewInstance(MAN);
           ObjectItem * ptr = new ObjectItem(this, NI,_Name);
           Items.Add(ptr);
           return ptr;
        } return 0;
};
DataBuffer * ObjectComponent::EditPI (DataBuffer * PI,AnsiString System){
        if(MAN && _EditPI){
           return _EditPI(MAN,PI,System);
        } return PI;
};
DataBuffer * ObjectComponent::EditPD (DataBuffer * PD,AnsiString System){
        if(MAN && _EditPD){
           return _EditPD(MAN,PD,System);
        } return PD;
};

DataBuffer * ObjectComponent::EditUL (DataBuffer * UL,AnsiString System){
        if(MAN && _EditUL){
           return _EditUL(MAN,UL,System);
        } return UL;
};

DataBuffer * ObjectComponent::EditSL (DataBuffer * SL,AnsiString System){
        if(MAN && _EditSL){
           return _EditSL(MAN,SL,System);
        } return SL;
};

ObjectComponent::~ObjectComponent(){
          for(long i=0;i<Items.Count();i++){
                if(_CloseInstance)
                   _CloseInstance(Items[i]->GetInstance() );
          }
          FreeLibrary(hinstLib);
   };


//////////////////////////////////////////////////
//ObjectItem

ObjectItem::ObjectItem(ObjectComponent* _Parent, long _Instance, AnsiString _Name){
  Parent=_Parent;
  Instance=_Instance;
  Name=_Name;
};

void ObjectItem::Setup (DataBuffer * PI, DataBuffer *PD,DataBuffer *UL){
    if(Parent && Instance && Parent->_Setup)
       Parent->_Setup(Instance,PI,PD,UL);
};

void ObjectItem::SetupSlave (DataBuffer * SI, CParam *Par){
    if(Parent && Instance && Parent->_SetupSlave)
        Parent->_SetupSlave (Instance , SI, Par);
};

void ObjectItem::Refresh (CParam * Par){
    if(Parent && Instance && Parent->_Refresh)
            Parent->_Refresh(Instance,Par);
};

void ObjectItem::Run (){
       if(Parent && Instance && Parent->_Run)
            Parent->_Run(Instance);
};

bool ObjectItem::RunModal (){
          if(Parent && Instance && Parent->_RunModal)
            return Parent->_RunModal(Instance);
          else
            return false;

};

CParam * ObjectItem::GetParams(){
          if(Parent && Instance && Parent->_GetParams)
            return Parent->_GetParams(Instance);
          else
            return 0;

};

RowSet * ObjectItem::GetSelection(){
    if(Parent && Instance && Parent->_GetSelection)
       return Parent->_GetSelection(Instance);
    else
       return 0;

};


RowSet * ObjectItem::Filteration(RowSet * IRS){
    if(Parent && Instance && Parent->_Filteration)
       return Parent->_Filteration(Instance,IRS);
    else
       return 0;
};


////////////////////////////////////////////////////////
// DataSource


bool DataSource::IsConnected(){
  if(Instance)
    return GetCartrige()->IsConnected(Instance);
  else
    return false;
};

long DataSource::NewInstance(){
  return GetCartrige()->NewInstance();
};

void DataSource::CloseInstance(){
  if(Instance)
    GetCartrige()->CloseInstance(Instance);
};

DataSource::~DataSource(){
  CloseInstance();
};

long DataSource::OpenCursor(const char* _Query, CParam & _Parameters ){
  if(Instance){
    MyMemory m;
    m << _Parameters;
    return GetCartrige()->OpenCursor(Instance,_Query,*MakeOctets(m));
  }
  return 0;
};

RowSet* DataSource::GetRowSet(const char* _Query,  CParam & _Parameters ){
  if(Instance){
    MyMemory m,*m2;
    m << _Parameters;
    Octets * o= GetCartrige()->GetRowSet(Instance,_Query,*MakeOctets(m));
    m2=MakeMemory(*o);

    LZSS lz;
    DataBuffer * db =lz.Decode(m2->GetBuffer(),m2->BufferSize());
    m2->LoadBuffer(db->GetBuffer(),db->Length());
    delete db;

    RowSet * r = new RowSet;
    *m2 >> *r;
    delete m2;
  }
  return 0;

};

void DataSource::Connect(DataBuffer& _Info ){
   if(Instance){
    Octets * oc =MakeOctets(_Info);
    GetCartrige()->Connect(Instance,*oc);
    delete oc;
   }
};

void DataSource::DisConnect(){
   if(Instance){
    GetCartrige()->DisConnect(Instance);
   }
};

MyArray<AnsiString>* DataSource::GetConnectInfo(){
  Octets *o= GetCartrige()->GetConnectInfo();
  MyArray<AnsiString> * s = new MyArray<AnsiString>;
  MyMemory *m = MakeMemory(*o);
  if(m && s)
          (*m) >> (*s);
  delete m;
  delete o;
  return s;
};

RowData* DataSource::GetCursorRow( long _Cursor  ){
  if(Instance){
   Octets *o= GetCartrige()->GetCursorRow(Instance,_Cursor);

    MyMemory *m = MakeMemory(*o);

    // распаковка буфера
    LZSS lz;
    DataBuffer * db =lz.Decode(m->GetBuffer(),m->BufferSize());
    m->LoadBuffer(db->GetBuffer(),db->Length());
    delete db;


    RowData * rd = new RowData;
    if(m && rd){
       (*m) >> (*rd);
    }
    delete m;
    delete o;
    return rd;
  }
  return 0;
};

bool DataSource::NoMoreRows( long _Cursor  ){
  if(Instance){
    return GetCartrige()->NoMoreRows(Instance,_Cursor);
  }
  return true;
};

void DataSource::CloseCursor( long _Cursor   ){
  if(Instance){
    GetCartrige()->CloseCursor(Instance,_Cursor);
  }
};

MyArray<FieldHeader>* DataSource::GetCursorHeared(long _Cursor ){
  if(Instance){
    Octets *o= GetCartrige()->GetCursorHeared(Instance,_Cursor);
    MyMemory *m=MakeMemory(*o);

    // распаковка буфера
    LZSS lz;
    DataBuffer * db =lz.Decode(m->GetBuffer(),m->BufferSize());
    m->LoadBuffer(db->GetBuffer(),db->Length());
    delete db;

    MyArray<FieldHeader> * rd = new MyArray<FieldHeader>;
    if(m && rd)
      (*m) >> (*rd);
    delete m;
    delete o;
    return rd;
  }
  return 0;

};

void DataSource::Exec( const char* _Query,  CParam & _Parameters){
  if(Instance){
    MyMemory m;
    m << _Parameters;
    GetCartrige()->Exec(Instance,_Query,*MakeOctets(m));
  }
};

Cartrige_ptr  DataSource::GetCartrige(void)
{
        if (Fcartrige == NULL)
        {
           Fcartrige = Cartrige::_bind(Name.c_str());
           Instance = NewInstance();
        }
        return Fcartrige;
}

DataSource::DataSource(AnsiString _CorbaName,AnsiString _Name)
{
        Name=_Name;
        CorbaName=_CorbaName;
        Fcartrige=0;
        try{
          Fcartrige = Cartrige::_bind(CorbaName.c_str());
          Instance = NewInstance();
        }catch(const CORBA::Exception &e){
        }
}


//////////////////////////////////////////////////////
// ObjectManager
ObjDefStorage_ptr  ObjectManager::GetDS(void)
{
        if (FobjDefStorage == NULL)
        {
          try{
                  FobjDefStorage = ObjDefStorage::_bind("BAMI_APPSERVER");
          }catch(const CORBA::Exception &e){
          }
        }
        return FobjDefStorage;
}


MyArray<Build> * ObjectManager::GetBuilds(){
  if(!IsConnected())
    return 0;
  Octets * o=GetDS()->GetBuildList(ConnectID);
  MyMemory *m = MakeMemory(*o);
  delete o;
  MyArray<Build> *b = new MyArray<Build>;
  *m >> *b;
  delete m;
  return b;
};

ObjectManager::ObjectManager(){
  ConnectID=0;
  FobjDefStorage=0;
  try{
    FobjDefStorage = ObjDefStorage::_bind(SERVERNAME);
  }catch(const CORBA::Exception &e){
  }
};
ObjectManager::~ObjectManager(){
  if(FobjDefStorage)
    delete FobjDefStorage;
};

bool ObjectManager::LogOn(char * _UID, char * _PWD){
  UID=_UID;
  PWD=_PWD;
  ConnectID=GetDS()->Connect(_UID,_PWD,PLATFORM);
  return ConnectID;
};

void ObjectManager::LogOff(){
if(IsConnected())
  GetDS()->DisConnect(ConnectID);

};

void ObjectManager::RunBuild(Build *b){
  CurrentBuild.SystemName = b->SystemName;
  CurrentBuild.BuildName  = b->BuildName;
  ObjectItem * Main =(ObjectItem *) NewItem("MAIN",b->SystemName);
  if(Main)
    Main->Run();
};

DataSourceStub * ObjectManager::GetSource(AnsiString Name,AnsiString SystemName,AnsiString BuildName){
  DataSource * ds=0;
  for( long i=0;i<Sources.Count();i++){
        if(*(Sources[i]->GetName())== Name ){
           ds= Sources[i];
               break;
           }
  }
  if(!ds){
    try
    {
     MyArray<CartrigeParam> ps;
     Octets * oc =GetDS()->GetTargetPackSetup(
            ConnectID,
            BuildName.c_str(),
            SystemName.c_str(),
            Name.c_str());
     MyMemory *m=0;
     if(oc)
       m = MakeMemory(*oc);
     delete oc;
     DataSourceSetup dss;
     if(m)
       *m >> dss;
     delete m;
     ds= new DataSource(dss.TargetName,Name);
     Sources.Add(ds);
    }
     catch(const CORBA::Exception& e)
    {
         return 0;
    }
  }

  if(!ds->IsConnected()){
  try{
     MyArray<CartrigeParam> ps;
     Octets * oc =GetDS()->GetTargetPackSetup(
            ConnectID,
            BuildName.c_str(),
            SystemName.c_str(),
            Name.c_str());
     MyMemory *m;
     m = MakeMemory(*oc);
     delete oc;
     DataSourceSetup dss;
     *m >> dss;
     delete m;

     ds->Connect(dss.Info);
     }
     catch(const CORBA::Exception& e)
     {
        return 0;
     }
  };

  return ds;
};

TForm  *ObjectManager::GetForm(ManagerWindowType mwt){
  switch (mwt){
    case  mwtMDIForm:
        return MDIMain;
    case mwtMDIChild:
        return new TMDIChild (MDIMain);
    case  mwtPopup:
        return new TForm(MDIMain);

  };
  return 0;
};


ObjectItemStub * ObjectManager::FindItem(AnsiString ObjectName,AnsiString SystemName){
  for(long i=0;i<Items.Count();i++){
    if(Items[i]->GetName()==ObjectName)
      return Items[i];
  }
  return NewItem(ObjectName,SystemName);
};

ObjectComponent * ObjectManager::GetComponent(AnsiString ObjectName, AnsiString SystemName){
  ObjectComponent *c=0;
  Octets *oc=0;
  oc = GetDS()->GetObject(
      ConnectID,
      SystemName.c_str(),
      ObjectName.c_str()
  );

  MyMemory * m;
  if(oc){
    m=MakeMemory(*oc);
    //delete oc;
  }
  oc=0;
  ObjectInfo oi;
  *m >> oi;
  delete m;

  for(long i=0;i< Components.Count();i++){
    if(oi.ComponentName == Components[i]->Name()){
      c=Components[i];
      break;
    }
  }

  if(!c){
    /*
    // получаем описание компонента
    oc= GetDS()->GetComponent(
        ConnectID,
        PLATFORM,
        oi.ComponentName.c_str()
    );
    DataBuffer ci;
    if(oc)
       SetDataBuffer(ci,*oc);

    // вообще - то в ci должно лежать нечто осмысленное
    */
     AnsiString dll = "..\\";
    dll=dll+oi.ComponentName+"_dll\\"+oi.ComponentName;

      c = new ObjectComponent(this,dll,oi.ComponentName);
      if(c)
        Components.Add(c);
  }
  return c;
};

ObjectItemStub * ObjectManager::NewItem(AnsiString ObjectName,AnsiString SystemName){
  ObjectItemStub  *o=0;
  ObjectComponent *c=GetComponent(ObjectName,SystemName);
  Octets *oc=0;

  oc = GetDS()->GetObject(
      ConnectID,
      SystemName.c_str(),
      ObjectName.c_str()
  );

  MyMemory * m;
  if(oc){
    m=MakeMemory(*oc);
    //delete oc;
  }
  oc=0;
  ObjectInfo oi;
  *m >> oi;
  delete m;
  /*
  for(long i=0;i< Components.Count();i++){
    if(oi.ComponentName == Components[i]->Name()){
      c=Components[i];
      break;
    }
  }

  if(!c){

    // получаем описание компонента
    oc= GetDS()->GetComponent(
        ConnectID,
        PLATFORM,
        oi.ComponentName.c_str()
    );
    DataBuffer ci;
    if(oc)
       SetDataBuffer(ci,*oc);

    // вообще - то в ci должно лежать нечто осмысленное

     AnsiString dll = "..\\";
    dll=dll+oi.ComponentName+"_dll\\"+oi.ComponentName;

      c = new ObjectComponent(this,dll,oi.ComponentName);
    Components.Add(c);
  }
  */
  if(c)
     o =c->CreateObject(ObjectName);
  else
    return 0;

  //получаем дополнительные настройки объекта
  oc=0;
  DataBuffer ps;
  DataBuffer UL;
  try{
  oc =GetDS()->GetObjectPlatformSpecifics(
      ConnectID,
      CurrentBuild.SystemName.c_str(),
      ObjectName.c_str(),
      PLATFORM
  );


  if(oc)
    SetDataBuffer(ps,*oc);
  delete oc;
  oc=0;
  oc =GetDS()->GetObjectLocals(
      ConnectID,
      CurrentBuild.SystemName.c_str(),
      ObjectName.c_str(),
      UID.c_str()
  );

  if(oc)
    SetDataBuffer(UL,*oc);
  delete oc;
  oc=0;
  }catch(...){
  }
  o->Setup ( &oi.ObjectDef, &ps,&UL);
  return o;
};



MyArray<AnsiString> * ObjectManager::ObjectList(AnsiString System){
      Octets*oc= GetDS()->GetSystemObjectsList(
      ConnectID,
      System.c_str()
      );
      MyMemory *m = MakeMemory(*oc);
      MyArray<AnsiString> *s = new MyArray<AnsiString> ;
      *m >>*s;
      delete m;
      return s;

};

// Получить список источников данных в задаче
MyArray<AnsiString> * ObjectManager::SourceList(AnsiString System){
    Octets*oc= GetDS()->GetDataSourceList(
    ConnectID,
      System.c_str()
      );
      MyMemory *m = MakeMemory(*oc);
      MyArray<AnsiString> *s = new MyArray<AnsiString> ;
      *m >>*s;
      delete m;
      return s;
};

   // Получить локальную настройку
DataBuffer * ObjectManager::GetObjectUL(AnsiString ObjectName,AnsiString SystemName){
  Octets*  oc =GetDS()->GetObjectLocals(
      ConnectID,
      SystemName.c_str(),
      ObjectName.c_str(),
      UID.c_str()
  );
  DataBuffer *db = new DataBuffer();
  SetDataBuffer(*db,*oc);
  delete oc;
  return db;
};

DataBuffer * ObjectManager::GetObjectSL(AnsiString ObjectName,AnsiString SystemName){
  Octets*  oc;
  oc=GetDS()->GetObjectLocals(
      ConnectID,
      SystemName.c_str(),
      ObjectName.c_str(),
      UID.c_str()
  );

  DataBuffer  *db = new DataBuffer();
  SetDataBuffer( *db, *oc);
  delete oc;
  return db;
};


  // Сохранить локальную настройку объекта
void ObjectManager::SaveObjectUL(AnsiString ObjectName,DataBuffer *UL,AnsiString SystemName){
  Octets*  oc= MakeOctets(*UL);
  GetDS()->SetObjectLocals(
      ConnectID,
      SystemName.c_str(),
      ObjectName.c_str(),
      UID.c_str(),
      *oc
  );
  delete oc;

};



ObjectManager *man=0;
//---------------------------------------------------------------------------
#pragma package(smart_init)

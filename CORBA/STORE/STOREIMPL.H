//---------------------------------------------------------------------------
#ifndef StoreImplH
#define StoreImplH

#include "Store_S.hh"
#include "stackvalue.h"
#include "StoreObjects.h"




class  srvObjDefStorage : public  _sk_ObjDefStorage {
private:


  MyArray<ConnectInfo> Connections;

  MyArray<AnsiString> Platforms;
  MyArray<ComponentInfo> Components;


  MyArray<AnsiString> Targets;
  MyArray<TargetPackInfo> TargetPacks;

  MyArray<AnsiString> Systems;
  MyArray<ObjectInfo> Objects;
  MyArray<DataSourceDef> Sources;
  MyArray<Build> Builds;

  MyArray<ObjectPlatformSpecifics> Specifics;

  MyArray<UserInfo> Users;
  MyArray<UserRights> Rights;
  MyArray<UserObjectProperties> LocalSettings;
  MyArray<UserBuildRight> BuildRights;

  MyArray<DataSourceSetup> Setups;


  void LoadSrv();
  void SaveSrv();

  bool Connected(   CORBA::Long ConnectId);
  bool AdminUser(  CORBA::Long ConnectId);

  long UserID(const char * UserName);
  long SystemID(const char * SystemName);
  long TargetID(const char * TargetName);
  long PlatformID(const char * PlatformName);
  long ObjectID(const char * SystemName,const char * ObjectName);
  long SourceID(const char * SystemName,const char * ObjectName);
  long BuildID(const char * SystemName,const char * ObjectName);



protected:
  srvObjDefStorage(
      const char *_service_name,
      const CORBA::ReferenceData& _data):
        _sk_ObjDefStorage(_service_name,_data){
    LoadSrv();
  };
public:
  virtual ~srvObjDefStorage() {SaveSrv();}


  srvObjDefStorage(const char *_obj_name = (const char *)NULL):
  _sk_ObjDefStorage(_obj_name){
    LoadSrv();
  };


  virtual void DisConnect( CORBA::Long ConnectId) ;
  virtual CORBA::Long Connect(
      const char* UID,
      const char* PWD,
      const char* PlatformName
      );

  virtual void ChangePassword(
      CORBA::Long ConnectId,
      const char* Password
      );

  virtual Octets* GetUser(
      CORBA::Long ConnectId,
      const char* UserName);

/////////////////////////////////////////////////////////
  virtual Octets* GetTargetPackSetup(
      CORBA::Long ConnectId,
      const char* BuildName,
      const char* SystemName,
      const char* DataSource
      );
  virtual void SetComponent(
      CORBA::Long ConnectId,
      const char* Platform,
      const char* ComponentName,
      const Octets& Data
      );
  virtual Octets* GetObjectPlatformSpecifics(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* PlatformName
      );
  virtual void SetTargetPack(
      CORBA::Long ConnectId,
      const char* Target,
      const char* PackName,
      const char* PlatformName,
      const Octets& Data
      );
  virtual Octets* GetTargetList(      CORBA::Long ConnectId);
  virtual void DelTargetPack(
      CORBA::Long ConnectId,
      const char* Target,
      const char* PackName,
      const char* PlatformName
      );
  virtual Octets* GetComponent(
      CORBA::Long ConnectId,
      const char* Platform,
      const char* ComponentName
      );
  virtual Octets* GetBuildList(      CORBA::Long ConnectId);
  virtual void AddSystem(
      CORBA::Long ConnectId,
      const char* SystemName
      );
  virtual void SetObjectLocals(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* UserName,
      const Octets& Locals
      );
  virtual void AddTarget(
      CORBA::Long ConnectId,
      const char* TargetName
      );
  virtual void DelObjectLocals(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* UserName
      );
  virtual Octets* GetPlatformList(
        CORBA::Long ConnectId
  );
  virtual Octets* GetDataSourceList(
      CORBA::Long ConnectId,
      const char* SystemName
      );
  virtual void SetTargetPackSetup(
      CORBA::Long ConnectId,
      const char* BuildName,
      const char* SystemName,
      const char* DataSource,
      const char* TargetName,
      const Octets& Specifics
      );
  virtual Octets* GetSystemList(      CORBA::Long ConnectId);
  virtual void SetObjectPlatformSpecifics(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* PlatformName,
      const Octets& Specifics
      );
  virtual Octets* GetTargetPack(
      CORBA::Long ConnectId,
      const char* Target,
      const char* PackName,
      const char* PlatformName
      );
  virtual Octets* GetObjectLocals(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* UserName
      );
  virtual void DelObjectPlatformSpecifics(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* PlatformName
      );
  virtual void DelBuild(
      CORBA::Long ConnectId,
      const char* BuildName,
      const char* SystemName
      );
  virtual void AddBuild(
      CORBA::Long ConnectId,
      const char* BuildName,
      const char* SystemName
      );
  virtual void SetObjectRights(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* UserName,
      CORBA::Boolean Exec,
      CORBA::Boolean Read,
      CORBA::Boolean Write
      );
  virtual void DelObjectRights(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* UserName
      );
  virtual void AddPlatform(
      CORBA::Long ConnectId,
      const char* PlatformName
      );
  virtual Octets* GetUserList( CORBA::Long ConnectId);
  virtual void DelSystem(
      CORBA::Long ConnectId,
      const char* SystemName
      );
  virtual void DelTargetPackSetup(
      CORBA::Long ConnectId,
      const char* BuildName,
      const char* SystemName,
      const char* DataSource
      );
  virtual void DelTarget(
      CORBA::Long ConnectId,
      const char* TargetName
      );
  virtual Octets* GetObject(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName
      );
  virtual void DelUser(
      CORBA::Long ConnectId,
      const char* UserName
      );
  virtual void DelObject(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName
      );
  virtual void AddDataSource(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* SourceName
      );
  virtual void SetUser(
      CORBA::Long ConnectId,
      const char* UserName,
      const char* Password,
      CORBA::Boolean IsAdmin
      );
  virtual void DelDataSource(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* SourceName
      );
  virtual void DelPlatform(
      CORBA::Long ConnectId,
      const char* PlatformName
      );
  virtual void SetObject(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* ComponentName,      
      const Octets& ObjData
      );
  virtual Octets* GetComponentList(
        CORBA::Long ConnectId,
      const char* PlatformName
  );
  virtual Octets* GetObjectRights(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* UserName
      );
  virtual void DelComponent(
      CORBA::Long ConnectId,
      const char* Platform,
      const char* ComponentName
      );


//-------------------
  virtual void SetBuildRights(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _BuildName,
      const char* _UserName,
      CORBA::Boolean _AllowAccess
      );
virtual void DelBuildRights(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _BuildName,
      const char* _UserName
      );
virtual CORBA::Boolean GetBuildRights(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _BuildName,
      const char* _UserName
      );
virtual Octets* GetSystemBuildList(
      CORBA::Long _ConnectId,
      const char* _SystemName
      );
virtual Octets* GetSystemObjectsList(
      CORBA::Long _ConnectId,
      const char* _SystemName
      );


/////////////////////////////////////////////////////////
};


//---------------------------------------------------------------------------
#endif

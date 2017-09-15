//---------------------------------------------------------------------------
#ifndef SRVImplH
#define SRVImplH
#include "admin_s.hh"

class  srvCartrige : public  _sk_Cartrige {
protected:

  srvCartrige(const char *_obj_name = (const char *)NULL):
  _sk_Cartrige(_obj_name){};
  srvCartrige(
      const char *_service_name,
      const CORBA::ReferenceData& _data):
        _sk_Cartrige(_service_name,_data){};
  virtual ~srvCartrige() {}

public:


  virtual char* Name();
  virtual void Name(const char* _val);
  virtual CORBA::Long OpenCursor(
      const char* Query,
      const seqParams& Parameters
      );
  virtual CORBA::Boolean CreateDataBaseObject(
      const seqOctets& ObjDef
      );
  virtual RowSet* GetRowSet(
      const char* Query,
      const seqParams& Parameters
      );
  virtual void Disconnect();
  virtual CORBA::Boolean Connect(
      const seqParams& InitValues
      );
  virtual CORBA::Boolean Connected();
  virtual seqSetRow* GetCursorRow(
      CORBA::Long Cursor
      );
  virtual seqCartrigeParams* Parameters();
  virtual CORBA::Boolean NoMoreRows(
      CORBA::Long Cursor
      );
  virtual void CloseCursor(
      CORBA::Long Cursor
      );
  virtual seqSetHeader* GetCursorHeared(
      CORBA::Long Cursor
      );
  virtual void Exec(
      const char* Query,
      const seqParams& Parameters
      );

};


class  srvAppServer : public  _sk_AppServer {
protected:

  srvAppServer(const char *_obj_name = (const char *)NULL):
    _sk_AppServer(_obj_name){};
  srvAppServer(
      const char *_service_name,
      const CORBA::ReferenceData& _data):
        _sk_AppServer(
      _service_name,
      _data){};
  virtual ~srvAppServer() {}

public:

  virtual sList* GetSystemDomains(
      const char* SystemName
      ) = 0;
  virtual void Logoff();
  virtual char* CurrentSystem();
  virtual seqOctets* GetClientSideObject(
      const char* PlatformName,
      BaseType ObjBaseType,
      const char* ObjSubType
      );
  virtual Cartrige_ptr GetDomainCartrige(
      const char* Domain
      );
  virtual CORBA::Boolean Logon(
      const char* Name,
      const char* Pass
      );
  virtual void ChooseBuild(
      const char* SystemName,
      const char* BuildName
      );
  virtual CORBA::Boolean Connected();
  virtual seqTwiceStrings* GetBuids();
  virtual seqOctets* GetObject(
      BaseType ObjBaseType,
      const char* ObjSubType,
      const char* ObjectName
      );
  virtual void ChangePassword(
      const char* NewPassword
      );
  virtual seqOctets* GetTask(
      const char* Task
      );
  virtual char* CurrentBuild();
  


};



class  srvAppServerStorageManager : public  _sk_AppServerStorageManager {
protected:

  srvAppServerStorageManager(const char *_obj_name = (const char *)NULL):
          _sk_AppServerStorageManager(_obj_name)  {};

  srvAppServerStorageManager(
      const char *_service_name,
      const CORBA::ReferenceData& _data):
  _sk_AppServerStorageManager(
      _service_name,
      _data){};

  virtual ~srvAppServerStorageManager() {}

public:

  virtual sList* GetSystemDomains(
      const char* SystemName
      );
  virtual void Logoff();
  virtual sList* GetPlatforms();
  virtual CORBA::Boolean DelGroup(
      const char* GroupName
      );
  virtual char* CurrentSystem();
  virtual seqOctets* GetClientSideObject(
      const char* PlatformName,
      BaseType ObjBaseType,
      const char* ObjSubType
      );
  virtual Cartrige_ptr GetDomainCartrige(
      const char* Domain
      );
  virtual CORBA::Boolean AddGroup(
      const char* GroupName
      );
  virtual CORBA::Boolean DelSystemTask(
      const char* SystemName,
      const char* TaskName
      );
  virtual CORBA::Boolean AddSystem(
      const char* SystemName
      );
  virtual CORBA::Boolean GetBuildRight(
      BuildRight& Right
      );
  virtual CORBA::Boolean Logon(
      const char* Name,
      const char* Pass
      );
  virtual CORBA::Boolean AddSystemObject(
      const char* SystemName,
      const char* ObjectName,
      BaseType ObjectBaseType
      );
  virtual void ChooseBuild(
      const char* SystemName,
      const char* BuildName
      );
  virtual sList* GetGroupUsers(
      const char* GroupName
      );
  virtual CORBA::Boolean DelSystemBuildDomain(
      const char* SystemName,
      const char* BuildName,
      const char* DomainName
      );
  virtual CORBA::Boolean AddSystemBuild(
      const char* SystemName,
      const char* BuildName
      );
  virtual CORBA::Boolean DelUserFromGroup(
      const char* GroupName,
      const char* UserName
      );
  virtual CORBA::Boolean DelSystemObject(
      const char* SystemName,
      const char* ObjectName,
      BaseType ObjectBaseType
      );
  virtual CORBA::Boolean DelSystemBuild(
      const char* SystemName,
      const char* BuildName
      );
  virtual sList* GetSubTypes(
      BaseType ObjBaseType
      );
  virtual sList* GetGroups();
  virtual CORBA::Boolean AddUserToGroup(
      const char* GroupName,
      const char* UserName
      );
  virtual CORBA::Boolean SetupSystemObject(
      const char* SystemName,
      const char* ObjectName,
      const char* ObjectBaseType,
      const char* ObjectSubType,
      const seqOctets& Octets
      );
  virtual CORBA::Boolean Connected();
  virtual CORBA::Boolean AddClientObject(
      const char* PlatformName,
      BaseType ObjBaseType,
      const char* ObjSubType,
      const seqOctets& Octets
      );
  virtual CORBA::Boolean AddPlatform(
      const char* Name
      );
  virtual CORBA::Boolean SetupSystemTask(
      const char* SystemName,
      const char* TaskName,
      const seqOctets& Octets
      );
  virtual CORBA::Boolean GetTaskRight(
      TaskRight& Right
      );
  virtual seqTwiceStrings* GetBuids();
  virtual CORBA::Boolean GetObjectRight(
      ObjectRight& Right
      );
  virtual CORBA::Boolean DelSystem(
      const char* SystemName
      );
  virtual seqOctets* GetObject(
      BaseType ObjBaseType,
      const char* ObjSubType,
      const char* ObjectName
      );
  virtual CORBA::Boolean DelUser(
      const char* UserName
      );
  virtual CORBA::Boolean DelClientObject(
      const char* PlatformName,
      BaseType ObjBaseType,
      const char* ObjSubType
      );
  virtual CORBA::Boolean SetObjectRight(
      const ObjectRight& Right
      );
  virtual CORBA::Boolean AddSystemBuildDomain(
      const char* SystemName,
      const char* BuildName,
      const char* DomainName,
      const char* CartrigeName
      );
  virtual void ChangePassword(
      const char* NewPassword
      );
  virtual seqOctets* GetTask(
      const char* Task
      );
  virtual CORBA::Boolean SetBuildRight(
      const BuildRight& Right
      );
  virtual CORBA::Boolean AddSystemTask(
      const char* SystemName,
      const char* TaskName
      );
  virtual CORBA::Boolean SetupSystemBuildDomain(
      const char* SystemName,
      const char* BuildName,
      const char* DomainName,
      const seqParams& SetupValues
      );
  virtual sList* GetUsers();
  virtual CORBA::Boolean SetTaskRight(
      const TaskRight& Right
      );
  virtual char* CurrentBuild();
  virtual CORBA::Boolean AddUser(
      const char* UserName,
      const char* Password,
      CORBA::Boolean IsAdmin
      );


};



//---------------------------------------------------------------------------
#endif

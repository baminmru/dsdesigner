//---------------------------------------------------------------------------
#pragma hdrstop

#include "SRVImpl.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


char* srvCartrige::Name(){};
void srvCartrige::Name(const char* _val){};
CORBA::Long srvCartrige::OpenCursor(
      const char* Query,
      const seqParams& Parameters
      ){};
CORBA::Boolean srvCartrige::CreateDataBaseObject(
      const seqOctets& ObjDef
      ){};
RowSet* srvCartrige::GetRowSet(
      const char* Query,
      const seqParams& Parameters
      ){};
void srvCartrige::Disconnect(){};
CORBA::Boolean srvCartrige::Connect(
      const seqParams& InitValues
      ){};
CORBA::Boolean srvCartrige::Connected(){};
seqSetRow* GetCursorRow(
      CORBA::Long Cursor
      ){};
seqCartrigeParams* srvCartrige::Parameters(){};
CORBA::Boolean srvCartrige::NoMoreRows(
      CORBA::Long Cursor
      ){return 1; };
void srvCartrige::CloseCursor(
      CORBA::Long Cursor
      ){};
seqSetHeader* srvCartrige::GetCursorHeared(
      CORBA::Long Cursor
      ){};
void srvCartrige::Exec(
      const char* Query,
      const seqParams& Parameters
      ){};


sList* srvAppServer::GetSystemDomains(
      const char* SystemName
      ){};
void srvAppServer::Logoff(){};
char* srvAppServer::CurrentSystem(){};
seqOctets* srvAppServer::GetClientSideObject(
      const char* PlatformName,
      BaseType ObjBaseType,
      const char* ObjSubType
      ){};
Cartrige_ptr srvAppServer::GetDomainCartrige(
      const char* Domain
      ){};
CORBA::Boolean srvAppServer::Logon(
      const char* Name,
      const char* Pass
      ){};
void srvAppServer::ChooseBuild(
      const char* SystemName,
      const char* BuildName
      ){};
CORBA::Boolean srvAppServer::Connected(){};
seqTwiceStrings* srvAppServer::GetBuids(){};
seqOctets* srvAppServer::GetObject(
      BaseType ObjBaseType,
      const char* ObjSubType,
      const char* ObjectName
      ){};
void srvAppServer::ChangePassword(
      const char* NewPassword
      ){};
seqOctets* srvAppServer::GetTask(
      const char* Task
      ){};
char* srvAppServer::CurrentBuild(){};



sList* srvAppServerStorageManager::GetSystemDomains(
      const char* SystemName
      ){};
void srvAppServerStorageManager::Logoff(){};
sList* srvAppServerStorageManager::GetPlatforms(){};
CORBA::Boolean srvAppServerStorageManager::DelGroup(
      const char* GroupName
      ){};
char* srvAppServerStorageManager::CurrentSystem(){};
seqOctets* srvAppServerStorageManager::GetClientSideObject(
      const char* PlatformName,
      BaseType ObjBaseType,
      const char* ObjSubType
      ){};
Cartrige_ptr srvAppServerStorageManager::GetDomainCartrige(
      const char* Domain
      ){};
CORBA::Boolean srvAppServerStorageManager::AddGroup(
      const char* GroupName
      ){};
CORBA::Boolean srvAppServerStorageManager::DelSystemTask(
      const char* SystemName,
      const char* TaskName
      ){};
CORBA::Boolean srvAppServerStorageManager::AddSystem(
      const char* SystemName
      ){};
CORBA::Boolean srvAppServerStorageManager::GetBuildRight(
      BuildRight& Right
      ){};
CORBA::Boolean srvAppServerStorageManager::Logon(
      const char* Name,
      const char* Pass
      ){};
CORBA::Boolean srvAppServerStorageManager::AddSystemObject(
      const char* SystemName,
      const char* ObjectName,
      BaseType ObjectBaseType
      ){};
void srvAppServerStorageManager::ChooseBuild(
      const char* SystemName,
      const char* BuildName
      ){};
sList* srvAppServerStorageManager::GetGroupUsers(
      const char* GroupName
      ){};
CORBA::Boolean srvAppServerStorageManager::DelSystemBuildDomain(
      const char* SystemName,
      const char* BuildName,
      const char* DomainName
      ){};
CORBA::Boolean srvAppServerStorageManager::AddSystemBuild(
      const char* SystemName,
      const char* BuildName
      ){};
CORBA::Boolean srvAppServerStorageManager::DelUserFromGroup(
      const char* GroupName,
      const char* UserName
      ){};
CORBA::Boolean srvAppServerStorageManager::DelSystemObject(
      const char* SystemName,
      const char* ObjectName,
      BaseType ObjectBaseType
      ){};
CORBA::Boolean srvAppServerStorageManager::DelSystemBuild(
      const char* SystemName,
      const char* BuildName
      ){};
sList* srvAppServerStorageManager::GetSubTypes(
      BaseType ObjBaseType
      ){};
sList* srvAppServerStorageManager::GetGroups(){};
CORBA::Boolean srvAppServerStorageManager::AddUserToGroup(
      const char* GroupName,
      const char* UserName
      ){};
CORBA::Boolean srvAppServerStorageManager::SetupSystemObject(
      const char* SystemName,
      const char* ObjectName,
      const char* ObjectBaseType,
      const char* ObjectSubType,
      const seqOctets& Octets
      ){};
CORBA::Boolean srvAppServerStorageManager::Connected(){};
CORBA::Boolean srvAppServerStorageManager::AddClientObject(
      const char* PlatformName,
      BaseType ObjBaseType,
      const char* ObjSubType,
      const seqOctets& Octets
      ){};
CORBA::Boolean srvAppServerStorageManager::AddPlatform(
      const char* Name
      ){};
CORBA::Boolean srvAppServerStorageManager::SetupSystemTask(
      const char* SystemName,
      const char* TaskName,
      const seqOctets& Octets
      ){};
CORBA::Boolean srvAppServerStorageManager::GetTaskRight(
      TaskRight& Right
      ){};
seqTwiceStrings* srvAppServerStorageManager::GetBuids(){};
CORBA::Boolean GetObjectRight(
      ObjectRight& Right
      ){};
CORBA::Boolean srvAppServerStorageManager::DelSystem(
      const char* SystemName
      ){};
seqOctets* srvAppServerStorageManager::GetObject(
      BaseType ObjBaseType,
      const char* ObjSubType,
      const char* ObjectName
      ){};
CORBA::Boolean srvAppServerStorageManager::DelUser(
      const char* UserName
      ){};
CORBA::Boolean srvAppServerStorageManager::DelClientObject(
      const char* PlatformName,
      BaseType ObjBaseType,
      const char* ObjSubType
      ){};
CORBA::Boolean srvAppServerStorageManager::SetObjectRight(
      const ObjectRight& Right
      ){};
CORBA::Boolean srvAppServerStorageManager::AddSystemBuildDomain(
      const char* SystemName,
      const char* BuildName,
      const char* DomainName,
      const char* CartrigeName
      ){};
void srvAppServerStorageManager::ChangePassword(
      const char* NewPassword
      ){};
seqOctets* srvAppServerStorageManager::GetTask(
      const char* Task
      ){};
CORBA::Boolean srvAppServerStorageManager::SetBuildRight(
      const BuildRight& Right
      ){};
CORBA::Boolean srvAppServerStorageManager::AddSystemTask(
      const char* SystemName,
      const char* TaskName
      ){};
CORBA::Boolean srvAppServerStorageManager::SetupSystemBuildDomain(
      const char* SystemName,
      const char* BuildName,
      const char* DomainName,
      const seqParams& SetupValues
      ){};
sList* srvAppServerStorageManager::GetUsers(){};
CORBA::Boolean srvAppServerStorageManager::SetTaskRight(
      const TaskRight& Right
      ){};
char* srvAppServerStorageManager::CurrentBuild(){};
CORBA::Boolean srvAppServerStorageManager::AddUser(
      const char* UserName,
      const char* Password,
      CORBA::Boolean IsAdmin
      ){};





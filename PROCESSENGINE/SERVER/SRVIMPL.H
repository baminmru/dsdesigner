//---------------------------------------------------------------------------
#ifndef srvImplH
#define srvImplH

class srvProcessEngine  : public  _sk_ProcessEngine {
protected:

  srvProcessEngine(const char *_obj_name = (const char *)NULL);
  srvProcessEngine(
      const char *_service_name,
      const CORBA::ReferenceData& _data);
  virtual ~srvProcessEngine() {}

public:

  virtual void AddDocument(
      CORBA::Long ConnectId,
      CORBA::Long MasterIndex,
      const char* TypeName,
      const char* DocumentName,
      CORBA::Boolean AllowVersions
      );

  virtual CORBA::Boolean GetVersioning(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName
      );
  virtual void AddBaseline(
      CORBA::Long ConnectId,
      const char* ProductName,
      CORBA::Long Version
      );
  virtual void ChangePassword(
      CORBA::Long ConnectId,
      const char* Password
      );
  virtual void SetType(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* StoreName
      );
  virtual void SetWorksetItem(
      CORBA::Long ConnectId,
      const char* Owner,
      const char* Name,
      const char* TypeName,
      const char* DocName,
      CORBA::Long Version
      );
  virtual Octets* GetUserList(
      CORBA::Long ConnectId
      );
  virtual void DeleteVersion(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName,
      CORBA::Long Version
      );
  virtual void DisConnect(
      CORBA::Long ConnectId
      );
  virtual CORBA::Long NewInstance();
  virtual void DelUserRole(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* RoleName,
      const char* UID
      );
  virtual Octets* GetDocumentList(
      CORBA::Long ConnectId
      );
  virtual Octets* GetUserWorksetList(
      CORBA::Long ConnectId,
      const char* Owner
      );
  virtual void LockDocument(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName,
      const char* UID
      );
  virtual Octets* GetVersionList(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName
      );
  virtual Octets* GetPackageList(
      CORBA::Long ConnectId
      );
  virtual void DelCycle(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* FromStateName,
      const char* ToStateName,
      const char* RoleName
      );
  virtual void DelBaseline(
      CORBA::Long ConnectId,
      const char* ProductName,
      CORBA::Long Version
      );
  virtual void SetVersionState(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName,
      CORBA::Long Version,
      const char* StateName
      );
  virtual void Connect(
      const char* UID,
      const char* PWD
      );
  virtual Octets* GetStateList(
      CORBA::Long ConnectId,
      const char* TypeName
      );
  virtual Octets* GetTypeList(
      CORBA::Long ConnectId
      );
  virtual void UnLockDocument(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName,
      const char* UID
      );
  virtual void AddState(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* StateName,
      CORBA::Boolean IsFinal
      );
  virtual void AddPackage(
      CORBA::Long ConnectId,
      const char* SoftwareName,
      CORBA::Long Build
      );
  virtual Octets* GetProductPackageList(
      CORBA::Long ConnectId,
      const char* SoftwareName
      );
  virtual Octets* GetProductBaselineList(
      CORBA::Long ConnectId,
      const char* ProductName
      );
  virtual void AddVersionDep(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName,
      CORBA::Long Version,
      const char* ProductName,
      CORBA::Long Build
      );
  virtual void DelBaselineDep(
      CORBA::Long ConnectId,
      const char* ProductName,
      CORBA::Long Version
      );
  virtual void UnLockVersion(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName,
      CORBA::Long Version,
      const char* UID
      );
  virtual Octets* GetPackageItemList(
      CORBA::Long ConnectId,
      const char* SoftwareName,
      CORBA::Long Build
      );
  virtual char* GetTypeStore(
      CORBA::Long ConnectId,
      const char* TypeName
      );
  virtual void DelUser(
      CORBA::Long ConnectId,
      const char* UID,
      const char* PWD
      );
  virtual void SetPackageItem(
      CORBA::Long ConnectId,
      const char* SoftwareName,
      CORBA::Long Build,
      const char* ItemName,
      const Octets& Info
      );
  virtual Octets* GetPackageDepList(
      CORBA::Long ConnectId,
      const char* SoftwareName,
      CORBA::Long Build
      );
  virtual CORBA::Boolean IsUpdatedVersion(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName,
      const Octets& Info
      );
  virtual void AddPackageDep(
      CORBA::Long ConnectId,
      const char* SoftwareName,
      CORBA::Long Build,
      const char* DepOnProductName,
      CORBA::Long DepOnVersion
      );
  virtual Octets* GetProductList(
      CORBA::Long ConnectId
      );
  virtual CORBA::Long SetNewVersion(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName,
      const Octets& Info
      );
  virtual void AddRole(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* RoleName
      );
  virtual void DelPackage(
      CORBA::Long ConnectId,
      const char* SoftwareName,
      CORBA::Long Build
      );
  virtual Octets* GetCycleList(
      CORBA::Long ConnectId,
      const char* TypeName
      );
  virtual void DelType(
      CORBA::Long ConnectId,
      const char* TypeName
      );
  virtual Octets* GetBaselineList(
      CORBA::Long ConnectId
      );
  virtual Octets* GetRoleList(
      CORBA::Long ConnectId,
      const char* TypeName
      );
  virtual Octets* GetWorksetItemList(
      CORBA::Long ConnectId,
      const char* Owner,
      const char* Name
      );
  virtual void CloseInstance(
      CORBA::Long ConnectId
      );
  virtual void AddProduct(
      CORBA::Long ConnectId,
      const char* ProductName
      );
  virtual Octets* GetBaselineItemList(
      CORBA::Long ConnectId,
      const char* ProductName,
      CORBA::Long Version
      );
  virtual void SetBaselineItem(
      CORBA::Long ConnectId,
      const char* ProductName,
      CORBA::Long Version,
      const char* TypeName,
      const char* DocName,
      CORBA::Long DocVersion
      );
  virtual void SetUserRole(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* RoleName,
      const char* UID
      );
  virtual CORBA::Long GetDocumentIndex(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName
      );
  virtual void AddWorkset(
      CORBA::Long ConnectId,
      const char* Owner,
      const char* Name
      );
  virtual void LockVersion(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName,
      CORBA::Long Version,
      const char* UID
      );
  virtual Octets* GetRoleUserList(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* RoleName
      );
  virtual void DelState(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* StateName
      );
  virtual void DelBaselineItem(
      CORBA::Long ConnectId,
      const char* ProductName,
      CORBA::Long Version,
      const char* TypeName,
      const char* DocName
      );
  virtual void AddCycle(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* FromStateName,
      const char* ToStateName,
      const char* RoleName
      );
  virtual Octets* GetVersionDepList(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName,
      CORBA::Long Version
      );
  virtual void DelWorksetItem(
      CORBA::Long ConnectId,
      const char* Owner,
      const char* Name,
      const char* TypeName,
      const char* DocName
      );
  virtual void AddSoftware(
      CORBA::Long ConnectId,
      const char* SoftwareName
      );
  virtual Octets* GetVersion(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName,
      CORBA::Long Version
      );
  virtual void DelRole(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* RoleName
      );
  virtual CORBA::Boolean IsConnected(
      CORBA::Long ConnectId
      );
  virtual Octets* GetWorksetList(
      CORBA::Long ConnectId
      );
  virtual void DelPackageDep(
      CORBA::Long ConnectId,
      const char* SoftwareName,
      CORBA::Long Build
      );
  virtual void DelVersionDep(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName,
      CORBA::Long Version
      );
  virtual Octets* GetBaselineDepList(
      CORBA::Long ConnectId,
      const char* ProductName,
      CORBA::Long Version
      );
  virtual void DelProduct(
      CORBA::Long ConnectId,
      const char* ProductName
      );
  virtual char* GetVersionState(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName,
      CORBA::Long Version
      );
  virtual Octets* GetPossibleStates(
      CORBA::Long ConnectId,
      const char* TypeName,
      const char* DocumentName
      );
  virtual Octets* GetSoftwareList(
      CORBA::Long ConnectId
      );
  virtual void SetUser(
      CORBA::Long ConnectId,
      const char* UID,
      const char* PWD,
      CORBA::Boolean IsAdmin
      );
  virtual void AddBaselineDep(
      CORBA::Long ConnectId,
      const char* ProductName,
      CORBA::Long Version,
      const char* DepOnProductName,
      CORBA::Long DepOnVersion
      );
  virtual void DelPackageItem(
      CORBA::Long ConnectId,
      const char* SoftwareName,
      CORBA::Long Build,
      const char* ItemName
      );
  virtual void DelWorkset(
      CORBA::Long ConnectId,
      const char* Owner,
      const char* Name
      );
  virtual CORBA::Boolean IsAmin(
      CORBA::Long ConnectId,
      const char* UID,
      const char* PWD
      );
  virtual void DelSoftware(
      CORBA::Long ConnectId,
      const char* SoftwareName
      );

};
//---------------------------------------------------------------------------
#endif

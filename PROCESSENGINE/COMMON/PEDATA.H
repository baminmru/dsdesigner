//---------------------------------------------------------------------------
#ifndef PEDataH
#define PEDataH


struct StoreRef{
  AnsiString StoreName;
  AnsiString TypeName; // "" - for package
  AnsiString ItemName;
  long Version;        // 0 - for package
};

struct Product{
  AnsiString Name;
  long MaxVersion;
};

struct Software{
  AnsiString Name;
  long MaxBuild;
};

struct Type {
  AnsiString Name;
  AnsiString StoreName;
};

struct State{
  AnsiString TypeName;
  AnsiString StateName;
  bool IsFinal;
};

struct Role{
  AnsiString TypeName;
  AnsiString RoleName;
};

struct Cycle{
  AnsiString TypeName;
  AnsiString FromStateName; // may be empty for creation
  AnsiString ToStateName;
  AnsiString RoleName;
};

struct User{
  AnsiString UID;
  AnsiString PWD;
  bool IsAdmin;
};

struct UserRole{
  AnsiString TypeName;
  AnsiString RoleName;
  AnsiString UID;
};

struct Locking{
  AnsiString UID;
  long LockCount;
};

struct Document{
  AnsiString TypeName;
  AnsiString DocName;
  AnsiString Owner;
  bool AllowVersions;
  long MaxVersion;

  long Level; // X - level in Document Tree
};


struct VersionRout{
  AnsiString TypeName;
  AnsiString DocName;
  long Version;
  AnsiString UID;
  AnsiString StateName;
  TDateTime  Stamp;
};


struct Version{
  AnsiString TypeName;
  AnsiString DocName;
  AnsiString Owner;
  long Version;
  bool Expired;
  MyArray<Locking> UsedBy;
  StoreRef Info;
};



struct WorkSet{
  AnsiString Owner;
  AnsiString Name;
  MyArray<Locking> UsedBy;
};


struct WorkSetRef{
  AnsiString Owner;
  AnsiString Name;
  AnsiString TypeName;
  AnsiString DocName;
  long Version;      // may be -1 for latest version
};



struct BaseLine{
  AnsiString ProductName;
  long Version;
};


struct BaseLineRef{
  AnsiString ProductName;
  long Version;
  AnsiString TypeName;
  AnsiString DocName;
  long DocVersion;
  StoreRef Info;
};

struct DocumentDependences{
  AnsiString TypeName;
  AnsiString DocName;
  long Version;
  AnsiString DepOnProductName;
  long DepOnVersion;
};


struct BaseLineDependences{
  AnsiString ProductName;
  long Version;
  AnsiString DepOnProductName;
  long DepOnVersion;
};


struct Package{
  AnsiString SoftwareName;
  long Build;
};


struct PackageItem{
  AnsiString SoftwareName;
  long Build;
  AnsiString Name;
  StoreRef Info;
};


struct PackageDependences{
  AnsiString SoftwareName;
  long Build;
  AnsiString ProductName;
  long Version;
};

////////////////////////////////////////////
struct PEStore{
  MyArray<Product> Products;
  MyArray<Software> Softwares;
  MyArray<Type> Types;
  MyArray<State> States;
  MyArray<Role> Roles;
  MyArray<Cycle> Cycles;
  MyArray<User> Users;
  MyArray<UserRole> UserRoles;
  MyArray<Document> Documents;
  MyArray<VersionRout> VersionRouts;
  MyArray<Version> Versions;
  MyArray<WorkSet> WorkSets;
  MyArray<WorkSetRef> WorkSetRefs;
  MyArray<BaseLine> BaseLines;
  MyArray<BaseLineRef> BaseLineRefs;
  MyArray<DocumentDependences> DocumentDependencies;
  MyArray<BaseLineDependences> BaseLineDependencies;
  MyArray<Package> Packages;
  MyArray<PackageItem> PackageItems;
  MyArray<PackageDependences> PackageDependencies;

};


//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#ifndef StoreObjectsH
#define StoreObjectsH

#include "store_s.hh"
#include "stackvalue.h"
#include "convertions.h"


struct UserInfo{
        AnsiString UID;
        AnsiString PWD;
        bool IsAdmin;
        friend SimpleStream & operator >> (SimpleStream & s,UserInfo & i);
        friend SimpleStream & operator << (SimpleStream & s,UserInfo & i);
};


struct ObjectInfo {
        AnsiString SystemName;
        AnsiString ComponentName;
        AnsiString ObjectName;
        DataBuffer ObjectDef;
        friend SimpleStream & operator >> (SimpleStream & s,ObjectInfo & i);
        friend SimpleStream & operator << (SimpleStream & s,ObjectInfo & i);
};


struct ObjectPlatformSpecifics {
        AnsiString SystemName;
        AnsiString ObjectName;
        AnsiString PlatformName;
        DataBuffer SpecificsDef;
        friend SimpleStream & operator >> (SimpleStream & s,ObjectPlatformSpecifics & i);
        friend SimpleStream & operator << (SimpleStream & s,ObjectPlatformSpecifics & i);
};


struct ComponentInfo {
        AnsiString PlatformName;
        AnsiString ComponentName;
        DataBuffer ComponentData;
        friend SimpleStream & operator >> (SimpleStream & s,ComponentInfo & i);
        friend SimpleStream & operator << (SimpleStream & s,ComponentInfo & i);
};


struct UserRights{
    AnsiString SystemName;
    AnsiString ObjectName;
    AnsiString UserName;
    bool Read;
    bool Write;
    bool Execute;

    friend SimpleStream & operator >> (SimpleStream & s,UserRights & i);
    friend SimpleStream & operator << (SimpleStream & s,UserRights & i);
};


struct UserObjectProperties{
    AnsiString SystemName;
    AnsiString ObjectName;
    AnsiString UserName;
    DataBuffer Info;

    friend SimpleStream & operator >> (SimpleStream & s,UserObjectProperties & i);
    friend SimpleStream & operator << (SimpleStream & s,UserObjectProperties & i);
};



struct UserBuildRight{
    AnsiString SystemName;
    AnsiString BuildName;
    AnsiString UserName;
    bool AllowAccess;

    friend SimpleStream & operator >> (SimpleStream & s,UserBuildRight & i);
    friend SimpleStream & operator << (SimpleStream & s,UserBuildRight & i);
};


struct Build{
    AnsiString SystemName;
    AnsiString BuildName;

    friend SimpleStream & operator >> (SimpleStream & s,Build & i);
    friend SimpleStream & operator << (SimpleStream & s,Build & i);
};

struct DataSourceDef{
    AnsiString SystemName;
    AnsiString DataSourceName;

    friend SimpleStream & operator >> (SimpleStream & s,DataSourceDef & i);
    friend SimpleStream & operator << (SimpleStream & s,DataSourceDef & i);
};

struct DataSourceSetup{
    AnsiString SystemName;
    AnsiString DataSourceName;
    AnsiString BuildName;
    AnsiString TargetName;
    DataBuffer Info;

    friend SimpleStream & operator >> (SimpleStream & s,DataSourceSetup & i);
    friend SimpleStream & operator << (SimpleStream & s,DataSourceSetup & i);
};


struct TargetPackInfo {
        AnsiString TargetName;
        AnsiString PackName;
        AnsiString PlatformName;
        DataBuffer PackData;
        friend SimpleStream & operator >> (SimpleStream & s,TargetPackInfo & i);
        friend SimpleStream & operator << (SimpleStream & s,TargetPackInfo & i);
};


struct ConnectInfo{
  AnsiString UID;
  AnsiString PWD;
  AnsiString PlatformName;


  friend SimpleStream & operator >> (SimpleStream & s,ConnectInfo & i);
  friend SimpleStream & operator << (SimpleStream & s,ConnectInfo & i);
};






//---------------------------------------------------------------------------
#endif

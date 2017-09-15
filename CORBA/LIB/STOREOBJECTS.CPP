//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "StoreObjects.h"

////////////////////////////////

SimpleStream & operator >> (SimpleStream & s,UserInfo & i){
  s >> i.UID;
  s >> i.PWD;
  s >> i.IsAdmin;
  return s;
};
SimpleStream & operator << (SimpleStream & s,UserInfo & i){
  s << i.UID;
  s << i.PWD;
  s << i.IsAdmin;
  return s;

};

//////////////////////////////

SimpleStream & operator >> (SimpleStream & s,ObjectInfo & i){
  s >> i.SystemName;
  s >> i.ComponentName;
  s >> i.ObjectName;
  s >> i.ObjectDef;
  return s;

};

SimpleStream & operator << (SimpleStream & s,ObjectInfo & i){
  s << i.SystemName;
  s << i.ComponentName;
  s << i.ObjectName;
  s << i.ObjectDef;
  return s;
};
////////////////////////////////////
SimpleStream & operator >> (SimpleStream & s,ObjectPlatformSpecifics & i){
  s >> i.SystemName;
  s >> i.ObjectName;
  s >> i.PlatformName;
  s >> i.SpecificsDef;
  return s;
};
SimpleStream & operator << (SimpleStream & s,ObjectPlatformSpecifics & i){
  s << i.SystemName;
  s << i.ObjectName;
  s << i.PlatformName;
  s << i.SpecificsDef;
  return s;
};


///////////////////////////////////
SimpleStream & operator >> (SimpleStream & s,ComponentInfo & i){
  s >> i.PlatformName;
  s >> i.ComponentName;
  s >> i.ComponentData;
  return s;
};

SimpleStream & operator << (SimpleStream & s,ComponentInfo & i){
  s << i.PlatformName;
  s << i.ComponentName;
  s << i.ComponentData;
  return s;
};

///////////////////////////////////


///////////////////////////
SimpleStream & operator >> (SimpleStream & s,UserRights & i){
  s >> i.Read;
  s >> i.Write;
  s >> i.Execute;
  s >> i.SystemName;
  s >> i.ObjectName;
  s >> i.UserName;
  return s;
};
SimpleStream & operator << (SimpleStream & s,UserRights & i){
  s << i.Read;
  s << i.Write;
  s << i.Execute;
  s << i.SystemName;
  s << i.ObjectName;
  s << i.UserName;
  return s;
};


SimpleStream & operator >> (SimpleStream & s,UserObjectProperties & i){
  s >> i.UserName;
  s >> i.ObjectName;
  s >> i.SystemName;
  s >> i.Info;
  return s;

};

SimpleStream & operator << (SimpleStream & s,UserObjectProperties & i){
  s << i.UserName;
  s << i.ObjectName;
  s << i.SystemName;
  s << i.Info;
  return s;
};

////////////////////////////////
SimpleStream & operator >> (SimpleStream & s,UserBuildRight & i){
  s >> i.SystemName;
  s >> i.BuildName;
  s >> i.UserName;
  s >> i.AllowAccess;
  return s;

};
SimpleStream & operator << (SimpleStream & s,UserBuildRight & i){
  s << i.SystemName;
  s << i.BuildName;
  s << i.UserName;
  s << i.AllowAccess;
  return s;
};

////////////////////////////////////////////////
SimpleStream & operator >> (SimpleStream & s,Build & i){
  s >> i.SystemName;
  s >> i.BuildName;
  return s;
};
SimpleStream & operator << (SimpleStream & s,Build & i){
  s << i.SystemName;
  s << i.BuildName;
  return s;
};

////////////////////////////////////////////////
SimpleStream & operator >> (SimpleStream & s,DataSourceDef & i){
  s >> i.SystemName;
  s >> i.DataSourceName;
  return s;
};
SimpleStream & operator << (SimpleStream & s,DataSourceDef & i){
  s << i.SystemName;
  s << i.DataSourceName;
  return s;

};

////////////////////////////////////////////////
SimpleStream & operator >> (SimpleStream & s,DataSourceSetup & i){
  s >> i.SystemName;
  s >> i.DataSourceName;
  s >> i.BuildName;
  s >> i.TargetName;
  s >> i.Info;
  return s;

};
SimpleStream & operator << (SimpleStream & s,DataSourceSetup & i){
  s << i.SystemName;
  s << i.DataSourceName;
  s << i.BuildName;
  s << i.TargetName;
  s << i.Info;
  return s;

};


////////////////////////////////////////////////
SimpleStream & operator >> (SimpleStream & s,TargetPackInfo & i){
  s >> i.TargetName;
  s >> i.PackName;
  s >> i.PlatformName;
  s >> i.PackData;
  return s;
};

SimpleStream & operator << (SimpleStream & s,TargetPackInfo & i){
  s << i.TargetName;
  s << i.PackName;
  s << i.PlatformName;  
  s << i.PackData;
  return s;
};


///////////////////////////////

SimpleStream & operator >> (SimpleStream & s,ConnectInfo & i){
  s >> i.UID;
  s >> i.PWD;
  s >> i.PlatformName;
  return s;
};
SimpleStream & operator << (SimpleStream & s,ConnectInfo & i){
  s << i.UID;
  s << i.PWD;
  s << i.PlatformName;
  return s;

};


//---------------------------------------------------------------------------
#pragma package(smart_init)

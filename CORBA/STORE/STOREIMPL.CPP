//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "StoreImpl.h"
#include "Convertions.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
////////////////////////////////////////////////
bool srvObjDefStorage::Connected(   CORBA::Long ConnectId){
      ConnectInfo * cptr = (ConnectInfo*)ConnectId;
      if(Connections.Where(cptr)==-1) return false;
      if(UserID(cptr->UID.c_str()) >=0)  return true;
      return false;
  };
////////////////////////////////////////////////
bool srvObjDefStorage::AdminUser(  CORBA::Long ConnectId){
    if(!Connected(ConnectId)) return false;
    ConnectInfo * cptr = (ConnectInfo*)ConnectId;
      for(long i=0;i<Users.Count();i++){
        if(Users[i]->UID==cptr->UID){
          if(!Users[i]->IsAdmin) return false;
            return true;
        }
      }
      return false;
  };


////////////////////////////////////////////////
void srvObjDefStorage::LoadSrv(){
  MyMemory f;
  f.LoadFromFileDecode("SRV.SRV");
  f >> Platforms;
  f >> Components;
  f >> Targets;
  f >> TargetPacks;
  f >> Systems;
  f >> Objects;
  f >> Sources;
  f >> Builds;
  f >> Specifics;
  f >> Users;
  f >> Rights;
  f >> LocalSettings;
  f >> BuildRights;
  f >> Setups;
  if(!Users.Count()){
    UserInfo *ui = new UserInfo();
    ui->UID="bami";
    ui->PWD="master";
    ui->IsAdmin=true;
    Users.Add(ui);
  }

  if(!Platforms.Count()){
    Platforms.Add( new AnsiString("TEST"));
  }

};
////////////////////////////////////////////////
void srvObjDefStorage::SaveSrv(){
  MyMemory f;
  f << Platforms;
  f << Components;
  f << Targets;
  f << TargetPacks;
  f << Systems;
  f << Objects;
  f << Sources;
  f << Builds;
  f << Specifics;
  f << Users;
  f << Rights;
  f << LocalSettings;
  f << BuildRights;
  f << Setups;
  f.SaveToFileEncode("SRV.SRV");
};


/////////////////////////////////////////////
long srvObjDefStorage::UserID(const char * UserName){
      for(long j=0;j<Users.Count();j++){
        if(Users[j]->UID==UserName) return j;
      }
      return -1;
};

////////////////////////////////////////////////
long srvObjDefStorage::SystemID(const char * SystemName){
      for(long j=0;j<Systems.Count();j++){
        if(*Systems[j]==SystemName) return j;
      }
      return -1;
};
////////////////////////////////////////////////
long srvObjDefStorage::TargetID(const char * TargetName){
      for(long j=0;j<Targets.Count();j++){
        if(*Targets[j]==TargetName) return j;
      }
      return -1;
};
////////////////////////////////////////////////
long srvObjDefStorage::PlatformID(const char * PlatformName){
      for(long j=0;j<Platforms.Count();j++){
        if(*Platforms[j]==PlatformName) return j;
      }
      return -1;
};

////////////////////////////////////////////////
long srvObjDefStorage::ObjectID(const char * SystemName,const char * ObjectName){
    if (SystemID(SystemName)==-1) return -1;
    for(long j=0;j<Objects.Count();j++){
        if(Objects[j]->ObjectName==ObjectName &&
           Objects[j]->SystemName==SystemName
        ) return j;
    }
    return -1;

};

////////////////////////////////////////////////
long srvObjDefStorage::SourceID(const char * SystemName,const char * DataSourceName){
    if (SystemID(SystemName)==-1) return -1;
    for(long j=0;j<Sources.Count();j++){
        if(Sources[j]->DataSourceName==DataSourceName &&
           Sources[j]->SystemName==SystemName
        ) return j;
    }
    return -1;

};

////////////////////////////////////////////////
long srvObjDefStorage::BuildID(const char * SystemName,const char * BuildName){
    if (SystemID(SystemName)==-1) return -1;
    for(long j=0;j<Builds.Count();j++){
        if(Builds[j]->BuildName==BuildName &&
           Builds[j]->SystemName==SystemName
        ) return j;
    }
    return -1;
};

////////////////////////////////////////////////
void srvObjDefStorage::ChangePassword(
      CORBA::Long ConnectId,
      const char* Password
      ){
  if(!Connected(ConnectId)) return;
  ConnectInfo * cptr = (ConnectInfo*)ConnectId;
  long id = UserID(cptr->UID.c_str());
  if(id==-1) { DisConnect(ConnectId); return; }
  Users[id]->PWD=Password;
  cptr->PWD=Password;
  SaveSrv();
};


////////////////////////////////////////////////
void srvObjDefStorage::DisConnect(long ConnectId){
  if(!Connected(ConnectId)) return;
  ConnectInfo * cptr = (ConnectInfo*)ConnectId;
  cout << "User: " << cptr->UID.c_str() << " disconnected from server" << endl;
  Connections.Destroy(Connections.Where(cptr));
};

////////////////////////////////////////////////////
CORBA::Long srvObjDefStorage::Connect(
      const char* UID,
      const char* PWD,
      const char* PlatformName
   ){
  for(long i=0;i<Users.Count();i++){
    if (Users[i]->UID==UID && Users[i]->PWD==PWD){
      for(long j=0;j<Platforms.Count();j++){
        if(*Platforms[j]==PlatformName){
         ConnectInfo * ci = new ConnectInfo;
         ci->UID=UID;
         ci->PWD=PWD;
         ci->PlatformName=PlatformName;
         Connections.Add(ci);
         cout << "User: " << UID << " from platform "<< PlatformName  << " connected to server" << endl;
         return (long) ci;
        }
      }
      cout << "User: " <<UID <<  " from platform "<< PlatformName << " login failed (Unknown platform)" << endl;
      return -1;
    }
  }
  cout << "User: " <<UID <<  " from platform "<< PlatformName << " login failed (Unknown login)" << endl;
  return -1;
};


/////////////////////////////////////////////////////
Octets* srvObjDefStorage::GetTargetPackSetup(
      CORBA::Long ConnectId,
      const char* BuildName,
      const char* SystemName,
      const char* DataSource
      ){

     if(!Connected(ConnectId)) return new Octets;


      DataSourceSetup *tpi;
      for(long i=0;i<Setups.Count();i++){
        tpi=Setups[i];
        if(tpi &&
           tpi->BuildName==BuildName &&
           tpi->SystemName==SystemName &&
           tpi->DataSourceName==DataSource
           ){
        MyMemory m;
        m << *tpi;
        return MakeOctets(m);
       }
      }
      return new Octets;
};
////////////////////////////////////////////////
Octets* srvObjDefStorage::GetComponentList(
      CORBA::Long ConnectId,
      const char* Platform
      ){
      if(!Connected(ConnectId)) return new Octets;
      MyArray<AnsiString> s;
      for(long i=0;i< Components.Count();i++){
        if(Components[i] &&
           Components[i]->PlatformName == Platform){
             s.Add(new AnsiString(Components[i]->ComponentName));
           }
      }
      MyMemory m;
      m << s;
      return MakeOctets(m);
};


void srvObjDefStorage::SetComponent(
      CORBA::Long ConnectId,
      const char* Platform,
      const char* ComponentName,
      const Octets& Data
      ){

      if(!AdminUser(ConnectId)) return;


      for(long i =0;i<Components.Count();i++){
        if(Components[i] &&
           Components[i]->PlatformName == Platform &&
           Components[i]->ComponentName == ComponentName){
           SetDataBuffer(Components[i]->ComponentData,Data);
           SaveSrv();
           return;
           }
      }

      // check platforms
      for(long i=0;i<Platforms.Count();i++){
        if(*Platforms[i]==Platform){

            // create new component
            ComponentInfo * ptr = new ComponentInfo;
            Components.Add(ptr);
            ptr->PlatformName = Platform;
            ptr->ComponentName = ComponentName;
            SetDataBuffer(ptr->ComponentData,Data);
            SaveSrv();
            return;
        }
      }
};

////////////////////////////////////////////////
Octets* srvObjDefStorage::GetObjectPlatformSpecifics(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* PlatformName
){
     if(!Connected(ConnectId)) return new Octets;
     if(ObjectID(SystemName,ObjectName) ==-1) return new Octets;
     if(PlatformID(PlatformName)==-1) return new Octets;

     for(long i=0;i<Specifics.Count();i++){
      if(
        Specifics[i]->SystemName==SystemName &&
        Specifics[i]->ObjectName==ObjectName &&
        Specifics[i]->PlatformName==PlatformName
      ){
        return MakeOctets(Specifics[i]->SpecificsDef);
      }
     }

     return new Octets;
};
////////////////////////////////////////////////
void srvObjDefStorage::SetTargetPack(
      CORBA::Long ConnectId,
      const char* Target,
      const char* PackName,
      const char* PlatformName,
      const Octets& Data
      ){
 if(!AdminUser(ConnectId)) return;
 if(TargetID(Target)==-1) return;

      for(long i =0;i<TargetPacks.Count();i++){
        if(TargetPacks[i]->TargetName == Target &&
           TargetPacks[i]->PackName == PackName &&
           TargetPacks[i]->PlatformName == PlatformName
           ){
           SetDataBuffer(TargetPacks[i]->PackData,Data);
           SaveSrv();
           return;
           }
      }



      // create new component
      TargetPackInfo * ptr = new TargetPackInfo;
      TargetPacks.Add(ptr);
      ptr->TargetName = Target;
      ptr->PackName = PackName;
      ptr->PlatformName = PlatformName;
      SetDataBuffer(ptr->PackData,Data);
      SaveSrv();
      return;

};

////////////////////////////////////////////////
Octets* srvObjDefStorage::GetTargetList(  CORBA::Long ConnectId){
  if(!AdminUser(ConnectId)) return new Octets;
  MyMemory m;
  m<< Targets;
  return MakeOctets(m);
};

////////////////////////////////////////////////
void srvObjDefStorage::DelTargetPack(
      CORBA::Long ConnectId,
      const char* Target,
      const char* PackName,
      const char* PlatformName

      ){
      if(!AdminUser(ConnectId)) return;
      for(long i =0; i<TargetPacks.Count();i++){
         if(TargetPacks[i]->TargetName == Target &&
            TargetPacks[i]->PackName == PackName &&
            TargetPacks[i]->PlatformName == PlatformName
            ){
            TargetPacks.Destroy(i);
            SaveSrv();
            return;
         }

      }
};
////////////////////////////////////////////////
Octets* srvObjDefStorage::GetComponent(
      CORBA::Long ConnectId,
      const char* Platform,
      const char* ComponentName
      ){
  if(!Connected(ConnectId)) return new Octets;
  if(PlatformID(Platform) ==-1) return new Octets;
  for(long i=0;i<Components.Count();i++){
    if(
     Components[i]->PlatformName==Platform &&
     Components[i]->ComponentName==ComponentName
        ){
      return MakeOctets(Components[i]->ComponentData);
    }

  }
  return new Octets;
};
////////////////////////////////////////////////
Octets* srvObjDefStorage::GetBuildList(CORBA::Long ConnectId){
  if(!Connected(ConnectId)) return new Octets;
  ConnectInfo * cptr = (ConnectInfo*)ConnectId;
  AnsiString UID = cptr->UID;

  MyArray<Build> UsrBuilds;
  Build * ptr;
  for(long i=0;i<BuildRights.Count();i++){
    if(BuildRights[i]->UserName==UID && BuildRights[i]->AllowAccess){
      long id=BuildID(BuildRights[i]->SystemName.c_str(),BuildRights[i]->BuildName.c_str());
      if(id>=0){
         ptr = new Build();
         ptr->SystemName=BuildRights[i]->SystemName;
         ptr->BuildName=BuildRights[i]->BuildName;
         UsrBuilds.Add(ptr);
      }
    }
  }

  MyMemory m;
  m << UsrBuilds;
  return MakeOctets(m);

};
////////////////////////////////////////////////


void srvObjDefStorage::AddSystem(
      CORBA::Long ConnectId,
      const char* SystemName
      ){
      if(!AdminUser(ConnectId)) return;

      // no duplications
      if(SystemID(SystemName)>=0) return;

      Systems.Add(new AnsiString(SystemName));
};

////////////////////////////////////////////////
void srvObjDefStorage::SetObjectLocals(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* UserName,
      const Octets& Locals
      ){
      if(!Connected(ConnectId)) return;
      if(ObjectID(SystemName,ObjectName)==-1) return;

      // serch for existing settings
      for(long i=0;i<LocalSettings.Count();i++){
        if(
          LocalSettings[i]->SystemName==SystemName &&
          LocalSettings[i]->ObjectName==ObjectName &&
          LocalSettings[i]->UserName==UserName
        ){
          SetDataBuffer(LocalSettings[i]->Info,Locals);
          SaveSrv();
          return;
        }

      }

      // add new one
      UserObjectProperties * ptr = new UserObjectProperties;
      ptr->SystemName=SystemName;
      ptr->ObjectName=ObjectName;
      ptr->UserName=UserName;
      SetDataBuffer(ptr->Info,Locals);
      LocalSettings.Add(ptr);
      SaveSrv();

};
////////////////////////////////////////////////
void srvObjDefStorage::AddTarget(
      CORBA::Long ConnectId,
      const char* TargetName
){
    if(!AdminUser(ConnectId)) return;
    if(TargetID(TargetName)>=0) return;
    Targets.Add( new AnsiString(TargetName));
    SaveSrv();
};

////////////////////////////////////////////////
void srvObjDefStorage::DelObjectLocals(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* UserName
      )
{
  if(!AdminUser(ConnectId)) return;
      //if(ObjectID(SystemName,ObjectName)==-1) return;

      // serch for existing settings
      for(long i=0;i<LocalSettings.Count();i++){
        if(
          LocalSettings[i]->SystemName==SystemName &&
          LocalSettings[i]->ObjectName==ObjectName &&
          LocalSettings[i]->UserName==UserName
        ){
          LocalSettings.Destroy(i);
          SaveSrv();
          return;
        }

      }

};
////////////////////////////////////////////////
Octets* srvObjDefStorage::GetPlatformList(   CORBA::Long ConnectId){
  if(!AdminUser(ConnectId)) return new Octets;
  MyMemory m;
  m << Platforms;
  return MakeOctets(m);
};
////////////////////////////////////////////////

Octets* srvObjDefStorage::GetDataSourceList(
   CORBA::Long ConnectId,
   const char* SystemName
      ){
  if(!AdminUser(ConnectId)) return new Octets;
  if(SystemID(SystemName)==-1) return new Octets;

  MyArray<AnsiString> src;

  for(long i =0;i<Sources.Count();i++){
    if(Sources[i]->SystemName==SystemName){
      src.Add( new AnsiString(Sources[i]->DataSourceName));
    }
  }
  MyMemory m;
  m << src;
  return MakeOctets(m);

};

////////////////////////////////////////////////
void srvObjDefStorage::SetTargetPackSetup(
   CORBA::Long ConnectId,
      const char* BuildName,
      const char* SystemName,
      const char* DataSource,
      const char* TargetName,
      const Octets& Specifics
      ){
       if(!AdminUser(ConnectId)) return;
       if(BuildID(SystemName,BuildName)==-1) return;
       if(SourceID(SystemName,DataSource)==-1) return;
       if(TargetID(TargetName)==-1) return;

       DataSourceSetup *ptr;
       // serch for existing object
       for(long i=0;i<Setups.Count();i++){
         ptr=Setups[i];
        if(
            ptr->SystemName==SystemName &&
            ptr->DataSourceName==DataSource &&
            ptr->BuildName==BuildName
        ){
          SetDataBuffer(ptr->Info,Specifics);
          SaveSrv();
          return;
        }


       }

       // add new one
      ptr = new DataSourceSetup;
      ptr->SystemName=SystemName;
      ptr->DataSourceName=DataSource;
      ptr->BuildName=BuildName;
      ptr->TargetName=TargetName;
      SetDataBuffer(ptr->Info,Specifics);
      Setups.Add(ptr);
      SaveSrv();
};
////////////////////////////////////////////////

Octets* srvObjDefStorage::GetSystemList(   CORBA::Long ConnectId){
  if(!AdminUser(ConnectId)) return new Octets;
  MyMemory m;
  m << Systems;
  return MakeOctets(m);
};

////////////////////////////////////////////////
void srvObjDefStorage::SetObjectPlatformSpecifics(
   CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* PlatformName,
      const Octets& SpecificsData
      ){
       if(!AdminUser(ConnectId)) return;
       if(ObjectID(SystemName,ObjectName)==-1) return;
       if(PlatformID(PlatformName)==-1) return;

       ObjectPlatformSpecifics * ptr;
       for(long i=0;i<Specifics.Count();i++){
         ptr=Specifics[i];
         if(
          ptr->SystemName==SystemName &&
          ptr->ObjectName==ObjectName &&
          ptr->PlatformName==PlatformName
         ){
          SetDataBuffer(ptr->SpecificsDef,SpecificsData);
          SaveSrv();
          return;
         }
       }
        ptr = new ObjectPlatformSpecifics;
        ptr->SystemName=SystemName;
        ptr->ObjectName=ObjectName;
        ptr->PlatformName=PlatformName;
        SetDataBuffer(ptr->SpecificsDef,SpecificsData);
        Specifics.Add(ptr);
        SaveSrv();
      };

////////////////////////////////////////////////
Octets* srvObjDefStorage::GetTargetPack(
   CORBA::Long ConnectId,
      const char* Target,
      const char* PackName,
      const char* PlatformName
      ){
  if(!Connected(ConnectId)) return new Octets;
  if(TargetID(Target)==-1) return new Octets;
  if(PlatformID(PlatformName)==-1) return new Octets;
  for(long i=0;i<TargetPacks.Count();i++){
    if(
      TargetPacks[i]->TargetName==Target &&
      TargetPacks[i]->PackName==PackName &&
      TargetPacks[i]->PlatformName==PlatformName
    ){
      return MakeOctets(TargetPacks[i]->PackData);

    }
  }
  return new Octets;
};

////////////////////////////////////////////////
Octets* srvObjDefStorage::GetObjectLocals(
   CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* UserName
      ){
        if(!Connected(ConnectId)) return new Octets;
        ConnectInfo * cptr = (ConnectInfo*)ConnectId;
        if(ObjectID(SystemName,ObjectName)==-1) return new Octets;
        if(cptr->UID!=UserName && !AdminUser(ConnectId)) return new Octets;
        for(long i=0;i<LocalSettings.Count();i++){
          if(
          LocalSettings[i]->SystemName==SystemName &&
          LocalSettings[i]->ObjectName==ObjectName &&
          LocalSettings[i]->UserName==UserName

          ){
            return MakeOctets(LocalSettings[i]->Info);
          }

        }

        return new Octets;
};
////////////////////////////////////////////////
void srvObjDefStorage::DelObjectPlatformSpecifics(
   CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* PlatformName
      ){
       if(!AdminUser(ConnectId)) return ;
       if(ObjectID(SystemName,ObjectName)==-1) return ;
       if(PlatformID(PlatformName)==-1) return ;

        for(long i=0;i<Specifics.Count();i++){
              if(
                Specifics[i]->SystemName==SystemName &&
                Specifics[i]->ObjectName==ObjectName &&
                Specifics[i]->PlatformName==PlatformName
              ){
                Specifics.Destroy(i);
                SaveSrv();
                return ;
              }

        }

     };

////////////////////////////////////////////////
void srvObjDefStorage::DelBuild(
   CORBA::Long ConnectId,
      const char* BuildName,
      const char* SystemName
      ){
        if(!AdminUser(ConnectId)) return;
        long id=BuildID(SystemName,BuildName);
        if(id==-1) return;
        Builds.Destroy(id);
        SaveSrv();
};


////////////////////////////////////////////////
void srvObjDefStorage::AddBuild(
   CORBA::Long ConnectId,
      const char*BuildName,
      const char*SystemName
      )
{
        if(!AdminUser(ConnectId)) return;
        if(BuildID(SystemName,BuildName)>=0) return;
        Build * ptr = new Build;
        ptr->BuildName=BuildName;
        ptr->SystemName=SystemName;
        Builds.Add(ptr);
        SaveSrv();
};

////////////////////////////////////////////////

void srvObjDefStorage::SetObjectRights(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* UserName,
      CORBA::Boolean Exec,
      CORBA::Boolean Read,
      CORBA::Boolean Write
      ){
       if(!AdminUser(ConnectId)) return;
       if(ObjectID(SystemName,ObjectName)==-1) return;
       if(UserID(UserName)==-1) return;
       for(long i=0;i<Rights.Count();i++){
        if(
        Rights[i]->SystemName==SystemName &&
        Rights[i]->ObjectName==ObjectName &&
        Rights[i]->UserName==UserName
        ){
          Rights[i]->Execute=Exec;
          Rights[i]->Read=Read;
          Rights[i]->Write=Write;
          SaveSrv();
          return;
        }
       }

       UserRights * ptr = new UserRights;

       ptr->SystemName=SystemName;
       ptr->ObjectName=ObjectName;
       ptr->UserName=UserName;
       ptr->Execute=Exec;
       ptr->Read=Read;
       ptr->Write=Write;
       Rights.Add(ptr);
       SaveSrv();

      };
////////////////////////////////////////////////

void srvObjDefStorage::DelObjectRights(
   CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* UserName
      ){
       if(!AdminUser(ConnectId)) return;
       if(ObjectID(SystemName,ObjectName)==-1) return;
       if(UserID(UserName)==-1) return;
       for(long i=0;i<Rights.Count();i++){
        if(
        Rights[i]->SystemName==SystemName &&
        Rights[i]->ObjectName==ObjectName &&
        Rights[i]->UserName==UserName
        ){
          Rights.Destroy(i);
          SaveSrv();
          return;
        }
       }

      };

////////////////////////////////////////////////
void srvObjDefStorage::AddPlatform(
   CORBA::Long ConnectId,
      const char* PlatformName
      ){
       if(!AdminUser(ConnectId)) return;
       if(PlatformID(PlatformName)>=0) return;
       Platforms.Add( new AnsiString(PlatformName));
       SaveSrv();

      };

////////////////////////////////////////////////
Octets* srvObjDefStorage::GetUserList(   CORBA::Long ConnectId){
  if(!AdminUser(ConnectId)) return new Octets;

  MyArray<AnsiString> uu;

  for(long i=0;i<Users.Count();i++){
    uu.Add( new AnsiString(Users[i]->UID));
  }

  MyMemory m;
  m << uu;
  return MakeOctets(m);


};

////////////////////////////////////////////////
void srvObjDefStorage::DelSystem(
   CORBA::Long ConnectId,
      const char* SystemName
){
      if(!AdminUser(ConnectId)) return;
      for(long i=0;i<Systems.Count();i++){
        if(*Systems[i]==SystemName)
          Systems.Destroy(i);
      }
};

////////////////////////////////////////////////
void srvObjDefStorage::DelTargetPackSetup(
   CORBA::Long ConnectId,
      const char* BuildName,
      const char* SystemName,
      const char* DataSource
      ){
       if(!AdminUser(ConnectId)) return;
        DataSourceSetup *tpi;
        for(long i=0;i<Setups.Count();i++){
        tpi=Setups[i];
        if(tpi &&
           tpi->BuildName==BuildName &&
           tpi->SystemName==SystemName &&
           tpi->DataSourceName==DataSource
           ){
           Setups.Destroy(i);
           SaveSrv();
           return;
       }
      }


};

////////////////////////////////////////////////
void srvObjDefStorage::DelTarget(
   CORBA::Long ConnectId,
      const char* TargetName
      ){
       if(!AdminUser(ConnectId)) return;
       for(long i=0;i<Targets.Count();i++){
        if(*Targets[i]==TargetName){
          Targets.Destroy(i);
          SaveSrv();
        }
       }

};

////////////////////////////////////////////////
Octets* srvObjDefStorage::GetObject(
   CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName
){
  if(!Connected(ConnectId)) return new Octets;
  long id = ObjectID(SystemName,ObjectName);
  if(id==-1) return new Octets;
  MyMemory m;
  m << *(Objects[id]);
  return MakeOctets(m);

};

////////////////////////////////////////////////

void srvObjDefStorage::DelUser(
   CORBA::Long ConnectId,
      const char* UserName
      ){

       if(!AdminUser(ConnectId)) return;
       for(long i=0;i<Users.Count();i++){
        if(Users[i]->UID==UserName){
          Users.Destroy(i);
          SaveSrv();
          return;
        }
       }

      };

////////////////////////////////////////////////
void srvObjDefStorage::DelObject(
   CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName
      ){
       if(!AdminUser(ConnectId)) return;
       long i=ObjectID(SystemName,ObjectName);
       if(i>=0 ){
          Objects.Destroy(i);
          SaveSrv();
          return;
       }

      };

//////////////////////////////////////////////////////
Octets* srvObjDefStorage::GetUser(
      CORBA::Long ConnectId,
      const char* UserName){
       if(!AdminUser(ConnectId)) return new Octets ;
       MyMemory m;
       long i=UserID(UserName);

       if(i>=0){
        m << *(Users[i]);
       }
       return MakeOctets(m);
};


/////////////////////////////////////////////////////
void srvObjDefStorage::SetUser(
      CORBA::Long ConnectId,
      const char* UserName,
      const char* Password,
      CORBA::Boolean IsAdmin
      ){
       if(!AdminUser(ConnectId)) return;
       long i=UserID(UserName);
       if(i>=0){
        Users[i]->PWD=Password;
        Users[i]->IsAdmin=IsAdmin;
       }else{
        UserInfo * ptr = new UserInfo;
        ptr->UID=UserName;
        ptr->PWD=Password;
        ptr->IsAdmin=IsAdmin;
        Users.Add(ptr);
       }
       SaveSrv();

      };

/////////////////////////////////////////////////////
void srvObjDefStorage::AddDataSource(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* SourceName
      ){
       if(!AdminUser(ConnectId)) return;
       if(SourceID(SystemName,SourceName)>=0) return;
       DataSourceDef * ptr = new DataSourceDef;
       ptr->SystemName=SystemName;
       ptr->DataSourceName=SourceName;
       Sources.Add(ptr);
       SaveSrv();
};



void srvObjDefStorage::DelDataSource(
   CORBA::Long ConnectId,
      const char* SystemName,
      const char* SourceName
      ){
       if(!AdminUser(ConnectId)) return;
       long id = SourceID(SystemName,SourceName);
       if(id >=0){
         Sources.Destroy(id);
         SaveSrv();
       }
      };

///////////////////////////////////////////////////
void srvObjDefStorage::DelPlatform(
   CORBA::Long ConnectId,
      const char* PlatformName
      ){

       if(!AdminUser(ConnectId)) return;
       long id = PlatformID(PlatformName);
       if(id >=0){
         Platforms.Destroy(id);
         SaveSrv();
       }
      };

///////////////////////////////////////////////////
void srvObjDefStorage::SetObject(
   CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* ComponentName,
      const Octets& ObjData
      ){
        if(!AdminUser(ConnectId)) return;
        long id = ObjectID(SystemName,ObjectName);
        if(id>=0){
           SetDataBuffer(Objects[id]->ObjectDef,ObjData);
           Objects[id]->ComponentName=ComponentName;
        }else{
          ObjectInfo * ptr = new ObjectInfo;
          ptr->SystemName=SystemName;
          ptr->ObjectName=ObjectName;
          ptr->ComponentName=ComponentName;
          SetDataBuffer(ptr->ObjectDef,ObjData);
          Objects.Add(ptr);
        }

        SaveSrv();
      };


/////////////////////////////////////////////////////
Octets* srvObjDefStorage::GetObjectRights(
   CORBA::Long ConnectId,
      const char* SystemName,
      const char* ObjectName,
      const char* UserName
      ){
  if(!Connected(ConnectId)) return new Octets;
  ConnectInfo * cptr = (ConnectInfo*)ConnectId;
  if(ObjectID(SystemName,ObjectName)==-1) return new Octets;

  AnsiString UID = cptr->UID;

  if(UID!=UserName && !AdminUser(ConnectId)) return new Octets;

  for (long i=0;i<Rights.Count();i++){
    if(
      Rights[i]->SystemName==SystemName &&
      Rights[i]->ObjectName==ObjectName &&
      Rights[i]->UserName==UserName

    ){
      MyMemory m;
      m << *Rights[i];
      return MakeOctets(m);
    }

  }
  UserRights  ptr;
  ptr.SystemName=SystemName;
  ptr.ObjectName=ObjectName;
  ptr.UserName=UserName;
  ptr.Execute=false;
  ptr.Read=false;
  ptr.Write=false;

  MyMemory m;
  m << ptr;
  return MakeOctets(m);

};

/////////////////////////////////////////////////////
void srvObjDefStorage::DelComponent(
   CORBA::Long ConnectId,
      const char* Platform,
      const char* ComponentName
      ){
      if(!AdminUser(ConnectId)) return;
      for(long i=0;i<Components.Count();i++){
        if(Components[i]->PlatformName==Platform &&
           Components[i]->ComponentName==ComponentName){
           Components.Destroy(i);
           SaveSrv();
           return;
        }
      }

};



void srvObjDefStorage::SetBuildRights(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* BuildName,
      const char* UserName,
      CORBA::Boolean AllowAccess
      ){
        if(!AdminUser(ConnectId)) return;
        if(BuildID(SystemName,BuildName)==-1) return;
        if(UserID(UserName)==-1) return;
        for(long i=0;i<BuildRights.Count();i++){
           if(
              BuildRights[i]->SystemName==SystemName &&
              BuildRights[i]->BuildName==BuildName &&
              BuildRights[i]->UserName==UserName
           ){

                BuildRights[i]->AllowAccess = AllowAccess;
                SaveSrv();
                return;
           }
        }
        UserBuildRight * ptr =  new UserBuildRight;
        ptr->SystemName=SystemName;
        ptr->BuildName=BuildName;
        ptr->UserName=UserName;
        ptr->AllowAccess = AllowAccess;
        BuildRights.Add(ptr);
        SaveSrv();
        return;
      };
void srvObjDefStorage::DelBuildRights(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* BuildName,
      const char* UserName
      ){
        if(!AdminUser(ConnectId)) return;
        if(BuildID(SystemName,BuildName)==-1) return;
        if(UserID(UserName)==-1) return;
        for(long i=0;i<BuildRights.Count();i++){
           if(
              BuildRights[i]->SystemName==SystemName &&
              BuildRights[i]->BuildName==BuildName &&
              BuildRights[i]->UserName==UserName
           ){
                BuildRights.Destroy(i);
                SaveSrv();
                return;
           }
        }
};

CORBA::Boolean srvObjDefStorage::GetBuildRights(
      CORBA::Long ConnectId,
      const char* SystemName,
      const char* BuildName,
      const char* UserName
      ){
        if(!AdminUser(ConnectId)) return false;
        if(BuildID(SystemName,BuildName)==-1) return false;
        if(UserID(UserName)==-1) return false;
        for(long i=0;i<BuildRights.Count();i++){
           if(
              BuildRights[i]->SystemName==SystemName &&
              BuildRights[i]->BuildName==BuildName &&
              BuildRights[i]->UserName==UserName
           ){
                return   BuildRights[i]->AllowAccess;
           }
        }
        return false;
};
/////////////////////////////////////////////////////////

Octets* srvObjDefStorage::GetSystemBuildList(
      CORBA::Long ConnectId,
      const char* SystemName
      ){

  if(!AdminUser(ConnectId)) return new Octets;
  MyArray<AnsiString> UsrBuilds;
  for(long i=0;i<Builds.Count();i++){
    if(Builds[i]->SystemName==SystemName){
       UsrBuilds.Add(new AnsiString(Builds[i]->BuildName));
    }
  }

  MyMemory m;
  m << UsrBuilds;
  return MakeOctets(m);
};

Octets* srvObjDefStorage::GetSystemObjectsList(
      CORBA::Long ConnectId,
      const char* SystemName
      ){
  if(!AdminUser(ConnectId)) return new Octets;
  MyArray<AnsiString> UsrBuilds;
  for(long i=0;i<Objects.Count();i++){
    if(Objects[i]->SystemName==SystemName){
       UsrBuilds.Add(new AnsiString(Objects[i]->ObjectName));
    }
  }

  MyMemory m;
  m << UsrBuilds;
  return MakeOctets(m);


      };



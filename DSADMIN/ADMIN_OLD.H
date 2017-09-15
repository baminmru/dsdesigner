#ifndef _ADMIN_H_
#define _ADMIN_H_
#include "bamilib.h"

struct User{
  AnsiString Name;
  AnsiString Password;
  bool IsAdmin;
};

struct Group{
  AnsiString Name;
  MyArray<AnsiString> Users;
};
struct CartrigeParam{
  AnsiString Name;
  bool AllowNull;
};

struct Cartridge{
  AnsiString Name;
  MyArray<CartrigeParam> ParamDef;
};

struct CartrigeParamValue{
  AnsiString Name;
  ANsiString Value;
};

struct Release{
  AnsiString Name;
  AnsiString SystemName;
  AnsiString CartrigeName;
  MyArray<CartrigeParamValue> SetupValues;
};

struct ObjectDef{
  long ClassID;
  AnsiString Name;
  AnsiString Definition;
};


struct Menu{
  AnsiString Caption;
  AnsiString Definition;
  MyArray<Menu> SubMenu;
};

struct Task{
  AnsiString Name;
  AnsiString Caption;
  MyArray<Menu> MainMenu;
};


struct System{
  MyArray<ObjectDef> Objects;
  MyArray<Task> Tasks;
};


struct TaskRight{
  AnsiString ReleaseName;
  AnsiString TaskName;
  bool AllowExec;
  AnsiString GroupName;
  AnsiString UserName;
  bool IsUser;
};

struct ObjectRight{
  AnsiString ReleaseName;
  AnsiString TaskName;
  bool AllowExec;
  bool AllowRead;
  bool AllowWrite;

  AnsiString GroupName;
  AnsiString UserName;
  bool IsUser;
};

struct Server{
  AnsiString Signature;
  AnsiString Version;
  MyArray<User> Users;
  MyArray<Group> Groups;
  MyArray<System> Systems;
  MyArray<Cartrige> Cartriges;
  MyArray<Release>  Releases;
  MyArray<TaskRight> TaskRights;
  MyArray<ObjectRight> ObjectRights;
};

#endif

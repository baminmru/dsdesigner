//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include <corba.h>
#include <conio.h>
#include "StoreObjects.h"
#include "CartrigeObjects.h"
#include "store_c.hh"

USEUNIT("..\Lib\StackValue.cpp");
USEUNIT("..\Lib\mystream.cpp");
USEUNIT("..\Lib\Convertions.cpp");
USEIDL("..\Lib\Store.idl");
USEUNIT("..\Lib\Store_c.cpp");
USEUNIT("..\Lib\Store_s.cpp");
USEUNIT("..\Lib\StoreObjects.cpp");
USEUNIT("..\Lib\CartrigeObjects.cpp");
//---------------------------------------------------------------------------
#pragma argsused
int main(int argc, char* argv[])
{
  try
  {
     // Initialize the ORB and BOA
     CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
     //CORBA::BOA_var boa = orb->BOA_init(argc, argv);

    {
     // Bind to an storage.
    ObjDefStorage_var s = ObjDefStorage::_bind("BAMI_APPSERVER");

    cout << "Bound" << endl;

    // connecting
    CORBA::Long id;

    id = s->Connect("bami","master","TEST");
    s->SetUser(
      id,
      "aaa",
      "aaa",
      0
      );
    s->SetUser(
      id,
      "bbb",
      "bbb",
      0
      );

    if(id){
     cout << "Good connect =" << id << endl;

     s->AddSystem(id,"¿»—¡œ98");
     s->AddSystem(id,"¿»—¡œ99");
     s->AddBuild(id,"V1.0","¿»—¡œ98");
     s->AddBuild(id,"V2.0","¿»—¡œ98");
     s->AddBuild(id,"V2.0","¿»—¡œ99");
     s->AddBuild(id,"V1.0","¿»—¡œ99");

     s->SetBuildRights(
     id,
     "¿»—¡œ99",
     "V2.0",
     "bami",
     true);

     s->SetBuildRights(
     id,
     "¿»—¡œ99",
     "V1.0",
     "bami",
     true);


     s->SetBuildRights(
     id,
     "¿»—¡œ98",
     "V1.0",
     "bami",
     true);

     Octets * oc = s->GetBuildList(id);
     MyMemory * m = MakeMemory(*oc);
     delete oc;
     MyArray<Build> bl;
     *m >> bl;
     delete m;

     for(long i=0;i<bl.Count();i++){
        cout << endl << "System=" << bl[i]->SystemName.c_str() << " BUILD=" << bl[i]->BuildName.c_str() << endl;
     }


     oc = new Octets();



     s->AddTarget(id,"ODBC");

     s->SetComponent(id,"TEST","MENU",*oc);
     s->SetComponent(id,"TEST","GRID",*oc);
     s->SetComponent(id,"TEST","TREE",*oc);
     s->SetComponent(id,"TEST","FILTER",*oc);
     s->SetComponent(id,"TEST","FORM",*oc);
     s->SetComponent(id,"TEST","REPORT",*oc);
     s->SetObject(
      id,
      "¿»—¡œ99",
      "MAIN",
      "MENU",
      *oc
      );

     s->SetObject(
      id,
      "¿»—¡œ99",
      "FORM1",
      "GRID",
      *oc
      );

     s->SetObject(
      id,
      "¿»—¡œ99",
      "FORM2",
      "GRID" ,
      *oc
      );


     s->SetObjectRights(
      id,
      "¿»—¡œ99",
      "FORM2",
      "bami",
      1,
      1,
      1
      );

     s->SetObjectRights(
      id,
      "¿»—¡œ99",
      "MAIN",
      "bami",
      1,
      1,
      1
      );

    s->SetObjectRights(
      id,
      "¿»—¡œ99",
      "FORM1",
      "bami",
      1,
      1,
      1
      );


     s->AddDataSource(
        id,
        "¿»—¡œ99",
        "source"
     );

     s->AddDataSource(
        id,
        "¿»—¡œ98",
        "source"
     );



     MyArray<CartrigeParam> ps;
     CartrigeParam *p;

     p = new CartrigeParam;
     p->Name="DSN";
     p->Value.Set(stString,"aisbp99");
     ps.Add(p);

     p = new CartrigeParam;
     p->Name="UID";
     p->Value.Set(stString,"sa");
     ps.Add(p);

     p = new CartrigeParam;
     p->Name="PWD";
     p->Value.Set(stString,"");
     ps.Add(p);

     p = new CartrigeParam;
     p->Name="NAME";
     p->Value.Set(stString,"aisbp99");
     ps.Add(p);

     p = new CartrigeParam;
     p->Name="TIMEOUT";
     p->Value.SetN(0);
     ps.Add(p);

     MyMemory pm;
     pm << ps;
     Octets * poc = MakeOctets(pm);

    s->SetTargetPackSetup(
        id,
        "V1.0",
        "¿»—¡œ98",
        "source",
        "ODBC",
        *poc
    );
    s->SetTargetPackSetup(
        id,
        "V1.0",
        "¿»—¡œ99",
        "source",
        "ODBC",
        *poc
    );
    s->SetTargetPackSetup(
        id,
        "V2.0",
        "¿»—¡œ98",
        "source",
        "ODBC",
        *poc
    );
    s->SetTargetPackSetup(
        id,
        "V2.0",
        "¿»—¡œ99",
        "source",
        "ODBC",
        *poc
    );
     delete poc;

     s->DisConnect(id);
     cout << "DisConnect" << endl;
    }else
      cout << "Bad connect" << endl;


    }
    getch();
  }
  catch(const CORBA::Exception& e)
  {
     cerr << e << endl;
     return(1);
  }
  return 0;
}
//---------------------------------------------------------------------------

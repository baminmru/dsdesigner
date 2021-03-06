//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include <corba.h>
#include "SRVImpl.h"

USEUNIT("SRVImpl.cpp");
USEUNIT("odbcEng.cpp");
USEUNIT("fieldCont.cpp");
USEUNIT("..\Lib\CartrigeObjects.cpp");
USEIDL("..\Lib\cartrige.idl");
USEUNIT("..\Lib\cartrige_c.cpp");
USEUNIT("..\Lib\cartrige_s.cpp");
USELIB("..\Lib\LIB3.lib");
//---------------------------------------------------------------------------
#pragma argsused
int main(int argc, char* argv[])
{
  try
  {
     // Initialize the ORB and BOA
     CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
     CORBA::BOA_var boa = orb->BOA_init(argc, argv);

     srvCartrige odbc("ODBC");
     boa->obj_is_ready(&odbc);

     // Wait for incoming requests
     boa->impl_is_ready();
  }
  catch(const CORBA::Exception& e)
  {
     cerr << e << endl;
     return(1);
  }
  return 0;
}
//---------------------------------------------------------------------------

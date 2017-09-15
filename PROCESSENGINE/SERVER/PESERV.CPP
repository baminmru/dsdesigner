//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include <corba.h>
USEIDL("..\Common\ProcessEngine.idl");
USELIB("..\Lib\LIB3.lib");
USEUNIT("..\Common\PEData.cpp");
USEUNIT("..\Common\ProcessEngine_c.cpp");
USEUNIT("..\Common\ProcessEngine_s.cpp");
USEUNIT("srvImpl.cpp");
//---------------------------------------------------------------------------
#pragma argsused
int main(int argc, char* argv[])
{
  try
  {
     // Initialize the ORB and BOA
     CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
     CORBA::BOA_var boa = orb->BOA_init(argc, argv);
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

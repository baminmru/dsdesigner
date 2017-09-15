//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include <corba.h>

#include "StoreImpl.h"

USEUNIT("StoreImpl.cpp");
USEIDL("..\Lib\Store.idl");
USEUNIT("..\Lib\Store_c.cpp");
USEUNIT("..\Lib\Store_s.cpp");
USEUNIT("..\Lib\StoreObjects.cpp");
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



                 srvObjDefStorage AppServer("BAMI_APPSERVER");
                 boa->obj_is_ready(&AppServer);

                 cout << "Storage object ready for requests!" << endl;
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

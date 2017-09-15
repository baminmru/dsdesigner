//---------------------------------------------------------------------------

#include <corbapch.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include <corba.h>
#include <condefs.h>
USEIDL("admin.idl");
USEUNIT("admin_c.cpp");
USEUNIT("admin_s.cpp");
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

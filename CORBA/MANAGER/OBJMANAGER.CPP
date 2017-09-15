//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <corba.h>
USERES("ObjManager.res");
USEFORM("Login.cpp", frmLogin);
USEUNIT("Manager.cpp");
USEIDL("..\Lib\Store.idl");
USEUNIT("..\Lib\Store_c.cpp");
USEUNIT("..\Lib\Store_s.cpp");
USEIDL("..\Lib\cartrige.idl");
USEUNIT("..\Lib\cartrige_c.cpp");
USEUNIT("..\Lib\cartrige_s.cpp");
USEFORM("fMDIMain.cpp", MDIMain);
USEFORM("fMDIChild.cpp", MDIChild);
USEFORM("fTaskChoose.cpp", frmTaskChoose);
USEUNIT("..\Lib\StoreObjects.cpp");
USEUNIT("..\Lib\CartrigeObjects.cpp");
USEFORM("fTree.cpp", frmTree);
USEFORM("AddObject.cpp", frmAddObject);
USEFORM("EditUser.cpp", frmEditUser);
USEFORM("EditBuild.cpp", frmEditBuild);
USELIB("..\Lib\LIB3.lib");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 // Initialize the ORB and BOA
                 CORBA::ORB_var orb = CORBA::ORB_init(__argc, __argv);
                 CORBA::BOA_var boa = orb->BOA_init(__argc, __argv);
                 Application->CreateForm(__classid(TMDIMain), &MDIMain);
     Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------

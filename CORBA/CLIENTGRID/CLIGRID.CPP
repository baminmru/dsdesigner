//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <corba.h>
USERES("CliGrid.res");
USEFORM("CliForm.cpp", Form1);
USEUNIT("..\Lib\StackValue.cpp");
USEUNIT("..\Lib\mystream.cpp");
USEUNIT("..\Lib\Convertions.cpp");
USEIDL("..\Lib\cartrige.idl");
USEUNIT("..\Lib\cartrige_c.cpp");
USEUNIT("..\Lib\cartrige_s.cpp");
USEUNIT("..\Lib\CartrigeObjects.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 // Initialize the ORB and BOA
                 CORBA::ORB_var orb = CORBA::ORB_init(__argc, __argv);
                 CORBA::BOA_var boa = orb->BOA_init(__argc, __argv);
                 Application->CreateForm(__classid(TForm1), &Form1);
     Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------

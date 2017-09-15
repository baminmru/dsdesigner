//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("Project1.res");
USEIDL("admin.idl");
USEUNIT("admin_c.cpp");
USEUNIT("admin_s.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("DSDesign.res");
USEFORM("DSDesigner.cpp", frmStreamDesigner);
USELIB("..\MYLIB\mylib.lib");
USELIB("..\MYLIB\LIB\Odbc32.lib");
USEFORM("Changes.cpp", frmChanges);
USEFORM("Navigator.cpp", LMENavigator);
USEUNIT("Task.cpp");
USEFORM("TaskEditor.cpp", frmTasks);
USEFORM("ChooseTask.cpp", frmChooseTask);
USEFORM("About.cpp", AboutBox);
USEFORM("Filter.cpp", frmFilter);
USELIB("..\MYLIB\LIB\Crpe32.lib");
USEFORM("..\DSView\Report.cpp", frmReport);
USEFORM("SlaveGrid.cpp", frmSlave);
USEUNIT("dbLoadSave.cpp");
USEFORM("DBSave.cpp", frmDBSave);
USEUNIT("HTML.cpp");
USEFORM("ObjectList.cpp", frmObjectList);
USEFORM("LoadObj.cpp", frmLoadObject);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->Title = "ЛМИ Навигатор";
   Application->HelpFile = "";
     Application->CreateForm(__classid(TLMENavigator), &LMENavigator);
     Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------

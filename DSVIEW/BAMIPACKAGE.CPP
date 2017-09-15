//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("BAMIPackage.res");
USEUNIT("StreamGrid.cpp");
USEFORM("EditForm.cpp", frmEditForm);
USEFORM("SG_Garbage.cpp", SG_Garbage);
USELIB("..\MYLIB\mylib.lib");
USEUNIT("StreamCombo.cpp");
USEFORM("StreamDialog.cpp", StreamDlg);
USEUNIT("CRW\CRPrintEngine.cpp");
USELIB("..\MYLIB\LIB\Crpe32.lib");
USEFORM("Index.cpp", frmIndex);
USEFORM("ToHTML.cpp", frmToHTML);
USEFORM("ToEXCEL.cpp", frmToEXCEL);
USEPACKAGE("vclx50.bpi");
USEPACKAGE("vcl50.bpi");
USELIB("..\MyLib\LIB\ODBC32.LIB");
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//   Package source.
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
	return 1;
}
//---------------------------------------------------------------------------

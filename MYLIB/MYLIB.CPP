//---------------------------------------------------------------------------
#include <condefs.h>
#pragma hdrstop
USEUNIT("SRC\odbcEng.cpp");
USEUNIT("SRC\fieldCont.cpp");
USEUNIT("SRC\MiniParser.cpp");
USEUNIT("SRC\mystream.cpp");
USEUNIT("SRC\mystring.cpp");
USEUNIT("SRC\Num2str.cpp");
USEUNIT("SRC\ODBCDataStream.cpp");
USEUNIT("SRC\Date.cpp");
USELIB("LIB\Odbc32.lib");
USEUNIT("SRC\DataStream.cpp");
USEUNIT("SRC\Styles.cpp");
USEUNIT("SRC\StreamEditorDef.cpp");
USEUNIT("SRC\DataSource.cpp");
USEFORM("SRC\ErrorFrom.cpp", frmError);
USEFORM("SRC\LogIn.cpp", frmLogin);
USEUNIT("SRC\Excels.cpp");
//---------------------------------------------------------------------------
#define Library

// To add a file to the library use the Project menu 'Add to Project'.


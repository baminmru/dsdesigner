//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Test.h"
#include "EditForm.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "StreamGrid"
#pragma link "RXGrids"
#pragma resource "*.dfm"
TForm1 *Form1;
ODBCDataSource * c=0;
MyArray<ColumnDef> ColDef;
StreamEditorDef *s;
DataManager Manager;


////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------



void __fastcall TForm1::FormCreate(TObject *Sender)
{
	if(!c){
		c=new ODBCDataSource("aisbp99");
        Manager.AddSource(c,"aisbp99");
        if(!c->LogOn("sa",""))
        	Application->Terminate();
    }

	Manager.AddEditor (s=new StreamEditorDef,"TEST");

	s->ColDef.Clear();
	ColumnDef *ptr;
	EditableFieldAttr *ea;
	/*
	TESTID int IDENTITY (1, 1) NOT NULL ,
	INT_NN,	INT_N, CHAR_NN,	CHAR_N,	VC_NN, VC_N, DT_NN,	DT_N, MN_NN, MN_N
	*/


	ptr = new ColumnDef;
	ptr->SetHeader( "TESTID", 0);
	ptr->SetField( StreamField , "TESTID", 64);
	ptr->SetPosition(0, 0);
	s->ColDef.Add(ptr);

	ptr = new ColumnDef;
	ptr->SetHeader( "INT_N", 0);
	ptr->SetField( StreamField , "INT_N", 30);
	ptr->SetPosition(1, 0);
	s->FormDef.Add( ea=new 	EditableFieldAttr("INT_N", "Целое пустое", 	 ectSubStream) );
	ea->StreamName="TEST";
	ea->SyncField="TESTID";
    ea->ShowField="VC_NN";

	s->ColDef.Add(ptr);

	ptr = new ColumnDef;
	ptr->SetHeader( "INT_NN", 0);
	ptr->SetField( StreamField , "INT_NN", 30);
	ptr->SetPosition(2, 0);

	s->FormDef.Add( ea=new 	EditableFieldAttr("INT_NN","Целое не пустое",	 ectYesNo) );
	ea->SetAccess(faWrite,faRead);
	s->ColDef.Add(ptr);

	ptr = new ColumnDef;
	ptr->SetHeader( "CHAR_N", 0);
	ptr->SetField( StreamField , "CHAR_N", 30);
	ptr->SetPosition(3, 0);
	s->FormDef.Add( new 	EditableFieldAttr("CHAR_N","Фикс. строка пустое",	 ectText) );
	s->ColDef.Add(ptr);

	ptr = new ColumnDef;
	ptr->SetHeader( "CHAR_NN", 0);
	ptr->SetField( StreamField , "CHAR_NN", 30);
	ptr->SetPosition(4, 0);
	s->FormDef.Add( ea=new 	EditableFieldAttr("CHAR_NN","Фикс. строка не пустое",	 ectText) );
	ea->SetAccess(faWrite,faNone);
	s->ColDef.Add(ptr);


	ptr = new ColumnDef;
	ptr->SetHeader( "VC_N", 0);
	ptr->SetField( StreamField , "VC_N", 30);
	ptr->SetPosition(5, 0);
	s->FormDef.Add( new 	EditableFieldAttr( "VC_N","Строка пустое",	 ectMultiLineText) );
	s->ColDef.Add(ptr);

	ptr = new ColumnDef;
	ptr->SetHeader( "VC_NN", 0);
	ptr->SetField( StreamField , "VC_NN", 30);
	ptr->SetPosition(6, 0);
	s->FormDef.Add( new 	EditableFieldAttr( "VC_NN","Строка не пустое",	 ectMultiLineText) );
	s->ColDef.Add(ptr);

	ptr = new ColumnDef;
	ptr->SetHeader( "DT_N", 0);
	ptr->SetField( StreamField , "DT_N", 30);
	ptr->SetPosition(7, 0);
	s->FormDef.Add( new 	EditableFieldAttr( "DT_N","Дата пустое",	 ectDate) );
	s->ColDef.Add(ptr);

	ptr = new ColumnDef;
	ptr->SetHeader( "DT_NN", 0);
	ptr->SetField( StreamField , "DT_NN", 30);
	ptr->SetPosition(8, 0);
	s->FormDef.Add( new 	EditableFieldAttr( "DT_NN","Дата не пустое",	 ectDate) );
	s->ColDef.Add(ptr);

	ptr = new ColumnDef;
	ptr->SetHeader( "MN_N", 0);
	ptr->SetField( StreamField , "MN_N", 30);
	ptr->SetPosition(9, 0);
	s->FormDef.Add( new 	EditableFieldAttr( "MN_N",	"Деньга пуcтое", ectNumeric) );
	s->ColDef.Add(ptr);

	ptr = new ColumnDef;
	ptr->SetHeader( "MN_NN", 0);
	ptr->SetField( StreamField , "MN_NN", 30);
	ptr->SetPosition(10, 0);
	s->FormDef.Add( new 	EditableFieldAttr( "MN_NN",	"Деньга не пуcтое", ectNumeric) );
	s->ColDef.Add(ptr);

	s->ConnectionName="aisbp99";
	s->SQL=	"select * from TEST ";

	s->ROWSQL="select * from TEST where TESTID=^TESTID^";

	s->INSERT="INSERT INTO TEST(	INT_NN,	INT_N, CHAR_NN,	CHAR_N,	VC_NN, VC_N, DT_NN,	DT_N, MN_NN, MN_N)"
						" VALUES(	^INT_NN^,	^INT_N^, ^CHAR_NN^,	^CHAR_N^,"
						"	^VC_NN^, ^VC_N^, ^DT_NN^,	^DT_N^, ^MN_NN^, ^MN_N^)";

	s->sDELETE="delete from TEST where TESTID=^TESTID^";

	s->UPDATE="UPDATE TEST SET 	INT_NN=^INT_NN^,	INT_N=^INT_N^, CHAR_NN=^CHAR_NN^,	CHAR_N=^CHAR_N^,"
						"	VC_NN=^VC_NN^, VC_N=^VC_N^, DT_NN=^DT_NN^,	DT_N=^DT_N^, MN_NN=^MN_NN^, MN_N=^MN_N^ where  TESTID=^TESTID^";




}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	StreamGrid1->SetEditor(&Manager,"TEST");
	StreamGrid1->RunEditor();

}
//---------------------------------------------------------------------------



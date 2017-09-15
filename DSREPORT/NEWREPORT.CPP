//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "NewReport.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TNewReport *)
{
		new TNewReport(NULL);
}
//---------------------------------------------------------------------------
__fastcall TNewReport::TNewReport(TComponent* Owner)
		: TCustomPanel(Owner)
{
 Caption=' ';
 Left=((TControl*)Owner)->Width/2-100;
 Top=((TControl*)Owner)->Height/2-100;
 Width=200;
 Height=200;
 BevelOuter=bvNone;
 BorderStyle=bsSingle;
 HBar=new TScrollBar(this);
 HBar->Parent=this;
 HBar->Kind=sbHorizontal;
 HBar->Align=alBottom;
 HBar->OnScroll=ScrollBarScroll;
 VBar=new TScrollBar(this);
 VBar->Parent=this;
 VBar->Kind=sbVertical;
 VBar->Align=alRight;
 VBar->OnScroll=ScrollBarScroll;
 PrintDialog=new TPrintDialog(this);
 PrinterSetup=new TPrinterSetupDialog(this);
 FConnected=false;
 Enviro=new odbcEnvironment();
 OnResize=MeResize;
}
//---------------------------------------------------------------------------
namespace Newreport
{
		void __fastcall PACKAGE Register()
		{
				TComponentClass classes[1] = {__classid(TNewReport)};
		    RegisterComponents("System", classes, 0);
		}
}
//---------------------------------------------------------------------------
int __fastcall TNewReport::Connect(AnsiString DSN, AnsiString UID, AnsiString PWD, AnsiString SQL_Select)
{
 DC=GetDeviceContext(Handle);
 if (!FConnected)
 {
	if(!Connection)
  {
   try
   {
		Connection=Enviro->OpenConnection(DSN,DSN,UID,PWD,30);
   }
   catch(TODBCException *e)
   {
    e->Show();
		return -1;
   }
	 catch(...)
	 {
		ShowMessage("Неизвестная ошибка при попытке соединения с базой данных!");
		return -2;
	 }
	 FConnected=true;
	 DS=new ODBCDataStream(Connection, SQL_Select);
	 DW=new DataWindow(DS);
	 ////////////////////


	OutRect * or;
	DW->InitExpression="msgbox{\"Hello!\",\"Startup Expression\"}";

	DW->TitleDef.Rects.Add(or=new OutRect(StaticField,
	"Поле секции Title главного документа.  Многострочная с изменяемой высотой. Статика."));
	or->LT.Offset.cx=5;
	or->LT.Offset.cy=5;
	or->W.Set(10,700);
	or->H.Set(80);
	or->H.NoSizing = true;
	or->W.NoSizing = false;
	//or->W.NoSizing = false;
	//or->Visible = true;
	or->Style=&DefOutRectTitle;

//Detail
//////////////////////////////////////////////////////////////////////////
	DW->DetailDef.Rects.Add(or=new OutRect(StreamField,
	"Name"));
	or->LT.Offset.cx=5;
	or->LT.Offset.cy=0;
	or->W.Set(10,800);
	or->H.Set(30);
	or->W.NoSizing = false;


	DW->DetailDef.Rects.Add(or=new OutRect(StreamField,
	"Date1"));
	or->LT.Offset.cx=0;
	or->LT.Offset.cy=5;
	or->LT.FromObject=1;
	or->LT.FromPoint=LeftBottom;
	or->W.Set(10,800);
	or->H.Set(30);
	or->W.NoSizing = false;
	or->H.NoSizing = true;


	DW->DetailDef.GrowVertical = true;


	// Summary
	DW->SummaryDef.Rects.Add(or=new OutRect(StaticField,
	"Поле секции Summary. Статика."));
	or->LT.Offset.cx=50;
	or->LT.Offset.cy=50;
	or->W.Set(10,100);
	or->H.Set(40);
	or->Visible = true;
	or->Style=&DefOutRectSummary;
	or->H.NoSizing = true;
	or->W.NoSizing = false;

	 //////////////////////
	 DW->Resolve(0, DC, 0, 0);
	 TNRect *CR;
	 CR=new TNRect(ClientRect);
	 CR->Right-=VBar->Width;
	 CR->Bottom-=HBar->Height;

	 HBar->Min=0;
	 long mm;
	 mm=(DW->GetRight(0)-CR->Right/2)/10;
	 if(mm<0) mm=0;
	 HBar->Max=mm;
	 VBar->Min=0;
	 mm=(DW->GetBottom(0)-CR->Bottom/2)/10;
	 if(mm<0) mm=0;
	 VBar->Max=mm;
	 int sss=0;
	 //ScrollBarScroll(this,scPosition,sss);
	 Paint();
	 return 1;
	}
 }
 return 2;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TNewReport::ScrollBarScroll(TObject *Sender, TScrollCode ScrollCode, int &ScrollPos)
{
 /*if (DW)
 {
	TNRect *CR;
	CR=new TNRect(ClientRect);
	CR->Right-=VBar->Width;
	CR->Bottom-=HBar->Height;
	CR->MoveTo(10*HBar->Position+CR->Left,10*VBar->Position+CR->Top);
	DW->Paint(0,DC,CR,CR);
 }
 */
 Paint();
}

//---------------------------------------------------------------------------
void __fastcall TNewReport::MeResize(TObject *Sender)
{
 if (DW)
 {
	VBar->Enabled=true;
	HBar->Enabled=true;
	TNRect *CR;
	CR=new TNRect(ClientRect);
	CR->Right-=VBar->Width;
	CR->Bottom-=HBar->Height;

	HBar->Min=0;
	long mm;
	mm=(DW->GetRight(0)-CR->Right/2)/10;
	if(mm<0) mm=0;
	HBar->Max=mm;
	VBar->Min=0;
	mm=(DW->GetBottom(0)-CR->Bottom/2)/10;
	if(mm<0) mm=0;
	VBar->Max=mm;

	int sss=0;
 }else{
	VBar->Enabled=false;
	HBar->Enabled=false;
 }
}

//---------------------------------------------------------------------------
 void __fastcall TNewReport::Paint(void){
 if (DW)
 {
	TNRect *CR;
	CR=new TNRect(ClientRect);
	CR->Right-=VBar->Width;
	CR->Bottom-=HBar->Height;
	CR->MoveTo(10*HBar->Position+CR->Left,10*VBar->Position+CR->Top);
	DW->Paint(0,DC,CR,CR);
 }
 };
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


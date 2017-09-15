#include <SysUtils.hpp>
#include <DdeMan.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Windows.hpp>
#include <SysInit.hpp>
#include <System.hpp>

enum TCellDir { dirNone, dirUp, dirDown, dirLeft, dirRight };

enum TNewSheet {
FromTemplate,
FromActiveSheet,
Sheet,
ChartSelection,
Macro,
IntMacro,
Reserved,
VBModule,
Dialog
};

typedef int TExcelRow;

typedef int TExcelCol;

struct TExcelCell
{
	int Col;
	int Row;
} ;

struct TExcelSelectionArray
{
	TExcelCell TopLeft;
	TExcelCell BottomRight;
} ;

typedef TExcelSelectionArray *PExcelSelectionArray;

typedef System::AnsiString TFileExt;

class TCustomExcel : public Classes::TComponent
{
public:
	typedef Classes::TComponent inherited;
	Ddeman::TDdeClientConv* FDDEClientConv;
	Ddeman::TDdeClientItem* FDDEClientItem;
	System::AnsiString FileExt;
	System::AnsiString Executable;
	System::AnsiString ExeName;
	bool FConnected;
	bool FLoadExcel;
	void __fastcall SetExeName(const System::AnsiString Value);
	void __fastcall SetConnect(bool Value);
	bool __fastcall GetReady(void);
  AnsiString __fastcall GetSelection();
	System::AnsiString __fastcall GetCurrentSheet();
	System::AnsiString __fastcall StripQuotation(const System::AnsiString AString);

	__fastcall virtual TCustomExcel(Classes::TComponent* AOwner);
	__fastcall virtual ~TCustomExcel(void);

	void __fastcall SwitchTopic(const System::AnsiString NewTopic);
	void __fastcall SwitchToSystemTopic(void);

	int __fastcall Connect(void);
	void __fastcall Disconnect(void);
	System::AnsiString __fastcall Request(const System::AnsiString Item);
	void __fastcall Exec(const System::AnsiString Cmd);
	void __fastcall CloseExcel(void);
	//void __fastcall WaitUntilReady(void);
 void __fastcall LocateExcel();
 TStringList* __fastcall GetStrings(TStringList * List, const AnsiString Topic );
};



#define msgExcelNoReply "*** Excel - No Reply ***"
#define msgStrError "Ошибка передачи строки"
#define msgCouldNotLaunch "Не найден XL !"
#define msgCmdAcceptErr "\"%s\" команда не поддерживается программой Excel"
#define msgNoRowCol "Could not identify letters for rol / column"
#define msgBadCellFmt "Unexpected Excel cell format"
#define msgNoMacroFile "No open macro file"
#define msgTableNotReady "Table not ready"
#define msgReservedType "Use of reserved type"
#define msgArrayMove "Putting an array needs auto-moving cell!"
#define msgNotSupported "Data type not supported"


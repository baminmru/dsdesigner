#include "bamilib.h"

#define dec(x) (x--)
#define inc(x) (x++)

static long cntXL=0;
static int RScan(const char * S,char Chr)
{
  int Result = strlen(S);
  while( Result > 0 )
  {
    if( S[Result] == Chr )
      break;

    Result--;
  };
  return Result;
};


static int FindExcelColon(int x, const char *  s)
{
   int counter;
  counter = x;
  while( counter <= strlen(s) )
  {
    if( s[counter] == 0x39 )
    {
      inc(counter);
      while( (counter < strlen(s)) && (s[counter]!=0x39) )
        inc(counter);
      inc(counter);
    }
    else
    {
      if( s[counter] == ';' )
        break;
      else
        inc(counter);
    };
  };

  if( (s[counter] == ';') && (counter < strlen(s)) )
    return counter;
  else
    return 0;
};


	__fastcall TCustomExcel::TCustomExcel(TComponent *AOwner ):TComponent(AOwner)
{

  FileExt = ".xls";
  Executable = "EXCEL.EXE";

  FDDEClientConv = new TDdeClientConv(this);
  FDDEClientConv->ConnectMode = ddeManual;

  FDDEClientItem = new TDdeClientItem(this);
  FDDEClientItem->DdeConv = FDDEClientConv;

  SetExeName("Excel");
};




__fastcall TCustomExcel::~TCustomExcel()
{

  Disconnect();

  FDDEClientItem->DdeConv = 0;

  delete FDDEClientItem;
  FDDEClientItem = 0;

  delete FDDEClientConv;
  FDDEClientItem = 0;

};

void __fastcall  TCustomExcel::SetExeName(const TFileName Value )
{
  Disconnect();
  ExeName = ChangeFileExt(Value, "");
  FDDEClientConv->ServiceApplication = ExeName;
};


bool __fastcall  TCustomExcel::GetReady()
{
  Application->ProcessMessages();
  SwitchToSystemTopic();
  return Request("Status")=="Ready";
};

/*
TCustomExcel::GetFormats: TStringList;
{
  Result = GetStrings(FFormatsList, "Formats");
};

 TCustomExcel::GetTopics: TStringList;
{
  Result = GetStrings(FTopicsList, "Topics");
};

 TCustomExcel::GetProtocols: TStringList;
{
  Result = GetStrings(FProtocolsList, "Protocols");
};

 TCustomExcel::GetEditEnvItems: TStringList;
{
  Result = GetStrings(FEditEnvItemsList, "EditEnvItems");
};
*/

AnsiString __fastcall TCustomExcel::GetSelection()
{
  Application->ProcessMessages();
  SwitchToSystemTopic();
  return Request("Selection");
};


AnsiString __fastcall  TCustomExcel::GetCurrentSheet()
{
  int ExclPos;
  AnsiString Result = GetSelection();
  if( Result == msgExcelNoReply )
    return "";
  else
  {
    ExclPos = RScan(Result.c_str(), '!');
    if( ExclPos > 0 )
      Result.Delete(ExclPos, Result.Length());
  };
  return Result;

};



void __fastcall TABStringToStringList(char *TABString,  TStringList *List)
{
  char *StartPos, *TabPos ;
  if( TABString == 0 ) return;
  StartPos = TABString;
  TabPos = StrScan(StartPos, 0x9);
  while( TabPos != 0 )
  {
    TabPos[0] = 0x0; //{ replace #9 by #0 }
    List->Add(StrPas(StartPos)); //{ add format to list }
    StartPos = TabPos+1; //{ position to after TAB}
    TabPos = StrScan(StartPos, 0x9);
  };
  if( StrLen(StartPos) > 0 )
    List->Add(StartPos);
};


TStringList* __fastcall TCustomExcel::GetStrings(TStringList * List, const AnsiString Topic )
{
  char * Reply;
  Application->ProcessMessages();

  List->Clear();
  /*
  FDDEClientItem->Lines->Clear();
  FDDEClientItem->DdeItem=Topic;
  FDDEClientConv->RequestData(FDDEClientItem->DdeItem);

  while(FDDEClientConv->WaitStat)
   Application->ProcessMessages();
  for(long i=0;i<FDDEClientItem->Lines->Count;i++)
   List->Add(FDDEClientItem->Lines->Strings[i]);


  */


  SwitchToSystemTopic();
  try{


     Reply = FDDEClientConv->RequestData(Topic);
  }catch(...){
     Reply="";
  };
  try{
    TABStringToStringList(Reply, List);
  }catch(...){
    StrDispose(Reply);
  };

  StrDispose(Reply);

 return List;
};

/*
AnsiString __fastcall  TCustomExcel::StripQuotation(const AString: string): string;
{
  if( (Length(AString) > 0) &&
     (AString[Length(AString)]= #39) && (AString[1] = #39 ) ) { test for " }

    Result = Copy(AString, 2, Length(AString)-2)
  else
    Result = AString;
};
*/

int __fastcall TCustomExcel::Connect()
{

  if( ! FConnected )
  {
    FDDEClientConv->SetLink("Excel", "System");
    FDDEClientConv->ServiceApplication="";

    if( FDDEClientConv->OpenLink() )
    {
      FConnected = true;
      FLoadExcel = false;
      //MessageBox(0,"Excel loaded","",MB_OK);
      return 1;
    };

    LocateExcel();

    if( ! FDDEClientConv->OpenLink() )
      throw( Exception(msgCouldNotLaunch));
    FLoadExcel = true;
    FConnected = true;
  }
  //MessageBox(0,"Excel started","",MB_OK);
  return 0;

};


void __fastcall TCustomExcel::Disconnect()
{
  if( FConnected ){
    FDDEClientConv->CloseLink();
    FConnected=false;
  }
};


AnsiString __fastcall TCustomExcel::Request(const AnsiString Item)
{
  char *Reply=0;
  try{
    Reply = FDDEClientConv->RequestData(Item);
  }catch(...){
    Reply = 0;
  };

  if( Reply == 0 )
    return msgExcelNoReply;
  else{
    AnsiString Result = StrPas(Reply);
    StrDispose(Reply);
    return Result;
  }
};

void __fastcall TCustomExcel::Exec(const AnsiString Cmd)
{
  if(!FConnected) return;
  SwitchToSystemTopic();

  try{
    FDDEClientConv->ExecuteMacro(Cmd.c_str(), false);
  }catch(...){

  };
  try{
    do{
      Application->ProcessMessages();
      if(++cntXL==32){
        Sleep(100);
        cntXL=0;
      }
    }while(FDDEClientConv->WaitStat);
  }catch(...){};

};



void __fastcall TCustomExcel::LocateExcel()
{

int  BufferSize =  1024 ;

char *  Buffer,*StringPosition;
long  ReturnedData;

  Buffer = StrAlloc(BufferSize);
  try{
    //{ get the first entry, don"t bother about the version !}
    ReturnedData = BufferSize;
    StrPCopy(Buffer, FileExt);
    RegQueryValue(HKEY_CLASSES_ROOT, Buffer, Buffer, &ReturnedData);
    if( StrLen(Buffer) > 0 )
    {
      StrCat(Buffer, "\\shell\\Open\\command");
      ReturnedData = BufferSize;
      if( RegQueryValue(HKEY_CLASSES_ROOT, Buffer, Buffer, &ReturnedData) == ERROR_SUCCESS )
      {
        StringPosition = Sysutils::StrUpper(Buffer);

        //{ find _last_ occurence of the executable name }
          while( StrPos(StringPosition+1, Executable.c_str()) != 0 )
            StringPosition = StrPos(StringPosition+1, Executable.c_str());
        StrCopy(StringPosition + Executable.Length(), ""); //{ cut off string }

        if( Buffer[0] == '"' )
          ExeName = StrPas(Buffer+1);
        else
          ExeName = StrPas(Buffer);

/*        { if( it is in registry, it"s quite likely that the file exists, too

          Note: writing to ExeName has the side-effect that "->EXE" is cut off;
                this is done in compliance with the Win API docs
                Because of this we need to append it here again to see whether
                the file actually exists! } */
        if( ! FileExists(ExeName) )
          ExeName = "";
        else
         SetExeName(ExeName);

      };
    };
  }catch(...){
    StrDispose(Buffer);
    return;
  };
  StrDispose(Buffer);
};



void __fastcall TCustomExcel::CloseExcel()
{
  if( FConnected )
  {
    Exec("[QUIT]");
    FLoadExcel = false;
    Disconnect();
  }
};


/*
{ TExcel }

constructor TExcel->Create(AOwner: TComponent);
{
  inherited Create(AOwner);
  FMoveActiveCell = dirDown;
  FDecimalSeparator = ".";
};

void TExcel->SetFilter(DoFilter: boolean);
{
  FFilterReturnedData = DoFilter;

  if( ! (csDesigning in ComponentState) )
    FDDEClientConv->FormatChars = DoFilter;
};

void TExcel->AutoMoveActiveCell;
{
  case FMoveActiveCell of
    dirNone:  { do nothing };
    dirUp:    {
                Exec("[SELECT("R[-1]C")]");
              };

    dirDown:  {
                Exec("[SELECT("R[1]C")]");
              };

    dirLeft:  {
                Exec("[SELECT("RC[-1]")]");
              };

    dirRight: {
                Exec("[SELECT("RC[1]")]");
              };
  };
};

void TExcel->Select(Row: TExcelRow; Col: TExcelCol);
{
  if( (Row != 0) && (Col != 0) )
  {
    Exec(Format("[SELECT("R%dC%d")]", [Row, Col]));
  };
};

void TExcel->SelectSheet(const ASheet: string);
{
  SwitchTopic(StripQuotation(ASheet));
};

void TExcel->Move(deltaRow, deltaCol: Integer);
{
  Exec(Format("[SELECT("R[%d]C[%d]")]", [deltaRow, deltaCol]));
};

void TExcel->Insert(const s: string);
{
  Exec(Format("[FORMULA(%s)]", [s]));
  AutoMoveActiveCell;
};

void TExcel->InsertAt(Row: TExcelRow; Col: TExcelCol; const s: string);
{
  if( (Row <= 0)  || (Col<=0) )
    Insert(s)
  else
  {
    Exec(Format("[FORMULA(%s, "R%dC%d")]", [s, Row, Col]));
  };
};

void TExcel->PutExt(e: Extended);
{
  PutExtAt(0, 0, e);
};

void TExcel->PutExtAt(Row: TExcelRow; Col: TExcelCol; e: Extended);
var
  SepPos : Integer;
  ExtString: string[30];
{
  Str(e:0:FDecimals, ExtString);
  { this will always return an "American" style number }
  if( FDecimalSeparator != "->" )
  {
    SepPos = Pos("->", ExtString);
    if( SepPos > 0 )
      ExtString[SepPos] = FDecimalSeparator;
  };
  InsertAt(Row, Col, ExtString);
};

void TExcel->PutStr(const s: string);
{
  PutStrAt(0, 0, s);
};

void TExcel->PutStrAt(Row: TExcelRow; Col: TExcelCol; const s: string);
{
  InsertAt(Row, Col, Format(""%s"", [s]));
};


void TCustomExcel::GetCellChars;
var
  SelString: string;
  CharPos: integer;
{
  SelString = GetSelection; { get the whole lot }

  if( SelString = msgExcelNoReply ) { only parse if( Excel replied }
    exit;

  CharPos = RScan(SelString, "!"); { find the separator }
  if( CharPos > 0 )
  {
    { remove from the existing string }
    Delete(SelString, 1, CharPos);

    FRowChar = SelString[1]; { The first char is always the row char }

    CharPos = 2;                { Find occurence of col char }
    while( (CharPos < length(SelString)) &&
          (SelString[CharPos] in ["0"->->"9"]) do Inc(CharPos);

    FColChar = SelString[CharPos];
  };
};

 TCustomExcel::CellCharsOK: boolean;
{
  if( (FRowChar = #0)  || (FColChar = #0) )
    Result = false
  else
    Result = true;
};

 TCustomExcel::ParseSelEntry(var TopLeft, BottomRight: TExcelCell; const Sel: string): string;

  { parse cell part into number && remove it }
   GetNumber(var AString: string): integer;
  var
    CharPos : integer;
  {
    CharPos = 2; { this is specialized code that knows that the first char is non-numeric }
    while( (CharPos <= length(AString)) && (AString[CharPos] in ["0"->->"9"]) do
      inc(CharPos);

    { convert to number }
    Result = StrToInt(Copy(AString, 2, CharPos-2));

    { remove parsed part from string }
    Delete(AString, 1, pred(CharPos));
  };

var
  SeparatorPos: integer;
  CellAddress: string;
{
  TopLeft->Col = 0;  BottomRight->Col = 0;
  TopLeft->Row = 0;  BottomRight->Row = 0;

  Result = Sel;
  SeparatorPos = RScan(Result, "!"); { find the separator }

  if( SeparatorPos = 0 )
  {
    Result = ""; { should ! happen! - but don"t raise an exception }
    exit;
  };

  { copy the cell part into CellAddress}
  CellAddress = Copy(Result, succ(SeparatorPos), Length(Result));

  { && remove it from the existing string - return value }
  Delete(Result, SeparatorPos, Length(Result));
  Result = StripQuotation(Result);

  if( ! CellCharsOK )
  {
    GetCellChars;
    if( ! CellCharsOK )
      raise ExcelError->Create(msgNoRowCol);
  };
  { at this point we know the following:
      FRowChar && FColChar contain valid identifiers for an Excel row / column }


  { find the colon char (R1C10:R20C15) -> array selection;  || the semicolon
     -> multiple selection (R1C10;R20C40) ==> ";" + ":"

      R1C10;R20C20   [multiple selection]

      R1C10:R20C20   [array]

      R1C10

      R1             [single row]
      C1             [single column]

  }

  { find multiple selections which we are ! parsing !}
  { it is safe to use "Pos" here, as a potential semicolon in the name part will
    have been removed by now }
  SeparatorPos = Pos(";", CellAddress);
  if( SeparatorPos > 0 )
    { remove the parts we are ! going to parse }
    Delete(CellAddress, SeparatorPos, Length(CellAddress));


  { there must be always at least ONE valid char }
  if( CellAddress[1] = FRowChar )
    TopLeft->Row = GetNumber(CellAddress)
  else
  if( CellAddress[1] = FColChar )
    TopLeft->Col = GetNumber(CellAddress);

  if( (length(CellAddress)>0) ) { still something left ?}
    { assume that Excel does ! return garbage }
    if( CellAddress[1] = FRowChar )
      TopLeft->Row = GetNumber(CellAddress)
    else
    if( CellAddress[1] = FColChar )
      TopLeft->Col = GetNumber(CellAddress)
    else
      raise ExcelError->Create(msgBadCellFmt);


  { at this point we have parsed the left part of an array selection, so
    that at most something like ":R1->->->" is left over }

  {test whether we do have an array indeed (otherwise we have a problem !) }
  if( (length(CellAddress)>0) )
    if( (CellAddress[1] != ":") )
      raise ExcelError->Create(msgBadCellFmt)
    else
    {
      if( length(CellAddress) < 2 )
        raise ExcelError->Create(msgBadCellFmt);

      Delete(CellAddress, 1, 1); { remove colon }

      { what follows is effectively the same code as above, only
        with "BottomRight" instead of TopLeft }

      { there must be always at least ONE valid char }
      if( CellAddress[1] = FRowChar )
        BottomRight->Row = GetNumber(CellAddress)
      else
      if( CellAddress[1] = FColChar )
        BottomRight->Col = GetNumber(CellAddress);

      if( (length(CellAddress)>0) ) { still something left ?}
        { assume that Excel does ! return garbage }
        if( CellAddress[1] = FRowChar )
          BottomRight->Row = GetNumber(CellAddress)
        else
        if( CellAddress[1] = FColChar )
          BottomRight->Col = GetNumber(CellAddress)
        else
          raise ExcelError->Create(msgBadCellFmt);
    };


  { we have transferred all the text into the TopLeft + BottomRight record;
    now handle the special cases }

  if( (BottomRight->Col = 0) && (BottomRight->Row = 0) )
    BottomRight = TopLeft;
};

*/
void __fastcall TCustomExcel::SwitchTopic(const AnsiString NewTopic)
{

  {
    if( FDDEClientConv->DdeTopic != NewTopic )
    {
      if(FConnected )
        FDDEClientConv->CloseLink();

      FDDEClientConv->SetLink("Excel", NewTopic);

      if(FConnected )
        FDDEClientConv->OpenLink();
    };
  }
};

void __fastcall TCustomExcel::SwitchToSystemTopic()
{
  SwitchTopic("System");
};
/*
 TCustomExcel::GetRectSelection(var TopLeft, BottomRight: TExcelCell): string;
var
  DelimitPos: integer;
  CurrentSel : string;
{
  CurrentSel = this->Selection;
  { Parse ONLY the first item of a selection }
  DelimitPos = FindExcelColon(1, CurrentSel);
  if( DelimitPos > 0 )
    Delete(CurrentSel, DelimitPos, length(CurrentSel));

  Result = ParseSelEntry(TopLeft, BottomRight, CurrentSel);
};

void TCustomExcel::RetrieveSelection;
var
  DelimitPos: integer;
  CurrentSel: string;

  counter : integer;

  PAnExcelSelArray: PExcelSelectionArray;
{
  for counter = 0 to FSelectionList->Count-1 do
    with FSelectionList do
      if( Objects[counter]!=0 )
        dispose(PExcelSelectionArray(Objects[counter]));
  FSelectionList->Clear;

  CurrentSel = this->Selection;

  DelimitPos = 1;
  repeat
    DelimitPos = FindExcelColon(DelimitPos, CurrentSel);

    new(PAnExcelSelArray);
    try{
      if( DelimitPos > 0 )
        counter = FSelectionList->Add(
            ParseSelEntry( PAnExcelSelArray^->TopLeft, PAnExcelSelArray^->BottomRight,
                           Copy(CurrentSel, 1, DelimitPos-1) ) )
      else
        counter = FSelectionList->Add(
            ParseSelEntry( PAnExcelSelArray^->TopLeft, PAnExcelSelArray^->BottomRight,
                           CurrentSel ) );
    }catch(...){
      on E: Exception do
      {
        dispose(PAnExcelSelArray);
        raise;
      };
    };

    FSelectionList->Objects[counter] = TObject(PAnExcelSelArray);

    if( DelimitPos > 0 )
      Delete(CurrentSel, 1, DelimitPos);
  until DelimitPos = 0;
};

/*
 TExcel->GetData: string;
{
  Result = GetDataAt(0, 0);
};


 TExcel->GetDataAtFileSheet(Row: TExcelRow; Col: TExcelCol; const FileSheet: string): string;
var
  TopCell,
  BottomCell: TExcelCell;

  CRLFPos : integer;
{
  if( (Row <= 0)  || (Col <= 0) ) { if( "invalid" input get current selection }
  {
    GetRectSelection(TopCell, BottomCell);
    Row = TopCell->Row;
    Col = TopCell->Col;
  };

  SwitchTopic(StripQuotation(FileSheet));

  if( ! CellCharsOK )
  {
    GetCellChars;
    if( ! CellCharsOK )
      raise ExcelError->Create(msgNoRowCol);
  };

  Result = Request(Format("%s%d%s%d", [FRowChar, Row, FColChar, Col]));

  if( FStripCRLF && (Length(Result) > 1) )
  {
    CRLFPos = length(Result)-1;
    if( (Result[CRLFPos] = #13) && (Result[succ(CRLFPos)] = #10) )
      Delete(Result, CRLFPos, 2);
  };

};

 TExcel->GetDataAt(Row: TExcelRow; Col: TExcelCol): string;
{
  Result = GetDataAtFileSheet(Row, Col, CurrentSheet);
};

*/

/*
{ TAdvExcel }

void TAdvExcel->OpenMacroFile(const MacroFilename: TFileName);
var
  MFile: TFilename;
{
  MFile = UpperCase(ExtractFileName(MacroFilename));
  if( FMacro != MFile )
  {
    if( FMacro != "" )
      CloseMacroFile;

    Exec(Format("[OPEN("%s")]", [MacroFilename]));
    Exec("[HIDE()]");
    FMacro = MFile;
  };
};

void TAdvExcel->CloseMacroFile;
{
  if( FMacro != "" )
  {
    Exec(Format("[UNHIDE("%s")]", [FMacro]));
    Exec("[CLOSE(false)]");
    FMacro = "";
  };
};

void TAdvExcel->RunMacro(const MacroName: string);
{
  if( FMacro = "" )
    raise ExcelError->Create(msgNoMacroFile);

  Exec(Format("[RUN("%s!%s";false)]", [FMacro, MacroName]));
};

void TAdvExcel->StartTable;
{
  Exec("[APP->MINIMIZE()]");
  Exec("[NEW(1)]");
  PutStrAt(1, 1, msgTableNotReady);
};

void TAdvExcel->EndTable;
{
  PutStrAt(1, 1, "");
  Exec("[APP->RESTORE()]");
};

void TAdvExcel->NewSheet(SheetType: TNewSheet; const TemplateName: string);
{
  if( SheetType = Reserved )
    raise ExcelError->Create(msgReservedType);

  if( SheetType = FromTemplate )
    Exec(Format("[WORKBOOK->INSERT("%s")]", [TemplateName]))
  else
    Exec(Format("[WORKBOOK->INSERT(%d)]", [ord(SheetType)-1]));
};

void TAdvExcel->NewWorkbook(SheetType: TNewSheet; const TemplateName: string);
{
  if( SheetType = FromTemplate )
    Exec(Format("[NEW("%s")]", [TemplateName]))
  else
    Exec(Format("[NEW(%d)]", [ord(SheetType)-1]));
};

(*
void TAdvExcel->OpenWorkbook( const Filename: string; UpdateLinks: TExcelUpdateLinks);
{
  OpenWorkbookEx( Filename, UpdateLinks, false, excelDefaultDelimit,
                  "", "", true, excelDefaultOrigin, #0, false,
                  false, excelRevertToSaved, true, 0);
};

void TAdvExcel->OpenWorkbookEx( const Filename: string; UpdateLinks: TExcelUpdateLinks;
                          ReadOnly: boolean;
                          DelimitFormat: TExcelFileDelimiter;
                          const Password: string;
                          const EditPassword: string;
                          IgnoreReadOnlyRecommendation: boolean;
                          FileOrigin: TExcelFileOrigin;
                          CustomDelimiter: char;
                          AddToCurrentWorkbook: boolean;
                          Editable: boolean;
                          FileAccess: TExcelFileAccess;
                          NotifyUser: boolean;
                          Converter: integer);
const
  BoolName : array[boolean] of PChar = ("true", "false");
{
  Exec(Format("[OPEN("%s",%d,)]", [
                              Filename,
                              ord(UpdateLinks),
                              ReadOnly,
                              ord(DelimitFormat),
                              Password,
                              IgnoreReadOnlyRecommendation,
                              ord(FileOrigin),
                              CustomDelimiter,
                              AddToCurrentWorkbook,
                              Editable,
                              ord(FileAccess),
                              NotifyUser,
                              Converter
                         ]) );
};
*)

void TAdvExcel->EchoOn;
{
  Exec("[ECHO(true)]");
};

void TAdvExcel->EchoOff;
{
  Exec("[ECHO(false)]");
};

void TAdvExcel->DisableInput;
{
  Exec("[DISABLE->INPUT(true)]");
};

void TAdvExcel->EnableInput;
{
  Exec("[DISABLE->INPUT(false)]");
};

void TAdvExcel->HideCurrentWindow;
{
  Exec("[HIDE()]");
};

void TAdvExcel->UnhideWindow(const AWindowName: string);
{
  Exec( Format("[UNHIDE(%s)]", [AWindowName]) );
};

void TAdvExcel->PutInt(i: Longint);
{
  PutIntAt(0, 0, i);
};

void TAdvExcel->PutIntAt(Row: TExcelRow; Col: TExcelCol; i: Longint);
{
  InsertAt(Row, Col, IntToStr(i));
};

void TAdvExcel->PutDate(d: TDateTime);
{
  PutDateAt(0, 0, d);
};

void TAdvExcel->PutDateAt(Row: TExcelRow; Col: TExcelCol; d: TDateTime);
{
  PutStrAt(Row, Col, DateToStr(d));
};

void TAdvExcel->PutTime(d: TDateTime);
{
  PutTimeAt(0, 0, d);
};

void TAdvExcel->PutTimeAt(Row: TExcelRow; Col: TExcelCol; d: TDateTime);
{
  PutStrAt(Row, Col, TimeToStr(d));
};

void TAdvExcel->PutDateTime(d: TDateTime);
{
  PutDateTimeAt(0, 0, d);
};

void TAdvExcel->PutDateTimeAt(Row: TExcelRow; Col: TExcelCol; d: TDateTime);
{
  PutStrAt(Row, Col, DateTimeToStr(d));
};

void TAdvExcel->PutData(const AnArray: array of const);
{
  PutDataAt(0, 0, AnArray, MoveActiveCell);
};

void TAdvExcel->PutDataAt( Row: TExcelRow; Col: TExcelCol;
                               const AnArray: array of const;
                               FillDirection: TCellDir);
var
  i: Integer;
  AutoMove: TCellDir;
{
  Select(Row, Col);

  if( (FillDirection = dirNone) && (High(AnArray)>0) )
    raise ExcelError->Create(msgArrayMove);

  AutoMove = this->MoveActiveCell;
  this->MoveActiveCell = FillDirection;
  try{
    for i= Low(AnArray) to High(AnArray) do
    with AnArray[i] do
      case VType of
        vtExtended: PutExtAt(0, 0, VExtended^);
        vtInteger:  PutIntAt(0, 0, VInteger);
        vtChar:     PutStrAt(0, 0, VChar);
        vtString:   PutStrAt(0, 0, VString^);
        vtPChar:    PutStrAt(0, 0, StrPas(VPChar));
      {$IFDEF Win32}
        vtAnsiString: PutStrAt(0, 0, StrPas(VPChar));
      {$ENDIF}
      else
        raise ExcelError->Create(msgNotSupported);
      }; { case }
  }catch(...){
    this->MoveActiveCell = AutoMove;
  };
};
*/



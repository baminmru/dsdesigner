//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CRPrintEngine.h"
#pragma package(smart_init)

int TCRPrintEngine::CallCount=0;

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TCRPrintEngine *)
{
    new TCRPrintEngine(NULL);
}
//---------------------------------------------------------------------------
__fastcall TCRPrintEngine::TCRPrintEngine(TComponent* Owner)
    : TPanel(Owner)
{
    Caption="";
    OnResize=Resizing;
    Timer1=new TTimer(this);
    Timer1->Enabled=false;
    Timer1->Interval=100;
    Timer1->OnTimer=Timer1Timer;
    Formulas=new TList();
    spParameters=new TList();
    iniFormulas=false;
    iniSp=false;
    inStart=false;
    if(!CallCount)
        if (PEOpenEngine()==0)
        {
         ShowMessage("Не доступен модуль формирования отчетов!");
         return;
        }
    CallCount++;

}
//---------------------------------------------------------------------------
namespace Crprintengine
{
    void __fastcall PACKAGE Register()
    {
        TComponentClass classes[1] = {__classid(TCRPrintEngine)};
    		RegisterComponents("System", classes, 0);
    }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TCRPrintEngine::GetPropety(int Index)
{
    AnsiString Result;
    switch (Index)
    {
        case 1:
            Result=FFileName;
            break;
        case 2:
            Result=FSQLString;
            break;
        case 3:
            Result=FDataServerName;
            break;
        case 4:
            Result=FPassword;
            break;
        case 5:
            Result=FUserID;
            break;
        case 6:
            Result=FDBName;
            break;
        default:
            Result="";
  }
  return Result;
}
//---------------------------------------------------------------------------
void __fastcall TCRPrintEngine::SetPropety(int Index, AnsiString Value)
{
    switch (Index)
    {
        case 1:
            FFileName=Value;
            break;
        case 2:
            FSQLString=Value;
            break;
        case 3:
            FDataServerName=Value;
            break;
        case 4:
            FPassword=Value;
            break;
        case 5:
            FUserID=Value;
            break;
        case 6:
            FDBName=Value;
            break;
        default:
            return;
  }
  return;
}
//---------------------------------------------------------------------------
void __fastcall TCRPrintEngine::Timer1Timer(TObject *Sender)
{
    if (PEIsPrintJobFinished(Job)==1)
    {
        FOnFinish;
        Timer1->Enabled=false;
    }
    else
    {
        PEGetJobStatus(Job, &JobInfo);
    }
}
//---------------------------------------------------------------------------
void __fastcall TCRPrintEngine::Terminate()
{
    PECloseWindow(Job);
    PECancelPrintJob(Job);
    PEClosePrintJob(Job);
    PECanCloseEngine();
    CallCount--;
    if(!CallCount)
        PECloseEngine();
}
//---------------------------------------------------------------------------
bool __fastcall TCRPrintEngine::CheckSetup()
{
    if (FileExists(FFileName))
    {
        return true;
    }
    else
    {
        ShowMessage("Файл отчета "+FFileName+" не найден!");
        return false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TCRPrintEngine::RStart()
{
    short ErrCode, TextLength, Table;
    ProcCursor ccc;
    HANDLE TextHandle;
    PELogOnInfo MyInfo;
    if (!CheckSetup())
    {
        return;
    }

    Job=PEOpenPrintJob(FFileName.c_str());
    if (Job==0)
    {
        ErrCode=PEGetErrorCode(Job);
        PEGetErrorText(Job, &TextHandle, &TextLength);
        char *ErrorString=new char[TextLength+1];
        PEGetHandleString(TextHandle, ErrorString, TextLength);
        ShowMessage("PEOpenPrintJob ошибка. Код ошибки = "+IntToStr(ErrCode)+". "+ErrorString);
        delete [] ErrorString;
        return;
    }
    MyInfo.StructSize=PE_SIZEOF_LOGON_INFO;
    StrCopy(MyInfo.ServerName, FDataServerName.c_str());
    StrCopy(MyInfo.DatabaseName, FDBName.c_str());
    StrCopy(MyInfo.UserID, FUserID.c_str());
    StrCopy(MyInfo.Password, FPassword.c_str());
    Table=0;
    if (!PESetNthTableLogOnInfo(Job, Table, &MyInfo, True))
    {
        ShowMessage("PESetNthTableLogOnInfo ошибка!");
        PEClosePrintJob(Job);
        return;
    }
    if (SQLString!="")
    {
        if(PESetSQLQuery(Job,SQLString.c_str())!=1)
        {
            ShowMessage("PESetSQLQuery ошибка! \r\n" + SQLString);
            return;
        }
    }
    if (iniFormulas)
    {
        for (int i=0; i<Formulas->Count; i++)
        {
            FormulaStruct *Formula=(FormulaStruct*)Formulas->Items[i];
            //ShowMessage("PESetFormula \r\n " + Formula->Name + " := " + Formula->Text);
            if(!PESetFormula(Job, Formula->Name.c_str(), Formula->Text.c_str()))
            {
                ShowMessage("PESetFormula ошибка!\r\n " + Formula->Name + " := " + Formula->Text);
                return;
            }
            if(!PECheckFormula(Job,Formula->Name.c_str()))
            {
                ShowMessage("PECheckFormula ошибка!");
                return;
            }
        }
    }
    if (iniSp)
    {
        for (int i=0; i<spParameters->Count; i++)
        {
            spParametersStruct *spParameter=(spParametersStruct*)spParameters->Items[i];
            if(!PESetNthParam(Job, spParameter->spNumber, spParameter->spString.c_str()))
            {
                ShowMessage("PESetNthParam ошибка!");
                return;
            }
        }
    }
    if (!PEOutputToWindow(Job, "", 0, 0, Width, Height, (WS_VISIBLE||WS_CHILD), Handle))
    {
        ErrCode=PEGetErrorCode(Job);
        ShowMessage("PEOutputToWindow ошибка! Код ошибки = "+IntToStr(ErrCode));
        return;
    }

    Timer1->Enabled=true;
    if (!PEStartPrintJob(Job, false))
    {
        ShowMessage("PEStartPrintJob ошибка!");
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TCRPrintEngine::RStop()
{
    ProcCursor ccc;
    PECloseWindow(Job);
    PECancelPrintJob(Job);
    PEClosePrintJob(Job);
}
//---------------------------------------------------------------------------
void __fastcall TCRPrintEngine::ClearFormulas()
{
    for (int i=0; i<Formulas->Count; i++)
    {
        FormulaStruct *Formula=(FormulaStruct*)Formulas->Items[i];
        delete Formula;
    }
    Formulas->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TCRPrintEngine::ClearParameters()
{
    for (int i=0; i<spParameters->Count; i++)
    {
        spParametersStruct *spParameter=(spParametersStruct*)spParameters->Items[i];
        delete spParameter;
    }
    spParameters->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TCRPrintEngine::NewFormula(AnsiString FormulaName, AnsiString FormulaText)
{
    if (iniFormulas)
    {
        for (int i=0; i<Formulas->Count; i++)
        {
            FormulaStruct *Formula=(FormulaStruct*)Formulas->Items[i];
            if (Formula->Name==FormulaName)
            {
                Formula->Text=FormulaText;
                return;
            }
        }
    }
    FormulaStruct *Formula=new FormulaStruct;
    Formula->Name=FormulaName;
    Formula->Text=FormulaText;
    Formulas->Add(Formula);
    iniFormulas=true;
}
//---------------------------------------------------------------------------
void __fastcall TCRPrintEngine::NewParameter(short Number, AnsiString NewString)
{
    if (iniSp)
    {
        for (int i=0; i<spParameters->Count; i++)
        {
            spParametersStruct *spParameter=(spParametersStruct*)spParameters->Items[i];
            if (spParameter->spNumber==Number)
            {
                spParameter->spString=NewString;
                return;
            }
        }
    }
    spParametersStruct *spParameter=new spParametersStruct;
    spParameter->spNumber=Number;
    spParameter->spString=NewString;
    spParameters->Add(spParameter);
    iniSp=true;
}
//---------------------------------------------------------------------------
void __fastcall TCRPrintEngine::Start()
{
    if (inStart)
    {
        return;
    }
    inStart=true;
    RStop();
    RStart();
    inStart=false;
    Resizing(this);
}

//---------------------------------------------------------------------------
void __fastcall TCRPrintEngine::Resizing(TObject *Sender)
{
 if(!hWnd)     hWnd = PEGetWindowHandle(Job);
 if(hWnd)
  MoveWindow(
    hWnd,	// handle of window
    3,	    // horizontal position
    3,	    // vertical position
    Width-6,	// width
    Height-6,	// height
    true 	// repaint flag
   );
}


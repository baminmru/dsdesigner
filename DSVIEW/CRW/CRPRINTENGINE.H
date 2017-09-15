//---------------------------------------------------------------------------
#ifndef CRPrintEngineH
#define CRPrintEngineH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "crpe.h"
#include "StreamGrid.h"
//---------------------------------------------------------------------------
class PACKAGE TCRPrintEngine : public TPanel
{
    static int CallCount;
private:
    HWND hWnd;
    AnsiString FFileName, FSQLString, FDataServerName, FPassword, FUserID, FDBName;
    TTimer* Timer1;
    short Job;
    PEJobInfo JobInfo;
    bool inStart, iniFormulas, iniSp;
    TNotifyEvent FOnFinish;
    struct FormulaStruct
    {
        AnsiString Name, Text;
    };
    struct spParametersStruct
    {
        short spNumber;
        AnsiString spString;
    };
    TList *Formulas, *spParameters;
protected:
    AnsiString __fastcall GetPropety(int Index);
    void __fastcall SetPropety(int Index, AnsiString Value);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall Terminate();
    bool __fastcall CheckSetup();
    void __fastcall RStart();
    void __fastcall RStop();
    void __fastcall Resizing(TObject *Sender);
public:
    __fastcall TCRPrintEngine(TComponent* Owner);
    __fastcall ~TCRPrintEngine(){Terminate();};
    void __fastcall ClearFormulas();
    void __fastcall ClearParameters();
    void __fastcall NewFormula(AnsiString FormulaName, AnsiString FormulaText);
    void __fastcall NewParameter(short Number, AnsiString NewString);
    void __fastcall Start();
__published:
    __property TNotifyEvent OnFinish={read=FOnFinish, write=FOnFinish};
    __property AnsiString FileName={read=GetPropety, write=SetPropety, index=1, nodefault};
    __property AnsiString SQLString={read=GetPropety, write=SetPropety, index=2, nodefault};
    __property AnsiString DataServerName={read=GetPropety, write=SetPropety, index=3, nodefault};
    __property AnsiString Password={read=GetPropety, write=SetPropety, index=4, nodefault};
    __property AnsiString UserID={read=GetPropety, write=SetPropety, index=5, nodefault};
    __property AnsiString DBName={read=GetPropety, write=SetPropety, index=6, nodefault};
};
//---------------------------------------------------------------------------
#endif

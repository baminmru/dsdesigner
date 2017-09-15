//---------------------------------------------------------------------------
#ifndef NavigatorH
#define NavigatorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>

#include "task.h"
#include <Dialogs.hpp>
#include "dbLoadSave.h"

//---------------------------------------------------------------------------
class TLMENavigator : public TForm
{
__published:	// IDE-managed Components
 TMainMenu *MainMenu1;
 TMenuItem *mnuExit;
 TMenuItem *mnuDesign;
 TMenuItem *mnuLoad;
 TMenuItem *mnuSave;
 TMenuItem *Window1;
 TMenuItem *N2;
 TMenuItem *ArrangeAll1;
 TMenuItem *Cascade1;
 TMenuItem *Tile1;
 TOpenDialog *OpenDialog1;
 TMenuItem *N3;
 TSaveDialog *SaveDialog1;
 TMenuItem *mnuTasks;
 TMenuItem *mnuAbout;
 TMenuItem *N4;
 TMenuItem *mnuDBSave;
 TMenuItem *mnuDbLoad;
  TMenuItem *mnuMakeDoc;
  TMenuItem *LoadObject;
  TMenuItem *SaveObject;
  TMenuItem *N7;
  TMenuItem *N8;
  TMenuItem *N5;
  TMenuItem *mnuAppNew;
  TMenuItem *N6;
  TMenuItem *nmuChangeTask;
  TMenuItem *N9;
 void __fastcall mnuExitClick(TObject *Sender);
 void __fastcall mnuLoadClick(TObject *Sender);
 void __fastcall mnuSaveClick(TObject *Sender);
 void __fastcall N3Click(TObject *Sender);
 
 void __fastcall mnuRunClick(TObject *Sender);

 void __fastcall UserMenuClick(TObject *Sender);

 void __fastcall FormShow(TObject *Sender);
 void __fastcall mnuTasksClick(TObject *Sender);
 void __fastcall N1Click(TObject *Sender);
 void __fastcall ArrangeAll1Click(TObject *Sender);
 void __fastcall Cascade1Click(TObject *Sender);
 void __fastcall Tile1Click(TObject *Sender);
 void __fastcall mnuAboutClick(TObject *Sender);
 
 void __fastcall mnuDBSaveClick(TObject *Sender);
 void __fastcall mnuDbLoadClick(TObject *Sender);
  void __fastcall mnuMakeDocClick(TObject *Sender);
  void __fastcall SaveObjectClick(TObject *Sender);
  void __fastcall LoadObjectClick(TObject *Sender);
  void __fastcall mnuAppNewClick(TObject *Sender);
  void __fastcall nmuChangeTaskClick(TObject *Sender);
  void __fastcall N9Click(TObject *Sender);
  
private:	// User declarations
 Task *CurrentTask;
public:		// User declarations
 __fastcall TLMENavigator(TComponent* Owner);
 AnsiString AppFile;
 AnsiString TaskName;
 LMEAppDef App;
 ParamParser Parser;

};
//---------------------------------------------------------------------------
extern PACKAGE TLMENavigator *LMENavigator;
//---------------------------------------------------------------------------
#endif

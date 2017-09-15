//---------------------------------------------------------------------------
#ifndef TaskEditorH
#define TaskEditorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "streamgrid.h"
#include "task.h"
//#include "RXCtrls.hpp"
//#include "ToolEdit.hpp"
#include <Mask.hpp>
//#include "RXSpin.hpp"

//---------------------------------------------------------------------------
class TfrmTasks : public TForm
{
__published:	// IDE-managed Components
 TPageControl *PageControl1;
 TTabSheet *TabSheet1;
 TTabSheet *TabSheet2;
 TButton *btnTaskAdd;
 TButton *btnDelTask;
 TButton *btnMenuAdd;
 TButton *btnMenuDel;
 TEdit *txtTaskName;
 TEdit *txtTaskCaption;
 TEdit *txtTaskVersion;
 TLabel *Label1;
 TLabel *Label2;
 TLabel *Label3;
 TComboBox *cmbMenu;
 TGroupBox *GroupBox1;
 TEdit *txtMenuCaption;
 TComboBox *cmbMenuEditor;
 TComboBox *cmbMenuReport;
 TLabel *Label4;
 TComboBox *cmbMenuFilter;
 TButton *btnNoParent;
 TLabel *Label5;
 TButton *Button1;
 TButton *Button2;
 TButton *Button3;
 TLabel *Label6;
 TLabel *Label7;
 TTabSheet *TabSheet4;
 TTabSheet *TabSheet3;
 TButton *btnAddFilter;
 TButton *btnDelFilter;
 TGroupBox *GroupBox2;
 TGroupBox *GroupBox3;
 TRadioButton *chkTranNoAccess;
 TRadioButton *chkTranReadOnly;
 TRadioButton *chkTranWrite;
 TGroupBox *GroupBox4;
 TRadioButton *chkTranTypeString;
 TRadioButton *chkTranTypeNumeric;
 TRadioButton *chkTranTypeDate;
 TGroupBox *GroupBox5;
 TLabel *Label27;
 TLabel *Label28;
 TComboBox *cmbTranSubStream;
 TComboBox *cmbTranFieldsToShow;
 TCheckBox *chkTranReFillOnGetFocus;
 TCheckBox *chkTranReFillOnFormShow;
 TComboBox *cmbTranFieldToSync;
 TButton *btnTranFieldChanges;
 TComboBox *cmbTranControlType;
 TEdit *txtTranFieldCaption;
 TCheckBox *chkTranAllowNull;
 TButton *btnAddTranField;
 TButton *btnDelTranField;
 TEdit *txtFieldCondition;
 TLabel *Label8;
 TEdit *txtTranCaption;
 TLabel *Label9;
 TEdit *txtTranPrephics;
 TLabel *Label10;
 TEdit *txtTranPostphics;
 TLabel *Label11;
 TEdit *txtTranConcatenator;
 TLabel *Label12;
 TLabel *Label13;
 TButton *bntTranChanges;
 //TTextListBox *lstReport;
 TComboBox *cmbRepConnection;
 TLabel *Label14;
 TLabel *Label15;
 TEdit *txtRepName;
 TLabel *Label16;
 TLabel *Label17;
 TMemo *txtRepSQL;
 TGroupBox *GroupBox6;
 //TTextListBox *lstFormula;
 TButton *btnFormulaAdd;
 TButton *btnFormulaDel;
 TLabel *Label18;
 TEdit *txtFormulaName;
 TLabel *Label19;
 TMemo *txtFormulaText;
 //TFilenameEdit *txtRepFileName;
 TButton *btnRepAdd;
 TButton *btnRepDel;
 //TTextListBox *lstTran;
 //TTextListBox *lstTranField;
 //TTextListBox *lstMenu;
 //TTextListBox *lstTask;
 TMemo *txtCaptionExpression;
 TLabel *Label20;
    TMemo *txtBuildProc;
    TLabel *Label21;
  TListBox *lstTran;
  TListBox *lstTranField;
  TListBox *lstReport;
  TListBox *lstMenu;
  TListBox *lstTask;
  TListBox *lstFormula;
  TEdit *txtRepFileName;
    //TRxSpinButton *btnTranFieldMove;
 void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

 void __fastcall lstTaskClick(TObject *Sender);
 void __fastcall btnTaskAddClick(TObject *Sender);
 void __fastcall btnDelTaskClick(TObject *Sender);
 void __fastcall txtTaskNameChange(TObject *Sender);
 void __fastcall btnNoParentClick(TObject *Sender);
 void __fastcall btnMenuAddClick(TObject *Sender);
 void __fastcall lstMenuClick(TObject *Sender);
 void __fastcall txtMenuCaptionChange(TObject *Sender);
 void __fastcall Button3Click(TObject *Sender);
 void __fastcall Button2Click(TObject *Sender);
 void __fastcall Button1Click(TObject *Sender);
 void __fastcall btnMenuDelClick(TObject *Sender);
 
 void __fastcall txtTranFieldCaptionChange(TObject *Sender);
 void __fastcall lstTranFieldClick(TObject *Sender);
 void __fastcall lstTranClick(TObject *Sender);
 void __fastcall txtTranCaptionChange(TObject *Sender);
 void __fastcall btnDelFilterClick(TObject *Sender);
 void __fastcall btnAddTranFieldClick(TObject *Sender);
 void __fastcall btnDelTranFieldClick(TObject *Sender);
 void __fastcall btnAddFilterClick(TObject *Sender);
 void __fastcall cmbTranSubStreamChange(TObject *Sender);
 void __fastcall btnTranFieldChangesClick(TObject *Sender);
 void __fastcall bntTranChangesClick(TObject *Sender);
 void __fastcall lstReportClick(TObject *Sender);
 void __fastcall btnRepAddClick(TObject *Sender);
 void __fastcall btnRepDelClick(TObject *Sender);
 void __fastcall txtRepNameChange(TObject *Sender);
 void __fastcall btnFormulaAddClick(TObject *Sender);
 void __fastcall btnFormulaDelClick(TObject *Sender);
 void __fastcall lstFormulaClick(TObject *Sender);
 void __fastcall txtFormulaNameChange(TObject *Sender);
    void __fastcall btnTranFieldMoveBottomClick(TObject *Sender);
    void __fastcall btnTranFieldMoveTopClick(TObject *Sender);
  
private:	// User declarations
public:		// User declarations
 __fastcall TfrmTasks(TComponent* Owner);
 bool OnLstTask;
 bool OnLstMenu;
 bool OnLstSubMenu;
 bool OnLstTran;
 bool OnLstTranField;
 bool OnLstReport;
 bool OnLstFormulas;

 LMEAppDef *App;

 void __fastcall Init();
 void __fastcall FillMenuBoxes();
};

struct MenuHolder{
 long p;
 long c;
 MenuHolder(long _p,long _c){p=_p;c=_c;}
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmTasks *frmTasks;
//---------------------------------------------------------------------------
#endif

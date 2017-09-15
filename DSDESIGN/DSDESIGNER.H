//---------------------------------------------------------------------------
#ifndef DSDesignerH
#define DSDesignerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//#include "RXGrids.hpp"
//#include "RXSpin.hpp"
#include "StreamGrid.h"
#include <checklst.hpp>
#include <Grids.hpp>
#include <Mask.hpp>
//#include "RXCtrls.hpp"
#include <Buttons.hpp>
#include <CheckLst.hpp>
//---------------------------------------------------------------------------
class TfrmStreamDesigner : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *tabQuery;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TMemo *txtSQL;
	TMemo *txtRowSQL;
	TButton *btnCopyToRowSQL;
	TMemo *txtInsert;
	TButton *btnMakeInsert;
	TMemo *txtUpdate;
	TButton *btnMakeUpdate;
	TMemo *txtDelete;
	TButton *btnCopyToDelete;
	TButton *btnExec;
	TTabSheet *tabFNames;
	TLabel *Label6;
	TLabel *Label7;
	TEdit *txtFieldCaption;
	TGroupBox *InsertAccess;
	TRadioButton *InsNoAccess;
	TRadioButton *InsReadOnly;
	TRadioButton *InsReadWrite;
	TCheckBox *VisibleOnView;
	TGroupBox *UpdateAccess;
	TRadioButton *UpdNoAccess;
	TRadioButton *UpdReadOnly;
	TRadioButton *UpdReadWrite;
	TComboBox *cmbControlType;
	TGroupBox *SubStream;
	TLabel *LLL;
	TComboBox *cmbSubStream;
	TComboBox *cmbFieldsToShow;
	TCheckBox *ReFillOnGetFocus;
	TCheckBox *ReFillOnFormShow;
	TTabSheet *TabSheet1;
	TComboBox *cmbEditors;
	TButton *btnNewEditor;
	TGroupBox *Conection;
	TLabel *Label8;
	TLabel *Label9;
	TEdit *txtConnection;
	TEdit *txtDsn;
	TButton *btnNewConnection;
	TComboBox *cmbConnections;
	TTabSheet *TabSheet2;
	TPanel *Panel2;
	TComboBox *cmbTestEditors;
	TButton *btnTest;
	TCheckListBox *lstConnections;
	TButton *btnDeleteConnection;
	TButton *btnDelEditor;
	TTabSheet *TabSheet3;
	TButton *btnFieldList;
	TMaskEdit *txtFieldWidth;
	TGroupBox *GroupBox1;
	TRadioButton *typeString;
	TRadioButton *typeNumeric;
	TRadioButton *typeDateTime;
	TMaskEdit *txtHoriz;
	TMaskEdit *txtVert;
	TMaskEdit *txtHeaderVert;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
  TComboBox *cmbFieldToSync;
  TLabel *Label15;
  TMemo *txtSubwhere;
  TMemo *txtOrderBy;
  TLabel *Label16;
  TLabel *Label17;
  TEdit *txtParamName;
  TButton *btnAddParam;
  TButton *btnDelParam;
  TTabSheet *TabSheet4;
  TComboBox *cmbMaster;
  TComboBox *cmbSlave;
  TLabel *Label18;
  TLabel *Label19;
  TMemo *txtMasterCondition;
  TButton *Button2;
  TButton *btnAddLink;
  TButton *btnDelLink;
  TLabel *Label20;
  TButton *btnFieldChanges;
  TButton *btnAddField;
  TButton *btnDelField;
    TEdit *txtLinkName;
    TLabel *Label21;
    TTabSheet *TabSheet5;
    TComboBox *cmbLinks;
    TButton *btnCallLinked;
    TComboBox *cmbTrans;
    TButton *btnTran;
 TButton *Button3;
 TButton *Button4;
 TEdit *txtTranCaption;
 TLabel *Label22;
 TComboBox *cmbTransactionSource;
 TLabel *Label23;
 TLabel *Label24;
 TComboBox *cmbAfterTransaction;
 TLabel *Label25;
 TLabel *Label26;
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
 TListBox *lstTranField;
 TButton *btnAddTranField;
 TButton *btnDelTranField;
 TEdit *txtQryCaption;
 TComboBox *cmbTranEditor;
 TLabel *Label29;
 TEdit *txtTranMessage;
 TLabel *Label30;
 TButton *Button5;
 TLabel *Label31;
 TGroupBox *GroupBox6;
 TLabel *Label10;
 TEdit *txtUID;
 TLabel *Label11;
 TEdit *txtPWD;
 TButton *btnConnect;
 TButton *btnDisConnect;
 TMemo *txtTranCondition;
 TMemo *txtTransaction;
 TStreamGrid *StreamGrid1;
 //TTextListBox *lstIndex;
 TLabel *Label32;
 TEdit *txtIndexName;
 TLabel *Label33;
 TButton *btnAddIndex;
 TButton *btnDelIndex;
 TMemo *txtIndexOrderBy;
 TCheckBox *AllowNull;
 //TTextListBox *lstTran;
 //TTextListBox *lstLinks;
 //TTextListBox *lstParams;
 //TTextListBox *lstFields;
 TLabel *Label34;
 TMemo *txtSlaveCaption;
 //   TRxSpinButton *btnFieldMove;
    TBitBtn *btnRenumberFields;
 //   TRxSpinButton *btnTranFieldMove;
  TButton *btnCopyTran;
  TListBox *lstTran;
  TListBox *lstLinks;
  TListBox *lstFields;
  TListBox *lstParams;
  TListBox *lstIndex;
  TSpeedButton *SpeedButton1;
  TSpeedButton *SpeedButton2;
  TSpeedButton *SpeedButton3;
  TSpeedButton *btnTranFieldMoveDown;
	void __fastcall btnNewConnectionClick(TObject *Sender);
	void __fastcall btnConnectClick(TObject *Sender);
	void __fastcall lstConnectionsClickCheck(TObject *Sender);
	void __fastcall lstConnectionsClick(TObject *Sender);
	void __fastcall btnDeleteConnectionClick(TObject *Sender);
	void __fastcall btnDisConnectClick(TObject *Sender);
	void __fastcall btnLoadClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall txtConnectionChange(TObject *Sender);
	void __fastcall btnDelEditorClick(TObject *Sender);
	void __fastcall btnNewEditorClick(TObject *Sender);

	void __fastcall cmbEditorsClick(TObject *Sender);
	void __fastcall cmbSubStreamClick(TObject *Sender);
	void __fastcall btnExecClick(TObject *Sender);
	void __fastcall btnTestClick(TObject *Sender);


	void __fastcall btnFieldListClick(TObject *Sender);
	void __fastcall btnCopyToRowSQLClick(TObject *Sender);
	void __fastcall btnMakeInsertClick(TObject *Sender);
	void __fastcall btnCopyToDeleteClick(TObject *Sender);
	void __fastcall btnMakeUpdateClick(TObject *Sender);
	void __fastcall lstFieldsClick(TObject *Sender);
	void __fastcall SaveFieldsChanges(TObject *Sender);
	void __fastcall SaveQueries(TObject * Sender);





  void __fastcall btnFieldChangesClick(TObject *Sender);

  void __fastcall btnAddLinkClick(TObject *Sender);
  void __fastcall btnDelLinkClick(TObject *Sender);
  void __fastcall cmbMasterChange(TObject *Sender);
  void __fastcall lstLinksClick(TObject *Sender);
  void __fastcall Button2Click(TObject *Sender);
  void __fastcall btnAddParamClick(TObject *Sender);
  void __fastcall btnDelParamClick(TObject *Sender);
  void __fastcall txtParamNameChange(TObject *Sender);
  void __fastcall lstParamsClick(TObject *Sender);
  void __fastcall btnAddFieldClick(TObject *Sender);
  void __fastcall btnDelFieldClick(TObject *Sender);
    void __fastcall cmbTestEditorsClick(TObject *Sender);
    void __fastcall btnCallLinkedClick(TObject *Sender);
 

 void __fastcall Button3Click(TObject *Sender);
 void __fastcall lstTranClick(TObject *Sender);
 void __fastcall txtTranCaptionChange(TObject *Sender);
 void __fastcall Button4Click(TObject *Sender);
 void __fastcall btnAddTranFieldClick(TObject *Sender);
 void __fastcall btnDelTranFieldClick(TObject *Sender);
 void __fastcall lstTranFieldClick(TObject *Sender);
 void __fastcall txtTranFieldCaptionChange(TObject *Sender);
 void __fastcall cmbTranSubStreamClick(TObject *Sender);
 void __fastcall btnTranFieldChangesClick(TObject *Sender);

 void __fastcall Button5Click(TObject *Sender);
 void __fastcall btnTranClick(TObject *Sender);

 void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
 
 
 
 void __fastcall btnAddIndexClick(TObject *Sender);
 void __fastcall btnDelIndexClick(TObject *Sender);
 void __fastcall txtIndexNameChange(TObject *Sender);
 void __fastcall lstIndexClick(TObject *Sender);
 
 
 
    void __fastcall btnFieldMoveBottomClick(TObject *Sender);
    void __fastcall btnFieldMoveTopClick(TObject *Sender);
    void __fastcall btnTranFieldMoveBottomClick(TObject *Sender);
    void __fastcall btnTranFieldMoveTopClick(TObject *Sender);
    void __fastcall btnRenumberFieldsClick(TObject *Sender);
  void __fastcall btnCopyTranClick(TObject *Sender);
  
private:	// User declarations
    bool OnLstTran;
    bool OnLstTranField;
	   bool OnLstFields;
    bool OnLstConnections;
    bool OnCmbEditors;
    bool OnLstLinks;
    bool OnLstParams;
    bool OnLstIndex;    
public:		// User declarations
	DataManager *Manager;
	__fastcall TfrmStreamDesigner(TComponent* Owner);
	__fastcall ~TfrmStreamDesigner();
 void __fastcall Init();

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmStreamDesigner *frmStreamDesigner;
//---------------------------------------------------------------------------
#endif

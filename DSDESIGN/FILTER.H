//---------------------------------------------------------------------------
#ifndef FilterH
#define FilterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "bamilib.h"
//#include "CurrEdit.hpp"
#include "StreamGrid.h"
#include "StreamCombo.h"
#include "task.h"
#include <Mask.hpp>
//#include "RXSplit.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

struct FCtrlStorage{
	long ColIdx;
	TWinControl *ctrl;  // всякие разные контрольные элементики
	TMaskEdit *msk;			// специально для времени ectDateTime !!!
	TLabel *lbl;        // и меточки к ним
  									 // поле
	FieldAccessType InsertAccess;            // какой будет доступ при вставке
	FieldAccessType EditAccess;              // какой будет доступ при изменении

	long GetWidth(){
		return ctrl->Width+lbl->Width+ ctrl->Left-lbl->Left-lbl->Width;
	};
	long GetHeight(bool ForInsert){
		if((ForInsert ? InsertAccess:EditAccess)==faNone)
			return 0;
		return (lbl->Height>ctrl->Height? lbl->Height:ctrl->Height);
	};

 FCtrlStorage(FieldAccessType _InsertAccess,FieldAccessType _EditAccess){
		EditAccess= _EditAccess;
   InsertAccess=_InsertAccess;
 };

	~FCtrlStorage(){
		delete lbl;
		delete ctrl;
		if(msk) delete msk;
	};
};



class PACKAGE TfrmFilter : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TButton *btnSave;
	TButton *btnCancel;
	TScrollBox *cbox;
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall btnSaveClick(TObject *Sender);
	void __fastcall CreateWnd();
	void __fastcall DestroyWnd();

	void __fastcall Panel3Resize(TObject *Sender);

 void __fastcall FormShow(TObject *Sender);
private:	// User declarations

protected:
	DataManager *Manager;
	FilterFormDef *F;
	MyArray<FCtrlStorage> Controls;
    bool OnLoad;

public:		// User declarations
 bool SettedUp;
 Params NewRow;
 int __fastcall Setup(				// mrCancell or mrOK
     DataManager *_Manager,
   	FilterFormDef *_F
 );

	int __fastcall Run(				// mrCancell or mrOK
		);


	__fastcall void SetFieldValue(AnsiString Name,StackValue sv);
	__fastcall void BuildControls();
	__fastcall void InitControls();
	__fastcall void SaveControls();
	__fastcall bool CheckNulls();
    void __fastcall CtrlChange(TObject *Sender);
    void __fastcall InitOnChange();


	StackValue * __fastcall  GetFieldValue(AnsiString Name);

	__fastcall TfrmFilter(TComponent* Owner);
 AnsiString __fastcall MakeSubWhere();
 AnsiString __fastcall Parse(AnsiString pattern);
 AnsiString __fastcall GetValue(AnsiString Name);
};

//---------------------------------------------------------------------------
#endif

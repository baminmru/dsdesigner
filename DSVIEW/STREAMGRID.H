//---------------------------------------------------------------------------
#ifndef StreamGridH
#define StreamGridH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//#include <RxGrids.hpp>

#include "bamilib.h"
#include "SG_Garbage.h"
#include "EditForm.h"

class PACKAGE TStreamGrid;
class PACKAGE TSG_Garbage;

//---------------------------------------------------------------------------


class StreamGridParser: public MiniParser {
		long CurrentRow;
		TStreamGrid *sg;
		virtual bool RunFunction (const AnsiString &Name,  MyArray<StackValue>* FArgs);
		virtual bool SetVariable (const AnsiString &Name, const StackValue &Value);
		virtual bool GetVariable (const AnsiString &Name, StackValue &Storage);

	public:
		void SetCurrentRow(long r){CurrentRow=r;};
		virtual void PostInit();

		StreamGridParser(TStreamGrid *_sg):sg(_sg){
		};
};


struct TranInfo {
       StreamTransaction *Tran;
       TfrmEditForm *form;
       TranInfo(StreamTransaction *_Tran=0){
              Tran=_Tran;
              form=0;
       };
       ~TranInfo(){
            Tran=0;
            if(form)
              delete form;
            form=0;
       };
};


class PACKAGE TStreamGrid : public TCustomGrid
{
private:

   // ������ � �����������
   MyArray<TranInfo> Trans;

   // ���������� �������������� �������� �� ��������� ��������� �������
   TDateTime TmpRefreshed; // ����� ��������������� ��� ���������
   long TmpTotalCount;     // ����� ��������������� ��� ���������
   bool LockForEdit;       // true, ���� �������� ��������������

   TDateTime Refreshed;
   long TotalCount;
   long PrevRow;


   	// Runtime only
	  DataSnapshot *Snap;
	  MyArray<long> RowMap;   	  // ������ Grid � ������ Snap
	  DSVMatrix RC;                 // ������ �������� ��� ������

    // ���������� ����������
	  TSG_Garbage *MenuForm;
	  TfrmEditForm *_EditForm;
	  StreamGridParser *Parser;
	  DataStream *s;
	  StreamEditorDef *sed;
    DataManager *Manager;


    // ��������� ��� ����������� � ������
    Sections *Groups;
	   MyArray<DoubleBools> GroupResults;  // Groups.Count() � Snap.RowCount
    MyArray<bool> SelectedRow;
    bool FindAllColumns;
    bool FindUp;
    static bool HintState;


	//bool EditorStatus;
    AnsiString EditorName;

	void __fastcall ResolveRC(bool ReFillOnly=false); // ���������� �������� � ������ �����
													  // ������� � ��������� ������ RowMap
	void __fastcall  MakeGroupResults(void);


protected:

    // ���������� ����������
 DYNAMIC void __fastcall TopLeftChanged(void);
	virtual void __fastcall DrawCell(int ACol, int ARow, const Windows::TRect &ARect, TGridDrawState AState);
	virtual bool __fastcall SelectCell(int ACol, int ARow);
	void __fastcall InvalidateStreamRow(int Row);
 void __fastcall CreateWnd();
	void __fastcall DestroyWnd();

    // ���������� ������������
    /*
	DYNAMIC AnsiString __fastcall GetEditText(int ACol, int ARow);
	DYNAMIC void __fastcall SetEditText(int ACol, int ARow, const System::AnsiString Value);
	DYNAMIC bool __fastcall CanEditModify(void);
    */

    bool __fastcall ChangedWhileEdit();
    TNotifyEvent PrevDblClick;
    void __fastcall FlipMark(TObject * Obj);

public:
   AnsiString FindData;
	  bool ExclusiveSnapshot;
	  bool ScrollFullRow;

		AnsiString SubCondition;
		AnsiString OrderBy;

		EditFieldsDisables LocalFormDef;  // ��� �������� ���� ������� � ����� ����� ��������������
		Params InsertDefaults;            // ��� �������� ���������

    // Find
    bool __fastcall Find();
    bool __fastcall FindOnColumn();
    bool __fastcall FindNext();
    bool __fastcall FindPrev();

        // selections
    bool  __fastcall IsSelected(long RowID);
    void  __fastcall SelectRow(long RowID);
    void  __fastcall UnSelectRow(long RowID);
    void  __fastcall SelectAllRow();
    void  __fastcall UnSelectAllRow();
    long __fastcall  SelectedRowCount();

    // ���������� �� ����� ���������
		void __fastcall SetEditMark();
		void __fastcall ClearEditMark();

        // ��������� ��������� ���������
		void __fastcall MakeFieldsForInsert(Params *f);
   Params * __fastcall GetState();					 // �������� ������� ��������� ������ ���������
   Params * __fastcall GetState(long GRIDROW);// �������� ������� ��������� ������ ���������
   void  __fastcall SetState(Params *NewState); 	 // ������� ������� ��������� ������ ���������
		void __fastcall InsertRow(Params *f);
		bool __fastcall UpdateRow();
		void __fastcall DeleteSRow();


   // �������
   AnsiString __fastcall BuildHTML();
   void __fastcall BuildXL(AnsiString FileName);


		// ���������
   AnsiString GetEditorName(){ return EditorName; };
		DataManager * GetManager(){ return Manager; };
	  TfrmEditForm *  __fastcall  EditForm(){ return _EditForm;};			// ����� ��������������
		StreamEditorDef * __fastcall  GetStreamEditor(){ return sed; };     // �������� ���������
	    EditFieldsAttr * __fastcall  GetFieldsAttr(){                       // �������� ����� ����� ��������������
			if(sed) return &sed->FormDef;
		    else return 0;
		};
	    DataSnapshot * __fastcall GetSnap(){ return Snap;};     			// �������� ������� Snap
	    DataStream * __fastcall GetStream(){ return s;};					// �������� �����
	    long __fastcall GetStreamRow(){                         			// �������� ����� ������ � ������
			if(RowMap.Count()>Row)
				return *RowMap[Row];
			else
			return -1;
	    }
        long __fastcall GetRow(){
        	return Row;
        };

	__fastcall TStreamGrid(TComponent* Owner);
	__fastcall ~TStreamGrid();

	 AnsiString __fastcall CellValue(long ACol, long ARow);
  long __fastcall  CellAlign(long ARow, long ACol);

    // ���������� ������
	void __fastcall SetEditor(DataManager* Manager,AnsiString Name);  // ������� ��������
	void __fastcall RunEditor();                                      // ������ ����������� �����

    // ����������
	void __fastcall Refresh();                        // ������ �������������� �������� ���������
	void __fastcall RefreshData();                    // ������ ���������� Snap
 void __fastcall RefreshRow(){
   try{
    if(GetStreamRow()>=0)
    GetStream()->RefreshSnapRow(GetSnap(),GetStreamRow());
   }catch(...){
   }
   Invalidate();
 };
	void __fastcall Sort(SortDef *s);				 // ��������� / ������ ����������
	void __fastcall Filter(FilterDef *s);            // ��������� / ������ ����������

    // ��������� ��� Combo � Master-Slave
    bool __fastcall  Sync(AnsiString Field, StackValue *Value);	 // �������������
    StackValue *  __fastcall GetValue(AnsiString Field);		 // ��������� �������� ���� ��� ������� ������
    void  __fastcall InitSlave(EditFieldActions *Actions, Params *MasterRow); // �������������� ��������� ��� ������ � ����������� ������
    bool  __fastcall TestLink(AnsiString Condition);

    // ��������
    void __fastcall RunTransaction(AnsiString TranName);


    void __fastcall SGKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);

	__published:

	__property Align ;
	__property BorderStyle ;
	__property Color ;
	__property Ctl3D ;
	__property Enabled ;
	__property FixedColor ;
	__property ParentColor ;
	__property ParentCtl3D ;
	__property ParentShowHint ;
	__property ShowHint ;
	__property TabOrder ;
	__property TabStop ;
	__property Visible ;
 	__property RowCount ;

	__property OnClick ;
	__property OnDblClick ;
	__property OnKeyDown ;
	__property OnKeyPress ;
	__property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
	__property OnEnter ;
	__property OnExit ;

};
//---------------------------------------------------------------------------


#endif

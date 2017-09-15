//----------------------------------------------------------------------------
#ifndef SORTDIELOGH
#define SORTDIELOGH
//----------------------------------------------------------------------------
#include <System.hpp>
#include <Windows.hpp>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Controls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <checklst.hpp>
#include "RXSpin.hpp"
//----------------------------------------------------------------------------
class PACKAGE TsortDlg : public TForm
{
__published:
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
	TCheckListBox *SortList;
	TBitBtn *BitBtn1;
	TRxSpinButton *RxSpinButton1;
	void __fastcall SortListDblClick(TObject *Sender);

	void __fastcall BitBtn1Click(TObject *Sender);

	
	
	void __fastcall RxSpinButton1BottomClick(TObject *Sender);
	void __fastcall RxSpinButton1TopClick(TObject *Sender);
private:
public:
	virtual __fastcall TsortDlg(Classes::TComponent* AOwner);
};

//----------------------------------------------------------------------------
#endif

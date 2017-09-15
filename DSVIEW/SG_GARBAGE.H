//---------------------------------------------------------------------------
#ifndef SG_GarbageH
#define SG_GarbageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Outline.hpp>

#include "StreamGrid.h"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class PACKAGE TStreamGrid;

class PACKAGE TSG_Garbage : public TForm
{
__published:	// IDE-managed Components
	TPopupMenu *SGMenu;
	TMenuItem *mnuUpdate;
	TMenuItem *mnuInsert;
	TMenuItem *mnuDelete;
	TMenuItem *mnuRefresh;
	TMenuItem *MarkRow;
	TMenuItem *MarkAll;
	TMenuItem *FindCol;
	TMenuItem *FindAll;
  TMenuItem *FindNext;
	TMenuItem *N6;
	TMenuItem *N7;
  TMenuItem *FindPrev;
 TMenuItem *mnuIndex;
  TMenuItem *N1;
  TMenuItem *mnuToHTML;
  TMenuItem *mnuToXL;
	void __fastcall mnuUpdateClick(TObject *Sender);
	void __fastcall mnuInsertClick(TObject *Sender);
	void __fastcall mnuDeleteClick(TObject *Sender);
	void __fastcall mnuRefreshClick(TObject *Sender);
	void __fastcall mnuSortClick(TObject *Sender);
	void __fastcall mnuFilterClick(TObject *Sender);
	void __fastcall StreamGrid1DblClick(TObject *Sender);

	
	
	
	
	
	
	
	
	void __fastcall MarkRowClick(TObject *Sender);
	void __fastcall MarkAllClick(TObject *Sender);
  void __fastcall FindColClick(TObject *Sender);
  void __fastcall FindAllClick(TObject *Sender);
  void __fastcall FindNextClick(TObject *Sender);
  void __fastcall FindPrevClick(TObject *Sender);
 void __fastcall mnuIndexClick(TObject *Sender);
  void __fastcall mnuToHTMLClick(TObject *Sender);
  void __fastcall mnuToXLClick(TObject *Sender);
private:	// User declarations

public:		// User declarations
	TStreamGrid *StreamGrid1;
	__fastcall TSG_Garbage(TComponent* Owner);
};

//---------------------------------------------------------------------------
#endif

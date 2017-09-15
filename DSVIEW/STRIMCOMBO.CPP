//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "StrimCombo.h"
#pragma link "ToolEdit"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TStrimCombo *)
{
	new TStrimCombo(NULL);
}
//---------------------------------------------------------------------------
__fastcall TStrimCombo::TStrimCombo(TComponent* Owner)
	: TComboEdit(Owner)
{
}
//---------------------------------------------------------------------------
namespace Strimcombo
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TStrimCombo)};
		RegisterComponents("System", classes, 0);
	}
}
//---------------------------------------------------------------------------

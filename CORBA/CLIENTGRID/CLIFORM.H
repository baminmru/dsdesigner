//---------------------------------------------------------------------------
#ifndef CliFormH
#define CliFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "cartrige_c.hh"
#include <Grids.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TEdit *txtSQL;
        TButton *cmdCall;
        TPanel *Panel2;
        TStringGrid *SG;
        void __fastcall cmdCallClick(TObject *Sender);
private:	// User declarations
        Cartrige_ptr __fastcall Getcartrige();
        Cartrige_var Fcartrige;
        void __fastcall Setcartrige(Cartrige_ptr _ptr);
public:		// User declarations
        __fastcall TForm1(TComponent* Owner); 
        __property Cartrige_ptr cartrige = {read=Getcartrige, write=Setcartrige};
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

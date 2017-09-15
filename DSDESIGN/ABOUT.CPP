//---------------------------------------------------------------------
#include <vcl.h>
//nclude "VerInfo.hpp"

#pragma hdrstop

#include "About.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//---------------------------------------------------------------------
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{
  /*
  TVersionInfo  *Info = new TVersionInfo( Application->ExeName);
  if(Info->Valid)
  */
  lblBuild->Caption="версия 3.0";//+Info->FileVersion +" ( "+Info->ProductVersion+" )";
  //delete Info;
}
//---------------------------------------------------------------------






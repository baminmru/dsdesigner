//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TokenTest.h"
#include "stackvalue.h"
#include "Tokenizer.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
  Tokenizer t(Memo1->Text);
  ListBox1->Items->Clear();
  t.Spaces.Add(new AnsiString("\n\r"));
  t.Spaces.Add(new AnsiString("\r\n"));
  t.Spaces.Add(new AnsiString("\r"));
  t.Spaces.Add(new AnsiString("\n"));
  t.Spaces.Add(new AnsiString("\t"));
  t.Spaces.Add(new AnsiString(" "));

  t.Delimiters.Add(new AnsiString("::"));
  t.Delimiters.Add(new AnsiString(":"));
  t.Delimiters.Add(new AnsiString("->"));
  t.Delimiters.Add(new AnsiString("++"));
  t.Delimiters.Add(new AnsiString("--"));
  t.Delimiters.Add(new AnsiString("<<"));
  t.Delimiters.Add(new AnsiString(">>"));

  t.Delimiters.Add(new AnsiString("=="));
  t.Delimiters.Add(new AnsiString("!="));
  t.Delimiters.Add(new AnsiString(">="));
  t.Delimiters.Add(new AnsiString("<="));

  t.Delimiters.Add(new AnsiString(">"));
  t.Delimiters.Add(new AnsiString("<"));
  t.Delimiters.Add(new AnsiString(";"));

  t.Delimiters.Add(new AnsiString("["));
  t.Delimiters.Add(new AnsiString("]"));

  t.Delimiters.Add(new AnsiString("!"));
  t.Delimiters.Add(new AnsiString("+"));
  t.Delimiters.Add(new AnsiString("-"));
  t.Delimiters.Add(new AnsiString("#"));
  t.Delimiters.Add(new AnsiString("*"));
  t.Delimiters.Add(new AnsiString("="));
  t.Delimiters.Add(new AnsiString("/*"));
  t.Delimiters.Add(new AnsiString("("));
  t.Delimiters.Add(new AnsiString(")"));
  t.Delimiters.Add(new AnsiString("{"));
  t.Delimiters.Add(new AnsiString("}"));
  t.Delimiters.Add(new AnsiString("*/"));
  t.Delimiters.Add(new AnsiString("//"));
  t.Delimiters.Add(new AnsiString("/"));
  t.Delimiters.Add(new AnsiString("\""));
  t.Delimiters.Add(new AnsiString(","));


  t.Words.Add(new AnsiString("select"));
  t.Words.Add(new AnsiString("from"));
  t.Words.Add(new AnsiString("where"));
  t.Words.Add(new AnsiString("and"));
  t.Words.Add(new AnsiString("or"));
  t.Words.Add(new AnsiString("update"));
  t.Words.Add(new AnsiString("delete"));
  t.Words.Add(new AnsiString("create"));
  t.Words.Add(new AnsiString("table"));
  t.Words.Add(new AnsiString("view"));
  t.Words.Add(new AnsiString("as"));
  t.Words.Add(new AnsiString("numeric"));
  t.Words.Add(new AnsiString("datetime"));
  t.Words.Add(new AnsiString("string"));
  t.Words.Add(new AnsiString("binary"));
  t.Words.Add(new AnsiString("primary"));
  t.Words.Add(new AnsiString("foreign"));
  t.Words.Add(new AnsiString("unique"));
  t.Words.Add(new AnsiString("key"));
  t.Words.Add(new AnsiString("is"));
  t.Words.Add(new AnsiString("not"));
  t.Words.Add(new AnsiString("null"));
  t.Words.Add(new AnsiString("like"));




  StrTwins *tw = new StrTwins;
  tw->Start = "\"";
  tw->Finish = "\"";
  t.Twins.Add(tw);

  tw = new StrTwins;
  tw->Start = "/*";
  tw->Finish = "*/";
  tw->SkipAll = true;
  t.Twins.Add(tw);

  tw = new StrTwins;
  tw->Start = "--";
  tw->Finish = "\n";
  tw->SkipAll = true;
  t.Twins.Add(tw);


  AnsiString sss;
  while(1){
    sss=t.Next();
    if(sss=="") break;
    ListBox1->Items->Add(sss);

  }

}
//---------------------------------------------------------------------------

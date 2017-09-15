//---------------------------------------------------------------------------
#ifndef TokenizerH
#define TokenizerH


#define STATUS_UNKNOWN   -1
#define STATUS_SPACE     0
#define STATUS_DELIMITER 1
#define STATUS_WORD      2
#define STATUS_IDENT     3

struct StrTwins{
  AnsiString Start;
  AnsiString Finish;
  bool SkipAll;
  //bool AllowNested;
  long OutputStatus;

  StrTwins(){
    SkipAll=false;
  };
};



class Tokenizer{
  long Pos;
  AnsiString Info;
  long LastStatus;
  long mode;
public:
  MyArray<AnsiString> Spaces;
  MyArray<AnsiString> Delimiters;
  MyArray<AnsiString> Words;
  MyArray<StrTwins>   Twins;

  Tokenizer(AnsiString StringToParse){
    Pos=1;
    Info=StringToParse;
    LastStatus = STATUS_UNKNOWN;
    mode =-1;
  };

  AnsiString Rewind(){
    Pos=1;
    LastStatus = STATUS_UNKNOWN;
  };

  void SetPos(long NewPos){
    Pos = NewPos;
    LastStatus = STATUS_UNKNOWN;
  };

  AnsiString Next();
  long GetStatus(){
      return  LastStatus;

  };

};


//---------------------------------------------------------------------------
#endif

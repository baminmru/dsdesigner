//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "stackvalue.h"
#include "Tokenizer.h"

#define  UCase(s) (s.UpperCase())
#define  Mid( s, start, size) (s.SubString(start,size))
#define  Len(s) (s.Length())
#define  tvLen (TV.Count())
#define  Left(s, size) (s.SubString(1,size))
#define  Right(s, size) (s.SubString(s.Length()-size+1,size))



AnsiString Tokenizer::Next(){
	AnsiString buf;
  AnsiString S = Info;
	long i,j, st;
	st = Pos;
	buf = "";
eat_space:
	while (st <= Len(S)){

		if  (mode < 0){

      // Skip all trailing space
      if(buf==""){
        for (i = 0; i<Spaces.Count(); i++){
           if(UCase(Mid(S, st, Spaces[i]->Length())) == *(Spaces[i])){
              st = st + Spaces[i]->Length();
              goto eat_space;
           }
        }

        // Scaning for delimiter char
        for (i = 0; i<Delimiters.Count(); i++){
          	if  (UCase(Mid(S, st, Delimiters[i]->Length())) == *(Delimiters[i])){
						  st = st + Delimiters[i]->Length();
              LastStatus=STATUS_DELIMITER;
              // Check for mode changing
              for(long j=0;j<Twins.Count();j++){
                if(*Delimiters[i]==Twins[j]->Start){
                  mode = j;
                  if(Twins[j]->SkipAll)
                     goto eat_space;
                }
              }
              Pos=st;
       				return ( *Delimiters[i] );
            }
				}

        // Nothing to return or skip
 				buf = buf + Mid(S, st, 1);
     		st = st + 1;

      }else{

        // if buffer is not empty and space is a next token, return the buffer
        for (i = 0; i<Spaces.Count(); i++){
           if(UCase(Mid(S, st, Spaces[i]->Length())) == *(Spaces[i])){
              Pos = st ;

              // May be we catch a word ?
              for( j = 0 ; j<Words.Count();j++){
                if ( UCase(buf) == UCase((*Words[j])) ){
                  LastStatus=STATUS_WORD;
                  return ( *Words[j] );
                }
              }

              // No, It is a variable or a constant
              LastStatus=STATUS_IDENT;
              return ( buf );
           }
        }

        // if buffer is not empty and Delimiter is a next token, return the buffer
        for (i = 0; i<Delimiters.Count(); i++){
           if(UCase(Mid(S, st, Delimiters[i]->Length())) == *(Delimiters[i])){
              Pos = st ;

              // Is word already in buffer ?
              for( j = 0 ; j<Words.Count();j++){
                if ( UCase(buf) == UCase((*Words[j])) ){
                  LastStatus=STATUS_WORD;
                  return ( *Words[j] );
                }
              }

              // No, it is a variable or a constant...
              LastStatus=STATUS_IDENT;
              return ( buf );
           }
        }


      // Unknown token seek to next char
			buf = buf + Mid(S, st, 1);
     	st = st + 1;
     }

		} else {
      // Twins Mode
      // Looking for  Finish mark ...

      while(Twins[mode] &&
           !(
            UCase(Mid(S, st, Twins[mode]->Finish.Length())) == Twins[mode]->Finish
            ) && st <= Len(S) ) {
    				buf = buf + Mid(S, st, 1);
		    		st = st + 1;
		  }
      Pos=st;


		  if ( buf != "" ){
        LastStatus = Twins[mode]->OutputStatus;
        if(Twins[mode]->SkipAll){
          buf="";
          goto eat_space;
        }
        return buf;
			}else{
        LastStatus = Twins[mode]->OutputStatus;
        buf=Twins[mode]->Finish;
        Pos = st+Twins[mode]->Finish.Length();
        mode =-1;
        if(Twins[mode]->SkipAll){
          buf="";
          goto eat_space;
        }
        return buf;
      }

		}
  }

  if(buf==""){
    LastStatus = STATUS_UNKNOWN;
    return buf;
  }

  // Is word already in buffer ?
  for( j = 0 ; j<Words.Count();j++){
    if ( UCase(buf) == UCase((*Words[j])) ){
      LastStatus=STATUS_WORD;
      return ( *Words[j] );
    }
  }

  LastStatus=STATUS_IDENT;
  return buf;

};


//---------------------------------------------------------------------------
#pragma package(smart_init)

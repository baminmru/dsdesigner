#include "bamilib.h"


#define CCur(x) ((x).ToNumeric())
#define CVar(x) ((x))
#define CStr(x) ((x).ToStr())
#define CDate(x)((x).ToDate())
#define CInt(x) (((long)((x).ToNumeric())))
#define  UCase(s) (s.UpperCase())
#define  Mid( s, start, size) (s.SubString(start,size))
#define  Len(s) (s.Length())
#define  tvLen (TV.Count())
#define  Left(s, size) (s.SubString(1,size))
#define  Right(s, size) (s.SubString(s.Length()-size+1,size))


static long * NewLong(long IDX){
			long *l = new  long;
			*l=IDX;
			return l;
};

void MiniParser::PushValue(StackValue *v){
	varStack.push (v);
	//cout <<endl << "push value :" << v->ToStr().c_str();
};

void MiniParser::PushValue(StackValue v){
	PushValue (&v);
};

StackValue *MiniParser::PopValue(){
	StackValue *v = varStack.pop();
	//cout <<endl << "pop value :" << v->ToStr().c_str();
	return v;
};

void MiniParser::PushOperation(long *op){
	opStack.push(op);
	//if(token[*op])
	//	cout << endl << "push op :" <<token[*op]->S.c_str();
};


long * MiniParser::PopOperation(){
	long * op = opStack.pop();
	//if(token[*op])
	//	cout << endl << "pop op :" <<token[*op]->S.c_str();
	return op;
};

void MiniParser::MakeDelimiters(bool rebuild){
	if(rebuild || token.Count()!=Delimiters.Count()+Words.Count()+1){
		Delimiters.Clear();
		Words.Clear();
		for(long i=1;i<token.Count();i++){
			if(token[i]->Delimiter)
				Delimiters.Add(new ptrToken(& token[i]->S,token[i]->Class,i,token[i]->ArgCount));
			else
				Words.Add(new ptrToken(&token[i]->S,token[i]->Class,i,token[i]->ArgCount));
		}
	}
};

AnsiString MiniParser::NextToken(AnsiString S, long start, long & nextChar,
									 tokenType & tokenClass ,long & TokenIdx){

	MakeDelimiters(false);
	AnsiString buf, b1;
	long i,j, st;
	bool mode=false;

	st = start;
	buf = "";
	mode = false;
eat_space:
	while (st <= Len(S)){
		if  (! mode ){
		for (i = 0; i<Delimiters.Count(); i++){
				if  (UCase(Mid(S, st, Delimiters[i]->S->Length())) == *(Delimiters[i]->S)){
					if ( Delimiters[i]->Class == ttSpace){
						st = st + Delimiters[i]->S->Length();
						if  (buf == ""){
							goto eat_space;
						}
					}
					if ( buf == "" ){
						nextChar = st + Delimiters[i]->S->Length();
						tokenClass = Delimiters[i]->Class;
						TokenIdx = Delimiters[i]->Delimiter;
						return ( *Delimiters[i]->S );
					} else {
						nextChar = st ;
						b1 = Trim(buf) ;
						for( j = 0 ; j<Words.Count();j++){
							if ( UCase(b1) == UCase((*Words[j]->S)) ){
								tokenClass = Words[j]->Class;
								TokenIdx = Words[j]->Delimiter;
								return ( *Words[j]->S );
							}
						}
						tokenClass = ttVarConst;
						TokenIdx = 0;
						return ( buf );
					}
				}
			}
		} else {
			if ( Mid(S, st, 1) == "\"" ){
				buf = buf + Mid(S, st, 1);
				nextChar = st + 1;
				tokenClass = ttVarConst;
				TokenIdx = 0;
				return ( buf );
				}
		}

		if ( buf == "" ){
			if ( Mid(S, st, 1) == "\"" ){
				mode = True ;
			}
		}

		buf = buf + Mid(S, st, 1);
		st = st + 1;
	}

	nextChar = Len(S) + 1;
	b1 = Trim(buf);
	for( j = 0 ;j< Words.Count();j++){
		if (UCase(b1) == *Words[j]->S ){
			tokenClass = Words[j]->Class;
			TokenIdx = Words[j]->Delimiter;
		return ( *Words[j]->S );
		}
	}
	TokenIdx = 0;
	tokenClass = ttVarConst;
	return buf;
};

AnsiString   MiniParser::ParseExpression(AnsiString Ex  ){
	TokenList T;
	Labels L;
	AnsiString PE;
	StackValue *v1;

	opStack.Clear();
	varStack.Clear();
	RetStack.Clear();


	CompileExpression(Ex,&T,&L);
	ExecExpression(T,L);
	Log_Add_Mult();


	if ( opStack.Count()==0 ){

		while ( varStack.Count() > 0){
			v1 = PopValue();
     if(!PE.IsEmpty()) PE = PE + " ";
			PE = PE +  v1->ToStr();
		}
	} else {
		PE = "";
		 MessageBox (0, "Стек операций не пуст!",  "Ошибка!!!",MB_OK);
	}
	return PE;
};



AnsiString   MiniParser::ParseExpression(TokenList *T, Labels *L  ){
	StackValue *v1;
	AnsiString PE;
	ExecExpression(*T,*L);
	Log_Add_Mult();

	if ( opStack.Count()==0 ){

		while ( varStack.Count() > 0){
			v1 = PopValue();
			PE = PE + " " + v1->ToStr();
		}
	} else {
		PE = "";
		 MessageBox (0, "Стек операций не пуст!",  "Ошибка!!!",MB_OK);
	}
	return PE;
};



void MiniParser::Add_Mult(){
	 long *o1;
	 StackValue * v1, *v2;

	//DebugMe ("befor addmult")
	while ( opStack.Count() > 0 && varStack.Count() > 1){
		o1 = PopOperation();
		if ( (token[*o1]->Class == ttArifmetic2 || token[*o1]->Class == ttMult ) && varStack.Count() > 1 ){
				v1 = PopValue();
				v2 = PopValue();
				AnsiString ss=UCase(token[*o1]->S);
						if(ss == "+")
							PushValue (new StackValue(CCur(GetValue(v2)) + CCur(GetValue(v1))));
						if(ss == "&")
							PushValue (new StackValue(CStr(GetValue(v2)) + CStr(GetValue(v1))));
						if(ss == "-")
							PushValue (new StackValue(CCur(GetValue(v2)) - CCur(GetValue(v1))));
						if(ss == "*")
							PushValue (new StackValue(CCur(GetValue(v2)) * CCur(GetValue(v1))));
						if(ss == "/")
							PushValue (new StackValue(CCur(GetValue(v2)) / CCur(GetValue(v1))));
						if(ss == "%")
							PushValue (new StackValue((long)CCur(GetValue(v2)) % (long)CCur(GetValue(v1))));
						delete v1;
						delete v2;

		} else {
				PushOperation (o1);
				return ;
		}
		delete o1;
	}
};


void MiniParser::Add_Mult_EQ(){
	 long *o1;
	 StackValue * v1, *v2;
		while ( opStack.Count() > 0 && varStack.Count() > 1){
		Add_Mult();
		if ( opStack.Count() > 0 && varStack.Count() > 1){
		o1 = PopOperation();
		if ( token[*o1]->Class == ttLogical2 && varStack.Count() > 1 ){
				v1 = PopValue();
				v2 = PopValue();
				AnsiString ss=UCase(token[*o1]->S);
						if(ss == "==")
							PushValue (new StackValue( GetValue(v2) == GetValue(v1)));
						if(ss == "!=")
							PushValue (new StackValue(!(GetValue(v2) == GetValue(v1))));
						if(ss == ">=")
							PushValue (new StackValue((int) (CCur(GetValue(v2)) >= CCur(GetValue(v1)))));
						if(ss == "<=")
							PushValue (new StackValue((int) (CCur(GetValue(v2)) <= CCur(GetValue(v1)))));
						if(ss == ">")
							PushValue (new StackValue((int) (CCur(GetValue(v2)) > CCur(GetValue(v1)))));
						if(ss == "<")
							PushValue (new StackValue((int) (CCur(GetValue(v2)) < CCur(GetValue(v1)))));
				delete v1;
				delete v2;
		} else {
				PushOperation( o1);
				return ;
		}
		delete o1;
		}
	}

};


void MiniParser::Log_Add_Mult(){
	 long *o1;
	 StackValue *v1, *v2;

	while ( opStack.Count() > 0 && varStack.Count() > 1){
		Add_Mult_EQ();
		if ( opStack.Count() > 0 && varStack.Count() > 1 ){
			o1 = PopOperation();
			if ( (token[*o1]->Class == ttLogicalAdd || token[*o1]->Class == ttLogicalMult)
			 && varStack.Count() > 1 ){
				v1 = PopValue();
				v2 = PopValue();
				AnsiString ss=UCase(token[*o1]->S);

				if(ss == "AND")
					 PushValue (new StackValue(CInt(GetValue(v2)) && CInt(GetValue(v1))));
				if(ss == "OR")
					 PushValue (new StackValue(CInt(GetValue(v2)) || CInt(GetValue(v1))));
				if(ss == "XOR")
					 PushValue (new StackValue(CInt(GetValue(v2)) ^ CInt(GetValue(v1))));
				delete v1;
				delete v2;


			} else {
				PushOperation (o1);
				return ;
			}
			delete o1;
		}

	}
};

void MiniParser::Log_Mult(){
	 long *o1;
	 StackValue *v1, *v2;
	while ( opStack.Count() > 0 && varStack.Count() > 1){
		Add_Mult_EQ();
		o1 = PopOperation();
		if ( token[*o1]->Class == ttLogicalAdd  && varStack.Count() > 1 ){
				v1 = PopValue();
				v2 = PopValue();
				if(UCase(token[*o1]->S)=="AND")
							PushValue (new StackValue(CInt(GetValue(v2)) && CInt(GetValue(v1))));
				delete v1;
				delete v2;
		 } else {
				PushOperation( o1);
				return ;
		}
		delete o1;
	}
};



void MiniParser::Mult(){
	long *o1;
	StackValue *v1, *v2;
	while ( opStack.Count() > 0 && varStack.Count() > 1){
		o1 = PopOperation();
		if ( token[*o1]->Class == ttMult ){
			 v1 = PopValue();
			 v2 = PopValue();
			 AnsiString ss =UCase(token[*o1]->S);
			 if (ss== "*")
				 PushValue (new StackValue(CCur(GetValue(v2)) * CCur(GetValue(v1))));
			 if(ss== "/")
				 PushValue (new StackValue(CCur(GetValue(v2)) / CCur(GetValue(v1))));
			 if(ss== "%")
				 PushValue (new StackValue(CInt(GetValue(v2)) % CInt(GetValue(v1))));
			 delete v1;
			 delete v2;
		} else {
				PushOperation(o1);
				return ;
		}
		delete o1;
	}
};

// GET & SET
/////////////////////////////////////////////////////////////////////////////////////
StackValue MiniParser::GetValue(StackValue *v){
	StackValue  outValue;
	if(!v) return outValue;
	if ( v->type == stString ){
		outValue = *v;
 		if ( Mid(v->ToStr(), 1, 1) == "\""  ){
			outValue.type=stString;
			outValue.s = Mid(v->ToStr(), 2, Len(v->ToStr()) - 2);
            return outValue;
		}

        if ( Mid(v->ToStr(), 1, 1) == "'"){
            return outValue;
        }

        if(v->IsNumeric()){
               outValue=v->ToNumeric();
               return outValue;
        }

        if(v->IsDate()){
	           outValue=v->ToDate();
							 return outValue;

        }

        // непосредственное расширение
		if(GetVariable(v->ToStr(), outValue)) return outValue;

            // определенные пользователем переменные
		ValueStorage * vvv =Searcher.Find(v->ToStr());
		if(vvv && vvv->Dimention()==1){
			outValue=*(vvv->Get());
            return outValue;
        }

        // дополнительное расширение
		if(ValueFromVar)
			if (ValueFromVar(v->ToStr(), outValue)) return outValue;

        // никто не признался - ругаемся
        //MessageBox(0,"Неверное имя переменной",v->ToStr().c_str(),MB_OK);
          return outValue;
	
	} else {
		outValue = *v;
	}
	return outValue;
};


void MiniParser::SetVar(StackValue *var, StackValue *val,  AnsiString token){
	StackValue v;
	if( token=="=")
						v = *val;
	if( token=="+=")
						v =  StackValue(CCur(GetValue(var)) + CCur(*val));
	if( token=="-=")
						v = StackValue(CCur(GetValue(var)) - CCur(*val));
	if( token=="*=")
						v = StackValue(CCur(GetValue(var)) * CCur(*val));
	if( token=="/=")
						v = StackValue(CCur(GetValue(var)) / CCur(*val));
	if( token=="&=")
						v = StackValue(CStr(GetValue(var)) + CStr(*val));
	if( token=="%=")
						v = StackValue((long)CCur(GetValue(var)) % (long)CCur(*val));
	if( token=="--")
						v = StackValue(CCur(GetValue(var)) - 1);
	if( token=="++")
						v = StackValue(CCur(GetValue(var)) + 1);

	ValueStorage * vvv =Searcher.Find(var->ToStr());

	// virtual function
	if(SetVariable(var->ToStr(), v)) return;

	if(vvv && vvv->Dimention()==1)
		 vvv->Set(new StackValue(v));
		else
		if(SetValueToVar)
					SetValueToVar(var->ToStr(), v);
};




// CALLFUNCTION
////////////////////////////////////////////////////////////////////////////
void MiniParser::CallFunction(){
	MyArray<StackValue> FArgs; long  /* i,*/ *o1 ; StackValue  *v1,*v2;
	long y, D, mm, h, mn, S;
	o1 = PopOperation();
	FArgs.Clear();
	FArgs.Add(new StackValue());

	if ( token[*o1]->Class == ttFRightBrace ){

		// выбираем параметры из стека
		do {
			delete o1;
			Log_Add_Mult();
			v1=PopValue();
			v2=new StackValue(GetValue(v1));
			delete v1;
			FArgs.Insert(v2,1);
			o1 = PopOperation();
		} while (o1 &&  token[*o1]->Class == ttComa);

		if (o1 &&  token[*o1]->Class == ttFLeftBrace ){
			delete o1;
			o1 = PopOperation();
		} else {
			 MessageBox (0, "Несоответствие скобок {...} в вызове функции ",  "Ошибка!!!",MB_OK);
			return ;
		}
	}

	if(!o1){
		MessageBox (0, "Ожидалось имя функции!",  "Ошибка!!!",MB_OK);
		return ;
	}

	AnsiString ss=UCase(token[*o1]->S);

	if ( token[*o1]->Class == ttFunc ){



		 if ( FArgs.Count() > token[*o1]->ArgCount ){
				delete o1;

				// вызов виртуальной функции пользователя
				if(	RunFunction(ss,  &FArgs))  return;

				if(ss=="EXEC"){
					PushValue(new StackValue(ParseExpression(CStr(*FArgs[1]))));
					return;
				}


				if(ss=="DEF"){
					 if(FArgs.Count()==2)
						 Searcher.GetCurrent()->Add(new ValueStorage(1),0,UCase(FArgs[1]->ToStr()).c_str());
					 else
						 Searcher.GetCurrent()->Add(new ValueStorage(CInt(*FArgs[2])),0,UCase(FArgs[1]->ToStr()).c_str());
						 return;
				}

				if(ss=="GLOBAL"){
					 if(FArgs.Count()==2)
						 Vars.Add(new ValueStorage(1),0,UCase(FArgs[1]->ToStr()).c_str());
					 else
						 Vars.Add(new ValueStorage(CInt(*FArgs[2])),0,UCase(FArgs[1]->ToStr()).c_str());
						 return;
				}


				if(ss=="SET"){
					 Searcher.Find(UCase(CStr(*FArgs[1])))->Set(new StackValue(FArgs[3]),CInt(*FArgs[2]));
					 return;
				}

				if(ss=="GET"){
					 PushValue(new StackValue(Searcher.Find(UCase(CStr(*FArgs[1])))->Get(CInt(*FArgs[2]))));
					 return;
				}

				if(ss=="CDBL"){
					PushValue (new StackValue(FArgs[1]->ToNumeric()));
					return;
				}

				if(ss=="CDATE"){
					PushValue (new StackValue(FArgs[1]->ToDate()));
					return;
				}

				if(ss=="CSTR"){
					PushValue (new StackValue(FArgs[1]->ToStr()));
					return;
				}

				if(ss=="CSQL"){
					PushValue (new StackValue(FArgs[1]->ToSQLStr()));
					return;
				}

				if(ss=="YEAR"){
					unsigned short year,month,day;
					(CDate(*FArgs[1])).DecodeDate(& year, & month, & day);
					PushValue (new StackValue(year));
					return;
				}
				if(ss=="DAY"){
					unsigned short year,month,day;
					CDate(*FArgs[1]).DecodeDate(& year, & month, & day);
					PushValue (new StackValue(day));
					return;
				}
				if(ss=="MONTH"){
					unsigned short year,month,day;
					CDate(*FArgs[1]).DecodeDate(& year, & month, & day);
					PushValue (new StackValue(month));
					return;
				}
				if(ss=="HOUR"){
					unsigned short hour,  min,  sec, msec;
					CDate(*FArgs[1]).DecodeTime(& hour, & min, & sec, & msec);
					PushValue (new StackValue(hour));
					return;
				}

				if(ss=="MINUTE"){
					unsigned short hour,  min,  sec, msec;
					CDate(*FArgs[1]).DecodeTime(& hour, & min, & sec, & msec);
					PushValue (new StackValue(min));
					return;
				}

				if(ss=="SECOND"){
					unsigned short hour,  min,  sec, msec;
					CDate(*FArgs[1]).DecodeTime(& hour, & min, & sec, & msec);
					PushValue (new StackValue(sec));
					return;
				}


				if(ss=="MAKEDATE"){

                    try{
						TDateTime dd(CorrectYear(CInt(*FArgs[3])),CInt(*FArgs[2]), CInt(*FArgs[1]));
            	        PushValue (new StackValue(dd));
                    }catch(...){
	                    TDateTime dd;
    	                PushValue (new StackValue(dd));
                    }

					return;
				}
				if(ss=="MAKETIME"){
                	try {
						TDateTime dd(CInt(*FArgs[1]),CInt(*FArgs[2]), CInt(*FArgs[3]),0);
						PushValue (new StackValue(dd));
                    }catch(...){
						TDateTime dd;
						PushValue (new StackValue(dd));
                    }
					return;
				}
				if(ss=="MAKEDATETIME"){
                	try {
						TDateTime tt(CInt(*FArgs[4]),CInt(*FArgs[5]), CInt(*FArgs[6]),0);
						TDateTime dd(CorrectYear(CInt(*FArgs[3])),CInt(*FArgs[2]), CInt(*FArgs[1]));
						dd+=tt;
						PushValue (new StackValue(dd));
                    }catch(...){
						TDateTime dd;
						PushValue (new StackValue(dd));

                    }
  					return;
				}

				if(ss=="DDMMYYYY"){
										{
                     TDateTime d=CDate(*FArgs[1]);
                     unsigned short y,m,day;
			  		 d.DecodeDate(&y,&m,&day);
                     char buf[40];
                     sprintf(buf, "%02d/%02d/%04d",day,m,y);
					 PushValue (new StackValue(buf));
                     }
					return;
				}

				if(ss=="DDMMYY"){
                    {
                     TDateTime d=CDate(*FArgs[1]);
                     unsigned short y,m,day;
			  		 d.DecodeDate(&y,&m,&day);
                     char buf[40];
                     sprintf(buf, "%02d/%02d/%02d",day,m,y%100);
					 PushValue (new StackValue(buf));
                     }
        			return;
				}


				if(ss=="STRSTRIP"){
					if( FArgs[1]->type==stString){
                        AnsiString sss=CStr(*FArgs[1]);
						if ( (Left(sss, 1) == "'" && Right(sss, 1) == "'") ||
						(Left(sss, 1) == "\"" && Right(sss, 1) == "\"") ){
                            sss=Mid(sss, 2, Len(sss) - 2);
							PushValue (new StackValue(sss));
						} else {
							PushValue (new StackValue(sss));
						}
					}else {
						PushValue (new StackValue(FArgs[1]));
					}
                    return;
				}

				if(ss=="TODATE"){
					y = CorrectYear(CInt(StackValue(Mid(CStr(*FArgs[1]), 8, 4))));
					D = CInt(StackValue(Mid(CStr(*FArgs[1]), 5, 2)));
					mm = CInt(StackValue(Mid(CStr(*FArgs[1]), 2, 2)));
					h = CInt(StackValue(Mid(CStr(*FArgs[1]), 13, 2)));
					mn = CInt(StackValue(Mid(CStr(*FArgs[1]), 16, 2)));
					S = CInt(StackValue(Mid(CStr(*FArgs[1]), 19, 2)));
					TDateTime dd(y,mm, D);
					TDateTime tt(h,mn,S,0);
					dd+=tt;
					PushValue (new StackValue(dd));
					return;
				}

				if(ss=="PUSH"){
					if( FArgs[1]->type==stString){
						if ( Left(CStr(*FArgs[1]), 1) == "\"" ){
							PushValue (new StackValue(Mid(CStr(*FArgs[1]), 2, Len(CStr(*FArgs[1])) - 2)));
						} else {
							PushValue (new StackValue(FArgs[1]));
						}
					}else{
						PushValue (new StackValue(FArgs[1]));
					}
					return;
				}

				if(ss=="DATE"){
						PushValue (new StackValue(TDateTime::CurrentDate()));
						return;
				}

				if(ss=="NOW"){
						PushValue (new StackValue(TDateTime::CurrentDateTime()));
						return;
				}

				if(ss=="TIME"){
						PushValue (new StackValue(TDateTime::CurrentTime()));
						return;
				}


				if(ss=="LEN"){
						PushValue (new StackValue(Len(CStr(*FArgs[1]))));
						return ;
				}


				if(ss=="ISNULL") {
					PushValue (new StackValue(FArgs[1]->IsNull()));
					return;
				}

				if(ss=="NULL"){
						PushValue (new StackValue());
						return;
				}

				if(ss=="ISDATE"){
							PushValue (new StackValue(FArgs[1]->type==stDate));
							return;
				}

				if(ss=="TRIM"){
							PushValue (new StackValue(CStr(*FArgs[1]).Trim()));
							return;
				}


				if(ss=="NOT"){
							PushValue (new StackValue((CCur(*FArgs[1])==0.0)));
							return;
				}

				if(ss=="STRORNULL"){
							if (FArgs[1]->IsNull()) PushValue(new StackValue("NULL"));
							PushValue ( new StackValue(("'") + CStr(*FArgs[1]) +("'")));
							return;
				}

				if(ss=="IIF"){
							if(CCur(*FArgs[1])==0.0){
								 PushValue(new StackValue(FArgs[3]));
							}else{
								 PushValue(new StackValue(FArgs[3]));
							}
							return;
				}

				if(ss== "MID"){
							PushValue (new StackValue(Mid(CStr(*FArgs[1]), CInt(*FArgs[2]), CInt(*FArgs[3]))));
							return;
				}

				if(ss== "LEFT"){
							PushValue (new StackValue(Left(CStr(*FArgs[1]), CInt(*FArgs[2]))));
							return;
				}

				if(ss== "RIGHT"){
							PushValue (new StackValue(Right(CStr(*FArgs[1]), CInt(*FArgs[2]))));
							return;
				}

				if(ss== "ASC"){
							PushValue (new StackValue((FArgs[1]->ToStr().c_str())[0]));
							return;
				}
				if(ss=="CHR"){
							char c =(char)FArgs[1]->ToNumeric();
							PushValue (new StackValue(AnsiString(&c)));
							return;
				}

				if(ss=="ABS"){
							double ddd=CCur(*FArgs[1]);
							if(ddd<0) ddd=-ddd;
							PushValue(new StackValue(ddd));
                            return;
				}


				if(ss=="MSGBOX"){
							 MessageBox(0,FArgs[1]->ToStr().c_str(),	// address of text in message box
														FArgs[2]->ToStr().c_str(),	// address of title of message box
														MB_OK 	// style of message box
							 );
							 return;
				}

				if(ss=="YNBOX"){
							 PushValue(new StackValue(MessageBox(0,FArgs[1]->ToStr().c_str(),	// address of text in message box
																FArgs[2]->ToStr().c_str(),	// address of title of message box
																MB_YESNO 	// style of message box
							 )==IDYES));
							 return;
				}

				ProcedureUnit *pu = Units.Get(ss.c_str());
				if(pu){
					 ExecUnit(pu,&FArgs);
                     return;
				}else
					if(UserFunction)
						 if (UserFunction(ss,  &FArgs)) return;
 			    MessageBox(0, "Неизвестная функция", ss.c_str(), MB_OK );

		 } else {
					MessageBox(0, "Неверное количество аргументов", ss.c_str(), MB_OK );
		 }
	} else {
		MessageBox(0, "Ожидалось имя функции", ss.c_str() ,MB_OK );
	}

};

void MiniParser::token_Add(AnsiString s, tokenType t,long i,int argCnt){
		token.Add(new Token(UCase(s),t,i,argCnt));
};


void MiniParser::AddUserFunction(AnsiString FuncName, int ArgCount){
	token_Add( FuncName, ttFunc,  0, ArgCount);
};


void MiniParser::Init(){
	Vars.Clear();
	token.Clear();
	Searcher.Clear();
	Searcher.Push(&Vars);
	opStack.Clear();
	varStack.Clear();
	RetStack.Clear();
	Units.Clear();

	token_Add( "", ttVarConst, 0);

	token_Add( "*/", ttRemEnd, 1);
	token_Add( "/*", ttRem, 1);

	// логические операции
	token_Add( "==", ttLogical2, 1);
	token_Add( "!=", ttLogical2, 1);
	token_Add( ">=", ttLogical2, 1);
	token_Add( "<=", ttLogical2, 1);
	token_Add( ">", ttLogical2, 1);
	token_Add( "<", ttLogical2, 1);

	token_Add( "+=", ttSet, 1);
	token_Add( "-=", ttSet, 1);
	token_Add( "*=", ttSet, 1);
	token_Add( "/=", ttSet, 1);
	token_Add( "%=", ttSet, 1);
	token_Add( "&=", ttSet, 1);
	token_Add( "=", ttSet, 1 );
	token_Add( ";", ttSetEnd, 1);

	token_Add( "--", ttUnary, 1);
	token_Add( "++", ttUnary, 1);

	// произведения
	token_Add( "*", ttMult, 1);
	token_Add( "/", ttMult, 1);
	token_Add( "%", ttMult, 1);

	// суммирование
	token_Add( "+", ttArifmetic2, 1);
	token_Add( "-", ttArifmetic2, 1);
	token_Add( "&", ttArifmetic2, 1);

	//скобки
	token_Add( "(", ttLeftBrace, 1);
	token_Add( ")", ttRightBrace, 1);

	// вспомогательные для функций лексемы
	token_Add( "{", ttFLeftBrace, 1);
	token_Add( "}", ttFRightBrace, 1);
	token_Add( ",", ttComa, 1);



	// space tokens
	token_Add( " ", ttSpace, 1);
	token_Add( "\r\n", ttSpace, 1);
	token_Add( "\r", ttSpace, 1);
	token_Add( "\n", ttSpace, 1);
	token_Add( "\t", ttSpace, 1);

	// control statements
	token_Add( ":", ttLabel, 1);

	// операции
	token_Add( "or", ttLogicalAdd, 0);
	token_Add( "xor", ttLogicalAdd, 0);
	token_Add( "and", ttLogicalMult, 0);

	// функции

	// конверсия типов
	token_Add( "CDate", ttFunc, 0, 1);
	token_Add( "CStr", ttFunc, 0, 1);
	token_Add( "CDbl", ttFunc, 0, 1);
   	token_Add( "CSQL", ttFunc, 0, 1);

	// даты и раьота с ними
	token_Add( "Date", ttFunc, 0, 0);
	token_Add( "Now", ttFunc, 0, 0);
	token_Add( "Time", ttFunc, 0, 0);
	token_Add( "Year", ttFunc, 0, 1);
	token_Add( "Day", ttFunc, 0, 1);
	token_Add( "Month", ttFunc, 0, 1);
	token_Add( "Hour", ttFunc, 0, 1);
	token_Add( "Minute", ttFunc, 0, 1);
	token_Add( "Second", ttFunc, 0, 1);
	token_Add( "MakeDate", ttFunc, 0, 3);
	token_Add( "MakeTime", ttFunc, 0, 3);
	token_Add( "MakeDateTime", ttFunc, 0, 6);
	token_Add( "DDMMYYYY", ttFunc, 0, 1);
	token_Add( "DDMMYY", ttFunc, 0, 1);
	token_Add( "TODATE", ttFunc, 0, 1);
	token_Add( "IsDate", ttFunc, 0, 1);

	// всяко - разно
	token_Add( "IsNull", ttFunc, 0, 1);
	token_Add( "iif", ttFunc, 0, 3);
	token_Add( "NULL", ttFunc, 0, 0);
	token_Add( "Not", ttFunc, 0, 1);
	token_Add( "Abs", ttFunc, 0, 1);
	token_Add( "MSGBOX", ttFunc, 0, 2);
	token_Add( "YNBOX", ttFunc, 0, 2);
	token_Add( "PUSH", ttFunc, 0, 1);
	token_Add( "DEF", ttFunc, 0, 1);
	token_Add( "GLOBAL", ttFunc, 0, 1);
	token_Add( "EXEC", ttFunc, 0, 1);

	// массивы
	token_Add( "GET", ttFunc, 0, 2);       // array,index --> value
	token_Add( "SET", ttFunc, 0, 3);       // array,index,value

	//строки
	token_Add( "Len", ttFunc, 0, 1);
	token_Add( "Trim", ttFunc, 0, 1);
	token_Add( "StrOrNull", ttFunc, 0, 1);
	token_Add( "Left", ttFunc, 0, 2);
	token_Add( "Right", ttFunc, 0, 2);
	token_Add( "Asc", ttFunc, 0, 1);
	token_Add( "Chr", ttFunc, 0, 1);
	token_Add( "MID", ttFunc, 0, 3);
	token_Add( "STRSTRIP", ttFunc, 0, 1);


	////////////////////////////////////////////////
	// работа с процедурами
	token_Add( "PROC", ttProc, 0);
	token_Add( "BEGIN", ttBegin, 0);
	token_Add( "ENDP", ttEndProc, 0);
	token_Add( "FORGET", ttForgetEnd, 0,0);



	// управляющи конструкции
	token_Add( "IF", ttIF, 0);
	token_Add( "THEN", ttTHEN, 0);
	token_Add( "ELSE", ttELSE, 0);
	token_Add( "ENDIF", ttEndIF, 0);
	token_Add( "GOTO", ttGoto, 0);
	token_Add( "GOSUB", ttGosub, 0);
	token_Add( "END", ttEnd, 0);
	token_Add( "RETURN", ttReturn, 0);
	PostInit();
	MakeDelimiters(true);
};


void * MiniParser::SetVFV  (bool (*VFV)(AnsiString Name, StackValue & Value)){
		void *ptr =ValueFromVar;
		ValueFromVar=VFV;
		return ptr;
};

void * MiniParser::SetSVTV (bool (*SVTV)(AnsiString Name, StackValue & Value)){
		void *ptr =SetValueToVar;
		SetValueToVar=SVTV;
		return ptr;
};

void * MiniParser::SetUF   (bool (*UF)(AnsiString Name,  MyArray<StackValue>* FArgs)){
		void *ptr =UserFunction;
		UserFunction=UF;
		return ptr;
};


void  MiniParser::ExecUnit(ProcedureUnit *pu,  MyArray<StackValue>* FArgs){

	// clear local vars
	pu->Vars.Clear();

	// load arguments to array pu->Vars
	// load ArgumentValues
	for(long i=1;i<FArgs->Count() && i<=pu->Args.Count() ;i++){
		ValueStorage *vs=new ValueStorage();
		vs->Set(new StackValue(FArgs->Get(i)));
		pu->Vars.Add(vs,0,pu->Args[i-1]->c_str());
	}
	// push LocVars
	Searcher.Push(&pu->Vars);

	// exec unit
	ExecExpression(pu->TV,pu->Label);

	// pop LocVars
	Searcher.Pop();
};

#define GetClass(IDX) 		if(IDX<0 || IDX>=token.Count()) \
										Class=ttVarConst; \
										else               \
										Class = token[IDX]->Class;

void  MiniParser::CompileExpression(AnsiString Ex,TokenList *TV, Labels *Label ){
	long /*ifCnt,*/IDX,  i,Check[9];
	AnsiString nt;
	tokenType Class ;
	//long *o1;  StackValue *v1, *v2;
	i = 1;

	Label->Clear();

	// первый проход
	// Создаем список меток
	// проверяем синтаксис выражения
	Check[1]=Check[2]=Check[3]=Check[4]=Check[5]=Check[6]=Check[7]=Check[8]=0;
	while ( i <= Len(Ex)){
		nt = NextToken(Ex, i, i, Class, IDX);
		//cout << endl <<nt.c_str();
		TV->Add(new TokenVal(nt,IDX));
		switch( Class){
			case ttRem:
			 {
				// убираем из скомпилированных строк '/*'
				TV->Destroy(TV->Count()-1);

				// пропускаем все до конца комментария
				// с учетом вложенных коментариев
				long RemCnt=1;
				while ( i <= Len(Ex)){
					nt = NextToken(Ex, i, i, Class, IDX);
					if(Class==ttRem) RemCnt++;
					if(Class==ttRemEnd) {
						RemCnt--;
						if(!RemCnt) break;
					}
				}
				if(Class!=ttRemEnd || RemCnt){
					 MessageBox(0, "Ожидался конец коментария", "ОШИБКА!",MB_OK);
					 goto ClearArray ;
				}
				}
				break;

		case ttLabel:
			Label->Add(NewLong(TV->Count()),0, (nt=NextToken(Ex, i, i, Class, IDX)).c_str());
			TV->Add(new TokenVal(nt,IDX));
			break;

		case ttSet:
			Check[1] = Check[1] + 1;
			if ( Check[1] > 1 ){
				 MessageBox(0, "Присвоение внутри оператора присваения", "ОШИБКА!",MB_OK);
				goto ClearArray ;
			}


			if ( Check[2] > 0 ){
				MessageBox(0, "Присвоение внутри выражения", "ОШИБКА!",MB_OK);
				goto ClearArray ;
			}

			if ( Check[3] > 0 ){
				 MessageBox (0, "Присвоение в вызове функции",  "ОШИБКА!",MB_OK);
				goto ClearArray ;
			}
			break;

		case ttSetEnd:
			Check[1] = Check[1] - 1;
			if ( Check[1] < 0 ){
				 MessageBox (0, "Лишний символ  (;)", "ОШИБКА!",MB_OK);
				goto ClearArray ;
			}
			break;

		case ttLeftBrace:
			Check[2] = Check[2] + 1;
			break;

		case ttUnary:
			if ( Check[1] > 0 ){
				 MessageBox (0, "Операция ++ или -- в операторе присваения", "ОШИБКА!",MB_OK);
				goto ClearArray ;
			}

			if ( Check[2] > 0 ){
				 MessageBox (0, "Операция ++ или -- внутри выражения", "ОШИБКА!",MB_OK);
				goto ClearArray ;
			}

			if ( Check[3] > 0 ){
				 MessageBox (0, "Операция ++ или -- внутри вызова фнкции","ОШИБКА!",MB_OK);
				goto ClearArray ;
			}
			break;

		case ttRightBrace:
			Check[2] = Check[2] - 1;
			if ( Check[2] < 0 ){
				 MessageBox (0, "Лишняя закрывающая скобка в выражении","ОШИБКА!",MB_OK);
				goto ClearArray ;
			}
			break;

		case ttFLeftBrace:
			Check[3] = Check[3] + 1;
			break;

		case ttFRightBrace:
			Check[3] = Check[3] - 1;
			if ( Check[3] < 0 ){
				 MessageBox (0, "Лишняя закрывающая скобка в вызове функции","ОШИБКА!",MB_OK);
				goto ClearArray ;
			}
			break;

		 case ttBegin:
		 {
			long SaveI=i;
			AnsiString Expr;
			while ( i <= Len(Ex)){
				nt = NextToken(Ex, i, i, Class, IDX);
				switch( Class){
				 case ttRem:
						{
								// пропускаем все до конца комментария
								// с учетом вложенных коментариев
								long RemCnt=1;
								while ( i <= Len(Ex)){
									nt = NextToken(Ex, i, i, Class, IDX);
									if(Class==ttRem) RemCnt++;
									if(Class==ttRemEnd) {
										RemCnt--;
										if(!RemCnt) break;
									}
								}
								if(Class!=ttRemEnd || RemCnt){
									MessageBox(0, "Ожидался конец коментария", "ОШИБКА!",MB_OK);
									goto ClearArray ;
								}
						}
						break;

					case ttLabel:
						nt=NextToken(Ex, i, i, Class, IDX);
						break;
					case ttEndProc:
						 Expr=Mid(Ex,SaveI,i-SaveI);
						 goto ExprBuilt;
					}

			}
			MessageBox(0,"Ожидался символ ENDP","Ошибка!!!",MB_OK);
			goto ClearArray;

	ExprBuilt:
			// знаем что надо делать в процедуре

			ProcedureUnit *pu = new ProcedureUnit();
			AnsiString ProcName="";
			AnsiString ArgName="";

			long ttt;

			// хотим знать название и аргументы
			for(ttt=TV->Count()-1;ttt>=0;ttt--){
				if(!TV->Get(ttt))
					cout<<endl<<ttt;
				if(TV->Get(ttt)->i<0 || TV->Get(ttt)->i>=token.Count())
					cout<<endl<<ttt;
				switch(token[TV->Get(ttt)->i]->Class){
					case ttBegin:
						break;
					case ttComa:
						pu->Args.Insert(new AnsiString(UCase(ProcName)),0);
						ArgName="";
						ProcName="";
						break;

					case ttVarConst:
						ArgName = ProcName;
						ProcName=TV->Get(ttt)->v;
						ProcName=UCase(ProcName);
						break;

					case ttProc:
						if(ArgName!=""){
							pu->Args.Insert(new AnsiString(UCase(ArgName)),0);
							ArgName="";
						}
						goto HeaderBuilt;

					default:
						MessageBox(0,"Неверный заголовок процедуры","Ошибка!!!",MB_OK);
						goto ClearArray;

				}

			}
			MessageBox(0,"Неверный заголовок процедуры","Ошибка!!!",MB_OK);
			goto ClearArray;


	 HeaderBuilt:
			while(TV->Count()-1>ttt){
				cout << TV->Get(TV->Count()-1)->v.c_str();
				TV->Destroy(TV->Count()-1);

			}


			// регистрируем процедуру
			AddUserFunction( ProcName, pu->Args.Count());

			// компилируем
			CompileExpression(Expr,&pu->TV,&pu->Label);

			// добавляем в библиотеку
			Units.Add(pu,0,ProcName.c_str());

		 }
		 break;

		 case ttIF:
			Check[4] = Check[4] + 1;
			break;

		 case ttEndIF:
			Check[5] = Check[5] + 1;
			if ( Check[5] > Check[4] ){
				 MessageBox (0, "ENDIF без IF","ОШИБКА!",MB_OK);
				goto ClearArray ;
			}
			break;

		 case ttTHEN:
			Check[6] = Check[6] + 1;
			if ( Check[6] > Check[4] ){
				 MessageBox (0, "THEN без IF","ОШИБКА!",MB_OK);
				goto ClearArray ;
			}
			break;

		 case ttELSE:
			Check[7] = Check[7] + 1;
			if ( Check[7] > Check[4] ){
				 MessageBox (0, "ELSE без IF","ОШИБКА!",MB_OK);
				goto ClearArray ;
			}

			if ( Check[7] > Check[6] ){
				 MessageBox (0, "ELSE без THEN", "ОШИБКА!",MB_OK);
				goto ClearArray ;
			}
			if ( Check[4] == Check[5] ){
				 MessageBox (0, "ELSE вне условного выражения","ОШИБКА!",MB_OK);
				goto ClearArray ;
			}
			break;
		}
	}

	if ( Check[1] != 0 ){
		 MessageBox (0, "Ошибка в операторе присваения","ОШИБКА!",MB_OK);
		goto ClearArray ;
	}

	if ( Check[2] != 0 ){
		 MessageBox (0, "Несоответствие скобок в выражении","ОШИБКА!",MB_OK);
		goto ClearArray ;
	}

	if ( Check[3] != 0 ){
		 MessageBox (0, "Несоответствие скобок в вызове функции","ОШИБКА!",MB_OK);
		goto ClearArray ;
	}

	if ( Check[4] != Check[6] || Check[4] != Check[5] ){
		 MessageBox (0, "Несоответствие IF - THEN - ENDIF","ОШИБКА!",MB_OK);
		goto ClearArray ;
	}
	return;

ClearArray:
	TV->Clear();
	Label->Clear();

};



AnsiString   MiniParser::ExecExpression(TokenList &TV, Labels &Label){
	long ifCnt,IDX,  i;//,Check[9];
	AnsiString nt,PE;
	tokenType Class ;
	long *o1;  StackValue *v1, *v2;
	i = 1;

	PE = "";

	// второй проход разбор выражений
	i = -1;
	if ( tvLen > 0 ){
	endfind:
	endfind2:

	while ( i < tvLen-1){ //Len(ex)

		i = i + 1;
		nt = TV[i]->v;
		IDX = TV[i]->i;
		GetClass(IDX);		//cout << endl << nt.c_str();

		switch( Class){

		case ttLogical2:
			Add_Mult_EQ();
			PushOperation (NewLong(IDX));

			break;

		case ttArifmetic2:
			//вытесняем произведения и суммирования
			Add_Mult();
			PushOperation (NewLong(IDX));
			break;

		case ttLogicalAdd:
    	//вытесняем логические операции
			Log_Add_Mult();
     PushOperation (NewLong(IDX));
			break;

		case ttLogicalMult:
    	//вытесняем логический AND
     Log_Mult();
     PushOperation (NewLong(IDX));
			break;

		case ttFLeftBrace:
			PushOperation (NewLong(IDX));
			break;

		case ttFunc:
			PushOperation (NewLong(IDX));
			if ( token[IDX]->ArgCount == 0 ){
			 CallFunction();
			}
			break;
		case ttComa:
		case ttLeftBrace:
			PushOperation (NewLong(IDX));
			break;

		case ttFRightBrace:
			PushOperation (NewLong(IDX));
			CallFunction();
			break;

		case ttVarConst:
				PushValue (new StackValue(nt));

			break;

		case ttRightBrace:
			//вытесняем все операции до левой скобки
			Log_Add_Mult();


			// проверяем наличие левой скобки и выбрасываем ее со стека
			if ( token[*PopOperation()]->Class != ttLeftBrace ){
				 MessageBox (0, "Ошибка НЕСООТВЕТСТВИЕ СКОБОК","",MB_OK);
				return ( PE );
			}
			break;
		case ttMult:
			// вытесняем произведения
			Mult();
			PushOperation (NewLong(IDX));
			break;

		case ttSet:
			PushOperation (NewLong(IDX));
			break;

		case ttSetEnd:    //var [= | -= | += | *= | /= | %= | &= ] expr ;
			Log_Add_Mult();
			if ( varStack.Count() >= 2 && opStack.Count() >= 1 ){
				v1 = PopValue();
				v2 = PopValue();

				// снять операцию =
				o1 = PopOperation();
				if ( token[*o1]->Class != ttSet ){
					 MessageBox (0, "Ожидалось присвоение!","Ошибка",MB_OK);
					return ( PE );
				} else {
					SetVar (v2, &GetValue(v1), token[*o1]->S);
				}
			}
			break;
		case ttUnary:  // ++ --
				if ( varStack.Count() >= 1 ){
					v1 = PopValue();
					SetVar (v1, 0, token[IDX]->S);
				}
				break;
//''''''''''''''''''''''''''''''''''''''''''''''''''''
		 case ttLabel:
			i = i + 1;
			break;

		 case ttGoto:
			i = i + 1;
			nt = TV[i]->v;
			IDX = TV[i]->i;
			GetClass(IDX);
			{
			long *l1 = Label.Get(nt.c_str());
			if ( l1 ){
					i = *l1;
			} else {
				 MessageBox (0, "Метка не найдена ", nt.c_str(),MB_OK);
				return ( PE );
			}
			}
			break;

		 case ttGosub:
			i = i + 1;
			nt = TV[i]->v;
			IDX = TV[i]->i;
			GetClass(IDX);
			{
			long *l1 = Label.Get(nt.c_str());
			if ( l1 ){
					RetStack.push (NewLong(i));
					i = *l1;
			} else {
				 MessageBox (0, "Метка не найдена ", nt.c_str(),MB_OK);
				return ( PE );
			}
			}
			break;

		 case ttReturn:
				if ( RetStack.Count() > 0 ){
					i = *RetStack.pop();
				} else {
					 MessageBox(0, "RETURN без GOSUB", "Ошибка!!!",MB_OK);
					return ( PE );
				}
				break;

		case ttForgetEnd:
				Units.Clear();
		case ttEnd:
			return ( PE );

//'''''''''''''''''''''''''''' IF THEN ELSE ENDIF
		 case ttIF:
			PushOperation (NewLong(IDX));
			break;




		 case ttTHEN:
			Log_Add_Mult();
			if (opStack.Count()>0 ){
				o1=PopOperation();
				if (!token[*o1] || token[*o1]->Class != ttIF ){
					 MessageBox(0, "Ошибка обработки выражения", nt.c_str(),MB_OK);

				}
			}else{
				 MessageBox(0, "Ошибка обработки выражения", nt.c_str(),MB_OK);
				 return ( PE );
			}
			if ( CCur(*PopValue())==0.0 ){
				ifCnt = 0;
				// выбираем все до else (endif)

				while ( i < tvLen-1){
					i = i + 1;
					nt = TV[i]->v;
					//Class = TV[i]->t;
					IDX = TV[i]->i;
					GetClass(IDX);
					switch( Class){
						case ttIF:
							ifCnt = ifCnt + 1;
							break;
						case ttEndIF:
						case ttELSE:
							if ( ifCnt == 0 ){
								goto endfind;
							} else {
							 ifCnt = ifCnt - 1;
							}
							break;
					 case ttLabel:
							i = i + 1 ;
					}
				}
			}
			break;

//endfind:

		 case ttELSE:
		 // выбираем все до endif
				while ( i < tvLen-1){
					i = i + 1;
					nt = TV[i]->v;
					//Class = TV[i]->t;
					IDX = TV[i]->i;
					GetClass(IDX);
					switch( Class){
						case ttIF:
							ifCnt = ifCnt + 1;
							break;
						case ttEndIF:
							if ( ifCnt == 0 ){
								goto endfind2;
							} else {
							 ifCnt = ifCnt - 1;
							}
							break;
						 case ttLabel:
							i = i + 1;
					}
				}
				break;
//endfind2:

		case ttEndIF:
		 Log_Add_Mult();
//'''''''''''''''''''''''''''''' IF THEN ELSE ENDIF

		}
	}
	}


	return PE;

};

////////////////////////////////////////////////////////////////////////////////////////////

SimpleStream &operator << (SimpleStream& out_data, TokenVal & P){
	out_data  << P.v << P.i;
	return out_data;
};

SimpleStream &operator << (SimpleStream& out_data, Token & P){
	 out_data<<P.S;
	 out_data<<P.Class;
	 out_data<<P.ArgCount<<P.Delimiter;
	return out_data;
};

SimpleStream &operator << (SimpleStream& out_data, ProcedureUnit & P){
	out_data<<P.Args;
	out_data<<P.TV;
	out_data<<P.Label;
	return out_data;
};

SimpleStream &operator << (SimpleStream& out_data, MiniParser & P){
	out_data << P.Units;
	out_data<<P.token;
	out_data<<P.Vars;
	return out_data;
};

////////////////////////////////////////////////////////////////////////
SimpleStream &operator >> (SimpleStream& out_data, TokenVal & P){
	out_data  >> P.v >> P.i;
	return out_data;
};

SimpleStream &operator >> (SimpleStream& out_data, Token & P){
	out_data>>P.S;
	out_data>>P.Class;
	out_data>>P.ArgCount>>P.Delimiter;
	return out_data;
};

SimpleStream &operator >> (SimpleStream& out_data, ProcedureUnit & P){
	long cnt;
	out_data >> P.Args;
	out_data >> P.TV;
	out_data >> P.Label;
	return out_data;
};

SimpleStream &operator >> (SimpleStream& out_data, MiniParser & P){
	P.Init();
	out_data >> P.Units;
	out_data>>P.token;
	out_data>>P.Vars;
	P.MakeDelimiters(true);
	return out_data;
};


SimpleStream &operator >> (SimpleStream& out_data, tokenType & P){
	int pp;
	out_data >> pp;
	P = (tokenType) pp;
	return out_data;
};
SimpleStream &operator << (SimpleStream& out_data, tokenType & P){
	int pp=P;
	out_data << pp;
	return out_data;
};


SimpleStream &operator >> (SimpleStream& out_data, StackType & P){
	int pp;
	out_data >> pp;
	P = (StackType) pp;
	return out_data;
};
SimpleStream &operator << (SimpleStream& out_data, StackType & P){
	int pp=P;
	out_data << pp;
	return out_data;
};

SimpleStream &operator >> (SimpleStream& out_data, StackValue & P){
	double d;
	out_data >> P.type>>P.s>>P.n>>d;
		P.d=d;
	return out_data;
};

SimpleStream &operator << (SimpleStream& out_data, StackValue & P){
	out_data << P.type<<P.s<<P.n<<(double)P.d;
	return out_data;
};

SimpleStream &operator >> (SimpleStream& out_data, ValueStorage & P){
	for(long i=0;i<P.Dimention();i++){
	 ifdel(P.Data[i]);
	}
	ifdela(P.Data);

	out_data>>P.MaxDimention;
	P.Data = new StackValue*[P.MaxDimention];
	for(long i=0;i<P.Dimention();i++){
		P.Data[i] = new StackValue();
		out_data>>*P.Data[i];
	}
	return out_data;
};
SimpleStream &operator << (SimpleStream& out_data, ValueStorage & P){
	out_data << P.Dimention();
	for(long i=0;i<P.Dimention();i++){
		out_data<<*P.Data[i];
	}
	return out_data;
};


bool ParamParser::SetVariable (const AnsiString &Name, const StackValue &Value){
 if(!P) return false;
 StackValue * ptr=P->Get(Name);
 if(!ptr) return false;
 *ptr=Value;
 return true;
};

bool ParamParser::GetVariable (const AnsiString &Name, StackValue &Value){
 if(!P) return false;
 StackValue * ptr=P->Get(Name);
 if(!ptr) return false;
 Value=*ptr;
 return true;
};

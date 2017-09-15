#ifndef MINIPARSER_H
#define MINIPARSER_H

enum tokenType {
	ttSpace = 0 ,
	ttLogical2  ,
	ttArifmetic2,
	ttMult      ,
	ttLeftBrace ,
	ttRightBrace,
	ttSet       ,
	ttSetEnd    ,
	ttFunc      ,
	ttFLeftBrace,
	ttFRightBrace,
	ttComa       ,
	ttVarConst   ,
	ttIF         ,
	ttTHEN       ,
	ttELSE       ,
	ttEndIF      ,
	ttLabel      ,
	ttGoto       ,
	ttGosub      ,
	ttReturn     ,
	ttEnd        ,
	ttLogicalAdd ,
	ttLogicalMult,
	ttUnary      ,
	ttRem        ,
	ttRemEnd		 ,
	ttProc			 ,
	ttBegin			 ,
	ttEndProc		 ,
	ttForgetEnd
};


/////////////////////////////////////////////////////////////////////////////////////////
struct TokenVal {
	AnsiString v;  // строка из исходного текста
	long i;        // индекс в массиве
	TokenVal(AnsiString _v="",long _i=0) { v=_v; i=_i;};
};

struct Token{
	AnsiString S;      // картинка
	tokenType Class;   // класс
	long ArgCount;     // количество аргументов
	int Delimiter;     // разделитель или нет
	Token(	AnsiString _S="",
						tokenType _Class=ttSpace ,
						int _Delimiter=0,
						long _ArgCount=0){
				S=_S.UpperCase();
				Class=_Class;
				ArgCount=_ArgCount;
				Delimiter=_Delimiter;
		};
};

struct ptrToken{
	AnsiString *S;     // картинка
	tokenType Class;   // класс
	long ArgCount;     // количество аргументов
	int Delimiter;     // разделитель или нет
	ptrToken( AnsiString *_S,
						tokenType _Class=ttSpace ,
						int _Delimiter=0,
						long _ArgCount=0){
				S=_S;
				Class=_Class;
				ArgCount=_ArgCount;
				Delimiter=_Delimiter;
		};
};



typedef MyList<long> Labels;
typedef MyArray<Token> Tokens;


class ValueStorage{

public:

	long MaxDimention;
	StackValue** Data;


	long Dimention() { return MaxDimention;};
	ValueStorage(long _MaxDimention=1){
		if(_MaxDimention<=0) MaxDimention=1; else MaxDimention = _MaxDimention;
		Data = new StackValue*[MaxDimention];
		for(long i=0;i<MaxDimention;i++)
			Data[i]=new StackValue();
	};

	void Set(StackValue * ptr,long Index =0){
		if(Index>=0 && Index<MaxDimention){
			ifdel(Data[Index]);
			Data[Index]=ptr;
		}
	};

	StackValue* Get(long Index=0){
		if(Index>=0 && Index<MaxDimention)
			return Data[Index];
		else
			return 0;
	};

	~ValueStorage(){
		for(long i=0;i<MaxDimention;i++){
			ifdel(Data[i]);
		}
		ifdela(Data);
	};

};

typedef MyList<ValueStorage> VarList;

class VarSearcher {
	 MyArray<VarList> queue;
	public:
	 VarSearcher(){queue.DeleteOrder(false);};
	 ValueStorage * Find(AnsiString Name){
	 ValueStorage * ptr;
	 AnsiString UName=Name.UpperCase();
		for(long i=queue.Count()-1;i>=0;i--){
			ptr =queue[i]->Get(UName.c_str());
			if(ptr) return ptr;
		}
		return 0;
	 };
	 void Push( VarList * NewLocalsLevel){ queue.Add(NewLocalsLevel);};
	 void Pop(){if(queue.Count()) queue.Destroy(queue.Count()-1);};
	 void Clear(){queue.Clear();};

	 VarList *GetCurrent(){
		 if(queue.Count()>0)
			return queue[queue.Count()-1];
		 else
			return 0;
	 };

};


typedef MyArray<TokenVal> TokenList;

class ProcedureUnit{
 public:
		Labels Label;
		TokenList TV;
		VarList Vars;
		MyArray<AnsiString> Args;
};



typedef MyList< ProcedureUnit> PUnits;
//template PUnits;

class MiniParser{

		MyArray<ptrToken> Delimiters;
		MyArray<ptrToken> Words;

	protected:
		MyStack<long> opStack;
		MyStack<StackValue> varStack;
		MyStack<long> RetStack;
		VarSearcher Searcher;


		bool (*ValueFromVar)(AnsiString Name, StackValue & Value);
		bool (*SetValueToVar)(AnsiString Name, StackValue & Value);
		bool (*UserFunction)(AnsiString Name,  MyArray<StackValue>* FArgs);


		// реализация операций
		void Log_Add_Mult();
		void Add_Mult();
		void Mult();
		void Log_Add();
		void Log_Mult();
		void Add_Mult_EQ();

		// исполнение функций
		void CallFunction();
		virtual bool RunFunction(const AnsiString &Name,  MyArray<StackValue>* FArgs){return false;};

		// работа с переменными
		void  SetVar (StackValue* Name, StackValue *Value, AnsiString Token);
		virtual bool SetVariable (const AnsiString &Name, const StackValue &Value){return false;};

		StackValue GetValue(StackValue *v);
		virtual bool GetVariable (const AnsiString &Name, StackValue &Storage){return false;};

		// работа со стеками
		void PushValue(StackValue *v);
		void PushValue(StackValue v);
		StackValue *PopValue();
		void PushOperation(long *op);
		long * PopOperation();

		AnsiString NextToken(AnsiString S, long start, long & nextChar,tokenType & tokenClass ,long & TokenIdx);
		void token_Add(AnsiString s, tokenType t,long i,int argCnt=0);

		AnsiString   ExecExpression(TokenList &t, Labels &l);                        //*
		void  ExecUnit(ProcedureUnit *pu,  MyArray<StackValue>* FArgs);

		ProcedureUnit  * AddUnit(AnsiString UnitName);                     //*

	public:
		void  CompileExpression(AnsiString Ex,TokenList *t, Labels *l  );

		Tokens token;
		PUnits Units;
		VarList Vars;

		// сбросить в исходное состояние
		void Init();
		virtual void PostInit(){};
		void MakeDelimiters( bool rebuild);

		MiniParser(){
					Init();
					ValueFromVar=0;
					SetValueToVar=0;
					UserFunction=0;
		};


		// главная функция
		AnsiString   ParseExpression(AnsiString Ex  );
		AnsiString   ParseExpression(TokenList *t, Labels *l );


		// функции для установки расширения
		void AddUserFunction(AnsiString FuncName, int argCnt = 0);
		void * SetVFV  (bool (*VFV)(AnsiString Name, StackValue & Value)=0);
		void * SetSVTV (bool (*SVTV)(AnsiString Name, StackValue & Value)=0);
		void * SetUF   (bool (*UF)(AnsiString Name,  MyArray<StackValue>* FArgs)=0);
};

// << >>
///////////////////////////////////////////////////////////////////////////
SimpleStream &operator << (SimpleStream& out_data, TokenVal & P);
SimpleStream &operator << (SimpleStream& out_data, TokenList & P);
SimpleStream &operator << (SimpleStream& out_data, Token & P);
SimpleStream &operator << (SimpleStream& out_data, Tokens & P);
SimpleStream &operator << (SimpleStream& out_data, Labels & P);
SimpleStream &operator << (SimpleStream& out_data, ProcedureUnit & P);
SimpleStream &operator << (SimpleStream& out_data, MiniParser & P);


SimpleStream &operator >> (SimpleStream& out_data, Token & P);
SimpleStream &operator >> (SimpleStream& out_data, Tokens & P);
SimpleStream &operator >> (SimpleStream& out_data, TokenVal & P);
SimpleStream &operator >> (SimpleStream& out_data, TokenList & P);
SimpleStream &operator >> (SimpleStream& out_data, Labels & P);
SimpleStream &operator >> (SimpleStream& out_data, ProcedureUnit & P);
SimpleStream &operator >> (SimpleStream& out_data, MiniParser & P);


SimpleStream &operator >> (SimpleStream& out_data, tokenType & P);
SimpleStream &operator << (SimpleStream& out_data, tokenType & P);

SimpleStream &operator >> (SimpleStream& out_data, StackType & P);
SimpleStream &operator << (SimpleStream& out_data, StackType & P);


SimpleStream &operator >> (SimpleStream& out_data, StackValue & P);
SimpleStream &operator << (SimpleStream& out_data, StackValue & P);

SimpleStream &operator >> (SimpleStream& out_data, ValueStorage & P);
SimpleStream &operator << (SimpleStream& out_data, ValueStorage & P);

SimpleStream &operator >> (SimpleStream& out_data, VarList & P);
SimpleStream &operator << (SimpleStream& out_data, VarList & P);


///////////////////////////////////////////////////////////////////////////
class Params;

class ParamParser: public MiniParser {
   Params *P;
		virtual bool SetVariable (const AnsiString &Name, const StackValue &Value);
		virtual bool GetVariable (const AnsiString &Name, StackValue &Storage);

	public:
		void SetParams(Params *_P){P=_P;};
		ParamParser():MiniParser(){
    P=0;
		};
};



#endif

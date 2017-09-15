#ifndef FIELDCONT_H
#define FIELDCONT_H
#pragma pack(push,1)

unsigned short CorrectYear(unsigned short Year);

union prolog fieldBuf{
		char 				    *fChar;
		double 			    *fDouble;
		odbcTimeStamp   *fTimeStamp;
		unsigned char   *fBinary;
		void   			    *fPtr;

		/*
		char 				    *fTiny;
		unsigned char 	*fUTiny;
		odbcTime        *fTime;
		odbcDate        *fDate;
		short 				  *fShort;
		unsigned short 	*fUShort;
		long 				    *fLong;
		unsigned long 	*fULong;
		float 				  *fFloat;
		*/
};

class StackValue;

class prolog fieldCont
{
	 static char StaticBuffer1024[1024+1];

	public:
    //fieldCont *Clone(){return new fieldCont;};
		fieldBuf F;
		bool Null;
		long ctype;
		long len;
		void LoadValue(const fieldCont & f,bool ContentOnly=true);
		void SaveValue(fieldCont &f,bool Allocate =true,bool ContentOnly = true) const;
		void Free();
		void Clear();
		void Allocate(long _ctype,long _len=0);
		void LoadFromString(char * buf);
		char * printValue(char * buf=StaticBuffer1024);
		~fieldCont();
		fieldCont(){
			F.fPtr=0;
			ctype=0;
			len=-1;
			Null=true;
		};
		fieldCont(const fieldCont & c);
		fieldCont *Clone(){
			return new fieldCont(*this);
		};

		bool IsNumeric();
		bool IsDate();
		bool IsString();
		bool IsBinary();
		bool IsNull(){return Null;};

		// преобразования
		fieldCont * ToNumeric();
		/*fieldCont * ToString();
		fieldCont * ToDate();
		*/

		// задание значений
		void SetNull(){Free();Null=true;};
		/*
		void SetNumeric(double n);
		void SetString(const char *s);
		void SetBinary(const unsigned char *b, long l);
		void SetDateTime(const odbcTimeStamp & dt );

		// получение значений
		double Numeric();
		char *  String();
		odbcTimeStamp DateTime();
		void * Binary();

		// Установка данных
		void Now();
		void Date();
		*/

		StackValue _StackValue();

};



enum FilterCondition{
	fcNone=0,
	fcEQ,
	fcNE,
	fcLT,
	fcLE,
	fcGT,
	fcGE,
	fcBETWEEN,
	fcLikeLeft,
	fcLikeCenter,
	fcLikeRight,
	fcIsNull,
	fcIsNotNull

	//,fcIn
	//,fcNotIn
};


enum StackType{
		stNull    = -1,
		stString  = 0,
		stDate    = 1,
		stNumeric = 2
};


SimpleStream & operator >> (SimpleStream & s,StackType &ft);
SimpleStream & operator << (SimpleStream & s,StackType &ft);

class StackValue{
	public:
		StackType type;
		bool NullNumber;
		double n;
		AnsiString s;
		TDateTime d;

		void SetN( double _n){n=_n; NullNumber=false;};


		void Clear(){
			s="";
			n=0;
			d=0.0;
			type = stNull;
			NullNumber=false;
		};

		void SetToNull(){Clear();};

		StackValue(){
			Clear();
		};


		StackValue(const StackValue & x ){
				type=x.type;
				NullNumber=x.NullNumber;
				s=x.s;
				d=x.d;
				n=x.n;
		};

		StackValue(const StackValue * x ){
				type=x->type;
				NullNumber=x->NullNumber;
				s=x->s;
				d=x->d;
				n=x->n;
		};

		StackValue(AnsiString S){
				Clear();
				type=stString;
				s=S;

		};

		StackValue(TDateTime  D){
				Clear();
				type=stDate;
				d=D;

		};
		StackValue(double  N){
				Clear();
				type=stNumeric;
				n=N;
				NullNumber=false;

		};


		void	Set(StackType _type,AnsiString src){
				Clear();
				type=_type;
				try{
				switch(_type){
				case stString:
						s=src;
						break;
				case stNumeric:
						{

						StackValue zz(src);
						if(zz.IsNull() || src.UpperCase()=="NULL")
							NullNumber=true;
						else
							n=zz.ToNumeric();
						}
						break;
				case stDate:
					 {
					 StackValue zz(src);
					 d=zz.ToDate();
					 }
					 break;
				};
				}catch(...){
				}
		};

	StackValue(StackType _type,AnsiString src){
			Set(_type,src);
	};

	void Erase();
	AnsiString ToStr() const;
	AnsiString ToSQLStr() const;
	TDateTime ToDate() const;
	double ToNumeric() const;
	bool IsNull() const;
	bool IsDate() const;
	bool IsNumeric() const;

	StackValue & operator =(const StackValue & x);
	bool operator ==(const StackValue & x);
	fieldCont _fieldCont();

	bool LessThen(const StackValue & x) const;
	bool IsEqual(const StackValue & x) const;
	bool Filter(FilterCondition Condition, const StackValue & From, const StackValue & To) const;
	friend SimpleStream & operator >> (SimpleStream & s,StackValue &ft);
	friend SimpleStream & operator << (SimpleStream & s,StackValue &ft);


};


struct DynaStackValue{
	StackValue *ptr;
	bool AllowDelete;
	void Set(	StackValue *_ptr,bool _AllowDelete);

	DynaStackValue(){
		AllowDelete=false;
		ptr=0;
	};

	DynaStackValue(	StackValue *_ptr,bool _AllowDelete){
		ptr=_ptr;
		AllowDelete=_AllowDelete;
	};

	~DynaStackValue(){
		if(AllowDelete && ptr){
			delete ptr;
		}
	};
};

typedef MyArray<DynaStackValue> DynaStackValues;
typedef MyArray<DynaStackValues> DSVMatrix;


#pragma pack(pop)
#endif
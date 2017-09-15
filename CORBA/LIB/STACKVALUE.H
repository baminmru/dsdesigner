#ifndef STACKVALUE_H
#define STACKVALUE_H
#pragma pack(push,1)
#include "macro.h"
#include "template.h"
#include "mystream.h"

unsigned short CorrectYear(unsigned short Year);

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
		stNumeric = 2,
    stBinary  = 3
};


SimpleStream & operator >> (SimpleStream & s,StackType &ft);
SimpleStream & operator << (SimpleStream & s,StackType &ft);

class StackValue{
  virtual AnsiString BinarySQLString() const{return "NULL";}
  virtual AnsiString BinaryString()const {return "(binary)";}
	public:
		StackType type;
		bool NullNumber;
		double n;
		AnsiString s;
		TDateTime d;
    DataBuffer b;

		void Clear(){
			s="";
			n=0;
			d=0.0;
      b.Clear();
			type = stNull;
			NullNumber=false;
		};


		void SetN( double _n){Clear(); n=_n; NullNumber=false;};



		void SetToNull(){
      Clear();
    };

    void Set(const StackValue & x){
    	type=x.type;
    	s=x.s;
    	d=x.d;
    	n=x.n;
      b.SetBuffer(x.b.Length(),x.b.GetBuffer());
    	NullNumber=x.NullNumber;
    };

    void Set(const StackValue * x){
    	type=x->type;
    	s=x->s;
    	d=x->d;
    	n=x->n;
      b.SetBuffer(x->b.Length(),x->b.GetBuffer());
    	NullNumber=x->NullNumber;
    };

		void Set(TDateTime  D){
				Clear();
				type=stDate;
				d=D;
		};
		void Set(double  N){
				Clear();
				type=stNumeric;
				n=N;
				NullNumber=false;
		};

		void Set(DataBuffer * B){
				Clear();
				type=stBinary;
        if(B)
 				  b.SetBuffer(B->Length(),B->GetBuffer());
		};

    void Set(AnsiString S){
				Clear();
				type=stString;
				s=S;
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
				case stBinary:
					 {
					 b.SetBuffer(src.Length(),src.c_str());
					 }
					 break;

				};
				}catch(...){
				}
		};

	  StackValue(StackType _type,AnsiString src){
			Set(_type,src);
	  };



  	StackValue(){
			Clear();
		};


		StackValue(const StackValue & x ){
				Set(&x);
		};

		StackValue(const StackValue * x ){
        Set(x);
    };

		StackValue(AnsiString S){
    		Set(S);
		};

		StackValue(TDateTime  D){
		  Set(D);
		};
		StackValue(double  N){
      Set(N);
		};

		StackValue(DataBuffer * B){
      Set(B);
		};


	void Erase();
  void SetNull(){
    Erase();
  };
	AnsiString ToStr() const;
	AnsiString ToSQLStr() const;
	TDateTime ToDate() const;
	double ToNumeric() const;
	bool IsNull() const;
	bool IsDate() const;
	bool IsNumeric() const;
	bool IsBinary() const;

	StackValue & operator =(const StackValue & x);
	friend bool operator ==(const StackValue & y,const StackValue & x);
	friend bool operator <(const StackValue & y,const StackValue & x);

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
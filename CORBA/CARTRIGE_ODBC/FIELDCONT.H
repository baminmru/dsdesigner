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

		StackValue* _StackValue();

};



//typedef MyArray<DynaStackValue> DynaStackValues;
//typedef MyArray<DynaStackValues> DSVMatrix;

#pragma pack(pop)
#endif
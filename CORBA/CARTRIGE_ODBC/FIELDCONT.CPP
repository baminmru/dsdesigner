#include "CARTRIGEINI.h"


unsigned short CorrectYear(unsigned short Year){
	TDateTime dd =TDateTime::CurrentDate();
	unsigned short year,month,day;
	dd.DecodeDate(& year, & month, & day);
		if(Year<1899){
			return year/100*100+Year%100;
		}
		return Year;
};


// fieldCont ***********************************************************************
void  fieldCont::Allocate(long cType,long Len){
		 Free();
		 long realLen = Len;
			 ctype=cType;
		 memset(&F,0,sizeof(F));
		 switch(cType){
			 case SQL_C_CHAR:
							//if (realLen >8000 || realLen <0) realLen = 8000;
						 if (realLen >MAXMEMO || realLen <0) realLen = MAXMEMO;
						 realLen++;
						 F.fChar = new char[realLen+1];
					break;
			 case SQL_C_BINARY:
							if (realLen >MAXMEMO || realLen <0) realLen = MAXMEMO;
							 F.fBinary = new unsigned  char[realLen];
					break;
			 /*
			 case SQL_C_FLOAT:
							 F.fFloat = new float;
					realLen=4;
					break;
				*/
				case SQL_C_DOUBLE	:
							 F.fDouble = new double;
					realLen=8;
					break;
				/*
				case	SQL_C_STINYINT:
							 F.fTiny = new char;
					realLen=1;
					break;
				case SQL_C_SSHORT:
								F.fShort = new short;
					 realLen=2;
					 break;
				case SQL_C_SLONG:
								F.fLong = new long;
					 realLen=4;
					 break;
				case SQL_C_DATE:
								F.fDate = new odbcDate();
					 realLen = sizeof(odbcDate);
					 break;
				case SQL_C_TIME:
								F.fTime = new odbcTime();
					 realLen = sizeof(odbcTime);
					 break;
				*/
				case SQL_C_TIMESTAMP:
					 F.fTimeStamp = new odbcTimeStamp;
				     realLen = sizeof(odbcTimeStamp);
					 break;
						default :
							realLen =-1;
			 };
			 len=realLen;
					//Debug("Allocated %ld", len);
};

void  fieldCont::Clear(){
	Null=true;
};

fieldCont::~fieldCont(){
	Free();
};

void  fieldCont::Free(){
	 Null=true;
	 if (len>=0 &&   F.fPtr)
		 switch(ctype){
				 case SQL_C_CHAR:
					ifdela (F.fChar);
					break;
					 case SQL_C_BINARY:
					ifdela (F.fBinary);
					break;
				/*case SQL_C_FLOAT:
					ifdel (F.fFloat);
					break;
				*/
				case SQL_C_DOUBLE	:
					ifdel (F.fDouble);
					break;
				/*
				case	SQL_C_STINYINT:
					ifdel (F.fTiny);
					break;
				case SQL_C_SSHORT:
					ifdel (F.fShort);
					break;
				case SQL_C_SLONG:
					ifdel (F.fLong);
					break;
				case SQL_C_DATE:
					ifdel (F.fDate);
					break;
				case SQL_C_TIME:
					ifdel (F.fTime);
					break;
				*/
				case SQL_C_TIMESTAMP:
					ifdel (F.fTimeStamp);
					break;
			 };
			 ctype=0;
			 len=-1;
};

fieldCont::fieldCont(const fieldCont & c){
	fieldCont::fieldCont();
	LoadValue(c);
	Null=c.Null;
};



void fieldCont::LoadValue(const fieldCont &f, bool ContentOnly){
		 Free();
		 f.SaveValue(*this,true,ContentOnly);
/*		 switch(cType){
			 case SQL_C_CHAR:
							 memcpy(F.fPtr,f.F.fPtr,realLen);
					break;
			 case SQL_C_BINARY:
							 memcpy(F.fPtr,f.F.fPtr,realLen+1);
							 break;
				case SQL_C_FLOAT:
							*F.fFloat = *f.F.fFloat;
					break;
				case SQL_C_DOUBLE	:
							*F.fDouble = *f.F.fDouble;
					break;
				case	SQL_C_STINYINT:
							*F.fTiny = *f.F.fTiny;
					break;
				case SQL_C_SSHORT:
							*F.fShort = *f.F.fShort;
					break;
				case SQL_C_SLONG:
							 *F.fLong = *f.F.fLong;
					break;
				case SQL_C_DATE:
								*F.fDate = *f.F.fDate;
					 break;
				case SQL_C_TIME:
								*F.fTime = *f.F.fTime;
					 break;
				case SQL_C_TIMESTAMP:
								*F.fTimeStamp = *f.F.fTimeStamp;
					 break;
			 };
 */
}

void fieldCont::SaveValue(fieldCont &f ,bool Allocate, bool ContentOnly) const {
	if (Allocate && !ContentOnly){
		f.Allocate(ctype,len);
	}
	 long realLen=len;
	 char *buffer=0;
	 f.Null = false;
	 switch(ctype){
			 case SQL_C_CHAR:
							if(ContentOnly){
								SetupString(F.fChar,buffer);
								StrTrim(buffer);
								realLen=strlen(buffer)+1;
							}else
								buffer=F.fChar;
							if (Allocate && ContentOnly){
								f.Allocate(ctype,realLen);
								f.Null= false;
							}
							memcpy(f.F.fPtr,buffer,realLen);
							if(ContentOnly) delete [] buffer;
					break;
			 case SQL_C_BINARY:
							if (Allocate && ContentOnly){
								f.Allocate(ctype,realLen);
								f.Null= false;
							}
							memcpy(f.F.fPtr,F.fPtr,realLen);
							break;
				/*
				case SQL_C_FLOAT:
					*f.F.fFloat = *F.fFloat;
					break;
				*/
				case SQL_C_DOUBLE	:
					if (Allocate && ContentOnly){
								f.Allocate(ctype,realLen);
								f.Null= false;
					}
					*f.F.fDouble = *F.fDouble;
					break;
				/*
				case	SQL_C_STINYINT:
					*f.F.fTiny = *F.fTiny;
					break;
				case SQL_C_SSHORT:
					 *f.F.fShort = *F.fShort;
					 break;
				case SQL_C_SLONG:
							*f.F.fLong = *F.fLong;
					 break;
				case SQL_C_DATE:
					 *f.F.fDate = *F.fDate;
					 break;
				case SQL_C_TIME:
					 *f.F.fTime = *F.fTime;
					 break;
				*/
				case SQL_C_TIMESTAMP:
					 if (Allocate && ContentOnly){
								f.Allocate(ctype,realLen);
								f.Null= false;
					 }

					 *f.F.fTimeStamp = *F.fTimeStamp;
					 break;
						default:
							f.Null=true;
			 };
			 f.len=realLen;
			 f.ctype=ctype;

}

void fieldCont::LoadFromString(char * buf){
	Null = false;
	switch(ctype){
	case SQL_C_CHAR:
			memset(F.fChar,0,len);
			strncpy(F.fChar,buf,len); break;
	/*
	case SQL_C_FLOAT:
			sscanf(buf,"%f",F.fFloat); break;
	*/
	case SQL_C_DOUBLE	:
			sscanf(buf,"%lf",F.fDouble); break;
	/*
	case	SQL_C_STINYINT:
			*F.fTiny=*buf; break;
	case SQL_C_SSHORT:
			sscanf(buf,"%d",F.fShort); break;
	case SQL_C_SLONG:
			sscanf(buf,"%ld",F.fLong); break;
	case SQL_C_DATE:
			sscanf(buf,"%02u.%02u.%04u",
				&F.fDate->d,
				&F.fDate->m,
				&F.fDate->y
				); break;
	case SQL_C_TIME:
			sscanf(buf,"%02u:%02u:%02u",
			&F.fTime->hour,
			&F.fTime->min,
			&F.fTime->sec
			); break;
	*/
	case SQL_C_TIMESTAMP:
			memset(F.fTimeStamp,0, sizeof(*F.fTimeStamp));
			sscanf(buf,"%02u.%02u.%04u %02u:%02u:%02u-%02u ",
			&F.fTimeStamp->date.d,
			&F.fTimeStamp->date.m,
			&F.fTimeStamp->date.y,
			&F.fTimeStamp->time.hour,
			&F.fTimeStamp->time.min,
			&F.fTimeStamp->time.sec,
			&F.fTimeStamp->time.hsec
			); break;
	 default:
		Null=true;
	};
};


char * fieldCont::printValue(char *buf){
	 if (len >=0){
	switch(ctype){
	 case SQL_C_CHAR:
		memset(buf,0,len);
		strncpy(buf,F.fChar,len-1); buf[len-1] = 0; break;
	/*
	case SQL_C_FLOAT:
		sprintf(buf,"%-15.2f",*F.fFloat); break;
	*/
	case SQL_C_DOUBLE	:
		sprintf(buf,"%-18.2lf",*F.fDouble); break;
	/*
	case	SQL_C_STINYINT:
			sprintf(buf,"%-3d",(short)(*F.fTiny)); break;
	case SQL_C_SSHORT:
			sprintf(buf,"%-5d",*F.fShort); break;
	case SQL_C_SLONG:
			sprintf(buf,"%ld",*F.fLong); break;
	 case SQL_C_BINARY:
				 sprintf(buf,"двоична€ информаци€ (%p)",F.fBinary); break;
	case SQL_C_DATE:
			sprintf(buf,"%02u.%02u.%04u",
				F.fDate->d,
				F.fDate->m,
				F.fDate->y
				); break;
	case SQL_C_TIME:
			sprintf(buf,"%02u:%02u:%02u",
					F.fTime->hour,
					F.fTime->min,
					F.fTime->sec
			); break;
	*/
	case SQL_C_TIMESTAMP:
			sprintf(buf,"%02u.%02u.%04u %02u:%02u:%02u-%02u ",
					F.fTimeStamp->date.d,
					F.fTimeStamp->date.m,
					F.fTimeStamp->date.y,
					F.fTimeStamp->time.hour,
					F.fTimeStamp->time.min,
					F.fTimeStamp->time.sec,
					F.fTimeStamp->time.hsec
			); break;
	};
	 }else{
		 if (ctype==SQL_C_CHAR && len != SQL_NULL_DATA){
			strncpy(buf,F.fChar,len-1); buf[len] = 0;
		 }else
			buf[0]=0;
	 }
	return buf;
};


	bool fieldCont::IsNumeric(){
			switch (ctype){
				case SQL_C_FLOAT:
				case SQL_C_DOUBLE	:
				case SQL_C_STINYINT:
				case SQL_C_SSHORT:
				case SQL_C_SLONG:
					return true;
				case SQL_C_CHAR:
					 return false;
				case SQL_C_BINARY:
					 return false;

				case SQL_C_DATE:
				case SQL_C_TIME:
				case SQL_C_TIMESTAMP:
					 return false;
				default :
					 return false;
			 };
		};

		bool fieldCont::IsDate(){
			switch (ctype){
				case SQL_C_FLOAT:
				case SQL_C_DOUBLE	:
				case SQL_C_STINYINT:
				case SQL_C_SSHORT:
				case SQL_C_SLONG:
					return false;
				case SQL_C_CHAR:
					 return false;
				case SQL_C_BINARY:
					 return false;

				case SQL_C_DATE:
				case SQL_C_TIME:
				case SQL_C_TIMESTAMP:
					 return true;
				default :
					 return false;
			 };
		 };
		bool fieldCont::IsString(){
			switch (ctype){
				case SQL_C_FLOAT:
				case SQL_C_DOUBLE	:
				case SQL_C_STINYINT:
				case SQL_C_SSHORT:
				case SQL_C_SLONG:
					return false;
				case SQL_C_CHAR:
					 return true;
				case SQL_C_BINARY:
					 return false;
				case SQL_C_DATE:
				case SQL_C_TIME:
				case SQL_C_TIMESTAMP:
					 return false;
				default :
					 return false;
			 };
		};

		bool fieldCont::IsBinary(){
			switch (ctype){
				case SQL_C_FLOAT:
				case SQL_C_DOUBLE	:
				case SQL_C_STINYINT:
				case SQL_C_SSHORT:
				case SQL_C_SLONG:
					return false;
				case SQL_C_CHAR:
					 return false;
				case SQL_C_BINARY:
					 return true;
				case SQL_C_DATE:
				case SQL_C_TIME:
				case SQL_C_TIMESTAMP:
					 return false;
				default :
					 return false;
			 };
		};

 // преобразовани€
		fieldCont * fieldCont::ToNumeric(){
			if(IsNumeric())
				return new fieldCont(*this);
			if(IsBinary())
				return NULL;
			if(IsDate()){
				long double d;
				d=F.fTimeStamp->TS2L();
				fieldCont *fc=new fieldCont();
				fc->Allocate(SQL_C_DOUBLE,8);
				*(fc->F.fDouble)=d;
				return fc;
			}
			if (IsString()){
				double d;
				if(sscanf(F.fChar,"%lf",&d)==1){
					fieldCont *fc=new fieldCont();
					fc->Allocate(SQL_C_DOUBLE,8);
					*(fc->F.fDouble)=d;
					return fc;
				}

				return NULL;
			};
			return NULL;
		};

		/*
		fieldCont * fieldCont::ToString(){
			return NULL;
		};
		fieldCont * fieldCont::ToDate(){
			return NULL;
		};


		// задание значений
		void fieldCont::SetNumeric(double n){
			Free();
			Allocate(SQL_C_DOUBLE,8);
			*(F.fDouble) = n;
		};

		void fieldCont::SetString(const char *s){
		};
		void fieldCont::SetBinary(const unsigned char *b, long l){
		};
		void fieldCont::SetDateTime(const odbcTimeStamp & dt ){
		};

		// получение значений
		double fieldCont::Numeric(){
			if(IsNumeric())
				return *F.fDouble;
			else {
				fieldCont *tmp=ToNumeric();
				double d=0;
				if(tmp){
					d=*tmp->F.fDouble;
					delete tmp;
				}
				return d;
			}
		};

		char *  fieldCont::String(){
			return NULL;
		};

		odbcTimeStamp fieldCont::DateTime(){
			odbcTimeStamp ts;
			if (IsDate()){
				switch(ctype){

				case SQL_C_DATE:
					ts.date=*F.fDate;
					ts.time.Clear();
					break;
				case SQL_C_TIME:
					ts.date.Clear();
					ts.time=*F.fTime;
					break;

				case SQL_C_TIMESTAMP:
					 ts=*F.fTimeStamp;

				};

			}
			return ts;

		};

		void * fieldCont::Binary(){
						return NULL;
		};

		// ”становка данных
		void fieldCont::Now(){
		};
		void fieldCont::Date(){
		};
		*/

StackValue *fieldCont::_StackValue(){
	StackValue *sv = new StackValue;
	if(IsNull()) return sv;
	switch(ctype){
      case SQL_C_BINARY:

      sv->type=stBinary;
			sv->b.SetBuffer(len,F.fChar);
        break;
		case SQL_C_CHAR:
			sv->type=stString;
			sv->s=F.fChar;
			break;
		case SQL_C_DOUBLE:
			sv->type=stNumeric;
			sv->n=*F.fDouble;
			break;
		case SQL_C_TIMESTAMP:
			sv->type=stDate;
			{
			try{
			TDateTime d(CorrectYear(F.fTimeStamp->date.y),
			F.fTimeStamp->date.m,
			F.fTimeStamp->date.d);


			TDateTime t(F.fTimeStamp->time.hour,
			F.fTimeStamp->time.min,
			F.fTimeStamp->time.sec,
                        0);
			//(F.fTimeStamp->time.hsec%1000));
			sv->d=d+t;
			}catch(...){
                          try{
        			TDateTime d(CorrectYear(F.fTimeStamp->date.y),
	        		F.fTimeStamp->date.m,
		        	F.fTimeStamp->date.d);
                                sv->d=d;
                          }catch(...){
	        	    TDateTime d;
                  	    sv->d=d;
                          }
			}
			}

	}
	return sv;

};










#include "vcl.h"
#include "stackvalue.h"
#include "stdio.h"

unsigned short CorrectYear(unsigned short Year){
	TDateTime dd =TDateTime::CurrentDate();
	unsigned short year,month,day;
	dd.DecodeDate(& year, & month, & day);
		if(Year<1899){
			return year/100*100+Year%100;
		}
		return Year;
};



//////////////////////////////////////////////////////////////////
SimpleStream & operator >> (SimpleStream & s, StackType &ft){
			int f;
			s >> f;
			ft=(StackType) f;
			return s;
		};
SimpleStream & operator << (SimpleStream & s,StackType &ft){
					return s <<(int) ft;
};



/////////////////////////////////////////////////////////////////////////////////

SimpleStream & operator >> (SimpleStream & s,StackValue &ft){
	s >> ft.type;
  switch (ft.type){
    case stNull:
      break;
    case stString :
      s>>ft.s;
      break;
    case stDate    :
      s>>ft.d >> ft.NullNumber;
      break;
    case stNumeric :
      s>>ft.n >> ft.NullNumber;
      break;
    case stBinary :
      s>>ft.b;
      break;

  };
  return s;
};
SimpleStream & operator << (SimpleStream & s,StackValue &ft){
	s << ft.type;
  switch (ft.type){
    case stNull:
      break;
    case stString :
      s<<ft.s;
      break;
    case stDate    :
      s<<ft.d << ft.NullNumber;
      break;
    case stNumeric :
      s<<ft.n << ft.NullNumber;
      break;
    case stBinary :
      s<<ft.b;
      break;
  };
  return s;
};


AnsiString StackValue::ToStr() const{
			 if(IsNull())
				return "";
			 switch(type){
				case stString:
						return s;
				case stNumeric:
						if(NullNumber)
							return "";
						if(n==0.0)
							return AnsiString("0");
						//return AnsiString(n);
   						char buf[40];
						sprintf(buf,"%15.2lf",n);
						return AnsiString(buf);

				case stDate:{
										 unsigned short y,m,day,hour,minute,sec,hsec;
 		         d.DecodeDate(&y,&m,&day);
			 d.DecodeTime(&hour,&minute,&sec,&hsec);
         		 char buf[40];
                         if(!hour && !minute && !sec)
 			   sprintf(buf, "%02d/%02d/%04d ",day,m,y);
                         else
                         if(y==1899 && m==12 && day==30)
			    sprintf(buf, "%02d:%02d:%02d",hour,minute,sec);
                         else
			    sprintf(buf, "%02d/%02d/%04d %02d:%02d:%02d",day,m,y,hour,minute,sec);
			    return AnsiString(buf);
							 }
        case stBinary:
          return BinaryString();
				default:
					 return "";
			};
		};
AnsiString StackValue::ToSQLStr() const{
			if(IsNull())
				return " NULL ";

			 switch(type){
				case stString:
						return AnsiString(AnsiString("'")+ s +AnsiString("'")) ;
				case stNumeric:{
						char buf[40];
						sprintf(buf,"%lf",n);
						return AnsiString(buf);
										 }
				case stDate:{
										 unsigned short y,m,day,hour,minute,sec,hsec;
						 d.DecodeDate(&y,&m,&day);
					 d.DecodeTime(&hour,&minute,&sec,&hsec);
										 char buf[40];
										 sprintf(buf, "'%02d/%02d/%04d %02d:%02d:%02d'",m,day,y,hour,minute,sec);
					 return AnsiString(buf);
										 }
        case stBinary:
          return BinarySQLString();
				default:
					 return "NULL";
			};
		};


TDateTime StackValue::ToDate() const{
		if(type==stDate)
			return d;
        if(type==stNumeric){
        	try{
			return TDateTime(n);
            }catch(...){
                TDateTime ddd;
								return ddd;
						}
				}
				if(type==stString){
					try{
						TDateTime ddd(s);
								return ddd;
						}catch(...){
								unsigned short y,m,day,h,mi,sec;
								int res;
					char *fmts [2]={
						"%hd/%hd/%hd %hd:%hd:%hd",
						"%hd.%hd.%hd %hd:%hd:%hd"
					};

					for(int jjj=0;jjj<2;jjj++){
					try{
							res=sscanf(s.c_str(),fmts[jjj], &day, &m, &y, &h, &mi,&sec);
									if(res>=3 ){
										TDateTime ddd(CorrectYear(y),m,day);
						if(res==4){
												TDateTime ttt(h,0,0,0);
													ddd+=ttt;
													return ddd;
											}
											if(res==5){
												TDateTime ttt(h,mi,0,0);
													ddd+=ttt;
													return ddd;
											}
						if(res==6){
												TDateTime ttt(h,mi,sec,0);
													ddd+=ttt;
													return ddd;
											}
											return ddd;
									}
								 }catch(...){
								}
								try{
							res=sscanf(s.c_str(),fmts[jjj], &m, &day, &y, &h, &mi,&sec);
									if(res>=3 ){
										TDateTime ddd(CorrectYear(y),m,day);
						if(res==4){
												TDateTime ttt(h,0,0,0);
													ddd+=ttt;
													return ddd;
											}
											if(res==5){
												TDateTime ttt(h,mi,0,0);
													ddd+=ttt;
													return ddd;
												}
						if(res==6){
												TDateTime ttt(h,mi,sec,0);
													ddd+=ttt;
													return ddd;
											}
											return ddd;
									}
								}catch(...){
								}
							 }
			return TDateTime();

						}
				}
				if(type==stNull || type==stBinary){
						TDateTime ddd;
				return ddd;
				}
				return TDateTime();

};

double StackValue::ToNumeric() const{
	if(type==stNumeric){
		if(NullNumber)
			return 0.0;
		else
			return n;
	}else{
		if(type==stDate){
			return (double)d;
		}

		if(type==stString){
		try {
			return s.ToDouble();
		}
		catch(... ){
						double dbl;
						if(sscanf(s.c_str(),"%lf",&dbl)==1){
							return dbl;
						}
						if(s=="T" || s=="t" || s=="Y" || s=="y" || s=="Ä" || s=="ä" ||
							 s.UpperCase()=="TRUE" || s.UpperCase()=="YES" || s.UpperCase()=="ÄÀ" || s.UpperCase()=="ÈÑÒÈÍÀ")
							return 1.0;
			return 0.0;
		}
		}
		}
	return 0.0;
};

bool StackValue::IsNull() const{
	if(type==stNull) return true;
	if(type==stString && (s.IsEmpty() || s=="")) return true;
	if(type==stDate && NullNumber) return true;
	if(type==stNumeric && NullNumber) return true;
  if(type==stBinary && b.Length()==0) return true;
	return false;
};

bool StackValue::IsBinary() const{
	if(type==stBinary) return true;
  return false;
};


StackValue & StackValue::operator =(const StackValue & x){
	type=x.type;
	s=x.s;
	d=x.d;
	n=x.n;
  b.SetBuffer(x.b.Length(),x.b.GetBuffer());
	NullNumber=x.NullNumber;
  return *this;
};

bool StackValue::IsEqual(const StackValue & x) const{
 if(type==x.type){
	switch (type){
		case stDate:
		 return d==x.d;
		case stString:
		 return s==x.s;
		case stNumeric:
		 return NullNumber==x.NullNumber && n==x.n;
		case stNull:
		 return true;
    case stBinary:
      if(b.Length()!=x.b.Length())
        return false;
      if(!b.Length())
        return true;
      return !memcmp(b.GetBuffer(),x.b.GetBuffer(),b.Length());
		default:
		 return false;
	 };
	}
	return (ToStr()==x.ToStr());
};



bool operator <(const StackValue & y,const StackValue & x){
 if(y.type==x.type){
	switch (y.type){
		case stDate:
		 return y.d<x.d;
		case stString:
		 return y.s<x.s;
		case stNumeric:
		 return (!y.NullNumber && !x.NullNumber && y.n<x.n) || (!y.NullNumber && x.NullNumber );
		case stNull:
		 return true;
    case stBinary:
      if(y.b.Length() > x.b.Length())
        return false;
      if(!y.b.Length() && x.b.Length() )
        return true;
      return memcmp(y.b.GetBuffer(),x.b.GetBuffer(),y.b.Length())>0;
		default:
		 return false;
	 };
	}
	return (y.ToStr()<x.ToStr());
};


bool operator ==(const StackValue & y,const StackValue & x)
{
	return y.IsEqual(x);
};

bool StackValue::IsNumeric() const {
	if(type==stNull) return false;
	if(type==stNumeric) return !NullNumber;
	if(type==stDate) return false;
	if(type==stBinary) return false;
	if(type==stString){
		try{
		s.ToDouble();
		return true;
		}
		catch(...){
				double dbl;
			if(sscanf(s.c_str(),"%lf",&dbl)==1)
							return true;
			// bool to Numeric
			if( s.UpperCase()=="T" || s.UpperCase()=="Y" || s.UpperCase()=="Ä" || s.UpperCase()=="È" ||
					s.UpperCase()=="TRUE" || s.UpperCase()=="YES" || s.UpperCase()=="ÄÀ" || s.UpperCase()=="ÈÑÒÈÍÀ")
			return true;

		}
	}
	return false;
};


bool StackValue::IsDate() const{
	if(type==stNull) return false;
	if(type==stNumeric) return false;
	if(type==stDate) return true;
	if(type==stBinary) return false;
	if(type==stString){
		try{
				TDateTime ddd(s);
		return true;
		}
		catch(...){

		///
		unsigned short y,m,day,h,mi,sec;
		int res;

		char *fmts [2]={
		"%hd/%hd/%hd %hd:%hd:%hd",
		"%hd.%hd.%hd %hd:%hd:%hd"
		};

		for(int jjj=0;jjj<2;jjj++){
		try{
				res=sscanf(s.c_str(),fmts[jjj], &day, &m, &y, &h, &mi,&sec);
				if(res>=3 ){
									TDateTime ddd(CorrectYear(y),m,day);
					if(res==4){
											TDateTime ttt(h,0,0,0);
												return true;
										}
										if(res==5){
											TDateTime ttt(h,mi,0,0);
												return true;
										}
					if(res==6){
											TDateTime ttt(h,mi,sec,0);
												return true;
										}
										return true;
					}
			 }catch(...){
			 }
		 try{

					res=sscanf(s.c_str(),fmts[jjj], &m, &day, &y, &h, &mi,&sec);
					if(res>=3 ){

									TDateTime ddd(CorrectYear(y),m,day);
					if(res==4){
											TDateTime ttt(h,0,0,0);
												return true;
										}
										if(res==5){
											TDateTime ttt(h,mi,0,0);
												return true;
										}
					if(res==6){
											TDateTime ttt(h,mi,sec,0);
												return true;
										}
										return true;
				 }
			 }catch(...){
			 }
			 }
		 return false;
		///
		}
	}
	return false;
};

void StackValue::Erase(){
	d=0;
	n=0;
	s="";
  b.Clear();
	if(type==stNumeric || type==stDate)
		NullNumber = true;
};



bool StackValue::LessThen(const StackValue & x) const{
 if(type==x.type){
	switch (type){
		case stDate:
		 return d<x.d;

		case stString:
		 return s<x.s;

		case stNumeric:
		 if(x.NullNumber && NullNumber)
			return false;

		 if(NullNumber)
			 return true;

		 if(x.NullNumber)
			 return false;

		 return   n < x.n;

		case stNull:
    case stBinary:
		 return false;

		default:
		 return false;
	 };
	}
	return (ToStr()< x.ToStr());

};
bool StackValue::Filter(FilterCondition Condition, const StackValue & sFrom, const StackValue & sTo) const{

	StackValue From,To;

	// ïðèâåëè òèïû
	From.Set(type,sFrom.ToStr());
	To.Set(type,sTo.ToStr());

	switch(Condition){
	case fcNone:
		return true;
	case 	fcEQ:
		return IsEqual(From);
	case 	fcNE:
		return !IsEqual(From);
	case 	fcLT:
		return LessThen(From);
	case 	fcLE:
		return LessThen(From) || IsEqual(From);
	case 	fcGT:
		return !LessThen(From) && !IsEqual(From);
	case 	fcGE:
		return !LessThen(From);
	case 	fcBETWEEN:
				return (LessThen(To) || IsEqual(To)) && !LessThen(From);

	case 	fcLikeLeft:
				return ToStr().UpperCase().Pos(sFrom.ToStr().UpperCase())==1;

	case 	fcLikeCenter:
				return ToStr().UpperCase().Pos(sFrom.ToStr().UpperCase())>0 ;

	case 	fcLikeRight:
				{
					AnsiString s  =	ToStr().UpperCase();
					AnsiString s2 =	sFrom.ToStr().UpperCase();
					if(s.Length()<=s2.Length())
						return false;
					//	             10            5           5
					return s.SubString(s.Length()-s2.Length(),s2.Length())==	 s2;
				}

	case 	fcIsNull:
		return IsNull();

	case 	fcIsNotNull:
		return !IsNull();
	default:
		return true;
	}
};


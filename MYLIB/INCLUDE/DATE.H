/****************************************************
 *         DATE.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 *                                                  *
 *  модифицирован: 1 Октября 1995, Воскресенье      *
 *                                                  *
 *  Copyright (C) Дмитрий А. Зорин (модификация)    *
 *  Согласие на модификацию получено еще в августе  *
 ****************************************************/

#ifndef DATE_H
#define DATE_H

extern char *DayName[];
extern char *MonthName1[];
extern char *MonthName2[];

class prolog odbcDate
{
  int LeapYear();

public:
  short y;
  short m;
  short d;



  odbcDate( int d = 0, int m = 0, int y = 0 );

  long D2L();
  void L2D( long NofDays );
  char *D2C( char *str );
  void C2D( char *str );
  char *M2C( char *str ); // Месяц (без числа)
  void C2M( char *str );

  void Today();
  void BOY();
  void BOQ();
  void BOM();
  void EOY();
  void EOQ();
  void EOM();
  short DayOfWeek();

  void Clear();
  void Set( int d, int m, int y );
  int OK();
  int Empty()
  {
    if( D2L() == 0 )
        return -1;
	 else
        return 0;
  };

  int EQL(odbcDate *AnotherodbcDate );
  int GE(odbcDate *AnotherodbcDate );
  int GT(odbcDate *AnotherodbcDate );
  int LE(odbcDate *AnotherodbcDate );
  int LT(odbcDate *AnotherodbcDate );

  void Add(odbcDate *AnotherodbcDate );
  void Sub(odbcDate *AnotherodbcDate );

  void Add( long days );
  void Sub( long days );
  odbcDate& operator = (const odbcDate& b )
  {
    d = b.d;
    m = b.m;
    y = b.y;
    return *this;
  };
  void operator += ( long b )
  {
    Add( b );
  };
  void operator -= ( long b )
  {
	 Sub( b );
  };

  char *PrtDate( char *buf, short week = 0 );
};

int prolog operator !=(odbcDate &a,odbcDate &b);
int prolog   operator ==(odbcDate &a,odbcDate &b);
int prolog   operator > (odbcDate &a,odbcDate &b);
int prolog   operator >=(odbcDate &a,odbcDate &b);
int prolog   operator < (odbcDate &a,odbcDate &b);
int prolog   operator <=(odbcDate &a,odbcDate &b);

class  prolog odbcTime{
 public:
  short hour;
  short min;
  short sec;
  short hsec;



  void C2T(char *s);
  char *T2C(char *s);
  long T2L(void);
  void L2T(long l);
  void Clear(void) { Set(); };
  void Set(char h=0,char m=0,char s=0, char hs=0 );
  void Now(void);

  odbcTime&  operator = (const odbcTime& b);
  void operator +=(odbcTime& b);
  void operator -=(odbcTime& b);
  odbcTime(char h=0,char m=0,char s=0, char hs=0 );
  ~odbcTime(){};
};

int   prolog operator !=(odbcTime &a,odbcTime &b);
int   prolog operator ==(odbcTime &a,odbcTime &b);
int   prolog operator > (odbcTime &a,odbcTime &b);
int   prolog operator >=(odbcTime &a,odbcTime &b);
int   prolog operator < (odbcTime &a,odbcTime &b);
int   prolog operator <=(odbcTime &a,odbcTime &b);

int  prolog Periods(odbcDate *bdate,odbcDate *edate, short type, short b_e = 1 );

class  prolog odbcTimeStamp{
public:
	odbcDate date;
	odbcTime time;

	odbcTimeStamp(){};

   odbcTimeStamp(const odbcTimeStamp & t){
   	date=t.date;
      time = t.time;
   };


   long double TS2L(){
   	return ((long double) date.D2L())*10000000.00+time.T2L();
   };

   void L2TS(long double ld){
   	 date.L2D((long)(ld/10000000.00));
       time.L2T(
         (long)(ld-(10000000.00*(long)(ld/10000000.00)))
       );
   };

  int EQL(odbcTimeStamp *Another ){
  	return TS2L()==Another->TS2L();
  };
  int GE(odbcTimeStamp *Another ){
  	return TS2L()<=Another->TS2L();
  };
  int GT(odbcTimeStamp *Another ){
  	return TS2L()>Another->TS2L();
  };
  int LE(odbcTimeStamp *Another ){
  	return TS2L()<=Another->TS2L();
  };
  int LT(odbcTimeStamp *Another ){
  	return TS2L()<Another->TS2L();
  };

  odbcTimeStamp& operator = (const odbcTimeStamp& b )
  {
    date = b.date;
    time = b.time;
    return *this;
  };



};

#endif

/****************************************************
 *       DATE.CPP                                   *
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
#include "windows.h"
#include "stdio.h"
#include "date.h"


char * DayName[] =
{
  "?",
  "Понедельник",
  "Вторник",
  "Среда",
  "Четверг",
  "Пятница",
  "Суббота",
  "Воскресенье"
};

char *MonthName1[] =
{
  "?",
  "Январь",
  "Февраль",
  "Март",
  "Апрель",
  "Май",
  "Июнь",
  "Июль",
  "Август",
  "Сентябрь",
  "Октябрь",
  "Ноябрь",
  "Декабрь"
};

char *MonthName2[] =
{
  "?",
  "Января",
  "Февраля",
  "Марта",
  "Апреля",
  "Мая",
  "Июня",
  "Июля",
  "Августа",
  "Сентября",
  "Октября",
  "Ноября",
  "Декабря"
};

static int ml[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

int odbcDate::LeapYear()
{
  int leap = 0;
  if( y/4*4 == y ) leap = 1;
  if( y/100*100 == y ) leap = 0;
  if( y/400*400 == y ) leap = 1;
  return leap;
};

odbcDate::odbcDate( int _d, int _m, int _y )
{
  Set(_d,_m,_y );
};

long odbcDate::D2L(void){
  long e = 0;
  long y1;
  if (y > 1 ){
	 e = (y - 1)/400 * 146097L;
	 y1 = (y - 1)%400;
	 e = e + y1*365 + (y1/4) - (y1/100);
  }

  for(int i = 1;i<12;i++){
    if( i < m ){
      e = e + ml[i - 1];
      if( i == 2) {
        e = e + LeapYear();
      }
    }
  }
  e = e + d;
  return e;
};

void odbcDate::L2D(long e){
  long ee=0;
  long y1;
  d = 0;
  m = 1;
  y = (e / 146097L)*400 + (e%146097L)/366 + 1;
  ee = D2L();
  y1 = e - ee;
  if (y1 > 365) {
	 y = y + 1;
	 ee = D2L();
	 y1 = e - ee;
  }
  ml[1] = ml[1] + LeapYear();
  for(int i = 0 ;i<=11;i++){
	 if (y1 > ml[i]) {
		m = m + 1;
      y1 = y1 - ml[i];
    }else{
      break;
    }
  }
  ml[1] = ml[1] - LeapYear();
  d = y1;
};

char * odbcDate::D2C( char *str )
{
    sprintf( str, "%02d.%02d.%04d", d, m, y );
	 return str;
};

void odbcDate::C2D( char *str )
{
    sscanf( str, "%d.%d.%d", &d, &m, &y );
    if( !OK() )
    {
        d = m = 0; y = 0;
    }
    else if( y < 100 )
        y += 1900;
};

char *odbcDate::M2C( char *str )
{
    sprintf( str, "%02d.%04d", m, y );
    return str;
}

void odbcDate::C2M( char *str )
{
    sscanf( str, "%d.%d", &m, &y );
    d = 1;
    if( !OK() )
    {
        d = m = 0; y = 0;
    }
	 else if( y < 100 )
        y += 1900;
}


void odbcDate::Today(void){
  SYSTEMTIME ddd;
  GetSystemTime(&ddd);
  d=ddd.wDay;
  m=ddd.wMonth;
  y=ddd.wYear;
};


void odbcDate::BOY(void){
  d=1;
  m=1;
};
void odbcDate::BOQ(void){
  d=1;
  if(m<4) m=1;
  else
    if(m<7) m=4;
    else
      if(m<10) m=7;
      else m=10;
};

void odbcDate::BOM(void){
  d=1;
};

void odbcDate::EOY(void){
  d=31;
  m=12;
};

void odbcDate::EOQ(void){
  if(m<4) m=3;
  else
	 if(m<7) m=6;
    else
      if(m<10) m=9;
      else m=12;
  EOM();
};

void odbcDate::EOM(void){
  if( m){
    ml[1]+=LeapYear();
    d=ml[m-1];
    ml[1]-=LeapYear();
  }
};

void odbcDate::Clear(void){
  d=m=0;
  y=0;
};

void odbcDate::Set(int _d,int _m,int _y){
  d=_d;
  m=_m;
  y=_y;
  if(!OK()){
    d=m=0;
    y=0;
  }
};

int odbcDate::OK(){
  int best=0;
  if(y>=0)
    if(m>0 && m<13){
      ml[1]+=LeapYear();
      if(d>0 && d<=ml[m-1])
        best=1;
      ml[1]-=LeapYear();
    }
  return best;
};

int odbcDate::EQL(odbcDate * AnotherodbcDate){
  if(d==AnotherodbcDate->d &&
	  m==AnotherodbcDate->m &&
     y==AnotherodbcDate->y )
      return 1;
  else
	 return 0;
};

int odbcDate::GT(odbcDate * AnotherodbcDate){
  if(D2L()>AnotherodbcDate->D2L())
    return 1;
  else
    return 0;
};

int odbcDate::LT(odbcDate * AnotherodbcDate){
  if(D2L()<AnotherodbcDate->D2L())
    return 1;
  else
    return 0;
};

int odbcDate::GE(odbcDate * AnotherodbcDate){
  if(D2L()>=AnotherodbcDate->D2L())
    return 1;
  else
    return 0;
};

int odbcDate::LE(odbcDate * AnotherodbcDate){
  if(D2L()<=AnotherodbcDate->D2L())
    return 1;
  else
	 return 0;
};

void odbcDate::Add(odbcDate * AnotherodbcDate){
  Add(AnotherodbcDate->D2L());
};
void odbcDate::Sub(odbcDate * AnotherodbcDate){
  Sub(AnotherodbcDate->D2L());
};
void odbcDate::Add(long days){
  long cur=D2L();
  cur+=days;
  if(cur<0)
    cur=0;
  L2D(cur);
};

void odbcDate::Sub(long days){
  long cur=D2L();
  cur-=days;
  if(cur<0)
    cur=0;
  L2D(cur);
};

int  operator ==(odbcDate &a,odbcDate &b){
  return (a.d==b.d && a.m==b.m && a.y==b.y);
};

int  operator !=(odbcDate &a,odbcDate &b){
  return !(a.d==b.d && a.m==b.m && a.y==b.y);
};

int  operator >(odbcDate &a,odbcDate &b){
  return (a.D2L()>b.D2L());
};

int  operator >=(odbcDate &a,odbcDate &b){
  return (a.D2L()>=b.D2L());
};

int  operator <(odbcDate &a,odbcDate &b){
  return (a.D2L()<b.D2L());
};

int  operator <=(odbcDate &a,odbcDate &b){
  return (a.D2L()<=b.D2L());
};


long odbcTime::T2L(void){
  return (((hour*60+min)*60+sec)*100+hsec);
};
void odbcTime::L2T(long l){
  long l1;
  hsec =l%100;
  l1=l/100;
  sec=l1%60;
  l1/=60;
  min=l1%60;
  hour=l1/60;
  hour=hour%24;
};

void odbcTime::C2T(char *str){
  short h=0,m=0,s=0,hs=0;
  sscanf(str,"%d:%d:%d.%d",&h,&m,&s,&hs);
  hour=h%24;
  min=m%60;
  sec=s%60;
  hsec=hs%100;
};

char *odbcTime::T2C(char *s){
  sprintf(s,"%02d:%02d:%02d-%02d", hour,min,sec,hsec);
  return s;
};

void odbcTime::Set(char h,char m,char s, char hs ){
  hour= h%24;
  min = m%60;
  sec = s%60;
  hsec= hs%100;
};

void odbcTime::Now(void){
  SYSTEMTIME t;
  GetSystemTime(&t);
  hour=t.wHour;
  min=t.wMinute;
  sec=t.wSecond;
  hsec=t.wMilliseconds * 10;
};


odbcTime&  odbcTime::operator = (const odbcTime& b){
  hour= b.hour%24;
  min = b.min%60;
  sec = b.sec%60;
  hsec= b.hsec%100;
  return *this;
};

void  odbcTime::operator +=(odbcTime& b){
  long l=T2L()+b.T2L();
  L2T(l);
};
void  odbcTime::operator -=(odbcTime& b){
  long l=T2L()-b.T2L();
  L2T(l);
};
odbcTime::odbcTime(char h,char m,char s, char hs ){
  Set(h,m,s,hs);
};

int  operator !=(odbcTime& a,odbcTime& b){
  return a.T2L()!=b.T2L();
};
int  operator ==(odbcTime& a,odbcTime& b){
  return a.T2L()==b.T2L();
};
int  operator > (odbcTime& a,odbcTime& b){
  return a.T2L()>b.T2L();
};
int  operator >=(odbcTime& a,odbcTime& b){
  return a.T2L()>=b.T2L();
};
int  operator < (odbcTime& a,odbcTime& b){
  return a.T2L()<b.T2L();
};
int  operator <=(odbcTime& a,odbcTime& b){
  return a.T2L()<=b.T2L();
};


int Periods(odbcDate * bdate ,odbcDate *edate, short type, short b_e){
  odbcDate b=*bdate,e=*edate,t;
  int cnt;
  if(e <= b)
    return 0;
  if(b_e){
    // считать по началу периода
    switch(type){
      case 1: // месяц
        t=b;
        t.BOM();
        if(b!=t){
          b.EOM();
          b.Add(1);
        }
        e.BOM();
		  cnt=0;
        while(b<=e){
         cnt++;
         b.EOM();
         b.Add(1);
        }
        return cnt;
      case 2: // квартал
        t=b;
        t.BOQ();
        if(b!=t){
          b.EOQ();
          b.Add(1);
        }
        e.BOQ();
        cnt=0;
        while(b<=e){
         cnt++;
         b.EOQ();
         b.Add(1);
        }
        return cnt;
      case 3: // год
        t=b;
        t.BOY();
        if(b!=t){
          b.EOY();
          b.Add(1);
		  }
        e.BOY();
        cnt=0;
        while(b<=e){
         cnt++;
         b.EOY();
         b.Add(1);
        }
        return cnt;
    }
  }else{
    // считать по концу периода
    switch(type){
      case 1: // месяц
        cnt=0;
        t=e;
        t.EOM();
        if(e!=t){
          e.BOM();
          e.Sub(1);
        }
        b.EOM();
        while(b<=e){
         cnt++;
         b.Add(1);
         b.EOM();
        }
        return cnt;
		case 2: // квартал
        cnt=0;
        t=e;
        t.EOQ();
        if(e!=t){
          e.BOQ();
          e.Sub(1);
        }
        b.EOQ();
        while(b<=e){
         cnt++;
         b.Add(1);
         b.EOQ();
        }
        return cnt;

      case 3: // год
        cnt=0;
        t=e;
        t.EOY();
        if(e!=t){
          e.BOY();
          e.Sub(1);
        }
        b.EOY();
        while(b<=e){
         cnt++;
         b.Add(1);
         b.EOY();
        }
        return cnt;
    }
  }
  return 0;
};
short odbcDate::DayOfWeek(void)
{
    short century, yr, dw, y2=y, m2=m, d2=d;

    if(m2 < 3)
        {
        m2 += 10;
        --y2;
        }
    else
        m2 -= 2;

    century = y2 / 100;
    yr = y2 % 100;
    dw = (((26 * m2 - 2) / 10) + d2 + yr + (yr / 4) + (century / 4) -
                (2 * century)) % 7;

    if(dw < 0)
        dw += 7;

    if(!dw) dw=7;

    return dw;
};

char *odbcDate::PrtDate(char *buf,short week){
  if(week)
    sprintf(buf,"%d %s %04d, %s",d,MonthName2[m],y,DayName[DayOfWeek()]);
  else
    sprintf(buf,"%d %s %04d",d,MonthName2[m],y);

  return buf;
}



/****************************************************
 *   MYSTRING.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "stdio.h"
#include "vcl.h"
#include "mystring.h"
#undef TRACE
#define TRACE(X)


void  StrClear(char * s,int l,char f){
  TRACE("StrClear");
  memset(s,f,l);
  s[l]=0;
};

int    upper(char ch){
  if(ch>='a' && ch <='z') return ch-32;
  if(ch>='а' && ch<='я') return ch-32;
	if(ch=='ё') return 'Ё';
	return ch;
};

char *   StrUpper(char *source,char *dest){
	TRACE("StrUpper");
  unsigned char *s=(unsigned char *)source;
  unsigned char *buf=(unsigned char *)dest;
	if(!buf)
		buf=s;
	for(int i=0; i<strlen((char*)s);i++)
		buf[i]=upper(s[i]);
	return (char*)buf;
};

char*   StrTrim(char *s,char *buf){
	TRACE("StrTrim");
	if(!buf)
    buf=s;
	unsigned int i,j;
	i=j=0;

	for(j=0;j<strlen(s);j++){
		if(s[j]!=' ' && s[j] !='\t' && s[j] !=10 && s[j] !=13)
			buf[i++]=s[j];
		else
			if(i!=0 && buf[i-1]!=' ')
				buf[i++]=' ';
	}

	if(i && buf[i-1]==' ') i--;

	buf[i]=0;

	return buf;
};

int     StrWrap(char *s, int l){
	 TRACE("StrWrap");

		if(strlen(s)<=l)
			return strlen(s)-1;

		for(int i=l-1 ;i>=0;i--)
			if( s[i]==' ' ||          s[i]=='.' ||
					s[i]==',' ||          s[i]=='+' ||
					s[i]=='-' ||          s[i]=='/' ||
					s[i]=='\\'||          s[i]=='%' ||
					s[i]=='*' ||          s[i]==')' ||
					s[i]==']' ||          s[i]=='&' ||
					s[i]=='!' ||          s[i]=='@' ||
					s[i]=='$' ||          s[i]=='\''||
					s[i]=='|' ||          s[i]=='?' ||
					s[i]=='{' ||          s[i]=='}' ||
					s[i]=='[' ||          s[i]=='(' ||
					s[i]=='#' ||          s[i]=='~' ||
					s[i]=='>' ||          s[i]=='<' ||
					s[i]=='№' ||          s[i]=='`' ||
					s[i]=='^' ||          s[i]==0 )
						return i;

	 return 0;

};


char*   StrL(char *s,char *buf, int l,int mode){
	TRACE("StrL");
	StrClear(buf,l);
	if(strlen(s)<l){
		strncpy(buf,s,strlen(s));
	}else{
		switch ( mode )
		{
		case 0:
			strncpy(buf,s,l);
			break;

		case 1:
			{
			int g=StrWrap(s,l);
			if(g){
				//g++;
				strncpy(buf,s,g);
			}else
				strncpy(buf,s,l);
			break;
      }
    default:
      StrClear(buf,l,mode);

    }
  }
  return buf;
};

char*   StrR(char *s,char *buf, int l,int mode){
  TRACE("StrR");

  StrClear(buf,l);
  if(strlen(s)<l){
    strncpy((buf+l-strlen(s)),s,strlen(s));
  }else{
    switch ( mode )
    {
    case 0:
      strncpy(buf,s,l);
      break;

    case 1:
      {
      int g=StrWrap(s,l);
      if(g){
        //g++;
        strncpy(buf+l-g,s,g);
      }else
        strncpy(buf,s,l);
      break;
      }
    default:
      StrClear(buf,l,mode);

    }
  }
  return buf;
};

char*   StrC(char *s,char *buf, int l,int mode){
  TRACE("StrC");
  StrClear(buf,l);
  if(strlen(s)<l){
    strncpy((buf+(l-strlen(s))/2),s,strlen(s));
  }else{
    switch ( mode )
    {
    case 0:
      strncpy(buf,s,l);
      break;

    case 1:
      {
      int g=StrWrap(s,l);
      if(g){
        //g++;
        strncpy(buf+(l-g)/2,s,g);
      }else
        strncpy(buf,s,l);
      break;
      }
    default:
      StrClear(buf,l,mode);

    }
  }
  return buf;
};
char*   StrJ(char *s,char *buf, int l,int mode){
  TRACE("StrJ");
  StrClear(buf,l);
  char *c=new char [strlen(s)+1];
  StrTrim(s,c);
  int sp=0;
  int mult=0;
  int mod=0;
  int i,j=0;

  for(i=0 ;i< strlen(c);i++)
    if(c[i]==' ') sp++;

  if(strlen(c)<l){
    if(sp){
      mult=(l-strlen(c))/sp;
      mod=(l-strlen(c))%sp;
      for(i=0;i<strlen(c);i++){
        if(c[i]==' '){
          if(mod){ buf[j++]=' '; mod--;}
          for(int k=0;k<=mult;k++)
            buf[j++]=' ';
        }else{
          buf[j++]=c[i];
        }
      }
    }else{
      strncpy(buf,c,strlen(c));
    }

  }else{
    switch ( mode )
    {
    case 0:
      strncpy(buf,s,l);
      break;

    case 1:
      {
      int g=StrWrap(c,l);
      if(g){
          //g++;
          sp=0;
          for(int i=0 ;i< g;i++)
            if(c[i]==' ') sp++;
          if(sp){
            mult=(l-g)/sp;
            mod=(l-g)%sp;
            for(i=0;i<g;i++){
              if(c[i]==' '){
                if(mod){ buf[j++]=' '; mod--;}
                for(int k=0;k<=mult;k++)
                  buf[j++]=' ';
              }else
                buf[j++]=c[i];
            }
          }else{
            strncpy(buf,c,g);
          }


      }else
        strncpy(buf,s,l);
      break;
      }
    default:
      StrClear(buf,l,mode);

    }
  }
  delete c;
  return buf;
};





int     StrForm(char *s,char * buf[], int l,int array){
  TRACE("StrForm");
  int curl=0,ll,i;
  for(i=0;i<array;i++){
    if(curl<strlen(s)){
      ll=StrWrap(s+curl,l);
      StrClear(buf[i],l);

      if(!ll)
        ll=l;
      else
        ll++;
      strncpy(buf[i],s+curl,ll);
      curl+=ll;
      if(s[curl]==' ')
        curl++;
    }else
      break;
  }
 return i;
};

int     StrEmpty(char *s){
    TRACE("StrEmpty");
    for(int i=0 ;i<=strlen(s);i++)
      if( !(s[i]==' ' || s[i]==0 || s[i]=='\r' || s[i] =='\t' || s[i]=='\n' ))
            return 0;

   return 1;

};

int   StrRead(char *buf ,char *format, ...){
  TRACE("StrRead");
  int i,p[100],fp[100];
  char **ptr=&format;
  char *b=new char[strlen(buf)+1];
  char *f=new char[strlen(format)+1];
  strcpy(b,buf);
  strcpy(f,format);
  StrTrim(b);
  StrTrim(f);

  int fj=1,j=1,k=strlen(b);
  memset(p,0,sizeof(int)*100);
  memset(fp,0,sizeof(int)*100);

  // разбиваем на строки
  for(i=0;i<k ;i++){
    if(b[i]==' '){
       p[j++]=i+1;
       b[i]=0;
       if(j==100) break;
    }
  }

  k=strlen(f);
  for(i=0;i<k ;i++){
    if(f[i]==' '){
       fp[fj++]=i+1;
       f[i]=0;
       if(fj==100) break;
    }
  }

  int res,out=0;
  for(i=0;i<fj;i++){
    res=0;
    if(f[fp[i]]=='['){
      fp[i]++;
      k=strlen(&(f[fp[i]]));
      f[fp[i]+k]=0;
      for(k=0;k<j;k++){
			 if(res= sscanf(&(b[p[k]]),&(f[fp[i]]),ptr[i+1]) ) break;
      }
    }else
      res=sscanf(&(b[p[i]]),&(f[fp[i]]),ptr[i+1]);
    if(res) out++;
  }
  delete b;
  delete f;
  return out;
};


char*   PackFloat(char *d, char *s,short size){
	StrTrim(s);
	char *point=strchr(s,'.');
	char b[41];
	StrClear(b,40);
	short idx=39, digs=strlen(s);
	unsigned int i,j;
	// записали то что после точки
	if(point){
		for(i=1;i<=strlen(point);i++)
			b[idx--]=point[strlen(point)-i];
		digs-=strlen(point);
	}

	// теперь переписываем по три цифры и ставим запятую

	for(i=1;i<=digs;i+=3){
		for(j=0;j<3 && i+j <= digs;j++){
			b[idx--]=s[digs-i-j];
		}
		if(j==3)
			b[idx--]=',';
	}

	StrTrim(b);
	if((b[0]=='+' || b[0]=='-') && b[1]==','){
		b[1]=b[0];
		b[0]=' ';
		StrTrim(b);
	}
	if(b[0]==','){
    b[0]=' ';
    StrTrim(b);
  }
  StrR(b,d,size);
  return d;
};


static char    TempFileBuf[4096];
const char  *MakeTemp (const char *path,const char *ext){
 TRACE("MakeTemp");
 if (strlen(path)  > 4070) return NULL;
 SYSTEMTIME T;
 long count=0;
 while(++count < 10000){
  GetSystemTime(&T);
  if(path){
  	if(path[strlen(path)-1]=='\\')
      sprintf(TempFileBuf,"%s%04d%02d%02d%02d%02d%02d%02d.%c%c%c",
      path,T.wYear,T.wMonth,T.wDay,T.wHour,T.wMinute,T.wSecond,T.wMilliseconds,ext[0],ext[1],ext[2]);
    else
      sprintf(TempFileBuf,"%s\\%04d%02d%02d%02d%02d%02d%02d.%c%c%c",
      path,T.wYear,T.wMonth,T.wDay,T.wHour,T.wMinute,T.wSecond,T.wMilliseconds,ext[0],ext[1],ext[2]);
  }else
    sprintf(TempFileBuf,"%04d%02d%02d%02d%02d%02d%02d.%c%c%c",
    T.wYear,T.wMonth,T.wDay,T.wHour,T.wMinute,T.wSecond,T.wMilliseconds,ext[0],ext[1],ext[2]);


  HANDLE h=CreateFile(TempFileBuf,  GENERIC_READ|GENERIC_WRITE,
    0,   0,    OPEN_EXISTING,		FILE_ATTRIBUTE_NORMAL,    0   );
  if( h==INVALID_HANDLE_VALUE )
    return TempFileBuf;

  CloseHandle(h);
 }
 return NULL;
};


AnsiString StrClearAllChar(const AnsiString & s  ,char CharToErase){
  AnsiString buf;
  for(long i=1;i<=s.Length();i++){
    if( s[i]!=CharToErase)
      buf=buf+s[i];
  }
  return buf;
};

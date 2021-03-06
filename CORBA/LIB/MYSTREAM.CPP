#include "mystream.h"
#include "math.h"
#include "LZss.h"
#define PORTION_SIZE 250
#define IN_PORTION_SIZE 255
// SimpleStream
// >> and << for standart type
///////////////////////////////////////////////////////////////////////////////
SimpleStream &operator << (SimpleStream&out_data,int&P){
 out_data.Write(sizeof(int),&P);
	return out_data;
};

SimpleStream &operator << (SimpleStream&out_data,bool&P){
   out_data.Write(sizeof(bool),&P);
	return out_data;
};

SimpleStream &operator << (SimpleStream&out_data,long&P){
   //cout << endl <<"<< long" << P;
   out_data.Write(sizeof(long),&P);
	return out_data;
};
SimpleStream &operator << (SimpleStream&out_data,float&P){
   out_data.Write(sizeof(float),&P);
	return out_data;
};
SimpleStream &operator << (SimpleStream&out_data,double&P){
   out_data.Write(sizeof(double),&P);
	return out_data;
};
SimpleStream &operator << (SimpleStream&out_data,long double&P){
   out_data.Write(sizeof(long double),&P);
	return out_data;
};
SimpleStream &operator << (SimpleStream&out_data,char &P){
   out_data.Write(sizeof(long double),&P);
	return out_data;
};

SimpleStream &operator << (SimpleStream&out_data,unsigned char &P){
   out_data.Write(sizeof(unsigned char),&P);
	return out_data;
};

SimpleStream &operator << (SimpleStream & out_data, const char *P){
   //cout << endl <<"<< char*" << P;
   long z=0;
   if(!P){
     out_data.Write(sizeof(long),&z);
   }else{
   	long l = strlen(P)+1;
	   long parts=l/PORTION_SIZE;
	   if (parts*PORTION_SIZE<l ) parts++;
	   out_data.Write(sizeof(long),&parts);
	   const char *ptr=P;
	   while(l){
   	   z=(l>PORTION_SIZE)?PORTION_SIZE:l;
			out_data.Write(z,(void*)ptr);
	      ptr+=z;
   	   l-=z;
	   }
   }
	return out_data;
};

SimpleStream &operator << (SimpleStream& out_data,const AnsiString  P){
		return out_data << P.c_str();
};


SimpleStream &operator >> (SimpleStream& out_data, AnsiString & P){
    char * buf=0;
    out_data >> &buf;
    P=buf;
    delete buf;
    return out_data;
};

SimpleStream &operator >> (SimpleStream&out_data,int&P){
   P=0;
   out_data.Read(sizeof(int),&P);
	return out_data;
};
SimpleStream &operator >> (SimpleStream&out_data,bool&P){
   P=false;
   out_data.Read(sizeof(bool),&P);
	return out_data;
};

SimpleStream &operator >> (SimpleStream&out_data,long&P){
 P=0;
 out_data.Read(sizeof(long),&P);
	return out_data;
};
SimpleStream &operator >> (SimpleStream&out_data,float&P){
 P=0.0;
 out_data.Read(sizeof(float),&P);
	return out_data;
};
SimpleStream &operator >> (SimpleStream&out_data,double&P){
  P=0.0;
   out_data.Read(sizeof(double),&P);
	return out_data;
};
SimpleStream &operator >> (SimpleStream&out_data,long double&P){
  P=0.0;
  out_data.Read(sizeof(long double),&P);
	 return out_data;
};
SimpleStream &operator >> (SimpleStream&out_data,char &P){
  P=0.0;
  out_data.Read(sizeof(char),&P);
	 return out_data;
};
SimpleStream &operator >> (SimpleStream&out_data,unsigned char &P){
  P=0;
  out_data.Read(sizeof(unsigned char),&P);
	 return out_data;
};

SimpleStream &operator >> (SimpleStream&out_data,char **P){
   ifdela(*P);
   long parts=0;
   unsigned char sz;
   out_data.Read(sizeof(long),&parts);
   if(parts>0){
	   char  *buf =new char[IN_PORTION_SIZE*(parts+1)];
   	memset(buf,0,IN_PORTION_SIZE*(parts+1));
	   char *ptr=buf;
   	while(parts){
       sz=out_data.Read(IN_PORTION_SIZE,ptr);
	      ptr+=sz;
   	  parts--;
	   };
   	SetupString(buf,*P);
	   delete [] buf;
   }
   return out_data;
};

////////////////////////////////////////////////////////////////////////////////



/*
        j=i*i;
        l1=j&0xF;
        l2=(j&0xF0) >> 4;
        l3=(j&0xF00) >> 8;
        l4=(j&0xF000) >> 12;
        j= (l1<<12)|(l2)|(l3<<4)|(l4<<8);

        // from
        l1=j&0xF;
        l2=(j&0xF0) >> 4;
        l3=(j&0xF00) >> 8;
        l4=(j&0xF000) >> 12;
        j= (l1<<4)|(l2<<8)|(l3<<12)|(l4);
        k=sqrt(j);

*/


void MyMemory::LoadFromFileDecode(char * fname){
    Clear();
    int h=-1;
    h = open(fname,acRead,noPermission);
    if (h==-1) return;
    long l,s,ll;
    l=filelength(h);
    ll=l;
    char * ptr;
    unsigned char *bbb = new unsigned char[l];
    ptr=(char*)bbb;
    while(l){
     if(l>16384) s=16384; else s=l;
     l-=s;
     read(h,ptr,s);
     ptr+=s;
    }
    close(h);

    // ����������
    LZSS lz;
    DataBuffer * db =lz.Decode(bbb,ll);

    delete bbb;
    // ������������
    Buf = new char[db->Length()];

    unsigned char l1,l2,l3,l4,j,k;
    unsigned char *sptr =(unsigned char*)db->GetBuffer();
    for(long i=0;i<db->Length();i++){
        j = sptr[i];

        l1=j&0xF;
        l2=(j&0xF0) >> 4;
        j=(l1<<4)|(l2);
        j=~j;

        Buf[i]=j; //k;
    }

    BufSize=db->Length();
    Total=db->Length();
    delete db;
 };

 void MyMemory::LoadFromFile(char * fname){
    Clear();
    int h=-1;
    h = open(fname,acRead,noPermission);
    if (h==-1) return;
    long l,s;
    l=filelength(h);
    char * ptr;
    Buf = new char[l];
    BufSize=l;
    Total=l;
    ptr=Buf;
    while(l){
     if(l>16384) s=16384; else s=l;
     l-=s;
     read(h,ptr,s);
     ptr+=s;
    }
    close(h);
 };

 void MyMemory::LoadBuffer(unsigned char * Buffer,long Length){
	Clear();
	Buf= new char[Length];
  memcpy(Buf, Buffer,Length);
	BufSize=Length;
	Total=Length;
 };
 void MyMemory::SaveToFile(char * fname){
    int h=-1;
    h = open(fname,acWrite,noPermission);
    if (h==-1) return;
    long l=BufSize,s;
    char *ptr;
    ptr=Buf;
    while(l){
     if(l>16384) s=16384; else s=l;
     l-=s;
     write(h,ptr,s);
     ptr+=s;
    }
    close(h);
 };
void MyMemory::SaveToFileEncode(char * fname){
    int h=-1;
    h = open(fname,acWrite,noPermission);
    if (h==-1) return;
    long l=BufSize,s;


    // �����������
    unsigned char * bbb = new unsigned char [l];
    unsigned char l1,l2,j;
    for(long i=0;i<l;i++){
        j=Buf[i];
        l1=j&0xF;
        l2=(j&0xF0) >> 4;
        j=(l1<<4)|(l2);
        j=~j;
        bbb[i]=j ;
    }

    // ��������
    LZSS lz;
    DataBuffer *db= lz.Encode(bbb,l);

    delete bbb;
    char *ptr;
    ptr=(char*)db->GetBuffer();
    l=db->Length();
    while(l){
     if(l>16384) s=16384; else s=l;
     l-=s;
     write(h,ptr,s);
     ptr+=s;
    }
    close(h);
    delete db;
 };


unsigned char MyMemory::Write(unsigned char Len, void * buf){
  if(BufSize+Len+1>Total){
	  while(BufSize+Len+1>Total){
	  	Total+=Delta;
	  }
     //cout << endl <<"MemoryBufferSize is "<<Total;
     char *Buf2 = new char[Total];
     memset(Buf2,0,Total);
     if(BufSize) memcpy(Buf2,Buf,BufSize);
     ifdela(Buf);
     Buf=Buf2;
  }
  Buf[WritePtr++]=Len;
  memcpy(Buf+WritePtr,buf,Len);
  WritePtr+=Len;
  BufSize+=Len+1;

  return Len;
};

unsigned char MyMemory::Read(unsigned char Len, void * buf){
   if(Len ){
      unsigned char sz,ss;
      memset(buf,0,Len);
      if(BufSize>ReadPtr)
      	sz=Buf[ReadPtr++];
      else
      {
        return 0;
      }
		if(sz + ReadPtr > BufSize) sz=BufSize-ReadPtr;
      if(sz>Len) ss=Len; else ss=sz;
      memcpy(buf,Buf+ReadPtr,ss);
      ReadPtr+=sz;
      return ss;
    }
    return 0;
 };



unsigned char MyFile::Write(unsigned char Len, void * buf){
  if(h>=0 && a!=acRead){
      write(h,&Len,1);
      write(h,buf,Len);
  }
  return Len;
};

unsigned char MyFile::Read(unsigned char Len, void * buf){
   long i=0;
   if(h>=0 && Len && a==acRead){
      unsigned char sz=0;
      if(read(h,&sz,1)!=1){
        return 0;
      }
      i=(Len>sz)? sz:Len;
      i=read(h,buf,i);
      sz-=i;
      if(sz)
        lseek(h, sz,SEEK_CUR);
    }
    return i;
 };


SimpleStream &operator >> (SimpleStream& out_data, TDateTime &P){
	double d;
	out_data>>d;
	P=d;
	return out_data;
};
SimpleStream &operator << (SimpleStream& out_data, TDateTime &P){
	return out_data <<(double) P;
};

bool StreamSavedFile::PathOnly=true;

SimpleStream &operator << (SimpleStream& stream, StreamSavedFile& F){
    int h=-1;
    long l,s,L=-1;
    unsigned char u;
    char ptr[PORTION_SIZE];
    stream << F.Path; // path

    if(F.PathOnly){
     stream << L; // -1!
     return stream;
    }

    h = open(F.Path.c_str(),acRead,noPermission);
    if (h==-1)
     l=0;
    else
     l=filelength(h);
    L=l;
    stream << L;

    while(l){
     if(l>PORTION_SIZE) u=PORTION_SIZE; else u=l;
     l-=u;
     if(h>=0) read(h,ptr,u);
     stream.Write(u,ptr);
    }

    if(h>=0)close(h);
    return stream;
 };


SimpleStream &operator >> (SimpleStream& stream, StreamSavedFile& F){
    int L;
    long l;
    unsigned char u;
    char  ptr[IN_PORTION_SIZE];
    int h=-1;

    stream >> F.Path; // path

    stream >> L;

    if(!F.PathOnly && L >0) h = open(F.Path.c_str(),acWrite,noPermission);
    l=L;
    if(L<0) l=0;

    while(l){
     if(l>IN_PORTION_SIZE) u=IN_PORTION_SIZE; else u=l;
     u=stream.Read(u,ptr);
     l-=u;
     if(h>=0) write(h,ptr,u);
    }
    if(h>=0) close(h);

    return stream;
};



SimpleStream &operator << (SimpleStream& stream, DataBuffer& F){

    long l;
    unsigned char u,*bptr;
    bptr = F.Buffer;
    l=F.BufSize;
    stream << l;

    while(l){
     if(l>PORTION_SIZE) u=PORTION_SIZE; else u=l;
     stream.Write(u,bptr);
     l-=u;
     bptr+=u;
    }

    return stream;
 };


SimpleStream &operator >> (SimpleStream& stream, DataBuffer& F){
    long l;
    unsigned char u;
    char  ptr[IN_PORTION_SIZE];
    unsigned char *bptr;

    stream >> l;

    F.SetBuffer(l,0);
    bptr=F.Buffer;

    while(l){
     if(l>IN_PORTION_SIZE) u=IN_PORTION_SIZE; else u=l;
     u=stream.Read(u,ptr);
     memcpy(bptr,ptr,u);
     bptr+=u;
     l-=u;

    }

    return stream;
};


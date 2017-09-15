#ifndef MYSTREAM_H
#define MYSTREAM_H

#include "macro.h"
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#include <limits.h>
#include <vcl.h>


// SimpleStream
class SimpleStream{
 public :
   virtual unsigned char Write(unsigned char Len, void * buf){return 0;};
   virtual unsigned char Read(unsigned char Len, void * buf){return 0;};
};

// >> and << for standart type
///////////////////////////////////////////////////////////////////////////////
SimpleStream &operator << (SimpleStream& out_data, bool& P);
SimpleStream &operator << (SimpleStream& out_data, int& P);
SimpleStream &operator << (SimpleStream& out_data, long& P);
SimpleStream &operator << (SimpleStream& out_data, float& P);
SimpleStream &operator << (SimpleStream& out_data, double& P);
SimpleStream &operator << (SimpleStream& out_data, long double& P);
SimpleStream &operator << (SimpleStream& out_data, char &P);
SimpleStream &operator << (SimpleStream& out_data, unsigned char & P);
SimpleStream &operator << (SimpleStream& out_data, const char *P);
SimpleStream &operator >> (SimpleStream& out_data, bool& P);
SimpleStream &operator >> (SimpleStream& out_data, int& P);
SimpleStream &operator >> (SimpleStream& out_data, long& P);
SimpleStream &operator >> (SimpleStream& out_data, float& P);
SimpleStream &operator >> (SimpleStream& out_data, double& P);
SimpleStream &operator >> (SimpleStream& out_data, long double& P);
SimpleStream &operator >> (SimpleStream& out_data, char & P);
SimpleStream &operator >> (SimpleStream& out_data, unsigned char & P);
SimpleStream &operator >> (SimpleStream& out_data, char **P);
SimpleStream &operator >> (SimpleStream& out_data, AnsiString & P);
SimpleStream &operator << (SimpleStream& out_data, const AnsiString P);
SimpleStream &operator >> (SimpleStream& out_data, TDateTime &P);
SimpleStream &operator << (SimpleStream& out_data, TDateTime &P);


////////////////////////////////////////////////////////////////////////////////

// MyFile
enum Access{
	 acRead = O_RDONLY|O_BINARY,
	 acWrite = O_WRONLY|O_CREAT|O_TRUNC|O_BINARY,
	 acAppend =O_WRONLY|O_APPEND|O_BINARY
};
enum Permission {
		noAccess =0,
		noRead = S_IWRITE,
		noWrite = S_IREAD,
		noPermission = S_IREAD|S_IWRITE
};




class MyMemory: public SimpleStream{
	char * Buf;
	long BufSize;
	long ReadPtr;
	long WritePtr;
	 int Delta;
	 long Total;
public:
	MyMemory(int _Delta=4096){
	 Buf=0;
	 BufSize=0;
	 ReadPtr=0;
	 WritePtr=0;
	 Total=0;
	 Delta=_Delta;
	 };
	 ~MyMemory(){
		Clear();
			//cout << endl <<"~MyMemory";
	 }
 virtual unsigned char Write(unsigned char Len, void * buf);
 virtual unsigned char Read(unsigned char Len, void * buf);

 void LoadFromFile(char * fname);

 void SaveToFile(char * fname);

 void SetDelta( int _Delta){Delta=_Delta;};

 void Clear(){
	ifdela(Buf);
	 BufSize=0;
	 ReadPtr=0;
	 WritePtr=0;
	 Total=0;
 };
};

class MyFile:  public SimpleStream {
 int h;
 Access a;
 Permission p;
 char *fname;

public:

 MyFile(char * name,Access access ,Permission  permission =noPermission){
			h=-1;
			a=access;
			p=permission;
			fname=0;
			SetupString(name,fname);
			h=open(name,access,permission);
			//if(h==-1)
			//cerr<<"File Not opened!!!" << endl;
 };

 ~MyFile( ){
	 if (h!=-1) close(h);
	 ifdela(fname);
	 //cout <<endl<<"~MyFile";
 };

 unsigned char GetNextSize(){
			unsigned char sz;
		 if(h>=0 && a==acRead){
			read(h,&sz,1);
			lseek(h,-1,SEEK_CUR);
		 }
		 return sz;
 };

 virtual unsigned char Write(unsigned char Len, void * buf);
 virtual unsigned char Read(unsigned char Len, void * buf);
};


class StreamSavedFile{
public:
 static bool PathOnly;
   AnsiString Path;
   StreamSavedFile(AnsiString _Path=""){
    Path=_Path;
   };
   friend SimpleStream &operator << (SimpleStream& out_data, StreamSavedFile& F);
   friend SimpleStream &operator >> (SimpleStream& out_data, StreamSavedFile& F);
};


////////////////////////////////////////////////////////////////////////////////


#endif

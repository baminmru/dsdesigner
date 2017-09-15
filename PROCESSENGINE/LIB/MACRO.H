#ifndef MACRO_H
#define MACRO_H

#define DebugVersion 1
#ifndef prolog
#define prolog
#endif

#define ifdel(s)  \
if ( s) delete s; \
s = 0;

#define ifdela(s)    \
if ( s) delete [] s; \
s = 0;

#define SetupString(s,d) 		  \
{										  \
	 if(s){                       \
	d = new char[strlen((char*)s)+1];   \
	strcpy((char*)d,(char*)s);                 \
} else  d=0;}


#define SetupString2(s,d) 		  \
{										  \
	 ifdela(d);                   \
	 if(s){                       \
	d = new char[strlen((char*)s)+1];   \
	strcpy((char*)d,(char*)s);                 \
} else  d=0;}


void prolog Debug(char *format,...);

#define TRACE(x) Debug("\n %s %d>>>%s\n",__FILE__,__LINE__,x);



#define Min(x,y) ((x)>(y)?(y):(x))
#define Max(x,y) ((x)>(y)?(x):(y))


#define STREAM_ARRAY_DEF(T,NewName)  typedef MyArray<T> NewName;
#define STREAM_LIST_DEF(T,NewName) typedef MyList<T> NewName;
#define STREAM_ARRAY_H(T)
#define STREAM_ARRAY(T)
#define STREAM_LIST_H(T)
#define STREAM_LIST(T)

/*
#define STREAM_ARRAY_DEF(T,NewName) SimpleStream & operator << (SimpleStream & s, MyArray<T>& x);\
SimpleStream & operator >> (SimpleStream & s, MyArray<T>& x); \
typedef MyArray<T> NewName;



#define STREAM_LIST_DEF(T,NewName) SimpleStream & operator << (SimpleStream & s, MyList<T>& x);  \
SimpleStream & operator >> (SimpleStream & s, MyList<T>& x);					                           \
typedef MyList<T> NewName;


#define STREAM_ARRAY_H(T) SimpleStream & operator << (SimpleStream & s, MyArray<T>& x); \
SimpleStream & operator >> (SimpleStream & s, MyArray<T>& x);


#define STREAM_LIST_H(T) SimpleStream & operator << (SimpleStream & s, MyList<T>& x); \
SimpleStream & operator >> (SimpleStream & s, MyList<T>& x);



#define STREAM_ARRAY(T) SimpleStream & operator << (SimpleStream & s, MyArray<T>& x){  \
	s << x.Count();                                                    \
	for(long i=0;i< x.Count();i++){                                     \
		s << *(x.Get(i));                                                  \
	}                                                                  \
	return s;                                                          \
};                                                                   \
																																		 \
SimpleStream & operator >> (SimpleStream & s, MyArray<T>& x){				 \
	long cnt;                                                          \
	s >> cnt;                                                          \
	x.Clear();                                                         \
	for(long i=0;i<cnt;i++){                                           \
		x.Add(new T());                                                  \
		s >> *(x.Get(i));                                                \
	}                                                                  \
	return s;                                                          \
};


#define STREAM_LIST(T) SimpleStream & operator << (SimpleStream & s, MyList<T>& x){  \
	s << x.Count();                                                    \
	for(long i=0;i<x.Count();i++){                                     \
		s << x.Name(i) << *(x.GetI(i));                                  \
	}                                                                  \
	return s;                                                          \
};                                                                   \
																																		 \
SimpleStream & operator >> (SimpleStream & s, MyList<T>& x){				 \
	long cnt;  AnsiString Name;                                        \
	s >> cnt;                                                          \
	x.Clear();                                                         \
	for(long i=0;i<cnt;i++){                                           \
		s >> Name;																											 \
		x.Add(new T(),Name);                                             \
		s >> *(x.GetI(i));                                               \
	}                                                                  \
	return s;                                                          \
};
*/


#define Sortable(X) \
  friend bool operator == (const X & s, const X& x); \
  friend bool operator < (const X & s, const X& x);


#define Streamable(X)   \
  friend SimpleStream & operator >> (SimpleStream & s, X& x); \
  friend SimpleStream & operator << (SimpleStream & s, X& x);


#define Streamable1(X,f1)   \
  SimpleStream & operator >> (SimpleStream & s, X& x){ \
    s >> x.f1;  \
    return s;  \
  }; \
  SimpleStream & operator << (SimpleStream & s, X& x){ \
    s << x.f1;  \
    return s;  \
  };


#define Streamable2(X,f1,f2)   \
  SimpleStream & operator >> (SimpleStream & s, X& x){ \
    s >> x.f1;  \
    s >> x.f2;  \
    return s;  \
  }; \
  SimpleStream & operator << (SimpleStream & s, X& x){ \
    s << x.f1;  \
    s << x.f2;  \
    return s;  \
  };

#define Streamable3(X,f1,f2,f3)   \
  SimpleStream & operator >> (SimpleStream & s, X& x){ \
    s >> x.f1;  \
    s >> x.f2;  \
    s >> x.f3;  \
    return s;  \
  }; \
  SimpleStream & operator << (SimpleStream & s, X& x){ \
    s << x.f1;  \
    s << x.f2;  \
    s << x.f3;  \
    return s;  \
  };

#define Streamable4(X,f1,f2,f3,f4)   \
  SimpleStream & operator >> (SimpleStream & s, X& x){ \
    s >> x.f1;  \
    s >> x.f2;  \
    s >> x.f3;  \
    s >> x.f4;  \
    return s;  \
  }; \
  SimpleStream & operator << (SimpleStream & s, X& x){ \
    s << x.f1;  \
    s << x.f2;  \
    s << x.f3;  \
    s << x.f4;  \
    return s;  \
  };


#define Streamable5(X,f1,f2,f3,f4,f5)   \
  SimpleStream & operator >> (SimpleStream & s, X& x){ \
    s >> x.f1;  \
    s >> x.f2;  \
    s >> x.f3;  \
    s >> x.f4;  \
    s >> x.f5;  \
    return s;  \
  }; \
  SimpleStream & operator << (SimpleStream & s, X& x){ \
    s << x.f1;  \
    s << x.f2;  \
    s << x.f3;  \
    s << x.f4;  \
    s << x.f5;  \
    return s;  \
  };


#define Streamable6(X,f1,f2,f3,f4,f5,f6)   \
  SimpleStream & operator >> (SimpleStream & s, X& x){ \
    s >> x.f1;  \
    s >> x.f2;  \
    s >> x.f3;  \
    s >> x.f4;  \
    s >> x.f5;  \
    s >> x.f6;  \
    return s;  \
  }; \
  SimpleStream & operator << (SimpleStream & s, X& x){ \
    s << x.f1;  \
    s << x.f2;  \
    s << x.f3;  \
    s << x.f4;  \
    s << x.f5;  \
    s << x.f6;  \
    return s;  \
  };


#define Streamable7(X,f1,f2,f3,f4,f5,f6,f7)   \
  SimpleStream & operator >> (SimpleStream & s, X& x){ \
    s >> x.f1;  \
    s >> x.f2;  \
    s >> x.f3;  \
    s >> x.f4;  \
    s >> x.f5;  \
    s >> x.f6;  \
    s >> x.f7;  \
    return s;  \
  }; \
  SimpleStream & operator << (SimpleStream & s, X& x){ \
    s << x.f1;  \
    s << x.f2;  \
    s << x.f3;  \
    s << x.f4;  \
    s << x.f5;  \
    s << x.f6;  \
    s << x.f7;  \
    return s;  \
  };

#define Streamable8(X,f1,f2,f3,f4,f5,f6,f7,f8)   \
  SimpleStream & operator >> (SimpleStream & s, X& x){ \
    s >> x.f1;  \
    s >> x.f2;  \
    s >> x.f3;  \
    s >> x.f4;  \
    s >> x.f5;  \
    s >> x.f6;  \
    s >> x.f7;  \
    s >> x.f8;  \
    return s;  \
  }; \
  SimpleStream & operator << (SimpleStream & s, X& x){ \
    s << x.f1;  \
    s << x.f2;  \
    s << x.f3;  \
    s << x.f4;  \
    s << x.f5;  \
    s << x.f6;  \
    s << x.f7;  \
    s << x.f8;  \
    return s;  \
  };


#define Streamable9(X,f1,f2,f3,f4,f5,f6,f7,f8,f9)   \
  SimpleStream & operator >> (SimpleStream & s, X& x){ \
    s >> x.f1;  \
    s >> x.f2;  \
    s >> x.f3;  \
    s >> x.f4;  \
    s >> x.f5;  \
    s >> x.f6;  \
    s >> x.f7;  \
    s >> x.f8;  \
    s >> x.f9;  \
    return s;  \
  }; \
  SimpleStream & operator << (SimpleStream & s, X& x){ \
    s << x.f1;  \
    s << x.f2;  \
    s << x.f3;  \
    s << x.f4;  \
    s << x.f5;  \
    s << x.f6;  \
    s << x.f7;  \
    s << x.f8;  \
    s << x.f9;  \
    return s;  \
  };


#endif
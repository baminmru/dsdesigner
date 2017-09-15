#include "bamilib.h"

SimpleStream & operator >> (SimpleStream & s,FieldKinds &c){
	int d;
	s>>d;
	c=(FieldKinds)d;
	return s;
};
SimpleStream & operator << (SimpleStream & s,FieldKinds &c){
	s<<(int)c;
	return s;
};


SimpleStream & operator >> (SimpleStream & s,OutRectAlignmentStyle &c){
	int d;
	s>>d;
	c=(OutRectAlignmentStyle)d;
	return s;
};
SimpleStream & operator << (SimpleStream & s,OutRectAlignmentStyle &c){
	s<<(int)c;
	return s;
};


SimpleStream & operator >> (SimpleStream & s,TFont &f){
 long clr,pitch;
 bool _fsBold,_fsItalic ,_fsUnderline,_fsStrikeout;
 AnsiString Name;

 s >> f.Charset >> clr >> f.Height >> Name >>pitch >>f.Size>>
 _fsBold >>_fsItalic>>_fsUnderline>>_fsStrikeout;
 f.Style.Clear();
 f.Name=Name;

 if(_fsBold) f.Style << fsBold;
 if(_fsItalic) f.Style << fsItalic;
 if(_fsUnderline) f.Style << fsUnderline;
 if(_fsStrikeout) f.Style << fsStrikeOut;
 f.Pitch=(TFontPitch) pitch;
 f.Color=(TColor) clr;

 return s;
};

SimpleStream & operator << (SimpleStream & s,TFont &f){
 long clr,pitch;
 bool _fsBold=0,_fsItalic=0 ,_fsUnderline=0,_fsStrikeout=0;
 if( f.Style.Contains(fsBold)) _fsBold =true;
 if(f.Style.Contains(fsItalic)) _fsItalic=true;
 if(f.Style.Contains(fsUnderline)) _fsUnderline=true;
 if(f.Style.Contains(fsStrikeOut)) _fsStrikeout=true;

 s << f.Charset << (long)f.Color << f.Height << f.Name << (long)f.Pitch << f.Size <<
 _fsBold << _fsItalic << _fsUnderline << _fsStrikeout;

 return s;
};


SimpleStream & operator >> (SimpleStream & s,GridCellStyle &c){
	return s >> *c.font >> 	c.WordBreak >> c.TextAlignment;
};

SimpleStream & operator << (SimpleStream & s,GridCellStyle &c){
	return s << *c.font << 	c.WordBreak << c.TextAlignment;
};



SimpleStream & operator >> (SimpleStream & s,EditControlType &c){
	int d;
	s>>d;
	c=(EditControlType)d;
	return s;
};

SimpleStream & operator << (SimpleStream & s,EditControlType &c){
  s<<(int)c;
	return s;
};



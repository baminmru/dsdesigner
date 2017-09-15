#ifndef STYLES_H
#define STYLES_H

enum FieldKinds{
	 EmptyField, StaticField,	 StreamField,	 FormulaField,  SubReportField
};


SimpleStream & operator >> (SimpleStream & s,FieldKinds &c);
SimpleStream & operator << (SimpleStream & s,FieldKinds &c);

enum EditControlType {
	ectNone,
	ectInteger,
	ectNumeric,
	ectText,
	ectMultiLineText,
	ectPassword,
	ectDate,
	ectTime,
	ectDateTime,
	ectYesNo,
	ectSubStream,
 	ectStreamGrid /* only for multiselection filter */
	//ectComboBox,
	//ectStatic,
	//ectImageFile,
	//ectImage
	//ectMultiLineTextFile,
	//ectListBox,

};


SimpleStream & operator >> (SimpleStream & s,EditControlType &c);
SimpleStream & operator << (SimpleStream & s,EditControlType &c);


enum OutRectAlignmentStyle {
	saTopLeft,		saTopCenter,		saTopRight,
	saCenterLeft,	saCenterCenter,	saCenterRight,
	saBottomLeft,	saBottomCenter,	saBottomRight
};

SimpleStream & operator >> (SimpleStream & s,OutRectAlignmentStyle &c);
SimpleStream & operator << (SimpleStream & s,OutRectAlignmentStyle &c);



enum OutRectLineStyle{
	lsNone, lsSingle,lsDouble,ls3d
};


SimpleStream & operator >> (SimpleStream & s,Graphics::TFont &f);
SimpleStream & operator << (SimpleStream & s,Graphics::TFont &f);


struct GridCellStyle{
	Graphics::TFont * font;
	bool WordBreak;
	OutRectAlignmentStyle TextAlignment;
	friend SimpleStream & operator >> (SimpleStream & s,GridCellStyle &c);
	friend SimpleStream & operator << (SimpleStream & s,GridCellStyle &c);
	GridCellStyle(){
		font = new Graphics::TFont();
	};
	~GridCellStyle(){
		delete font;
	};

};



class ProcCursor{
   TCursor Save_Cursor;

public:
    ProcCursor(){
      Save_Cursor = Screen->Cursor;
      Screen->Cursor = crHourGlass;
   };

   ~ProcCursor(){
      Screen->Cursor = Save_Cursor;
   };

};


#endif

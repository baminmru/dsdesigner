//---------------------------------------------------------------------------
#ifndef StreamEditorDefH
#define StreamEditorDefH
//---------------------------------------------------------------------------

// Группировка в StreamGrid
//////////////////////////////////////////////////////////////////////////////
class OutRectGroup {
	TokenList *t;
	Labels *l;

	public:
		AnsiString Condition;
		AnsiString Evaluate(MiniParser *p);
		OutRectGroup( AnsiString _GroupExpression ="");
		~OutRectGroup(){
			ifdel(t);
			ifdel(l);
		};

		friend SimpleStream & operator >> (SimpleStream & s,OutRectGroup &g);
		friend SimpleStream & operator << (SimpleStream & s,OutRectGroup &g);

};


STREAM_LIST_DEF(OutRectGroup, Sections);


struct DoubleBool{
	bool Befor;
	bool After;
	DoubleBool(bool B=false, bool A = false){
		Befor=B;
		After=A;
	};
	friend SimpleStream & operator >> (SimpleStream & s,DoubleBool &d );
	friend SimpleStream & operator << (SimpleStream & s,DoubleBool &d);

};


class DoubleBools: public MyArray<DoubleBool>{
 public:
	bool GetAfter(long i){
		DoubleBool * ptr=Get(i);
		if(ptr)
			return ptr->After;
		else
			return false;
	};
		bool GetBefor(long i){
		DoubleBool * ptr=Get(i);
		if(ptr)
			return ptr->Befor;
		else
			return false;
	}

	void SetAfter(long i, bool Val){
		DoubleBool * ptr=Get(i);
		if(ptr)
			ptr->After=Val;
	};
	void SetBefor(long i, bool Val){
		DoubleBool * ptr=Get(i);
		if(ptr)
			ptr->Befor=Val;
	};
};


// Связка между двумя потоками (Master-Slave / StreamCombo )
//////////////////////////////////////////////////////////////////////////////////////////


struct EditFieldChanges{
	AnsiString ParamName;    // имя параметра подпотока (или поля для defaults)
	bool SetAsValue;         // признак что делать задавать константу или значение одного из полей
	AnsiString ValueOrName;  // константа или имя поля

	EditFieldChanges(AnsiString _ParamName="", AnsiString _ValueOrName="",bool _SetAsValue=false){
		ParamName=_ParamName;
		ValueOrName=_ValueOrName;
		SetAsValue=_SetAsValue;
	}

	friend SimpleStream & operator >> (SimpleStream & s,EditFieldChanges &x);
	friend SimpleStream & operator << (SimpleStream & s,EditFieldChanges &x);
};



enum FieldAccessType{
	faNone,
	faRead,
	faWrite
};

SimpleStream & operator >> (SimpleStream & s, FieldAccessType &ft);
SimpleStream & operator << (SimpleStream & s,FieldAccessType &ft);


struct EditFieldDisables{
	AnsiString FieldName;										 // поле
	FieldAccessType InsertAccess;            // какой будет доступ при вставке
	FieldAccessType EditAccess;              // какой будет доступ при изменении
	EditFieldDisables(    AnsiString _FieldName="",
												FieldAccessType  _AccessOnInsert=faNone,
												FieldAccessType _AccessOnEdit=faNone
									 ){
		FieldName=_FieldName;
		InsertAccess=_AccessOnInsert;
		EditAccess=_AccessOnEdit;
	}

	friend SimpleStream & operator >> (SimpleStream & s,EditFieldDisables& d);
	friend SimpleStream & operator << (SimpleStream & s,EditFieldDisables& d);

};


STREAM_ARRAY_DEF(EditFieldDisables,EditFieldsDisables);
STREAM_ARRAY_DEF(EditFieldChanges, EditFieldsChanges);

// что надо делать чтобы поток выглядел как надо
struct EditFieldActions{
		// изменение базового запроса
		AnsiString SubCondition;			  // расширение запроса
		EditFieldsChanges Changes;	          // параметры

		// изменение значений при вставке новой записи
		EditFieldsChanges DefaultChanges;    // умолчания

		// запрет (или разрешение) доступа к полям
		EditFieldsDisables Disables;
    bool NotEmpty(){
         return !SubCondition.IsEmpty() ||
         Changes.Count() ||
         DefaultChanges.Count() ||
         Disables.Count();
    };

		friend SimpleStream & operator >> (SimpleStream & s,EditFieldActions &x);
		friend SimpleStream & operator << (SimpleStream & s,EditFieldActions &x);

};





// связи типа Master - Detail
struct StreamEditorLink{
	AnsiString Caption;
	AnsiString MasterName;                      // откуда
	AnsiString SlaveName;                       // куда
	AnsiString MasterCondition;					        // условие при котормом связь имеет смысл
  AnsiString SlaveCaption;					          // новый заголовок для подчиненного редактора
	EditFieldActions WhenChanged;               // правило задания параметров подпотока при инициализации

	friend SimpleStream & operator >> (SimpleStream & s,StreamEditorLink &x);
	friend SimpleStream & operator << (SimpleStream & s,StreamEditorLink &x);
};

// мешок связей типа Master - Detail
STREAM_LIST_DEF(StreamEditorLink,StreamEditorLinks);


// Форма редактирования
//////////////////////////////////////////////////////////////////////////////////////////


// атрибуты поля (используются для редактирования (сортировки и фильтрации)
struct EditableFieldAttr{

	AnsiString FieldName;												// поле
	AnsiString Label;														// метка на форме
	EditControlType ControlType;                // тип контрольного элемента

	FieldAccessType DefInsertAccess;            // доступ при вставке
	FieldAccessType DefEditAccess;              // доступ при редактировании


	// для заполнения
	EditFieldActions *WhenChanged;              // правило задания параметров подпотока при инициализации
	AnsiString StreamName;											// имя подпотока
	AnsiString SyncField;                       // поле для синхронизации
	AnsiString ShowField;												// поле для отображения

	// актуально только для подпотока ???
	bool FillOnGetFocus;                        // инициализировать при получении фокуса
	bool FillOnFormShow;                        // инициализировать при показе формы


	// некоторые данные для правильного редактирования
	bool AllowNull;
	StackType SQLType;
	long MaxFieldLen;


	friend SimpleStream & operator >> (SimpleStream & s,EditableFieldAttr &x);
	friend SimpleStream & operator << (SimpleStream & s,EditableFieldAttr &x);


	// текущие данные
	EditableFieldAttr(    AnsiString _FieldName="",
												AnsiString _Label="",
												EditControlType _ControlType=ectText,
												FieldAccessType  _AccessOnInsert=faWrite,
												FieldAccessType _AccessOnEdit=faWrite
									 ){
		FieldName=_FieldName;
		Label=_Label;
		ControlType=_ControlType;
		DefInsertAccess=_AccessOnInsert;
		DefEditAccess=_AccessOnEdit;
		WhenChanged=0;
		AllowNull=true;
 };

 void SetAccess(FieldAccessType InsertAccess,FieldAccessType EditAccess){
	DefInsertAccess=InsertAccess;
	DefEditAccess=EditAccess;
 };


 void SetStream(
							 AnsiString _StreamName,
							 AnsiString _SyncField,
							 AnsiString _ShowField,
							 EditFieldActions *_WhenChanged,
							 bool _FillOnGetFocus=false,
							 bool _FillOnFormShow=false
							 ){
		StreamName=_StreamName;
		SyncField=_SyncField;
		ShowField=_ShowField;
		FillOnGetFocus=_FillOnGetFocus;
		FillOnFormShow=_FillOnFormShow;
 };
};


// определение колонок для просмотра в DSView!!!
class ColumnDef{

	// несколько runtime-only  переменных для оптимизации
	// Для формул
	TokenList *t;
	Labels *l;

	// для статических полей
	StackValue *sv;

	// для номера группы ( или номера поля)
	long GroupID;


 public:

	bool  Visible;    // для исключения полей
	AnsiString Value; // Имя поля / формула / текст в зависимости от типа поля
	FieldKinds      FieldKind; // тип поля (статика, из потока данных, формула)

	AnsiString Style;			// Font + Alignment

	long Width; // ширина ячейки

	AnsiString Caption; 	// заголовок и в какой он будет строке
	long HeaderVert;

	long Vert;  // позиция по вертикали в выходной секции
	long Horiz; // позиция по горизонтали (и заголовок и выходная секция)


	AnsiString      GroupName;   	// привязка к группе
	bool           BeforChanged;  // до изменения значения или после

	friend SimpleStream & operator >> (SimpleStream & s,ColumnDef & x);
	friend SimpleStream & operator << (SimpleStream & s,ColumnDef & x);


	~ColumnDef(){
		ifdel(t);
		ifdel(l);
		ifdel(sv);
	}

	void SetHeader( AnsiString  _Caption, long _HeaderVert=0){
		Caption=_Caption;
		HeaderVert=_HeaderVert;
	};

	void SetField( FieldKinds _FieldKind , AnsiString _Value, long _Width=64){
		FieldKind=_FieldKind;
		Value=_Value;
		Width=_Width;
	};

	void SetPosition(long _Horiz, long _Vert=0){
			Horiz=_Horiz;
			Vert=_Vert;
	};

	void SetGroup(AnsiString _GroupName, bool _BeforChanged){
		GroupName=_GroupName;
		BeforChanged=_BeforChanged;
	};

	void InitExpression(MiniParser * Parser){
		if(FieldKind!=FormulaField) return;
		if(!t){
			t=new TokenList();
			l= new Labels;
		}
		Parser->CompileExpression(Value,t,l);
	};

	AnsiString ExecExpression(MiniParser * Parser){
		if(FieldKind!=FormulaField) return "";
		if(!t){
			t=new TokenList();
			l= new Labels;
			Parser->CompileExpression(Value,t,l);
		}
		return Parser->ParseExpression(t,l);
	};

	TokenList *GetTokens(){return t;};
	Labels *GetLabels(){return l;};
	StackValue *GetStatic(){ return sv;};
	StackValue* InitStatic(){
		if(!sv)
			sv=new StackValue(Value);
		return sv;
	};
	long GetGroupID(){return GroupID;};
	void SetGroupID(long ID){ GroupID=ID;};

	ColumnDef(){
		Width=0; 		Vert=0;  		Horiz=0; 		HeaderVert=0;
		GroupID=0; 		FieldKind=EmptyField; 		t=0; 		l=0; 		sv=0;
		Visible = true;
	};

	ColumnDef(FieldKinds _FieldKind , AnsiString _Value,
			long _Width=64,long _Horiz=0, long _Vert=0,
		AnsiString  _Caption="", long _HeaderVert=0){
		Width=0;  Vert=0; 	Horiz=0; 		HeaderVert=0;
		t=0; 		l=0; 		sv=0; 		GroupID=0;

		SetField(  _FieldKind , _Value,  _Width);
		SetPosition( _Horiz,  _Vert);
		SetHeader(   _Caption,  _HeaderVert);
	};

};

STREAM_ARRAY_H(ColumnDef);
STREAM_ARRAY_DEF(EditableFieldAttr,EditFieldsAttr);
STREAM_ARRAY_H(AnsiString);


struct StreamIndex{
	     AnsiString Caption;
      AnsiString OrderBy;
   	 friend SimpleStream & operator >> (SimpleStream & s,StreamIndex &x);
	     friend SimpleStream & operator << (SimpleStream & s,StreamIndex &x);
};

STREAM_ARRAY_DEF(StreamIndex,StreamIndexes);


class DataManager;

class StreamEditorDef{
		DataManager *Owner;

 public:

	// название объекта для заголовков
	AnsiString Caption;

	// соединенице ...
	AnsiString ConnectionName;

	// полное определение потока
	AnsiString SQL;
	AnsiString ROWSQL;
	AnsiString INSERT;
	AnsiString UPDATE;
	AnsiString sDELETE;
	MyArray<AnsiString> ParamNames;			// только имена параметров
	Params Defaults;
	AnsiString SubCondition;
	AnsiString OrderBy;                 // Default Index
 StreamIndexes Indexes;


private:

public:

	// получить поток согласно определению или 0, если не нашли Connect
	DataStream *GetDataStream(AnsiString *SubCondition,AnsiString *OrderBy);

	// описание полей формы редактирования
	EditFieldsAttr FormDef;


	// описание колоночек для просмотра
	MyArray<ColumnDef> ColDef;

	// описание групп для просмотров
	Sections Groups;


	friend SimpleStream & operator >> (SimpleStream & s,StreamEditorDef &x);

	friend SimpleStream & operator << (SimpleStream & s,StreamEditorDef &x);



	friend class DataManager;
};



struct StreamTransaction{
	     AnsiString Caption;
      AnsiString EditorName;
      AnsiString Message;
      EditFieldsAttr FormDef;
      int TransactionSource;
      /*
      0 - New Row (Nothing)
      1 - Current Row
      2 - AllSelected Row
      3 - Selected Or Current
      */
      int AfterTransaction;
      /*
      0 - Nothing
      1 - Refresh Current Row
      2 - Refresh All
      3 - Delete Row
      */
      AnsiString TranCondition;
      AnsiString Transaction;
	friend SimpleStream & operator >> (SimpleStream & s,StreamTransaction &x);

	friend SimpleStream & operator << (SimpleStream & s,StreamTransaction &x);
};


STREAM_ARRAY_DEF(StreamTransaction,StreamTransactions);

#endif

#ifndef _ADMIN_H_
#define _ADMIN_H_
#include "bamilib.h"
#include "Task.h"



// пользователи системы
struct User{
  AnsiString Name;
  AnsiString Password;
  bool IsAdmin;         // может менять описание

  friend SimpleStream & operator >>(SimpleStream &,User & F);
  friend SimpleStream & operator<<(SimpleStream &,User & F);

};

// Группы пользователей
struct Group{
  AnsiString Name;
  MyArray<AnsiString> Users;

  friend SimpleStream & operator >>(SimpleStream &,Group & F);
  friend SimpleStream & operator<<(SimpleStream &,Group & F);
};


// Описание параметра картиджа
struct CartrigeParam{
  AnsiString Name;
  bool AllowNull;

  friend SimpleStream & operator >>(SimpleStream &,CartrigeParam & F);
  friend SimpleStream & operator<<(SimpleStream &,CartrigeParam & F);

};

// Информация о картридже
struct Cartridge{
  AnsiString Name;
  MyArray<CartrigeParam> Parameters;

  friend SimpleStream & operator >>(SimpleStream &,Cartrige & F);
  friend SimpleStream & operator<<(SimpleStream &,Cartrige & F);
};

// Значение параметра картриджа
struct ParamValue{
  AnsiString Name;
  ANsiString Value;

  friend SimpleStream & operator >>(SimpleStream &,CartrigeParamValue & F);
  friend SimpleStream & operator<<(SimpleStream &,CartrigeParamValue & F);
};


// Описание поля данных
struct DataSetField{
  AnsiString Name;
  AnsiString Caption;
  long Type;
  bool AllowNull;
  long Size;
  long Precision;

  friend SimpleStream & operator >>(SimpleStream &,DataSetField & F);
  friend SimpleStream & operator<<(SimpleStream &,DataSetField & F);
};


// Описание процедуры
struct ProcAlgoritm{
  AnsiString Name;
  MyArray<DataSetField> Parameters;
  AnsiString Definition;

  friend SimpleStream & operator >>(SimpleStream &,ProcAlgoritm & F);
  friend SimpleStream & operator<<(SimpleStream &,ProcAlgoritm & F);
};


// Пары для связки потоков данных
struct JoinPair{
  AnsiString FromField;
  AnsiString ToField;

  friend SimpleStream & operator >>(SimpleStream &,JoinPair & F);
  friend SimpleStream & operator<<(SimpleStream &,JoinPair & F);
};


// способ связки потоков
struct JoinDef{
  AnsiString LeftSet;
  AnsiString RightSet;
  long JoinType;  // Inner / Left / Full / Cross
  MyArray<JoinPair> JoinFields;

  friend SimpleStream & operator >>(SimpleStream &,Join & F);
  friend SimpleStream & operator<<(SimpleStream &,Join & F);
};


// Описание потока данных
struct DataSet{
  AnsiString Name;
  MyArray<DataSetField> Fields;
  bool IsTable;
  JoinDef Join;

  friend SimpleStream & operator >>(SimpleStream &,DataSet & F);
  friend SimpleStream & operator<<(SimpleStream &,DataSet & F);
};



// Описание домена данных ( одна база данных)
struct Domain{
  AnsiString Name;
  MyArray<DataSet> DataSets;
  MyArray<ProcAlgoritm> Procedures;

  friend SimpleStream & operator >>(SimpleStream &,Domain & F);
  friend SimpleStream & operator<<(SimpleStream &,Domain & F);
};


// Конкретная реализация домена
struct DomainRelease{
  AnsiString DomainName;
  AnsiString CartrigeName;
  MyArray<ParamValue> SetupValues;

  friend SimpleStream & operator >>(SimpleStream &,DomainRelease & F);
  friend SimpleStream & operator<<(SimpleStream &,DomainRelease & F);
};


// Конкретная реализация системы (комплекса)
struct Release{
  AnsiString Name;
  AnsiString SystemName;
	MyArray<DomainRelease> Domains;

	friend SimpleStream & operator >> (SimpleStream &,Release & F);
	friend SimpleStream & operator << (SimpleStream &,Release & F);
};


// Описание меню
struct Menu{
	AnsiString Caption;
	AnsiString FilterName;
	AnsiString ViewerName;
	AnsiString DowbleViewerName;
	AnsiString ReportName;
	AnsiString GraphName;


	MyArray<Menu> SubMenu;

	friend SimpleStream & operator >> (SimpleStream &,Menu & F);
	friend SimpleStream & operator << (SimpleStream &,Menu & F);
};


// Описание задачи
struct Task{

	AnsiString Name;
	AnsiString Caption;
	MyArray<Menu> MainMenu;

	friend SimpleStream & operator >> (SimpleStream &,Task & F);
	friend SimpleStream & operator << (SimpleStream &,Task & F);
};


// изменения значения поля
struct FieldChange{
	AnsiString Field;
	AnsiString Expression;

	friend SimpleStream & operator >> (SimpleStream &,FieldChange & F);
	friend SimpleStream & operator << (SimpleStream &,FieldChange & F);
};


// изменение доступа к полю для всех операций
struct AccessChange{
	AnsiString Field;
	long NewAccess;

	friend SimpleStream & operator >> (SimpleStream &,AccessChange & F);
	friend SimpleStream & operator << (SimpleStream &,AccessChange & F);
};


// связь между просмотрами
struct ViewerLink{
	AnsiString Name;
	AnsiString MasterViewer;
	AnsiString SlaveViewer;
	MyArray<FieldChange> DataChanges;
	MyArray<AccessChange> AccessChanges;
	AnsiString NewSlaveCaption;
	AnsiString LinkCondition;
	AnsiString SubSetExpression;

	friend SimpleStream & operator >> (SimpleStream &,ViewerLink & F);
	friend SimpleStream & operator << (SimpleStream &,ViewerLink & F);
};



struct EditField : public DataSetField {

	long ControlType;   // что за контрольный элемент
	long AccessType;    // W/R/No

	AnsiString ComboViewer;
	AnsiString ComboSyncField;
	AnsiString ComboResult;
	ViewerLink ComboLink;


	friend SimpleStream & operator >>(SimpleStream &,EditField & F);
	friend SimpleStream & operator<<(SimpleStream &,EditField & F);
};



// Описание интерфейсного объекта "Операция"
struct TranDef{
	AnsiString Name;
	AnsiString ProcName;

	// Маппинг параметров и ???
	MyArray<EditField> FormDef;

	AnsiString MasterViewer; // может отсутствовать
	AnsiString Message;
	long TranSource;
	long AfterTran;

	friend SimpleStream & operator >> (SimpleStream &,TranDef & F);
	friend SimpleStream & operator << (SimpleStream &,TranDef & F);
};



// Информация о колонке просмотра
struct ViewerColumn{
	AnsiString Name; // название поля
	AnsiString Caption; // может отсутствовать тогда из поля
	long Horiz;
	long HeaderVert;
	long SectionVert;
	long Width;
	AnsiString Format;

	friend SimpleStream & operator >> (SimpleStream &,ViewerColumn & F);
	friend SimpleStream & operator << (SimpleStream &,ViewerColumn & F);
};


// Поля для сортировки
struct OrderFieldDef{
	AnsiString Name;
	bool Ascending;

	friend SimpleStream & operator >> (SimpleStream &,OrderFieldDef & F);
	friend SimpleStream & operator << (SimpleStream &,OrderFieldDef & F);

};


// Порядок вывода данных
struct OrderDef{
	AnsiString Name;
	MyArray<OrderFieldDef> Fields;

	friend SimpleStream & operator >> (SimpleStream &,OrderDef & F);
	friend SimpleStream & operator << (SimpleStream &,OrderDef & F);
};

// Описание интерфейсного объекта "Просмотр"
struct ViewDef{
	AnsiString Name;
	AnsiString Caption;

	long Style; // грид / дерево

	AnsiString DataSetDomain;
	AnsiString DataSetName;
	MyArray<ViewerColumn> Columns;

	// порядок вывода
	MyArray<OrderDef> Orders;
	AnsiString DefaultOrderName;

	// только для дерева
	long HierAlgoritm; // x,y / HIER / ref to parent
	AnsiString X;
	AnsiString Y;
	AnsiString Hier;
	AnsiString IdParent;
	AnsiString RefParent;
	AnsiString LabelField;


  MyArray<ParamValue> Parameters; // параметры

	friend SimpleStream & operator >> (SimpleStream &,ViewDef & F);
	friend SimpleStream & operator << (SimpleStream &,ViewDef & F);
};


// Описание интерфейсного объекта "Связные просмотры"
struct DoubleViewerDef{
	AnsiString Name;
	long Style; // два просмотра на форме  / независимые окна

	AnsiString LinkName;

	friend SimpleStream & operator >> (SimpleStream &,DoubleViewerDef & F);
	friend SimpleStream & operator << (SimpleStream &,DoubleViewerDef & F);
};


// Описание серии графика
struct GraphSeries{
	AnsiString Name;

	AsiString DomainName;
	AsiString SetName;

	long MarkType;
	long ZoomNumber; // масштаб по оси x
	long Min;
	long Max;
	long Legend;
  long Style;
	AnsiString ValueField;
  AnsiString ViewerLink;  //Инициализируется параметрами графика

	friend SimpleStream & operator >> (SimpleStream &,GraphSeries & F);
	friend SimpleStream & operator << (SimpleStream &,GraphSeries & F);

};

// Описание интерфейсного объекта "График"
struct GraphDef{
	Ansistring Name;
	Ansistring Caption;
	Ansistring XCaption;
	Ansistring YCaption;
	long Style;

  MyArray<ParamValue> Parameters; // параметры графика в целом
	MyArray<GraphSeries> Series;

	friend SimpleStream & operator >> (SimpleStream &,ReportDef & F);
	friend SimpleStream & operator << (SimpleStream &,ReportDef & F);

};


/************************************************
?????????????????????????????????????????????????
*************************************************/


// Описание интерфейсного объекта "Отчет" ****
struct ReportDef{
	AnsiString Name;
	AnsiString Definition;

	friend SimpleStream & operator >> (SimpleStream &,ReportDef & F);
	friend SimpleStream & operator << (SimpleStream &,ReportDef & F);
};



// Описание системы (комплекса)
struct System{
	MyArray<Domain> Domains;
	MyArray<ViewDef> Viewers;
	MyArray<DoubleViewerDef> DoubleViewers;
	MyArray<ReportDef> Reports;
	MyArray<GraphDef> Grapths;
	MyArray<ViewerLink> Links;
	MyArray<TranDef> Operations;
	MyArray<Task> Tasks;

	friend SimpleStream & operator >> (SimpleStream &,System & F);
	friend SimpleStream & operator << (SimpleStream &,System & F);

};


// Права на вызов задачи из конкретной реализации
struct TaskRight{
	AnsiString ReleaseName;
	AnsiString TaskName;
	bool AllowExec;
	AnsiString GroupName;
	AnsiString UserName;
	bool IsUser;

	friend SimpleStream & operator >> (SimpleStream &,TaskRight & F);
	friend SimpleStream & operator << (SimpleStream &,TaskRight & F);
};


// Права на вызов конкретного объекта в рамках задачи для конкретной реализации
struct ObjectRight{
	AnsiString ReleaseName;
	AnsiString TaskName;

	long ObjectType;
	AnsiString ObjectName;

	bool AllowExec;
	bool AllowRead;
	bool AllowWrite;

	AnsiString GroupName;
	AnsiString UserName;
	bool IsUser;

	friend SimpleStream & operator >> (SimpleStream &,ObjectRight & F);
	friend SimpleStream & operator << (SimpleStream &,ObjectRight & F);

};


// Общее хранилище (Application Server)
struct AppServer{
	AnsiString Signature;
	AnsiString Version;
	MyArray<User> Users;
	MyArray<Group> Groups;
	MyArray<System> Systems;
	MyArray<Cartrige> Cartriges;
	MyArray<Release>  Releases;
	MyArray<TaskRight> TaskRights;
	MyArray<ObjectRight> ObjectRights;

	friend SimpleStream & operator >> (SimpleStream &,AppServer & F);
	friend SimpleStream & operator << (SimpleStream &,AppServer & F);
};

#endif

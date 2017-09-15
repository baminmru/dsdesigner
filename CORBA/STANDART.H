///////////////////////////////////////////////
// структуры объектов интегррованной системы //
///////////////////////////////////////////////


//////////////////////////////////////////////
// вспомогательные структуры

// блок даных
struct DataBuffer{
   long BufSize;
   unsigned char *Buffer;
};

// основное хранилище значений
struct StackValue{
		StackType type;
		bool NullNumber;
		double     n;
		AnsiString s;
		TDateTime  d;
//    DataBuffer b;
};

// параметр
struct CartrigeParam{
  AnsiString Name;
  StackValue Value;
};

// заголовок резльтата запроса
struct FieldHeader{
  AnsiString Name;
  boolean AllowNull;
  long Type;
  long MaxSize;
};

// массив значений
struct RowData : public   MyArray<StackValue>{
};

// массив параметров
struct CParam : public   MyArray<CartrigeParam>{
};

// результат  запроса
struct RowSet{
  MyArray<FieldHeader> Header;
  MyArray<RowData> Rows;
};


class ObjectManagerStub;

////////////////////////////////////////////
// функции объекта доступные другим объектам
class ObjectItemStub{
public:
   // инициализация
   virtual void Setup (DataBuffer * PI, DataBuffer *PD,DataBuffer *UL)=0;
   // инициализация в подчиненном режиме
   virtual void SetupSlave (DataBuffer * SI, CParam *Par)=0;
   // обновление для новых параметров
   virtual void Refresh (CParam * Par)=0;
   // запуск в modeless режиме
   virtual void Run ()=0;
   // запуск в режиме modal
   virtual bool RunModal ()=0;
   // получение текущего оперативного состояния объекта
   virtual CParam * GetParams()=0;
   // Получение выбранных строк объекта
   virtual RowSet * GetSelection()=0;
   // запуск в режиме фильтрации
   virtual RowSet * Filteration(RowSet * IRS)=0;
};

////////////////////////////////////
// Функции источника данных
class DataSourceStub {
public:
  // открыть курсор
  virtual long OpenCursor(const char* _Query,CParam & _Parameters )=0;
  // открыт ли дступ к данным
  virtual bool IsConnected()=0;
  // получить все строки результатов и заголовки
  virtual RowSet* GetRowSet(const char* _Query,CParam & _Parameters )=0;
  // строка данных из курсора
  virtual RowData* GetCursorRow( long _Cursor  )=0;
  // передвинуть курсор и сообщить о результате
  virtual bool NoMoreRows( long _Cursor  )=0;
  // закрыть курсор
  virtual void CloseCursor( long _Cursor   )=0;
  // заголовок курсора доступен, пока курсор открыт
  virtual MyArray<FieldHeader>* GetCursorHeared(long _Cursor )=0;
  // исполнить SQL выражение
  virtual void Exec( const char* _Query,CParam & _Parameters)=0;

  //--------------- используется ObjectManager --------------
  // осуществить соедитнение
  virtual void Connect(DataBuffer& _Info )=0;
  // закрыть соединение
  virtual void DisConnect()=0;
  // описания параметров источника данных
  virtual MyArray<AnsiString>* GetConnectInfo()=0;
};


enum ManagerWindowType{
  mwtMDIForm,
  mwtMDIChild,
  mwtPopup
};

//////////////////////////////////////////
// менеджер объектов

class ObjectManagerStub{
public:
  //------- только для режима исполнения
  // получить источник данных
  virtual DataSourceStub *GetSource(AnsiString Name)=0;

  // найти или создать объект
  virtual ObjectItemStub * FindItem(AnsiString ObjectName)=0;

  // создать объект
  virtual ObjectItemStub * NewItem(AnsiString ObjectName)=0;

  // Получить окно
  virtual TForm  *GetForm(ManagerWindowType mwt)=0;

  // Получить локальную настройку
  virtual DataBuffer * GetObjectUL(AnsiString ObjectName)=0;

  // Сохранить локальную настройку объекта
  virtual void SaveObjectUL(AnsiString ObjectName,DataBuffer *UL)=0;


  //---------------только для режима администрирования
  // Получить список объектов в задче
  MyArray<AnsiString> * ObjectList()=0;

  // Получить список источников данных в задаче
  MyArray<AnsiString> * SourceList()=0;
};


////////////////////////////////////////////
// функции DLL кмпонента (extern "C")


// создание нового компонента такого типа
long NewInstance (ObjectManagerStub *);

// уничтожение компонента
void CloseInstance (long);

// инициализация
void Setup (
              long Instance,
              DataBuffer * PlatformIndependent,
              DataBuffer * PlatformDependent,
              DataBuffer * UserSettings );

// инициализация в подчиненном режиме
void SetupSlave (
              long Instance,
              DataBuffer * SlaveInit,
              CParam * Parameters);

// обновление для новых параметров
void Refresh (
              long Instance,
              CParam * Parameters
              );
// запуск в modeless режиме
void Run (long);

// запуск в режиме modal
bool RunModal (long);

// получение текущего оперативного состояния объекта
CParam * GetParams(long);

// Получение выбранных строк объекта
RowSet * GetSelection(long);

// запуск в режиме фильтрации
RowSet * Filteration(long, RowSet *);

DataBuffer * EditUL (ObjectManagerStub * manager,
                     DataBuffer * UserSettings);

//------------- только для режима администрирования
// редактирование установок
DataBuffer * EditPI (ObjectManagerStub * manager,
                     DataBuffer * PlatformIndependent);
DataBuffer * EditPD (ObjectManagerStub * manager,
                     DataBuffer * PlatformDependent);
DataBuffer * EditSL (ObjectManagerStub * manager,
                     DataBuffer * SlaveInit);


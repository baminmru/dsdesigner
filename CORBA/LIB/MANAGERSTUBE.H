//---------------------------------------------------------------------------
#ifndef ManagerStubeH
#define ManagerStubeH

#include "Store_c.hh"
#include "cartrige_c.hh"
#include "stackvalue.h"
#include "StoreObjects.h"
#include "CartrigeObjects.h"
#include "Convertions.h"

class ObjectManagerStub;

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
  virtual MyArray<AnsiString>* GetConnectInfo()=0;};

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
  virtual DataSourceStub *GetSource(AnsiString Name,AnsiString SystemName,AnsiString BuildName)=0;

  // найти или создать объект
  virtual ObjectItemStub * FindItem(AnsiString ObjectName,AnsiString SystemName)=0;

  // создать объект
  virtual ObjectItemStub * NewItem(AnsiString ObjectName,AnsiString SystemName)=0;

  // Получить окно
  virtual TForm  *GetForm(ManagerWindowType mwt)=0;

  // Получить настройку зависимости
  virtual DataBuffer * GetObjectSL(AnsiString ObjectName,AnsiString SystemName)=0;


  // Получить локальную настройку
  virtual DataBuffer * GetObjectUL(AnsiString ObjectName,AnsiString SystemName)=0;

  // Сохранить локальную настройку объекта
  virtual void SaveObjectUL(AnsiString ObjectName,DataBuffer *UL,AnsiString SystemName)=0;


  virtual AnsiString CurSystem()=0;
  virtual AnsiString CurBuild()=0;

  //---------------только для режима администрирования
  // Получить список объектов в задче
  virtual MyArray<AnsiString> * ObjectList(AnsiString System)=0;

  // Получить список источников данных в задаче
  virtual MyArray<AnsiString> * SourceList(AnsiString System)=0;
};

//---------------------------------------------------------------------------
#endif

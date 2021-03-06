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
   // �������������
   virtual void Setup (DataBuffer * PI, DataBuffer *PD,DataBuffer *UL)=0;
   // ������������� � ����������� ������
   virtual void SetupSlave (DataBuffer * SI, CParam *Par)=0;
   // ���������� ��� ����� ����������
   virtual void Refresh (CParam * Par)=0;
   // ������ � modeless ������
   virtual void Run ()=0;
   // ������ � ������ modal
   virtual bool RunModal ()=0;
   // ��������� �������� ������������ ��������� �������
   virtual CParam * GetParams()=0;
   // ��������� ��������� ����� �������
   virtual RowSet * GetSelection()=0;
   // ������ � ������ ����������
   virtual RowSet * Filteration(RowSet * IRS)=0;
};

class DataSourceStub {
public:
 // ������� ������
  virtual long OpenCursor(const char* _Query,CParam & _Parameters )=0;
  // ������ �� ����� � ������
  virtual bool IsConnected()=0;
  // �������� ��� ������ ����������� � ���������
  virtual RowSet* GetRowSet(const char* _Query,CParam & _Parameters )=0;
  // ������ ������ �� �������
  virtual RowData* GetCursorRow( long _Cursor  )=0;
  // ����������� ������ � �������� � ����������
  virtual bool NoMoreRows( long _Cursor  )=0;
  // ������� ������
  virtual void CloseCursor( long _Cursor   )=0;
  // ��������� ������� ��������, ���� ������ ������
  virtual MyArray<FieldHeader>* GetCursorHeared(long _Cursor )=0;
  // ��������� SQL ���������
  virtual void Exec( const char* _Query,CParam & _Parameters)=0;

  //--------------- ������������ ObjectManager --------------
  // ����������� �����������
  virtual void Connect(DataBuffer& _Info )=0;
  // ������� ����������
  virtual void DisConnect()=0;
  // �������� ���������� ��������� ������
  virtual MyArray<AnsiString>* GetConnectInfo()=0;};

enum ManagerWindowType{
  mwtMDIForm,
  mwtMDIChild,
  mwtPopup
};


//////////////////////////////////////////
// �������� ��������

class ObjectManagerStub{
public:
  //------- ������ ��� ������ ����������
  // �������� �������� ������
  virtual DataSourceStub *GetSource(AnsiString Name,AnsiString SystemName,AnsiString BuildName)=0;

  // ����� ��� ������� ������
  virtual ObjectItemStub * FindItem(AnsiString ObjectName,AnsiString SystemName)=0;

  // ������� ������
  virtual ObjectItemStub * NewItem(AnsiString ObjectName,AnsiString SystemName)=0;

  // �������� ����
  virtual TForm  *GetForm(ManagerWindowType mwt)=0;

  // �������� ��������� �����������
  virtual DataBuffer * GetObjectSL(AnsiString ObjectName,AnsiString SystemName)=0;


  // �������� ��������� ���������
  virtual DataBuffer * GetObjectUL(AnsiString ObjectName,AnsiString SystemName)=0;

  // ��������� ��������� ��������� �������
  virtual void SaveObjectUL(AnsiString ObjectName,DataBuffer *UL,AnsiString SystemName)=0;


  virtual AnsiString CurSystem()=0;
  virtual AnsiString CurBuild()=0;

  //---------------������ ��� ������ �����������������
  // �������� ������ �������� � �����
  virtual MyArray<AnsiString> * ObjectList(AnsiString System)=0;

  // �������� ������ ���������� ������ � ������
  virtual MyArray<AnsiString> * SourceList(AnsiString System)=0;
};

//---------------------------------------------------------------------------
#endif

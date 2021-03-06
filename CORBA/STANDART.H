///////////////////////////////////////////////
// ��������� �������� �������������� ������� //
///////////////////////////////////////////////


//////////////////////////////////////////////
// ��������������� ���������

// ���� �����
struct DataBuffer{
   long BufSize;
   unsigned char *Buffer;
};

// �������� ��������� ��������
struct StackValue{
		StackType type;
		bool NullNumber;
		double     n;
		AnsiString s;
		TDateTime  d;
//    DataBuffer b;
};

// ��������
struct CartrigeParam{
  AnsiString Name;
  StackValue Value;
};

// ��������� ��������� �������
struct FieldHeader{
  AnsiString Name;
  boolean AllowNull;
  long Type;
  long MaxSize;
};

// ������ ��������
struct RowData : public   MyArray<StackValue>{
};

// ������ ����������
struct CParam : public   MyArray<CartrigeParam>{
};

// ���������  �������
struct RowSet{
  MyArray<FieldHeader> Header;
  MyArray<RowData> Rows;
};


class ObjectManagerStub;

////////////////////////////////////////////
// ������� ������� ��������� ������ ��������
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

////////////////////////////////////
// ������� ��������� ������
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
  virtual MyArray<AnsiString>* GetConnectInfo()=0;
};


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
  virtual DataSourceStub *GetSource(AnsiString Name)=0;

  // ����� ��� ������� ������
  virtual ObjectItemStub * FindItem(AnsiString ObjectName)=0;

  // ������� ������
  virtual ObjectItemStub * NewItem(AnsiString ObjectName)=0;

  // �������� ����
  virtual TForm  *GetForm(ManagerWindowType mwt)=0;

  // �������� ��������� ���������
  virtual DataBuffer * GetObjectUL(AnsiString ObjectName)=0;

  // ��������� ��������� ��������� �������
  virtual void SaveObjectUL(AnsiString ObjectName,DataBuffer *UL)=0;


  //---------------������ ��� ������ �����������������
  // �������� ������ �������� � �����
  MyArray<AnsiString> * ObjectList()=0;

  // �������� ������ ���������� ������ � ������
  MyArray<AnsiString> * SourceList()=0;
};


////////////////////////////////////////////
// ������� DLL ��������� (extern "C")


// �������� ������ ���������� ������ ����
long NewInstance (ObjectManagerStub *);

// ����������� ����������
void CloseInstance (long);

// �������������
void Setup (
              long Instance,
              DataBuffer * PlatformIndependent,
              DataBuffer * PlatformDependent,
              DataBuffer * UserSettings );

// ������������� � ����������� ������
void SetupSlave (
              long Instance,
              DataBuffer * SlaveInit,
              CParam * Parameters);

// ���������� ��� ����� ����������
void Refresh (
              long Instance,
              CParam * Parameters
              );
// ������ � modeless ������
void Run (long);

// ������ � ������ modal
bool RunModal (long);

// ��������� �������� ������������ ��������� �������
CParam * GetParams(long);

// ��������� ��������� ����� �������
RowSet * GetSelection(long);

// ������ � ������ ����������
RowSet * Filteration(long, RowSet *);

DataBuffer * EditUL (ObjectManagerStub * manager,
                     DataBuffer * UserSettings);

//------------- ������ ��� ������ �����������������
// �������������� ���������
DataBuffer * EditPI (ObjectManagerStub * manager,
                     DataBuffer * PlatformIndependent);
DataBuffer * EditPD (ObjectManagerStub * manager,
                     DataBuffer * PlatformDependent);
DataBuffer * EditSL (ObjectManagerStub * manager,
                     DataBuffer * SlaveInit);


//---------------------------------------------------------------------------
#ifndef SRVImplH
#define SRVImplH
#include "cartrige_s.hh"
#include "CartrigeObjects.h"
#include "cartrigeIni.h"


struct InstanceStore{
  odbcConnection *con;
  MyArray<CartrigeParam> Parameters;
};

struct ConnectStore{
 odbcConnection *con;
 long Usage;
};



class srvCartrige: public _sk_Cartrige{
 private:

 bool Wait;
 odbcEnvironment env;
 MyArray<InstanceStore> Instances;
 MyArray<ConnectStore> Connects;

public:

  srvCartrige(const char *_obj_name = (const char *)NULL):
  _sk_Cartrige(_obj_name){Wait = false;};

  srvCartrige(
      const char *_service_name,
      const CORBA::ReferenceData& _data):
  _sk_Cartrige(
      _service_name,
       _data){Wait = false;};

  virtual void DisConnect(
      CORBA::Long Instance
      );
  virtual void CloseInstance(
      CORBA::Long Instance
      );


  virtual CORBA::Long OpenCursor(
      CORBA::Long Instance,
      const char* Query,
      const Octets& Parameters
      );
  virtual CORBA::Boolean IsConnected(
      CORBA::Long Instance
      );
  virtual Octets* GetRowSet(
      CORBA::Long Instance,
      const char* Query,
      const Octets& Parameters
      );
  virtual void Connect(
      CORBA::Long Instance,
      const Octets& Info
      );
  virtual Octets* GetConnectInfo();
  virtual CORBA::Long NewInstance();
  virtual Octets* GetCursorRow(
      CORBA::Long Instance,
      CORBA::Long Cursor
      );
  virtual CORBA::Boolean NoMoreRows(
      CORBA::Long Instance,
      CORBA::Long Cursor
      );
  virtual void CloseCursor(
      CORBA::Long Instance,
      CORBA::Long Cursor
      );
  virtual Octets* GetCursorHeared(
      CORBA::Long Instance,
      CORBA::Long Cursor
      );
  virtual void Exec(
      CORBA::Long Instance,
      const char* Query,
      const Octets& Parameters
      );
};
//---------------------------------------------------------------------------
#endif

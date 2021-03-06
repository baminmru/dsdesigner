#ifndef _Store_c_hh
#define _Store_c_hh

/**
 **************************************************************************
 *                                                                        *
 *                        -- DO NOT MODIFY --                             *
 *  This file is automatically generated by the VisiBroker IDL compiler.  *
 *  Generated code conforms to OMG's IDL-to-C++ 1.1 mapping as specified  *
 *  in OMG Document Number: 96-01-13                                      *
 *                                                                        *
 *  VisiBroker is copyrighted by Visigenic Software, Inc.                 *
 **************************************************************************
 */

#include "corba.h"

#include "vpre.h"
#ifndef OCTETS_H
#define OCTETS_H
class Octets;
typedef Octets *Octets_ptr;

class Octets {
private:
  CORBA::Octet *_contents;
  CORBA::ULong    _count;
  CORBA::ULong    _num_allocated;
  CORBA::Long     _ref_count;

  CORBA::Boolean  _release_flag;
public:
  
#if defined(MSVCNEWDLL_BUG)
  void *operator new(size_t ts);
  void *operator new(size_t ts, char*, int) {return operator new(ts);}
  void operator delete(void *p);

#endif

  static CORBA::Octet *allocbuf(CORBA::ULong _nelems);
  static void freebuf(CORBA::Octet *_data);
  Octets(CORBA::ULong _max=0);
  Octets(CORBA::ULong _max,
      CORBA::ULong _len,
      CORBA::Octet *_data,
      CORBA::Boolean _release=0);
  Octets(const Octets&);
  ~Octets();

  Octets& operator=(const Octets&);
  CORBA::ULong maximum() const { return _num_allocated; }
  void length(CORBA::ULong _len);
  CORBA::ULong length() const { return _count;}
  CORBA::Octet& operator[](CORBA::ULong _index) ;
  const CORBA::Octet& operator[](CORBA::ULong _index) const;

  friend VISostream& operator<<(VISostream&, const Octets&);
  inline friend VISostream& operator<<(VISostream& _strm, const Octets *_obj) {
    if ( _obj == (Octets*)NULL )
      throw CORBA::BAD_PARAM();
    else
      _strm << *_obj;
    return _strm;
  }

  friend VISistream& operator>>(VISistream&, Octets&);
  inline friend VISistream& operator>>(VISistream& _strm, Octets_ptr & _obj) {
    _obj = new Octets;
    _strm >> *_obj;
    return _strm;
  }

  friend Ostream& operator<<(Ostream&, const Octets&);
  inline friend Istream& operator>>(Istream& _strm, Octets& _obj) {
    VISistream _istrm(_strm);
    _istrm >> _obj;
    return _strm;
  }

  inline friend Istream& operator>>(Istream& _strm, Octets_ptr & _obj) {
    VISistream _istrm(_strm);
    _istrm >> _obj;
    return _strm;
  }

  static Octets *_duplicate(Octets* _ptr) {
    if (_ptr) _ptr->_ref_count++;
    return _ptr;
  }

  static void _release(Octets *_ptr) {
    if (_ptr && ( --_ptr->_ref_count == 0) ) delete _ptr;
  }

};

class Octets_out;

class Octets_var {
  friend class Octets_out;
private:
  Octets *_ptr;

public:
  Octets_var() : _ptr((Octets *)NULL) {}
  Octets_var(Octets *_p) :_ptr(_p) {}
  Octets_var(const Octets_var& _var) :
    _ptr(Octets::_duplicate((Octets *)_var)){}
  ~Octets_var() { Octets::_release(_ptr); }

  Octets_var& operator=(Octets *_p) {
    Octets::_release(_ptr);
    _ptr = _p;
    return *this;
  }

  Octets_var& operator=(const Octets_var& _var) {
    Octets::_release(_ptr);
    _ptr = Octets::_duplicate((Octets *)_var);
    return *this;
  }

  Octets *operator->() { return _ptr; }
  CORBA::Octet& operator[](CORBA::ULong _index) {
    return _ptr->operator[](_index);
  }

  const CORBA::Octet& operator[](CORBA::ULong _index) const {
    return _ptr->operator[](_index);
  }

  operator Octets *() const { return _ptr; }
  operator const Octets& () const { return *_ptr; }
  operator Octets& () { return *_ptr; }

  const Octets& in() const { return *_ptr; }
  Octets& inout() { return *_ptr; }
  Octets_ptr & out();
  Octets* _retn() {
    Octets* _tmp_ptr;
    _tmp_ptr = _ptr;
    _ptr = (Octets *)NULL;
    return _tmp_ptr;
  }
  inline friend VISostream& operator<<(
      VISostream& _strm,
      const Octets_var& _var) {
    if ( _var._ptr == (Octets *)NULL )
      throw CORBA::BAD_PARAM();
    else
      _strm << *_var._ptr;
    return _strm;
  }

  inline friend  VISistream& operator>>(
      VISistream& _strm,
      Octets_var& _var) {
    if ( _var._ptr == (Octets *)NULL )
      _var._ptr = new Octets;
    _strm >> *_var._ptr;
    return _strm;
  }

};

class Octets_out {
private:
  Octets_ptr & _ptr;
  static Octets* _nil() { return (Octets*)NULL; }
  void operator=(const Octets_out&);
  void operator=(const Octets_var&);

public:
  Octets_out(const Octets_out& _o) : _ptr(_o._ptr) {}
  Octets_out(Octets_ptr & _p) : _ptr(_p) { 
    _ptr = _nil();
  }
  Octets_out(Octets_var& _v) : _ptr(_v._ptr) {
    Octets::_release(_ptr); _ptr = _nil();
  }
  Octets_out& operator=(Octets* _p) {
    _ptr = _p;
    return *this;
  }
  operator Octets_ptr &() { return _ptr; }
  Octets_ptr & ptr() { return _ptr; }
};
#endif

#ifndef _ObjDefStorage_var_
#define _ObjDefStorage_var_

class ObjDefStorage;
typedef ObjDefStorage* ObjDefStorage_ptr;
typedef ObjDefStorage_ptr ObjDefStorageRef;

 VISistream& operator>>(VISistream&, ObjDefStorage_ptr&);
 VISostream& operator<<(VISostream&, const ObjDefStorage_ptr);

class ObjDefStorage_out;
class ObjDefStorage_var: public CORBA::_var {
  friend class ObjDefStorage_out;

private:
  ObjDefStorage_ptr _ptr;

public:
  void operator=(const ObjDefStorage_var&_v) {
    if ( _ptr ) _release(_ptr);
    if ( _v._ptr )
      _ptr = _duplicate(_v._ptr);
    else
      _ptr = (ObjDefStorage_ptr)NULL;
  }

  static ObjDefStorage_ptr _duplicate(ObjDefStorage_ptr);
  static void _release(ObjDefStorage_ptr);

  ObjDefStorage_var();
  ObjDefStorage_var(ObjDefStorage_ptr);
  ObjDefStorage_var(const ObjDefStorage_var&);
  ~ObjDefStorage_var();
  ObjDefStorage_var& operator=(ObjDefStorage_ptr);
  operator ObjDefStorage_ptr() const { return _ptr; }
  ObjDefStorage_ptr operator->() const { return _ptr; }
  ObjDefStorage_ptr in() const { return _ptr; }
  ObjDefStorage_ptr& inout() { return _ptr; }
  ObjDefStorage_ptr& out();
  ObjDefStorage_ptr _retn() {
    ObjDefStorage_ptr _tmp_ptr;
    _tmp_ptr = _ptr;
    _ptr = (ObjDefStorage_ptr)NULL;
    return _tmp_ptr;
  }
  friend VISistream& operator>>(VISistream&, ObjDefStorage_var&);
  friend VISostream& operator<<(VISostream&, const ObjDefStorage_var&);
  friend Istream& operator>>(Istream&, ObjDefStorage_var&);
  friend Ostream& operator<<(Ostream&, const ObjDefStorage_var&);
};

class ObjDefStorage_out {
private:
  ObjDefStorage_ptr & _ptr;
  static ObjDefStorage* _nil() { return (ObjDefStorage*)NULL; }
  void operator=(const ObjDefStorage_out&);
  void operator=(const ObjDefStorage_var&);

public:
  ObjDefStorage_out(const ObjDefStorage_out& _o) : _ptr(_o._ptr) {}
  ObjDefStorage_out(ObjDefStorage_ptr & _p) : _ptr(_p) { 
    _ptr = _nil();
  }
  ObjDefStorage_out(ObjDefStorage_var& _v) : _ptr(_v._ptr) {
      ObjDefStorage_var::_release(_ptr); _ptr = _nil();
  }
  ~ObjDefStorage_out() {}
  ObjDefStorage_out& operator=(ObjDefStorage_ptr _p) {
      _ptr = _p; return *this;
  }
  operator ObjDefStorage_ptr& () { return _ptr; }
  ObjDefStorage_ptr& ptr() { return _ptr; }
  ObjDefStorage_ptr operator->() { return _ptr; }
};


#endif

class ObjDefStorage : public virtual CORBA_Object {
private:
  static const CORBA::TypeInfo _class_info;
  ObjDefStorage(const ObjDefStorage&){ ___root = this; }
  void operator=(const ObjDefStorage&){}

protected:
  ObjDefStorage_ptr ___root;
  void set_root(ObjDefStorage_ptr root) {
    ___root = root;
  }
  
public:
  
  static  const CORBA::TypeInfo *_desc();
  virtual const CORBA::TypeInfo *_type_info() const;
  virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
  static CORBA::Object *_factory();

  ObjDefStorage_ptr _this();
protected:

  ObjDefStorage(const char *obj_name = NULL): CORBA_Object(obj_name, 1) { ___root = this; }
public:
  virtual ~ObjDefStorage() {}

  static ObjDefStorage_ptr _duplicate(ObjDefStorage_ptr _obj) {
    if ( _obj ) _obj->_ref();
    return _obj;
  }
  static ObjDefStorage_ptr _nil() { return (ObjDefStorage_ptr)NULL; }
  static ObjDefStorage_ptr _narrow(CORBA::Object *_obj);
  static ObjDefStorage_ptr _clone(ObjDefStorage_ptr _obj) {
    CORBA::Object_var _obj_var(__clone(_obj));
    
#if defined(_HPCC_BUG)
    return _narrow(_obj_var.operator CORBA::Object_ptr());
    
#else
    return _narrow(_obj_var);
    
#endif
  }

  static ObjDefStorage_ptr _bind(
      const char *_object_name = NULL,
      const char *_host_name = NULL,
      const CORBA::BindOptions* _opt = NULL,
      CORBA::ORB_ptr _orb = NULL);

  virtual Octets* GetTargetPackSetup(
      CORBA::Long _ConnectId,
      const char* _BuildName,
      const char* _SystemName,
      const char* _DataSource
      );

  virtual void SetComponent(
      CORBA::Long _ConnectId,
      const char* _Platform,
      const char* _ComponentName,
      const Octets& _Data
      );

  virtual Octets* GetObjectPlatformSpecifics(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _ObjectName,
      const char* _PlatformName
      );

  virtual void SetTargetPack(
      CORBA::Long _ConnectId,
      const char* _Target,
      const char* _PackName,
      const char* _PlatformName,
      const Octets& _Data
      );

  virtual void SetBuildRights(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _BuildName,
      const char* _UserName,
      CORBA::Boolean _AllowAccess
      );

  virtual Octets* GetTargetList(
      CORBA::Long _ConnectId
      );

  virtual void DelTargetPack(
      CORBA::Long _ConnectId,
      const char* _Target,
      const char* _PackName,
      const char* _PlatformName
      );

  virtual void DisConnect(
      CORBA::Long _ConnectId
      );

  virtual Octets* GetComponent(
      CORBA::Long _ConnectId,
      const char* _Platform,
      const char* _ComponentName
      );

  virtual Octets* GetBuildList(
      CORBA::Long _ConnectId
      );

  virtual void AddSystem(
      CORBA::Long _ConnectId,
      const char* _SystemName
      );

  virtual void SetObjectLocals(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _ObjectName,
      const char* _UserName,
      const Octets& _Locals
      );

  virtual void AddTarget(
      CORBA::Long _ConnectId,
      const char* _TargetName
      );

  virtual void DelObjectLocals(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _ObjectName,
      const char* _UserName
      );

  virtual void AddDataSource(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _SourceName
      );

  virtual Octets* GetPlatformList(
      CORBA::Long _ConnectId
      );

  virtual Octets* GetDataSourceList(
      CORBA::Long _ConnectId,
      const char* _SystemName
      );

  virtual CORBA::Long Connect(
      const char* _UID,
      const char* _PWD,
      const char* _PlatformName
      );

  virtual void SetTargetPackSetup(
      CORBA::Long _ConnectId,
      const char* _BuildName,
      const char* _SystemName,
      const char* _DataSource,
      const char* _TargetName,
      const Octets& _Specifics
      );

  virtual Octets* GetSystemList(
      CORBA::Long _ConnectId
      );

  virtual void SetObjectPlatformSpecifics(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _ObjectName,
      const char* _PlatformName,
      const Octets& _Specifics
      );

  virtual Octets* GetTargetPack(
      CORBA::Long _ConnectId,
      const char* _Target,
      const char* _PackName,
      const char* _PlatformName
      );

  virtual Octets* GetObjectLocals(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _ObjectName,
      const char* _UserName
      );

  virtual void DelObjectPlatformSpecifics(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _ObjectName,
      const char* _PlatformName
      );

  virtual void DelBuild(
      CORBA::Long _ConnectId,
      const char* _BuildName,
      const char* _SystemName
      );

  virtual void AddBuild(
      CORBA::Long _ConnectId,
      const char* _BuildName,
      const char* _SystemName
      );

  virtual void SetObjectRights(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _ObjectName,
      const char* _UserName,
      CORBA::Boolean _Exec,
      CORBA::Boolean _Read,
      CORBA::Boolean _Write
      );

  virtual void DelObjectRights(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _ObjectName,
      const char* _UserName
      );

  virtual CORBA::Boolean GetBuildRights(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _BuildName,
      const char* _UserName
      );

  virtual void AddPlatform(
      CORBA::Long _ConnectId,
      const char* _PlatformName
      );

  virtual Octets* GetUserList(
      CORBA::Long _ConnectId
      );

  virtual void DelSystem(
      CORBA::Long _ConnectId,
      const char* _SystemName
      );

  virtual void DelTargetPackSetup(
      CORBA::Long _ConnectId,
      const char* _BuildName,
      const char* _SystemName,
      const char* _DataSource
      );

  virtual void DelTarget(
      CORBA::Long _ConnectId,
      const char* _TargetName
      );

  virtual Octets* GetObject(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _ObjectName
      );

  virtual void DelUser(
      CORBA::Long _ConnectId,
      const char* _UserName
      );

  virtual void DelObject(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _ObjectName
      );

  virtual void ChangePassword(
      CORBA::Long _ConnectId,
      const char* _Password
      );

  virtual void SetUser(
      CORBA::Long _ConnectId,
      const char* _UserName,
      const char* _Password,
      CORBA::Boolean _IsAdmin
      );

  virtual void DelDataSource(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _SourceName
      );

  virtual Octets* GetSystemBuildList(
      CORBA::Long _ConnectId,
      const char* _SystemName
      );

  virtual void DelPlatform(
      CORBA::Long _ConnectId,
      const char* _PlatformName
      );

  virtual void DelBuildRights(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _BuildName,
      const char* _UserName
      );

  virtual void SetObject(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _ObjectName,
      const char* _ComponentName,
      const Octets& _ObjData
      );

  virtual Octets* GetComponentList(
      CORBA::Long _ConnectId,
      const char* _Platform
      );

  virtual Octets* GetObjectRights(
      CORBA::Long _ConnectId,
      const char* _SystemName,
      const char* _ObjectName,
      const char* _UserName
      );

  virtual void DelComponent(
      CORBA::Long _ConnectId,
      const char* _Platform,
      const char* _ComponentName
      );

  virtual Octets* GetSystemObjectsList(
      CORBA::Long _ConnectId,
      const char* _SystemName
      );

  friend VISistream& operator>>(VISistream& _strm, ObjDefStorage_ptr& _obj);
  friend VISostream& operator<<(VISostream& _strm, const ObjDefStorage_ptr _obj);
  friend Ostream& operator<<(Ostream& _strm, const ObjDefStorage_ptr _obj) {
    _strm << (CORBA::Object_ptr)_obj;
    return _strm;
  }

  friend Istream& operator>>(Istream& _strm, ObjDefStorage_ptr& _obj) {
    VISistream _istrm(_strm);
    _istrm >> _obj;
    return _strm;
  }

};


#include "vpost.h"

#endif


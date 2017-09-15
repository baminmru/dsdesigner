//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Convertions.h"


unsigned char * MakeOctetsCopy(const Octets& ConectInf){
  unsigned char * Buffer = new unsigned char [ConectInf.length()];
  for(long i=0;i<ConectInf.length();i++){
    Buffer[i]=ConectInf[i];
  }
  return Buffer;
};

void SetDataBuffer(DataBuffer & db, Octets & oc){
  db.SetMyBuffer( oc.length(), MakeOctetsCopy(oc));
}

MyMemory *MakeMemory(Octets &oc){
  MyMemory *m =new MyMemory;
  m->LoadBuffer(MakeOctetsCopy(oc),oc.length());
  return m;
};

Octets *MakeOctets(MyMemory &m){
  unsigned char * Buffer=new unsigned char[m.BufferSize()];
  memcpy(Buffer,m.GetBuffer(),m.BufferSize());
  return new Octets(m.BufferSize(),m.BufferSize(),Buffer,1);
};

Octets *MakeOctets(DataBuffer & m){
  unsigned char * Buffer=new unsigned char[m.Length()];
  memcpy(Buffer,m.GetBuffer(),m.Length());
  return new Octets(m.Length(),m.Length(),Buffer,1);
};

//---------------------------------------------------------------------------
#pragma package(smart_init)

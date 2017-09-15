//---------------------------------------------------------------------------
#ifndef ConvertionsH
#define ConvertionsH
#include "Octets_c.hh"
#include "stackvalue.h"

unsigned char * MakeOctetsCopy(const Octets& oc);
MyMemory *MakeMemory(Octets &oc);
Octets *MakeOctets(MyMemory &m);
Octets *MakeOctets(DataBuffer & db);
void SetDataBuffer(DataBuffer & db, Octets & oc);


//---------------------------------------------------------------------------
#endif
 
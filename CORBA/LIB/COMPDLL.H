//---------------------------------------------------------------------------
#ifndef COMPDLL_H
#define COMPDLL_H

#include "ManagerStube.h"


extern "C" {

__declspec(dllexport)   long NewInstance (ObjectManagerStub *);
__declspec(dllexport)   void CloseInstance (long);
__declspec(dllexport)   void Setup (long, DataBuffer *, DataBuffer *,DataBuffer *);
__declspec(dllexport)   void SetupSlave (long, DataBuffer *, CParam *);
__declspec(dllexport)   void Refresh (long,  CParam *);
__declspec(dllexport)   void Run (long);
__declspec(dllexport)   bool RunModal (long);
__declspec(dllexport)   CParam * GetParams(long);
__declspec(dllexport)   RowSet * GetSelection(long);
__declspec(dllexport)   RowSet * Filteration(long, RowSet *);
__declspec(dllexport)   DataBuffer * EditPI (ObjectManagerStub * manager,DataBuffer * PI,AnsiString System);
__declspec(dllexport)   DataBuffer * EditPD (ObjectManagerStub * manager,DataBuffer * PD,AnsiString System);
__declspec(dllexport)   DataBuffer * EditUL (ObjectManagerStub * manager,DataBuffer * UL,AnsiString System);
__declspec(dllexport)   DataBuffer * EditSL (ObjectManagerStub * manager,DataBuffer * UL,AnsiString System);

};


//---------------------------------------------------------------------------
#endif
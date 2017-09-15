
#include <condefs.h>
#pragma hdrstop
#include "stdio.h"

//---------------------------------------------------------------------------
USELIB("..\ProcessEngine\Lib\LIB3.lib");
USEUNIT("MemoryBase.cpp");
//---------------------------------------------------------------------------
#pragma argsused

#include "MemoryBase.H"





int main(int argc, char* argv[])
{

  MyArray<FieldDef> *fda = new MyArray<FieldDef>;
  FieldDef *fd = new FieldDef;
  fd->Name="Name";
  fd->Type=stString;
  fda->Add(fd);

  {
  MemoryTable mt("TEST",fda);

  for(long i=2000;i>0;i-=2){
    StackValue *sv = new StackValue(i);
    MyArray<StackValue> *sva = new MyArray<StackValue>;
    sva->Add(sv);
    mt.AddRow(sva);
  }

  for(long i=-5;i<=1005;i++){

    MyArray<StackValue>* r =mt.GetRow(i);
    if(r)
      printf("\n%d - %s",i, r->Get(0)->ToStr().c_str());
    else
      printf("\n%d - NotFound",i);

  }

  MyMemory m;
  m << mt;
  m.SaveToFile("test.dbm");

  }

  return 0;
}

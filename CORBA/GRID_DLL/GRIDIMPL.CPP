//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "GridImpl.h"
#include "EditGrid.h"
#include <grids.hpp>


struct OrderBy{
  Streamable(OrderBy)
  AnsiString Name;
  AnsiString OrdBy;
};

Streamable2(OrderBy, Name,OrdBy)


struct Parameter{
  Streamable(Parameter)
  AnsiString Name;
  StackValue DefValue;
};

Streamable2(Parameter,Name,DefValue);



struct GridColumn{
  Streamable(GridColumn)
  AnsiString Caption;
  AnsiString Name;
  long Width;
  long OutputStyle;
};

Streamable4(GridColumn, Caption,Name, Width,OutputStyle);

// описания не зависящие от платформы
struct PIData{
  Streamable(PIData)
  AnsiString DataSource;
  AnsiString MainQuery;
  AnsiString RowQuery;
  MyArray<OrderBy> Orders;
  MyArray<GridColumn> Columns;
  MyArray<Parameter> Parameters;
  long DefaultOrder;
  AnsiString Caption;
};

Streamable8(PIData, DataSource,MainQuery,
 RowQuery,Orders,Columns,Parameters,DefaultOrder,Caption);


struct UserColSettings{
  Streamable(UserColSettings)
  long ColID;
  bool Visible;
  long Width;
};

Streamable3(UserColSettings, ColID,Visible,Width);


struct ULData{

  Streamable(ULData)
  MyArray<UserColSettings> Cols;
  long DefaultOrder;

};

Streamable2(ULData,Cols,DefaultOrder);


class Inst : public TObject{
public:
  ObjectManagerStub *OM;
  //DataBuffer PI;
  DataBuffer PD;
  //DataBuffer UL;
  DataBuffer SL;
  ULData ULD;
  PIData PID;
  CParam Params;
  TForm *Form;
  TStringGrid *Grid;
  TPopupMenu *GMenu;
  MyArray<FieldHeader> *fh;
  Inst(){
    Form=0;
    Grid=0;
    GMenu=0;
    fh=0;
    OM=0;
  };

  virtual __fastcall ~Inst(){
    try{
    if (fh) delete fh;
    if (Form) delete Form;
    if (GMenu) delete GMenu;
    } catch(...) {}
  };

__published:

//void __fastcall  EventMapper(TObject* Sender);

};


MyArray<Inst> Instances;


/*__declspec(dllexport)*/
long NewInstance (ObjectManagerStub *om){
  Inst * ptr = new Inst;
  ptr->OM=om;
  ptr->Form=0;
  ptr->Grid=0;
  Instances.Add(ptr);
  return (long) ptr;
};


/*__declspec(dllexport)*/
void CloseInstance (long I){
  Inst * ptr=(Inst*) I;
  long i=Instances.Where(ptr);
  if(i>=0)
    Instances.Destroy(i);
};

/*__declspec(dllexport)*/
void Setup (long I, DataBuffer * _PI, DataBuffer * _PD,DataBuffer *_UL){
  Inst * ptr=(Inst*) I;
  long i=Instances.Where(ptr);
  if(i==-1) return;
  MyMemory m;
  /*
  ptr->PI.Clear();
  ptr->PD.Clear();
  ptr->UL.Clear();
  */

  if(i>=0){
    if(_PI){
    m.LoadBuffer(_PI->GetBuffer(),_PI->Length());
    m >>ptr->PID;
    }
    /*
    if(_PD){
    m.LoadBuffer(_PI->GetBuffer(),_PD->Length());
    m >>ptr->PD;
    }
    */
    if(_UL){
    m.LoadBuffer(_PI->GetBuffer(),_UL->Length());
    m >>ptr->ULD;
    }
  }

};

/*__declspec(dllexport)*/
void SetupSlave (long I, DataBuffer *_SL, CParam *Par){
  Inst * ptr=(Inst*) I;
  long i=Instances.Where(ptr);
  if(i==-1) return;
  MyMemory m;
  ptr->SL.Clear();
  ptr->Params.Clear();


  if(i>=0){
    if(_SL){
    m.LoadBuffer(_SL->GetBuffer(),_SL->Length());
    m >>ptr->SL;
    }
    if(Par){
    m << *Par;
    m >>ptr->Params;
    }
  }

};

/*__declspec(dllexport)*/
void Refresh (long I,  CParam *Par){
  Inst * ptr=(Inst*) I;
  long i=Instances.Where(ptr);
  if(i==-1) return;
  MyMemory m;
  ptr->Params.Clear();
  if(i>=0){
    if(Par){
    m << *Par;
    m >>ptr->Params;
    }

    Run(I);
  }

};



/*__declspec(dllexport)*/
void Run (long I){
  Inst * ptr=(Inst*) I;
  long i=Instances.Where(ptr);
  if(i==-1) return;

  if(!ptr->Form){
    ptr->Form=ptr->OM->GetForm(mwtMDIChild);
  }

  if(!ptr->Grid){
    ptr->Grid = new TStringGrid(ptr->Form);
    ptr->Grid->Parent = ptr->Form;
    ptr->Grid->Align = alClient;
  }
  ptr->Form->Show();
  DataSourceStub *ds = ptr->OM->GetSource(ptr->PID.DataSource,ptr->OM->CurSystem(),ptr->OM->CurBuild());
  if(ds){
    try
    {
        CParam cp;
        long curs = ds->OpenCursor( ptr->PID.MainQuery.c_str(),cp);
        long cnt=0;
        if(curs)
        {
          ptr->Form->Caption = ptr->PID.Caption;

          // читаем названия колонок из запроса
          if(ptr->fh) delete ptr->fh;
          ptr->fh =ds->GetCursorHeared(curs);


          ptr->Grid->Visible = false;

          // загрузка заголовков
          ptr->Grid->RowCount=2;
          ptr->Grid->ColCount=ptr->fh->Count();
          for(long i=0;i<ptr->fh->Count();i++){
            ptr->Grid->Cells[i][0]= ptr->fh->Get(i)->Name;
          }

          AnsiString s;

          // загрузка строк
          RowData *rd;
          while(!ds->NoMoreRows(curs)){
            rd=ds->GetCursorRow(curs);
            cnt++;
            ptr->Grid->RowCount=1+cnt;
            for(long i=0;i<rd->Count();i++){
              s= rd->Get(i)->ToStr();
              ptr->Grid->Cells[i][cnt]=rd->Get(i)->ToStr();
            }
            delete rd;
          };


          ds->CloseCursor(curs);
          ptr->Grid->Visible = true;
        } // curs

     }catch(...){
        ptr->Grid->Visible = true;
     }
  } // ds
};

/*__declspec(dllexport)*/
bool RunModal (long){
  return true;
};            // Ok / Cacell
/*__declspec(dllexport)*/
CParam * GetParams(long){
  return 0;
};    // Params
/*__declspec(dllexport)*/
RowSet * GetSelection(long){
return 0;
}; // RowSet
/*__declspec(dllexport)*/
RowSet * Filteration(long, RowSet *){
  return 0;
}; // RowSet --> RowSet
/*__declspec(dllexport)*/
DataBuffer * EditPI (ObjectManagerStub * manager,DataBuffer * PI,AnsiString System){

    frmEditGrid = new  TfrmEditGrid(Application);
    MyArray<AnsiString> *sl = manager->SourceList(System);
    frmEditGrid->cmbSources->Items->Clear();
    for(long i=0;i<sl->Count();i++){
      frmEditGrid->cmbSources->Items->Add(*sl->Get(i));
    }
    PIData PID2;
    MyMemory m;

    if(PI){
      m.LoadBuffer(PI->GetBuffer(),PI->Length());
      m >>PID2;
    }

    delete sl;
    frmEditGrid->cmbSources->ItemIndex=frmEditGrid->cmbSources->Items->IndexOf(PID2.DataSource);
    frmEditGrid->txtMainQuery->Text = PID2.MainQuery;
    frmEditGrid->txtCaption->Text = PID2.Caption;

    if(frmEditGrid->ShowModal()!=mrOk){
      delete frmEditGrid;
      return PI;
    }

    PID2.DataSource=frmEditGrid->cmbSources->Text;
    PID2.MainQuery=frmEditGrid->txtMainQuery->Text;
    PID2.Caption=frmEditGrid->txtCaption->Text;
    m.Clear();
    m <<PID2;
    delete frmEditGrid;
    return m.MakeDataBuffer();


};
/*__declspec(dllexport)*/
DataBuffer * EditPD (ObjectManagerStub * manager,DataBuffer * PD,AnsiString System){
  Application->MessageBox("Нет специфических настроек","Компонент: GRID",0);
  return PD;
};
/*__declspec(dllexport)*/
DataBuffer * EditUL (ObjectManagerStub * manager,DataBuffer * UL,AnsiString System){
  Application->MessageBox("Нет настроек пользователя","Компонент: GRID",0);
  return 0;
};
/*__declspec(dllexport)*/
DataBuffer * EditSL (ObjectManagerStub * manager,DataBuffer * SL,AnsiString System){
  Application->MessageBox("Нет настроек для зависимого использования","Компонент: GRID",0);
  return 0;
};
//---------------------------------------------------------------------------
#pragma package(smart_init)

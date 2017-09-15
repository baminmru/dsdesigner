//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Impl.h"
#include "MenuEditor.h"

class TT : public TPersistent{
public:
  ObjectManagerStub *OM;
  TForm *MainForm;
  TMainMenu *Menu;
  DataBuffer PI;
  DataBuffer PD;
  DataBuffer UL;
  DataBuffer SL;
  CParam Params;
  MyArray<MenuDef> md;

  TT(){
    Menu=0;
  };

  void SetupMenu();

__published:

void __fastcall  EventMapper(TObject* Sender);

};

void __fastcall  TT::EventMapper(TObject* Sender){
  TMenuItem * m = (TMenuItem *)Sender;

  for(long i=0;i<md.Count();i++){
    if(Sender==md[i]->mnuptr){
      for(long j=0;j<md[i]->Calls.Count();j++){
        try{
         ObjectItemStub * G = OM->NewItem(md[i]->Calls[j]->ObjectName,OM->CurSystem());

         if(G)

          if(md[i]->Calls[j]->RunModal)
           G->RunModal();
          else
           G->Run();

        } catch(...){
        }
      }
    }
  }


};



MyArray<TT> Instances;

void TT::SetupMenu(){
    if(!Menu){
      Menu = new TMainMenu(MainForm);

      // читаем определение меню
      MyMemory m;
      m.LoadBuffer( PI.GetBuffer(),PI.Length());
      m >> md;


      //Menu->Items->Clear();

      // подсчитываем уровень вложенности меню
      long maxlvl=0;
      for(long i=0;i<md.Count();i++){
        if(maxlvl<md[i]->Level)
          maxlvl=md[i]->Level;
      }

      // массив родительских узлов на каждом уровне
      TMenuItem * *n = new TMenuItem * [maxlvl+2];
      n[0]=0;


      // начинаем заполнение
      for(long i=0;i<md.Count();i++){
        if(md[i]->Level>0){

         TMenuItem *ptr1 = new TMenuItem(n[md[i]->Level]);
         n[md[i]->Level+1]=ptr1;
         ptr1->Caption = md[i]->Caption;

         // если в этом меню есть вызовы цепл€ем обработчик
         if(md[i]->Calls.Count()>0)
           ptr1->OnClick = EventMapper;

         // добавл€ем пункт к родительскому узлу
         TMenuItem *ptr =n[(md[i]->Level)];
         ptr->Add(ptr1);

         // сохран€ем адресок, чтобы пон€ть кто где
         md[i]->mnuptr = ptr1;

        }else{

          // все тоже самое, но это главное меню
          if(md[i]->Level<=0){
           n[1]=new TMenuItem(Menu);
           n[1]->Caption=md[i]->Caption;
           if(md[i]->Calls.Count()>0)
             n[1]->OnClick = EventMapper;

           // подцепл€ем в главное меню
           Menu->Items->Add(n[1]);

           // сохран€ем адресок, чтобы пон€ть кто где
           md[i]->mnuptr = n[1];
          }
        }
      }

    }

    // задаем главное меню
    MainForm->Menu=Menu;

};



/*__declspec(dllexport)*/
long NewInstance (ObjectManagerStub *om){
  TT * ptr = new TT;
  ptr->MainForm=0;
  ptr->OM=om;
  Instances.Add(ptr);
  return (long) ptr;
};


/*__declspec(dllexport)*/
void CloseInstance (long I){
  TT * ptr=(TT*) I;
  long i=Instances.Where(ptr);
  if(i>=0)
    Instances.Destroy(i);
};
/*__declspec(dllexport)*/
void Setup (long I, DataBuffer * _PI, DataBuffer * _PD,DataBuffer *_UL){
  TT * ptr=(TT*) I;
  long i=Instances.Where(ptr);
  if(i==-1) return;
  ptr->PI.Clear();
  ptr->PD.Clear();
  ptr->UL.Clear();


  if(i>=0){
    if(_PI){
    ptr->PI.SetBuffer(_PI->Length(),_PI->GetBuffer());
    }
    if(_PD){
      ptr->PD.SetBuffer(_PD->Length(),_PD->GetBuffer());    }
    if(_UL){
      ptr->UL.SetBuffer(_UL->Length(),_UL->GetBuffer());    }
  }

};

/*__declspec(dllexport)*/
void SetupSlave (long I, DataBuffer *_SL, CParam *Par){
  TT * ptr=(TT*) I;
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
  TT * ptr=(TT*) I;
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
  TT * ptr=(TT*) I;
  long i=Instances.Where(ptr);
  if(i==-1) return;

  if(!ptr->MainForm){
    ptr->MainForm=ptr->OM->GetForm(mwtMDIForm);
  }
  ptr->SetupMenu();

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
  DataBuffer *db=PI;
  frmMenuEdit = new TfrmMenuEdit(0);
  frmMenuEdit->SetMenuDef(PI);
  frmMenuEdit->SetCombo(manager->ObjectList(System));
  if(frmMenuEdit->ShowModal()==mrOk)
    db=frmMenuEdit->GetMenuDef();
  delete frmMenuEdit;
  return db;
};
/*__declspec(dllexport)*/
DataBuffer * EditPD (ObjectManagerStub * manager,DataBuffer * PD,AnsiString System){
  Application->MessageBox("Ќет специфических настроек"," омпонент: MENU",0);
  return 0;
};
/*__declspec(dllexport)*/
DataBuffer * EditUL (ObjectManagerStub * manager,DataBuffer * UL,AnsiString System){
  Application->MessageBox("Ќет настроек пользовател€"," омпонент: MENU",0);
  return 0;
};
/*__declspec(dllexport)*/
DataBuffer * EditSL (ObjectManagerStub * manager,DataBuffer * SL,AnsiString System){
  Application->MessageBox("Ќет настроек дл€ зависимого использовани€"," омпонент: MENU",0);
  return 0;
};

//---------------------------------------------------------------------------
#pragma package(smart_init)

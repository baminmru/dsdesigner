#include "bamilib.h"

//dataField
////////////////////////////////////////////////////////////////////////////////
dataField::dataField(
				 long _ID,
				 AnsiString _Name,
				 long _cType,
				 long _sqlType,
				 long _realLen,
				 long _Scale,
				 long _Prec,
				 SWORD _Nullable,
				 SWORD _AutoInc
	 ){
			 Name=_Name;
			 sqlType=_sqlType;
			 realLen=_realLen;
			 Prec=_Prec;
			 Scale=_Scale;
			 Nullable=_Nullable;
			 AutoInc = _AutoInc;
			 ID=_ID;
			 if(_cType== SQL_C_CHAR)
			 realLen =2*realLen+1;
			 value.Allocate(_cType,realLen);
			 defLen=value.len;
			 defValue = NULL;
};

dataField::~dataField(){
	 value.Free();
};

void *dataField::GetBinaryPrt(){
	if (value.ctype == SQL_C_BINARY)
		return value.F.fBinary;
	 else
		return NULL;
};

long dataField::GetLastSize(){
	if (realLen > 0)
		return realLen;
	 else
		return 0;
};

void dataField::Clear(){
	value.Clear();
};

dataField::dataField(const dataField & d){
	sqlType=d.sqlType;
	ID=d.ID;
	Nullable=d.Nullable;
	AutoInc=d.AutoInc;
	Prec=d.Prec;
	Scale=d.Scale;
	defLen=d.defLen;
	realLen=d.realLen;
	Name=NULL;
	d.Name,Name=d.Name;
	value.Allocate(d.value.ctype,d.realLen);
	memcpy(value.F.fPtr,d.value.F.fPtr,value.len);
	 if(!d.defValue)
		 defValue =NULL;
	 else{
		defValue = new fieldCont;
		defValue->Allocate(d.defValue->ctype,defValue->len);
			memcpy(defValue->F.fPtr,d.defValue->F.fPtr,defValue->len);
	 }
};


// Params
////////////////////////////////////////////////////////////////////////////////
Params::Params(const Params &d){
		for(int i=0;i<d.Count();i++){
			Add(new StackValue( *(d.Get(i))),0,d.Name(i));
		}
};

//DataStream
////////////////////////////////////////////////////////////////////////////////
void DataStream::SnapRowToFields(DataSnapshot* ds,long RowID){
			for (int i=0;i<fields.Count();i++){
					StackValue *ptr,*ptr2=ds->GetRow(RowID)->Get(i);
					ptr= fields[i];
					*ptr=*ptr2;
					//fields.Set(i, (ptr=new StackValue(ptr2)));
					//Debug("\nSnapRowToFields RowID =%ld: %s-->%s",RowID,(fields.Name(i)).c_str(),ptr->ToSQLStr().c_str());
			}
};

void DataStream::FieldsToSnapRow(DataSnapshot* ds,long RowID){
      if(ds->GetRow(RowID)->Count() !=fields.Count()){
		ds->GetRow(RowID)->Clear();
		for (int i=0;i<fields.Count();i++){
			 ds->GetRow(RowID)->Add(new StackValue(fields[i]));
		}
      }else{
      	for (int i=0;i<fields.Count();i++){
			 *ds->GetRow(RowID)->Get(i)=*fields[i];
		}
      }

};

void DataStream::KillSnapshot(DataSnapshot* ds){
	if(ds->IsLocked()) {
    	 Snapshots.Destroy(ds);
				 return;
    }
    if(!ds->DecUsage()){
		Snapshots.Destroy(ds);
    }
};

void DataStream::RestoreParams(DataSnapshot* ds){
	 params.Clear();
	 // перегружаем параметры запроса из Snap
	 for (int i=0;i<ds->params.Count();i++)
		params.Add(new StackValue(ds->params[i]),0,ds->params.Name(i));

	 OrderBy=ds->OrderBy;
	 SubCondition=ds->SubCondition;
};


void DataStream::Insert(DataSnapshot* ds,Params *_fields, bool FullRefresh){
	 if(!AllowInsert()) return;

	RestoreParams(ds);

	// инициализируем значениями по умолчанию или пустыми
	 BeforInsert();
	//	fields.Clear();

	 // загружаем тоько нужные поля
	 if(_fields)
		for(int i=0;i<_fields->Count();i++){
				for(int j=0;j<fields.Count();j++){
					if(fields.Name(j)==_fields->Name(i)){
							 *fields[j]=*(*_fields)[i];
							 break;
						}
				 }
			}


	 // непосредственная вставка
	if (Inserting()){
		if(FullRefresh)
				RefreshSnapshot(ds);
			else{
				RefreshRow();
								ds->LastRowRefresh=TDateTime::CurrentDateTime();
				ds->AddRow(new RowBuffer);
				FieldsToSnapRow(ds,ds->TotalRowCount()-1);
			}
	 }
};

void DataStream::Delete(DataSnapshot* ds,long RowID, bool FullRefresh){
	if(!AllowDelete()) return;

	 RestoreParams(ds);

	 SnapRowToFields(ds,RowID);

	 // непосредственная вставка
	if (Deleteing()){
		if(FullRefresh)
				RefreshSnapshot(ds);
			else{
				ds->DeleteRow(RowID);
			}
	 }

};
void DataStream::Update(DataSnapshot* ds,long RowID, bool FullRefresh){
	if(!AllowUpdate()) return;

	 RestoreParams(ds);

	 SnapRowToFields(ds,RowID);

	 // непосредственная вставка
	if (Updateing()){
		if(FullRefresh)
				RefreshSnapshot(ds);
			else{
				RefreshRow();
								ds->LastRowRefresh=TDateTime::CurrentDateTime();
					FieldsToSnapRow(ds,RowID);
			}
	 }
};



void DataStream::Refresh(){
	 //char buf[51];
	 fields.Clear();

	 /*
	 for(int j=0;j<5;j++){
			sprintf(buf,"F%03d",j);
			fields.Add(new dataField(j,buf));
			fields[j]->GetCont()->Allocate(SQL_C_CHAR,50);
			sprintf(buf,"field F%03d data",j);
			fields[j]->GetCont()->LoadFromString(buf);
	 }
	 */
};

void DataStream::RefreshRow(){
	// осуществление запроса одной строки данных
		//ds->LastRowRefresh=TDateTime::CurrentDateTime();
};

bool DataStream::Updateing(){
	// Осуществление изменения строки
	 return false;
};


bool DataStream::Deleteing(){
	// Осуществление удаления строки
	 return false;
};

bool DataStream::Inserting(){
	// Осуществление вставки строки
	 return false;
};

void DataStream::MakeInsertFields(Params * fieldsToInsert){
	 if(!fieldsToInsert) return;
	 fieldsToInsert->Clear();

	 StackValue * sv;

	 for(long i=0;i<fields.Count();i++){
		 fieldsToInsert->Add(sv=new StackValue(fields[i]),0,fields.Name(i));
		 sv->Erase();
	 }
		 /*
		for(int j=0;j<defaults.Count();j++){
		for (int i=0;i<fieldsToInsert->Count();i++){
			 if(defaults.Name(j)==fieldsToInsert->Name(i)){
						*fieldsToInsert->Get(i)=*defaults[j];
						break;
				 }
			}
	}
		*/

};


void DataStream::BeforInsert(){
	for (int i=0;i<fields.Count();i++){
			fields[i]->Erase();
		}
   /*
		// перегружаем умолчательные значения , если они заданы
		for(int j=0;j<defaults.Count();j++){
	 for (int i=0;i<fields.Count();i++){
				if(defaults.Name(j)==fields.Name(i)){
						*fields[i]=*defaults[j];
						break;
				 }
			}
	}
   */
};


bool DataStream::FetchNext(){
	return false;
};


void DataStream::SnapInit(DataSnapshot* ds){
	/*ds->ClearFieldDef();
	for (int i=0;i<fields.Count();i++)
		 ds->AddFieldDef(fields.Name(i),new SnapFieldDef(true,255));
 */
};

DataSnapshot* DataStream::GetSnapshot(bool Lock){
	if(Lock) return CreateSnapshot(Lock);
	for(long i=0;i<Snapshots.Count();i++){
			if(!Snapshots[i]->IsLocked()){
				if(Snapshots[i]->SubCondition==SubCondition && Snapshots[i]->OrderBy==OrderBy){
					for (int j=0;j<params.Count();j++){
						if(!(*(Snapshots[i]->params[j])==*params[j])){
							goto NextSnap;
						}
					}
                   	Snapshots[i]->IncUsage();
					return Snapshots[i];
				}
			}
		NextSnap:
	}
	return CreateSnapshot(Lock);
};


DataSnapshot* DataStream::CreateSnapshot(bool Lock){
	 DataSnapshot*ds;
	 long j;
	 ds = new DataSnapshot();
	 ds->IncUsage();

	 // выставляем признак возможности обновления
	 ds->SetLock(Lock);

	 // регистрируем новый Snap в коллекции
	 Snapshots.Add(ds);

	 // осуществляем запрос к данным
	 Refresh();

	 // для каждой стоки из результата
	 j=0;

	 // Инициализация
	 SnapInit(ds);


	 while (FetchNext()){
		// создаем буфер строки
		 ds->AddRow(new RowBuffer);

			// перегружаем значение полей
			for (int i=0;i<fields.Count();i++){
				 ds->AddRowField(j,new StackValue(fields[i]));

				 //fc->LoadValue(*fields[i]->GetCont());
			}
			j++;
	 }

	 Debug("\n%ld Rows fetched from Stream\n",j);

	 // перегружаем параметры запроса в Snap для будущего использования
	 for (int i=0;i<params.Count();i++)
		ds->params.Add(new StackValue(params[i]),0,params.Name(i));

	 ds->SubCondition=SubCondition;
	 ds->OrderBy=OrderBy;

	 ds->LastRowRefresh=TDateTime::CurrentDateTime();
	 ds->LastRefresh=TDateTime::CurrentDateTime();

	 return ds;
};

void DataStream::RefreshSnapshot(DataSnapshot *ds){
	 //if (ds->IsLocked()) return;

	 long j=0;

	 RestoreParams(ds);
	 /*
	 SortDef *sd;
	 FilterDef *fd;
	 sd=ds->CurrentSort;
	 fd=ds->CurrentFilter;
	 */

	 // все потерли
	 ds->Clear();

	 // осуществляем запрос к данным
	 Refresh();

	 // для каждой стоки из результата
	 while (FetchNext()){
		// создаем буфер строки
		 ds->AddRow( new RowBuffer);

			// перегружаем значение полей
			for (int i=0;i<fields.Count();i++){
				 ds->AddRowField(j,new StackValue(fields[i]));
			}
			j++;
	 }
	 ds->LastRowRefresh=TDateTime::CurrentDateTime();
	 ds->LastRefresh=TDateTime::CurrentDateTime();

	 /*
	 if(sd)
		 ds->SetSort(sd,&fields);

	 if(fd)
		 ds->SetFilter(fd,&fields);
   */
};



//DataSnapshot
/////////////////////////////////////////////////////////////////////////////////////////
void DataSnapshot::DeleteRow(long RowID){
	/*long SortRow,InfoRow;
	SortRow=*FilterMap[RowID];
	InfoRow=*SortMap[SortRow];
	Info.Destroy(InfoRow);
	SortMap.Destroy(SortRow);
	FilterMap.Destroy(RowID);

	for(long i=0;i<FilterMap.Count();i++){
		if(*FilterMap[i]>RowID)
			*(FilterMap[i])=*(FilterMap[i])-1;
	}

	for(long i=0;i<SortMap.Count();i++){
		if(*SortMap[i]>SortRow)
			*SortMap[i]=*SortMap[i]-1;
	}
	*/
	Info.Destroy(RowID);

};

/*
void DataSnapshot::SetSort(SortDef *Sort, Params *fields){


	ClearSort();
	CurrentSort=Sort;
	if(!Sort){
		return;
	}

	MyArray<long> fld; // номер поля внутри строки

	// строим индекс для номеров полей
	for(long j=0;j<Sort->Count();j++)
	 for(long k=0;k<fields->Count();k++)
		 if(Sort->Get(j)->Name==fields->Name(k)){
			 fld.Add( new long(k));
			 break;
		 }

	MyArray<DoubleLong> *Group,*NextGroup;
	StackValue cur,prev;
	long fff;
	bool asc;
	long start;
	bool changed=true;


	Group = new MyArray<DoubleLong>;
	DoubleLong *dl = new DoubleLong;
	dl->From=0;
	dl->To=Info.Count()-1;
	Group->Add(dl);


	for(long j=0;changed && j<Sort->Count();j++){

			fff=*fld[j];
			asc=Sort->Get(j)->Asc;
			NextGroup= new MyArray<DoubleLong>;
			changed=false;

			// просматриваем каждую из групп равных полей
			for(long g=0;g<Group->Count();g++){

				//Debug("\n Next group %ld ... %ld\n Befor Sort",(*Group)[g]->From,(*Group)[g]->To);

				//for(long ijk=0;ijk<SortMap.Count();ijk++){
				//					Debug("\n %ld: %ld (%s)",ijk,*SortMap[ijk], Info[*SortMap[ijk]]->Get(fff)->ToStr().c_str());

				//}


				// если группа состоит из одного поля, то делать ничего не надо !!!
				if((*Group)[g]->From!=(*Group)[g]->To){

					// честно сортируем внутри группы т. е. там где предыдущие поля строго равны
					for(long i1=(*Group)[g]->From;i1<(*Group)[g]->To;i1++){
						prev=*Info[*SortMap[i1]]->Get(fff);
						for(long i2=i1+1;i2<=(*Group)[g]->To;i2++){
							cur=*Info[*SortMap[i2]]->Get(fff);
							bool change = false;

							// надо ли менять
							if(!asc && prev.LessThen(cur))
								change=true;

							if(asc && !prev.LessThen(cur) && !prev.IsEqual(cur))
							 change=true;

							if(change){
							 long i1v,i2v;
							 i1v=*SortMap[i1];
							 i2v=*SortMap[i2];
							 *SortMap[i1]=i2v;
							 *SortMap[i2]=i1v;

							 //Debug("\nNext change %ld <-> %ld (%s,%s) \n",i1,i2,prev.ToStr().c_str(),cur.ToStr().c_str());
							 //prev=cur;
							 //for(long ijk=0;ijk<SortMap.Count();ijk++)
							 //		Debug("\n %ld: %ld (%s)",ijk,*SortMap[ijk], Info[*SortMap[ijk]]->Get(fff)->ToStr().c_str());

							 }

						}
					}


					// достраиваем следующий массив групп
					prev=*(Info[*SortMap[Group->Get(g)->From]]->Get(fff));

					start=(*Group)[g]->From;
					for(long i=(*Group)[g]->From+1;i<=(*Group)[g]->To;i++){
						cur=*Info[*SortMap[i]]->Get(fff);
						// накапливаем следующую группу
						if(!prev.IsEqual(cur)){
							DoubleLong *dl = new DoubleLong;
							dl->From=start;
							dl->To=i-1;
							NextGroup->Add(dl);
							start=i;
							prev=cur;
					 }
					}

					// если все кнчилось не одним полем надо добавить еще одну группу
					if(start!=(*Group)[g]->To){
							DoubleLong *dl = new DoubleLong;
							dl->From=start;
							dl->To=(*Group)[g]->To;
							NextGroup->Add(dl);
					}

					changed = true;
				}

		}  // конец цикла по группам

		delete Group;
		Group=NextGroup;

	}// конец цикла по полям в определении сортировки

	delete Group;


	// восстанавливаем фильтрацию
	SetFilter(CurrentFilter,fields);

};

void DataSnapshot::SetFilter(FilterDef *Filter, Params *fields){
	CurrentFilter=Filter;

	// если нет фильра просто разрешаем все записи
	if(!Filter){
		 ClearFilter();
		 return;
	}

	FilterMap.Clear();
	MyArray<long> fld; // номер поля внутри строки
	bool Yes;

	// строим индекс для номеров полей
	for(long j=0;j<Filter->Count();j++)
	 for(long k=0;k<fields->Count();k++)
		 if(Filter->Get(j)->Name==fields->Name(k)){
			 fld.Add( new long(k));
			 break;
		 }


	// выясняем какие строки пропускаются фильтром
	for(long i=0;i<SortMap.Count();i++){
		Yes=true;
		RowBuffer &rb=*GetSortedRow(i);
		for(long j=0;j<Filter->Count();j++){
			Yes = Yes && rb[*fld[j]]->Filter(Filter->Get(j)->Condition, Filter->Get(j)->From, Filter->Get(j)->To);
			if(!Yes)
				break;
		}
		if(Yes)
			FilterMap.Add(new long(i));
	}

};


bool DataSnapshot::AllowNull(AnsiString Name){
	if(GetFieldDef(Name))
		return GetFieldDef(Name)->AllowNull;

	return true;
};
*/


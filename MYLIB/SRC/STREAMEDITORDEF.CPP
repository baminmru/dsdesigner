//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "bamilib.h"


// OutRectGroup
////////////////////////////////////////////////////////////////////////////////

SimpleStream & operator >> (SimpleStream & s,OutRectGroup &a){
			return s >> a.Condition;
		};
SimpleStream & operator << (SimpleStream & s,OutRectGroup &a){
					return s << a.Condition;
		};



OutRectGroup::OutRectGroup(AnsiString _GroupExpression){
	 Condition=_GroupExpression;
	 t=0;
	 l=0;
};

AnsiString OutRectGroup::Evaluate(MiniParser *p){
	if(!Condition.IsEmpty()){
		if(!t){
			t=new TokenList;
			l=new Labels;
			p->CompileExpression(Condition,t,l);
		}
		return p->ParseExpression(t,l);
		} else
			return Condition;
};



SimpleStream & operator >> (SimpleStream & s,DoubleBool & b){
			return s >> b.Befor >> b.After;
};

SimpleStream & operator << (SimpleStream & s,DoubleBool & b){
					return s << b.Befor << b.After;
};



SimpleStream & operator >> (SimpleStream & s,EditFieldChanges & d){
			return s >> d.ParamName >> d.SetAsValue >> d.ValueOrName;
		};

SimpleStream & operator << (SimpleStream & s,EditFieldChanges & d){
					return s << d.ParamName << d.SetAsValue << d.ValueOrName;
		};

////////////////////////////////////////////////////////////////////////////////////////

SimpleStream & operator >> (SimpleStream & s, FieldAccessType &ft){
			int f;
			s >> f;
			ft=(FieldAccessType) f;
			return s;
		};
SimpleStream & operator << (SimpleStream & s,FieldAccessType &ft){
					return s <<(int) ft;
};


SimpleStream & operator >> (SimpleStream & s,EditFieldDisables & d){
			return s >> d.FieldName >> d.InsertAccess >> d.EditAccess;
		};
SimpleStream & operator << (SimpleStream & s,EditFieldDisables & d){
			return s << d.FieldName << d.InsertAccess << d.EditAccess;
		};


SimpleStream & operator >> (SimpleStream & s,EditFieldActions &a){
			return s >> a.SubCondition >> a.Changes >> a.DefaultChanges >> a.Disables;
};

SimpleStream & operator << (SimpleStream & s,EditFieldActions &a){
			return s << a.SubCondition << a.Changes << a.DefaultChanges << a.Disables;
};



SimpleStream & operator >> (SimpleStream & s,StreamEditorLink & l){
			return s >> l.MasterName >> l.SlaveName >> l.MasterCondition >> l.WhenChanged >> l.Caption >>l.SlaveCaption;
	};

SimpleStream & operator << (SimpleStream & s ,StreamEditorLink & l){
			return s << l.MasterName << l.SlaveName << l.MasterCondition << l.WhenChanged << l.Caption << l.SlaveCaption;
	};



SimpleStream &  operator >> (SimpleStream & s,EditableFieldAttr &a){
			s >> a.FieldName >> a.Label >> a.ControlType >> a.DefInsertAccess >> a.DefEditAccess >>
			a.StreamName >> a.SyncField >> a.ShowField >> a.FillOnGetFocus >> a.FillOnFormShow >>
			a.AllowNull >> a.SQLType >> a.MaxFieldLen ;
			long ptr;
			s  >> ptr;
			if(ptr){
				a.WhenChanged = new EditFieldActions();
				s>> *a.WhenChanged;
			}
			return s;
	};

SimpleStream & operator << (SimpleStream & s,EditableFieldAttr &a){
			s << a.FieldName << a.Label << a.ControlType << a.DefInsertAccess << a.DefEditAccess <<
			a.StreamName << a.SyncField << a.ShowField << a.FillOnGetFocus << a.FillOnFormShow <<
			a.AllowNull << a.SQLType << a.MaxFieldLen ;
			s  << (long)a.WhenChanged;
			if(a.WhenChanged){
				s<< *a.WhenChanged;
			}
			return s;
	};


SimpleStream & operator >> (SimpleStream & s,ColumnDef &c){
			return s >> c.Value >> c.FieldKind >> c.Width
			>> c.Caption >> c.HeaderVert >> c.Vert >> c.Horiz >>c.GroupName >> c.BeforChanged>> c.Style>>c.Visible;
	};

SimpleStream & operator << (SimpleStream & s,ColumnDef &c){
			return s << c.Value << c.FieldKind << c.Width
			<< c.Caption << c.HeaderVert << c.Vert << c.Horiz << c.GroupName << c.BeforChanged<<c.Style<<c.Visible;

	};


DataStream *StreamEditorDef::GetDataStream(AnsiString *SubCondition,AnsiString *OrderBy){
		return Owner->GetNewStream( this ,SubCondition,OrderBy);
};


STREAM_LIST(StackValue);

SimpleStream & operator >> (SimpleStream & s,StreamEditorDef &d){
		s >>  d.Caption;
		s >>  d.ConnectionName;
		s >>  d.SQL;
		s >>  d.ROWSQL;
		s >>  d.INSERT;
		s >>  d.UPDATE;
		s >>  d.sDELETE;
		s >>  d.ParamNames;
		s >>  d.Defaults;
		s >>  d.SubCondition;
		s >>  d.OrderBy;
		s >>  d.FormDef;
		s >>  d.ColDef;
		s >>  d.Groups;
   s >>  d.Indexes;
		return s;
	};

SimpleStream & operator << (SimpleStream & s,StreamEditorDef &d){
		s <<  d.Caption;
		s <<  d.ConnectionName;
		s <<  d.SQL;
		s <<  d.ROWSQL;
		s <<  d.INSERT;
		s <<  d.UPDATE;
		s <<  d.sDELETE;
		s <<  d.ParamNames;
		s <<  d.Defaults;
		s <<  d.SubCondition;
		s <<  d.OrderBy;
		s <<  d.FormDef;
		s <<  d.ColDef;
		s <<  d.Groups;
   s <<  d.Indexes;
		return s;
	};





SimpleStream & operator >> (SimpleStream & s,StreamTransaction &x){
     s >>  x.EditorName;
     s >>  x.FormDef;
     s >>  x.TransactionSource;
     s >>  x.AfterTransaction;
     s >>  x.TranCondition;
     s >>  x.Transaction;
     s >>  x.Caption;
     s >>  x.Message;
     return s;

};

SimpleStream & operator << (SimpleStream & s,StreamTransaction &x){
     s <<  x.EditorName;
     s <<  x.FormDef;
     s <<  x.TransactionSource;
     s <<  x.AfterTransaction;
     s <<  x.TranCondition;
     s <<  x.Transaction;
     s <<  x.Caption;
     s <<  x.Message;
     return s;
};


SimpleStream & operator >> (SimpleStream & s,StreamIndex &x){
	     s >>  x.Caption;
      s >>  x.OrderBy;
      return s;
};

SimpleStream & operator << (SimpleStream & s,StreamIndex &x){
	     s <<  x.Caption;
      s <<  x.OrderBy;
      return s;
};

//---------------------------------------------------------------------------
#pragma package(smart_init)

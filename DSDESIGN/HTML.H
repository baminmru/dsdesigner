#include "task.h"


class Documentator : public HTMLProto{
 static  AnsiString CTLNAME[13];
 AnsiString Path;
 AnsiString FilePrephics;
public:

  Documentator(AnsiString _Path, AnsiString _FilePrephics){
    FilePrephics=_FilePrephics;
    Path=_Path;
    CTLNAME[0] = "�� �������������";
  CTLNAME[1] = "�����"           ;
  CTLNAME[2] = "�����"           ;
  CTLNAME[3] = "�����"           ;
  CTLNAME[4] = "������������� �����" ;
  CTLNAME[5] = "������"              ;
  CTLNAME[6] = "����"                ;
  CTLNAME[7] = "�����"               ;
  CTLNAME[8] = "���� �����"          ;
  CTLNAME[9] = "��/���"              ;
  CTLNAME[10] = "���������� �������� (������)";

  }
  AnsiString DescribeEditor(LMEAppDef & App,AnsiString EditorName,bool RefOnly = true);
  AnsiString DescribeFilter(LMEAppDef & App,AnsiString FilterName,bool RefOnly = true);
  AnsiString DescribeReport(LMEAppDef & App,AnsiString ReportName,bool RefOnly = true);
  AnsiString DescribeTrans(LMEAppDef & App,AnsiString EditorName);
  AnsiString DescribeLinks(LMEAppDef & App,AnsiString EditorName);

  void  MakeHTML( LMEAppDef & App);
};

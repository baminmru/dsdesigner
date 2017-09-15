#include "task.h"


class Documentator : public HTMLProto{
 static  AnsiString CTLNAME[13];
 AnsiString Path;
 AnsiString FilePrephics;
public:

  Documentator(AnsiString _Path, AnsiString _FilePrephics){
    FilePrephics=_FilePrephics;
    Path=_Path;
    CTLNAME[0] = "Не редактируется";
  CTLNAME[1] = "Число"           ;
  CTLNAME[2] = "Число"           ;
  CTLNAME[3] = "Текст"           ;
  CTLNAME[4] = "Многострочный текст" ;
  CTLNAME[5] = "Пароль"              ;
  CTLNAME[6] = "Дата"                ;
  CTLNAME[7] = "Время"               ;
  CTLNAME[8] = "Дата Время"          ;
  CTLNAME[9] = "Да/Нет"              ;
  CTLNAME[10] = "Встроенный редактор (список)";

  }
  AnsiString DescribeEditor(LMEAppDef & App,AnsiString EditorName,bool RefOnly = true);
  AnsiString DescribeFilter(LMEAppDef & App,AnsiString FilterName,bool RefOnly = true);
  AnsiString DescribeReport(LMEAppDef & App,AnsiString ReportName,bool RefOnly = true);
  AnsiString DescribeTrans(LMEAppDef & App,AnsiString EditorName);
  AnsiString DescribeLinks(LMEAppDef & App,AnsiString EditorName);

  void  MakeHTML( LMEAppDef & App);
};

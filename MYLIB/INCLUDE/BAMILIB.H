#ifndef _BAMILIB_
#define _BAMILIB_

#include <vcl.h>
#include <vcl/dstring.h>
#include <vcl/sysdefs.h>


#ifdef __BAMI_DLL_
#define prolog __export
#else
#define prolog
#endif


#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <iostream.h>

#include "sql.h"
#include "sqlext.h"

#include "macro.h"
#include "mystream.h"
#include "template.h"
#include "date.h"
#include "num2str.h"
#include "mystring.h"

#include "fieldcont.h"
#include "MiniParser.h"
#include "DataStream.h"

#include "odbcEng.h"
#include "OdbcDataStream.h"

#include "styles.h"
#include "StreamEditorDef.h"
#include "DataSource.h"

#include "HTMLPROTO.h"
#include "excels.h"

#endif

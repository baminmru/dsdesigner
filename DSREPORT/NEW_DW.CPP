//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "New_dw.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
void TNRect::SetNull()
{
 Left=0;
 Top=0;
 Right=0;
 Bottom=0;
}
void TNRect::Set(int _Left, int _Top, int _Right, int _Bottom)
{
 Left=_Left;
 Top=_Top;
 Right=_Right;
 Bottom=_Bottom;
}
int TNRect::Width()
{
 return Right-Left;
}
int TNRect::Height()
{
 return Bottom-Top;
}
bool TNRect::Touches(TNRect& other)
{
 return (other.Right>Left && other.Left<Right && other.Bottom>Top && other.Top<Bottom);
}
TNRect& TNRect::MoveTo(int x, int y)
{
 Right=x+Width();
 Bottom=y+Height();
 Left=x;
 Top=y;
 return *this;
}
TNRect& TNRect::Inflate(int dx, int dy)
{
 Left-=dx;
 Top-=dy;
 Right+=dx;
 Bottom+=dy;
 return *this;
}
TNRect::TNRect()
{
 Left=0;
 Top=0;
 Right=0;
 Bottom=0;
}
TNRect::TNRect(int _Left, int _Top, int _Right, int _Bottom)
{
 Set(_Left, _Top, _Right, _Bottom);
}
TNRect::TNRect(TRect Rect)
{
 Left=Rect.Left;
 Top=Rect.Top;
 Right=Rect.Right;
 Bottom=Rect.Bottom;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)

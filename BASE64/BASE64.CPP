//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Base64.h"


#pragma package(smart_init)


namespace Base64
{


const unsigned char* TBase64::sm_pBase64Table = (const unsigned char*)
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";


const unsigned char TBase64::sm_chExtra = '=';


TBase64::TBase64()
{
}


TBase64::~TBase64()
{
}


AnsiString TBase64::Encode(AnsiString str)
{
	if (str.IsEmpty())
		return AnsiString();

	AnsiString strResult;
	unsigned char* lpsz8Buf;
	unsigned char* lpsz6Buf;
	int n8BufLen, n8Ctr;
	int n6BufLen, n6Ctr;
	int nExtraNum = 0;

	n8BufLen = str.Length();
	if (n8BufLen % 3 != 0)
	{
		nExtraNum = 3 - n8BufLen % 3;
		n8BufLen += nExtraNum;
	}
	n6BufLen = n8BufLen / 3 * 4;

	lpsz8Buf = new unsigned char[n8BufLen + 1];
	lpsz6Buf = new unsigned char[n6BufLen + 1];

	try
	{
		::ZeroMemory(lpsz8Buf, n8BufLen + 1);
		::ZeroMemory(lpsz6Buf, n6BufLen + 1);
		::strcpy(lpsz8Buf, str.c_str());

		for (n8Ctr = 0, n6Ctr = 0; n8Ctr < n8BufLen; n8Ctr += 3, n6Ctr += 4)
			O3ToS4(lpsz8Buf + n8Ctr, lpsz6Buf + n6Ctr);

		for (n6Ctr = 0; n6Ctr < n6BufLen - nExtraNum; n6Ctr++)
			lpsz6Buf[n6Ctr] = sm_pBase64Table[lpsz6Buf[n6Ctr]];
		if (nExtraNum > 0)
		{
			lpsz6Buf[n6BufLen - 1] = sm_chExtra;
			if (nExtraNum == 2)
				lpsz6Buf[n6BufLen - 2] = sm_chExtra;
		}

		strResult = AnsiString((char*)lpsz6Buf).Unique();
	}
	__finally
	{
		delete[] lpsz6Buf;
		delete[] lpsz8Buf;
	}

	return strResult;
}


AnsiString TBase64::Decode(AnsiString str)
{
	if (str.IsEmpty())
		return AnsiString();

	AnsiString strResult;
	unsigned char* lpsz8Buf;
	unsigned char* lpsz6Buf;
	int n8BufLen, n8Ctr;
	int n6BufLen, n6Ctr;
	int nExtraNum = 0;

	n6BufLen = str.Length();
	if (str[str.Length()] == sm_chExtra)
		nExtraNum++;
	if (str[str.Length() - 1] == sm_chExtra)
		nExtraNum++;
	n8BufLen = n6BufLen / 4 * 3;

	lpsz6Buf = new unsigned char[n6BufLen + 1];
	lpsz8Buf = new unsigned char[n8BufLen + 1];

	try
	{
		::ZeroMemory(lpsz8Buf, n8BufLen + 1);
		::ZeroMemory(lpsz6Buf, n6BufLen + 1);
		::strcpy(lpsz6Buf, str.c_str());

		for (n6Ctr = 0; n6Ctr < n6BufLen; n6Ctr++)
			lpsz6Buf[n6Ctr] = Decode64(lpsz6Buf[n6Ctr]);

		for (n6Ctr = 0, n8Ctr = 0; n6Ctr < n6BufLen; n6Ctr += 4, n8Ctr += 3)
			S4ToO3(lpsz6Buf + n6Ctr, lpsz8Buf + n8Ctr);

		strResult = AnsiString((char*)lpsz8Buf).Unique();
	}
	__finally
	{
		delete[] lpsz8Buf;
		delete[] lpsz6Buf;
	}

	return strResult;
}


void TBase64::O3ToS4(const unsigned char* lpsz8Buf, unsigned char* lpsz6Buf)
{
	lpsz6Buf[0] = lpsz8Buf[0] >> 2;
	lpsz6Buf[1] = ((lpsz8Buf[0] << 4) & 0x30) | (lpsz8Buf[1] >> 4);
	lpsz6Buf[2] = ((lpsz8Buf[1] << 2) & 0x3C) | (lpsz8Buf[2] >> 6);
	lpsz6Buf[3] = lpsz8Buf[2] & 0x3F;
}


void TBase64::S4ToO3(const unsigned char* lpsz6Buf, unsigned char* lpsz8Buf)
{
	lpsz8Buf[0] = (lpsz6Buf[0] << 2) | (lpsz6Buf[1] >> 4);
	lpsz8Buf[1] = (lpsz6Buf[1] << 4) | (lpsz6Buf[2] >> 2);
	lpsz8Buf[2] = (lpsz6Buf[2] << 6) | lpsz6Buf[3];
}


unsigned char TBase64::Decode64(unsigned char ch)
{
	if (ch == sm_chExtra)
		return 0;
	else
		return ::strchr((char*)sm_pBase64Table, (char)ch) - sm_pBase64Table;
}




void * TBase64::Encode(const void *InBuff, long InSize ,long * OutSize)
{

  if( OutSize ) *OutSize=0;
	if (!InBuff || !InSize )
		return 0;

	void * strResult;
	unsigned char* lpsz8Buf;
	unsigned char* lpsz6Buf;
	int n8BufLen, n8Ctr;
	int n6BufLen, n6Ctr;
	int nExtraNum = 0;

	n8BufLen = InSize;
	if (n8BufLen % 3 != 0)
	{
		nExtraNum = 3 - n8BufLen % 3;
		n8BufLen += nExtraNum;
	}
	n6BufLen = n8BufLen / 3 * 4;

	lpsz8Buf = new unsigned char[n8BufLen + 1];
	lpsz6Buf = new unsigned char[n6BufLen + 1];

	try
	{
		::ZeroMemory(lpsz8Buf, n8BufLen + 1);
		::ZeroMemory(lpsz6Buf, n6BufLen + 1);
		::memcpy(lpsz8Buf, InBuff,InSize);

		for (n8Ctr = 0, n6Ctr = 0; n8Ctr < n8BufLen; n8Ctr += 3, n6Ctr += 4)
			O3ToS4(lpsz8Buf + n8Ctr, lpsz6Buf + n6Ctr);

		for (n6Ctr = 0; n6Ctr < n6BufLen - nExtraNum; n6Ctr++)
			lpsz6Buf[n6Ctr] = sm_pBase64Table[lpsz6Buf[n6Ctr]];
		if (nExtraNum > 0)
		{
			lpsz6Buf[n6BufLen - 1] = sm_chExtra;
			if (nExtraNum == 2)
				lpsz6Buf[n6BufLen - 2] = sm_chExtra;
		}

		strResult = lpsz6Buf;
	}
	__finally
	{
		//delete[] lpsz6Buf;
		delete[] lpsz8Buf;
	}
  if( OutSize ) *OutSize=n6BufLen;
	return strResult;
}


void* TBase64::Decode(const void *InBuff, long InSize ,long *OutSize)
{
  if( OutSize ) *OutSize=0;
	if (!InBuff || !InSize )
		return 0;

	void * strResult;
	unsigned char* lpsz8Buf;
	unsigned char* lpsz6Buf;
	int n8BufLen, n8Ctr;
	int n6BufLen, n6Ctr;
	int nExtraNum = 0;

	n6BufLen = InSize;
	if (((unsigned char*)InBuff)[InSize-1] == sm_chExtra)
		nExtraNum++;
	if (((unsigned char*)InBuff)[InSize - 2] == sm_chExtra)
		nExtraNum++;
	n8BufLen = n6BufLen / 4 * 3;

	lpsz6Buf = new unsigned char[n6BufLen + 1];
	lpsz8Buf = new unsigned char[n8BufLen + 1];

	try
	{
		::ZeroMemory(lpsz8Buf, n8BufLen + 1);
		::ZeroMemory(lpsz6Buf, n6BufLen + 1);
		::memcpy(lpsz6Buf, InBuff,InSize);

		for (n6Ctr = 0; n6Ctr < n6BufLen; n6Ctr++)
			lpsz6Buf[n6Ctr] = Decode64(lpsz6Buf[n6Ctr]);

		for (n6Ctr = 0, n8Ctr = 0; n6Ctr < n6BufLen; n6Ctr += 4, n8Ctr += 3)
			S4ToO3(lpsz6Buf + n6Ctr, lpsz8Buf + n8Ctr);

		strResult = lpsz8Buf;
	}
	__finally
	{
		//delete[] lpsz8Buf;
		delete[] lpsz6Buf;
	}
  if( OutSize ) *OutSize=n8BufLen;
	return strResult;
}



}	// namespace Base64





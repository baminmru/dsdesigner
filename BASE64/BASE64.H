//---------------------------------------------------------------------------
#ifndef Base64ObjH
#define Base64ObjH
//---------------------------------------------------------------------------
namespace Base64
{
//---------------------------------------------------------------------------
class TBase64
{
private:
	static const unsigned char* sm_pBase64Table;
	static const unsigned char  sm_chExtra;

	static void O3ToS4(const unsigned char* lpsz8Buf, unsigned char* lpsz6Buf);
	static void S4ToO3(const unsigned char* lpsz6Buf, unsigned char* lpsz8Buf);
  static unsigned char Decode64(unsigned char ch);

protected:
	TBase64();
	virtual ~TBase64();

public:
	static AnsiString Encode(AnsiString str);
	static AnsiString Decode(AnsiString str);
	static void * Encode(const void *InBuff, long InSize ,long *OutSize);
	static void * Decode(const void *InBuff, long InSize ,long *OutSize);


};
//---------------------------------------------------------------------------
}	// namespace Base64
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Base64;
#endif
//---------------------------------------------------------------------------
#endif


//---------------------------------------------------------------------------
#ifndef SyncObjs2H
#define SyncObjs2H
//---------------------------------------------------------------------------
#include <SyncObjs.hpp>
//---------------------------------------------------------------------------
namespace Syncobjs
{
//---------------------------------------------------------------------------
//
//	Windows Mutex Object
//
//---------------------------------------------------------------------------
class TMutex : public THandleObject
{
private:
	HANDLE FHandle;
	DWORD FLastError;

public:
	__fastcall TMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes,
		bool bInitialOwner, const AnsiString& strName);
	__fastcall ~TMutex();

	virtual void __fastcall Acquire();
	virtual void __fastcall Release();
	virtual TWaitResult __fastcall WaitFor(DWORD dwTimeout);

	__property HANDLE Handle = { read = FHandle };
	__property DWORD LastError  = { read = FLastError };
};
//---------------------------------------------------------------------------
//
//	Windows Counted Semaphore Object
//
//---------------------------------------------------------------------------
class TSemaphore : public THandleObject
{
private:
	HANDLE FHandle;
	DWORD FLastError;

public:
	__fastcall TSemaphore(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
		long lInitialCount, long lMaximumCount, const AnsiString& strName);
	__fastcall ~TSemaphore();

	virtual void __fastcall Acquire();
	virtual void __fastcall Release();
	virtual TWaitResult __fastcall WaitFor(DWORD dwTimeout);

	__property HANDLE Handle = { read = FHandle };
	__property DWORD LastError  = { read = FLastError };
};
//---------------------------------------------------------------------------
//
//	Windows File Change Notification Semaphore Object
//
//---------------------------------------------------------------------------
enum TNotifyFilter
{
	nfChangeFileName   = FILE_NOTIFY_CHANGE_FILE_NAME,
	nfChangeDirName    = FILE_NOTIFY_CHANGE_DIR_NAME,
	nfChangeAttributes = FILE_NOTIFY_CHANGE_ATTRIBUTES,
	nfChangeSize       = FILE_NOTIFY_CHANGE_SIZE,
	nfChangeLastWrite  = FILE_NOTIFY_CHANGE_LAST_WRITE,
	nfChangeSecurity   = FILE_NOTIFY_CHANGE_SECURITY
};
//---------------------------------------------------------------------------
class TFileChangeSemaphore : public THandleObject
{
private:
	HANDLE FHandle;
	DWORD FLastError;

public:
	__fastcall TFileChangeSemaphore(const AnsiString& strPathName,
		bool bWatchSubtree, int iNotifyFilter);
	__fastcall ~TFileChangeSemaphore();

	virtual void __fastcall Acquire();
	virtual void __fastcall Release();
	virtual TWaitResult __fastcall WaitFor(DWORD dwTimeout);

	__property HANDLE Handle = { read = FHandle };
	__property DWORD LastError  = { read = FLastError };
};
//---------------------------------------------------------------------------
}	// namespace Syncobjs
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Syncobjs;
#endif
//---------------------------------------------------------------------------
#endif


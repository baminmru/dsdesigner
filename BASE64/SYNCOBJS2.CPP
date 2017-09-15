//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SyncObjs2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace Syncobjs
{
//---------------------------------------------------------------------------
//
//	Windows Mutex Object
//
//---------------------------------------------------------------------------
__fastcall TMutex::TMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes,
	bool bInitialOwner, const AnsiString& strName)
{
	FLastError = ERROR_SUCCESS;
	FHandle = ::CreateMutex(lpMutexAttributes, bInitialOwner, strName.c_str());
	if (!FHandle)
		FLastError = ::GetLastError();
}
//---------------------------------------------------------------------------
__fastcall TMutex::~TMutex()
{
	if (FHandle)
		::CloseHandle(FHandle);
}
//---------------------------------------------------------------------------
void __fastcall TMutex::Acquire()
{
	if (FHandle)
	{
		FLastError = ERROR_SUCCESS;
		if (::WaitForSingleObject(FHandle, INFINITE) == WAIT_FAILED)
			FLastError = ::GetLastError();
	}
}
//---------------------------------------------------------------------------
void __fastcall TMutex::Release()
{
	if (FHandle)
	{
		FLastError = ERROR_SUCCESS;
		if (!::ReleaseMutex(FHandle))
			FLastError = ::GetLastError();
	}
}
//---------------------------------------------------------------------------
TWaitResult __fastcall TMutex::WaitFor(DWORD dwTimeout)
{
	if (FHandle)
	{
		FLastError = ERROR_SUCCESS;
		switch (::WaitForSingleObject(FHandle, dwTimeout))
		{
			case WAIT_OBJECT_0:  return wrSignaled;
			case WAIT_ABANDONED: return wrAbandoned;
			case WAIT_TIMEOUT:   return wrTimeout;
			case WAIT_FAILED:    FLastError = ::GetLastError();
			default:             return wrError;
		}
	}
	return wrError;
}
//---------------------------------------------------------------------------
//
//	Windows Counted Semaphore Object
//
//---------------------------------------------------------------------------
__fastcall TSemaphore::TSemaphore(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
	long lInitialCount, long lMaximumCount, const AnsiString& strName)
{
	FLastError = ERROR_SUCCESS;
	FHandle = ::CreateSemaphore(
		lpSemaphoreAttributes, lInitialCount, lMaximumCount, strName.c_str());
	if (!FHandle)
		FLastError = ::GetLastError();
}
//---------------------------------------------------------------------------
__fastcall TSemaphore::~TSemaphore()
{
	if (FHandle)
		::CloseHandle(FHandle);
}
//---------------------------------------------------------------------------
void __fastcall TSemaphore::Acquire()
{
	if (FHandle)
	{
		FLastError = ERROR_SUCCESS;
		if (::WaitForSingleObject(FHandle, INFINITE) == WAIT_FAILED)
			FLastError = ::GetLastError();
	}
}
//---------------------------------------------------------------------------
void __fastcall TSemaphore::Release()
{
	if (FHandle)
	{
		long lDummy;
		FLastError = ERROR_SUCCESS;
		if (!::ReleaseSemaphore(FHandle, 1, &lDummy))
			FLastError = ::GetLastError();
	}
}
//---------------------------------------------------------------------------
TWaitResult __fastcall TSemaphore::WaitFor(DWORD dwTimeout)
{
	if (FHandle)
	{
		FLastError = ERROR_SUCCESS;
		switch (::WaitForSingleObject(FHandle, dwTimeout))
		{
			case WAIT_OBJECT_0:  return wrSignaled;
			case WAIT_ABANDONED: return wrAbandoned;
			case WAIT_TIMEOUT:   return wrTimeout;
			case WAIT_FAILED:    FLastError = ::GetLastError();
			default:             return wrError;
		}
	}
	return wrError;
}
//---------------------------------------------------------------------------
//
//	Windows File Change Notificaton Semaphore Object
//
//---------------------------------------------------------------------------
__fastcall TFileChangeSemaphore::TFileChangeSemaphore(
	const AnsiString& strPathName, bool bWatchSubtree, int iNotifyFilter)
{
	FLastError = ERROR_SUCCESS;
	FHandle = ::FindFirstChangeNotification(
		strPathName.c_str(), bWatchSubtree, iNotifyFilter);
	if (FHandle == INVALID_HANDLE_VALUE)
		FLastError = ::GetLastError();
}
//---------------------------------------------------------------------------
__fastcall TFileChangeSemaphore::~TFileChangeSemaphore()
{
	if (FHandle)
		::FindCloseChangeNotification(FHandle);
}
//---------------------------------------------------------------------------
void __fastcall TFileChangeSemaphore::Acquire()
{
	if (FHandle)
	{
		FLastError = ERROR_SUCCESS;
		if (::WaitForSingleObject(FHandle, INFINITE) == WAIT_FAILED)
			FLastError = ::GetLastError();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFileChangeSemaphore::Release()
{
	if (FHandle)
	{
		FLastError = ERROR_SUCCESS;
		if (!::FindNextChangeNotification(FHandle))
			FLastError = ::GetLastError();
	}
}
//---------------------------------------------------------------------------
TWaitResult __fastcall TFileChangeSemaphore::WaitFor(DWORD dwTimeout)
{
	if (FHandle)
	{
		FLastError = ERROR_SUCCESS;
		switch (::WaitForSingleObject(FHandle, dwTimeout))
		{
			case WAIT_OBJECT_0:  return wrSignaled;
			case WAIT_ABANDONED: return wrAbandoned;
			case WAIT_TIMEOUT:   return wrTimeout;
			case WAIT_FAILED:    FLastError = ::GetLastError();
			default:             return wrError;
		}
	}
	return wrError;
}
//---------------------------------------------------------------------------
}	// namespace Syncobjs
//---------------------------------------------------------------------------


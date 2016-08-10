#pragma once
#ifndef __THREADSAFE_H__
#define __THREADSAFE_H__

#include "gw.h"

#ifdef WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif
#include <string>

using namespace std;

GW_NAMESPACE_BEGIN

class ThreadSafe
{
public:
	ThreadSafe(void);
	virtual ~ThreadSafe(void);

	string GetName() const { return m_strName; }
	void SetName(const string& strName) { m_strName = strName; }

	void Lock();
	void Unlock();

#ifndef WIN32
	operator pthread_mutex_t*() { return &m_Mutex; }
#endif

protected:
	string m_strName;
#ifdef WIN32
	CRITICAL_SECTION m_csLock;
#else
	pthread_mutex_t m_Mutex;
#endif
};

class AutoLock
{
public:
	AutoLock(ThreadSafe* pMutex) : m_pMutex(pMutex) { pMutex->Lock(); };
	~AutoLock(void) { m_pMutex->Unlock(); };

private:
	ThreadSafe* m_pMutex;
};

GW_NAMESPACE_END

#endif

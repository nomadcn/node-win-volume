#include "ThreadSafe.h"

GW_NAMESPACE_BEGIN

ThreadSafe::ThreadSafe(void)
{
#ifdef WIN32
		InitializeCriticalSection(&m_csLock);
#else
		pthread_mutex_init(&m_Mutex, NULL);
#endif

}

ThreadSafe::~ThreadSafe(void)
{
#ifdef WIN32
		DeleteCriticalSection(&m_csLock);
#else
		pthread_mutex_destroy(&m_Mutex);
#endif
}

void ThreadSafe::Lock()
{
#ifdef WIN32
		EnterCriticalSection(&m_csLock);
#else
		pthread_mutex_lock(&m_Mutex);
#endif
}

void ThreadSafe::Unlock()
{
#ifdef WIN32
		LeaveCriticalSection(&m_csLock);
#else
		pthread_mutex_unlock(&m_Mutex);
#endif
}

GW_NAMESPACE_END
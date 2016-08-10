#ifndef GW_STATE_H
#define GW_STATE_H

#include "gw.h"
#include "ThreadSafe.h"

#define DECLARE_GW_STATE \
public: \
	int GetState() { return m_state.GetState(); } \
	const char* GetStateName() { return m_state.GetStateName(); } \
private: \
	GW::State m_state; \
	void SetState(const int state) { m_state.SetState(state); }

GW_NAMESPACE_BEGIN

enum LIFE_CYCLE_STATE
{
	eLCS_UNKNOWN = -1,

	eLCS_NONE,
	eLCS_INIT,
	eLCS_OPEN,
	eLCS_START,
    eLCS_OPENING = 0x102,
	eLCS_STOPPING = 0x102,

	// It is used like this. ex) eGS_START | eGS_ING
	eLCS_ING = 0x100,
};

class State
{
public:
	State(bool bThreadSafe = false) :
	m_state(eLCS_NONE), m_oldState(eLCS_NONE), m_pLock(NULL)
	{
		if(bThreadSafe) m_pLock = new ThreadSafe();
	}
	~State()
	{
		if(m_pLock) delete m_pLock;
	}

	int GetState()
	{
		Lock();
		const int state = m_state;
		Unlock();

		return state;
	}
	const char* GetStateName()
	{
		Lock();
		const char* state = GetStateName(m_state);
		Unlock();

		return state;
	}
	int GetOldState()
	{
		Lock();
		const int state = m_oldState;
		Unlock();

		return state;
	}
	void SetState(const int state)
	{
		Lock();
		m_oldState = m_state;
		m_state = state;
		Unlock();
	}

	bool IsStateChanging() { return ((m_state & eLCS_ING) != 0); }
	bool IsThreadSafe() const { return (m_pLock != NULL); }

	void Lock()
	{
		if(IsThreadSafe())
		{
			m_pLock->Lock();
		}
	}
	void Unlock()
	{
		if(IsThreadSafe())
		{
			m_pLock->Unlock();
		}
	}

    bool operator ==(const int state)
    {
        Lock();
        bool value = (m_state == state);
        Unlock();

        return value;
    }

    bool operator !=(const int state)
    {
        Lock();
        bool value = (m_state != state);
        Unlock();

        return value;
    }

	static const char* GetStateName(const int eState)
	{
		const char* stateName[] =
		{
			"none", "init", "open", "start"
		};

		if ( eState >= eLCS_NONE && eState <= eLCS_START )
		{
			return stateName[eState];
		}

		if ( eState == eLCS_STOPPING )
		{
			return "stopping";
		}

		return "unknown";
	}

private:
	ThreadSafe* m_pLock;
	int m_state;
	int m_oldState;
};

GW_NAMESPACE_END

#endif

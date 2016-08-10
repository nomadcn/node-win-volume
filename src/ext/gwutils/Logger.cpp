#include "Logger.h"
#include "gwlog.h"
#include <time.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <stdio.h>

#if !defined(WIN32) && !defined(WIN64)

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>

#endif

#if defined(ANDROID)

#include <android/log.h>
#define LOGD_LINE2(FILTER) \
	__android_log_print(ANDROID_LOG_DEBUG, FILTER, "%s(%d) %s", __FILE__, __LINE__, __FUNCTION__);

#endif

#if defined(LINUX)

#include <sys/types.h>

#endif

// -----------------------------------------------------------------------------
EXTERN_C_BEGIN

GW_NAMESPACE_NAME::Logger& GetLogger()
{
	static GW_NAMESPACE_NAME::Logger g_logger;
	return g_logger;
}

void gwlog_init(
	const char* szPathName, const int nFlags, const int nMinLogLevel)
{
	gw::Logger& logger = GetLogger();
	logger.SetFlags(nFlags);
	logger.SetMinLogLevel(nMinLogLevel);
	logger.Open(szPathName);
}

void gwlog_exit()
{
	gw::Logger& logger = GetLogger();
	logger.Close();
}

void gwlog_setEnable(bool value)
{
    GetLogger().SetEnable(value);
}

int gwlog_getLogLevel()
{
	return GetLogger().GetMinLogLevel();
}

void gwlog_setLogLevel(const int nLevel)
{
	return GetLogger().SetMinLogLevel(nLevel);
}

EXTERN_C_END
// --------------------------------------------------------------------------------

static const char* SZ_LOG_LEVEL[] =
{
	"",
	"",
	"VERBOSE",
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR"
};

GW_NAMESPACE_BEGIN

Logger::Logger(void) :
m_enable(false),
m_fp(NULL),
m_nMinLogLevel(eLL_VERBOSE),
m_nBufSize(0),
m_szMessage(NULL),
m_szLog(NULL),
m_flags(eLO_CONSOLE | eLO_FILE)
{
}

Logger::~Logger(void)
{
	Close();
}

void Logger::Lock()
{
	m_threadSafe.Lock();
}

void Logger::Unlock()
{
	m_threadSafe.Unlock();
}

long Logger::Open(const char* szPathName, int nBufSize)
{
	Close();

	AutoLock lock(&m_threadSafe);

	m_szMessage = new char [nBufSize];
	m_szLog = new char [nBufSize * 2];
	m_nBufSize = nBufSize;
	memset(&m_fileLogDate, 0, sizeof(m_fileLogDate));

	if( m_flags.GetFlags(eLO_FILE|eLO_APPEND) )
	{
        if (szPathName)
        {
            m_strPathName = szPathName;
            m_strLockPathName = m_strPathName + std::string(".lock");
        }
        else
        {
            m_strPathName.clear();
            m_strLockPathName.clear();
        }

        std::string fileMode = 
            (LockExists() || m_flags.GetFlags(eLO_APPEND)) ? "a+" : "w";

		int nRet = CreateLogFile(fileMode.c_str());

		if(nRet == 0)
		{
			CreateLockFile();
		}
	}

    SetEnable(true);

	return 0;
}

long Logger::Close()
{
	AutoLock lock(&m_threadSafe);

	if(m_fp)
	{
		fclose(m_fp);
		m_fp = NULL;
	}

	if(m_szMessage)
	{
		delete[] m_szMessage;
		m_szMessage = NULL;
	}

	if(m_szLog)
	{
		delete[] m_szLog;
		m_szLog = NULL;
	}

	DeleteLockFile();

	m_nBufSize = 0;
	m_strPathName.clear();
	m_strLockPathName.clear();

	return 0;
}

long Logger::Print(int nLevel, const char* szTAG, const char* szFormat, ...)
{
    // Don't log when m_enable is false.
    if (!m_enable)
    {
        return -1;
    }

	AutoLock lock(&m_threadSafe);

	if(m_szLog == NULL)
	{
		return -2;
	}

	if( m_nMinLogLevel > nLevel)
	{
		return 1;
	}

	va_list argptr;
	va_start(argptr, szFormat);
	vsnprintf(m_szMessage, m_nBufSize, szFormat, argptr);
	va_end(argptr);

	FillLog(nLevel, szTAG, m_szMessage);
	PrintToConsole(nLevel, szTAG);
	PrintToFile();

	return 0;
}

void Logger::PrintToConsole(const int nLevel, const char* szTAG)
{
	if( !m_flags.GetFlags(eLO_CONSOLE) )
	{
		return;
	}

#if defined(ANDROID)

	__android_log_write(nLevel, szTAG, m_szMessage);

#else

	printf("%s\n", m_szLog);

#endif
}

void Logger::PrintToFile()
{
	if( !m_flags.GetFlags(eLO_FILE) )
	{
		return;
	}

	if( m_flags.GetFlags(eLO_DATE_DIVISION) && IsItTimeToDivideLogFile() )
	{
		CloseLogFile();
		BackupLogFile();
		CreateLogFile("w");
	}

	if(m_fp)
	{
		fprintf(m_fp, "%s\n", m_szLog);
		fflush(m_fp);
	}
}

unsigned int Logger::GetCurrentThreadID() const
{
	unsigned int nThreadID;

#if defined(WIN32)

	nThreadID = GetCurrentThreadId();

#elif defined(OSX) || defined(LINUX)

	nThreadID = syscall(SYS_gettid);

#else
	// ANDROID
	nThreadID = gettid();

#endif

	return nThreadID;
}

void Logger::FillLog(const int nLevel, const char* szTAG, const char* szMessage)
{
	char szDate[128];
	unsigned int nThreadID;

	nThreadID = GetCurrentThreadID();

	FillCurrentDateTimeString(szDate, sizeof(szDate));

	gw_snprintf(m_szLog, m_nBufSize, "%s: %s/%s(%u): %s",
		szDate, SZ_LOG_LEVEL[nLevel], szTAG, nThreadID, szMessage);
}

void Logger::FillCurrentDateTimeString(char* szBuf, const int nBufSize)
{
	struct tm* pDate;
	const char* szDateTimeFormat = "%02d-%02d %02d:%02d:%02d.%03d";

#ifdef WIN32

	struct tm date = { 0 };
	SYSTEMTIME lt;
	::GetLocalTime(&lt);

	gw_snprintf(szBuf, nBufSize, szDateTimeFormat,
		lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);

	date.tm_year = lt.wYear - 1900;
	date.tm_mon = lt.wMonth - 1;
	date.tm_mday = lt.wDay;
	date.tm_hour = lt.wHour;
	date.tm_min = lt.wMinute;
	date.tm_sec = lt.wSecond;
	pDate = &date;

#else

	time_t curTime;
	int nMillisecond;

	struct timeval tvTime;
	gettimeofday(&tvTime, NULL);
	curTime = tvTime.tv_sec;
	nMillisecond = tvTime.tv_usec / 1000;

	pDate = localtime(&curTime);
	snprintf(szBuf, nBufSize, szDateTimeFormat,
		pDate->tm_mon+1, pDate->tm_mday,
		pDate->tm_hour, pDate->tm_min, pDate->tm_sec, nMillisecond);

#endif

	const unsigned int nMask = eLO_FILE | eLO_DATE_DIVISION;
	if(m_flags.GetFlags(nMask) == nMask)
	{
		memcpy(&m_fileLogDate, pDate, sizeof(struct tm));
	}
}

void Logger::SetFlags(unsigned int nFlags)
{
	m_flags.SetFlags(nFlags);
}

bool Logger::LockExists() const
{
    if (!m_strLockPathName.empty())
    {
        FILE* fp = fopen(m_strLockPathName.c_str(), "r");
        if (fp)
        {
            fclose(fp);
            return true;
        }
    }

	return false;
}

int Logger::CreateLogFile(const char* szMode)
{
	if(m_strPathName.empty())
	{
		return -1;
	}

	FILE* fp = fopen(m_strPathName.c_str(), szMode);

	if(fp == NULL)
	{
		return -1;
	}

	m_fp = fp;

	struct tm* pDate;
	time_t curTime = time(NULL);
	pDate = localtime(&curTime);
	memcpy(&m_fileCreationDate, pDate, sizeof(struct tm));

	return 0;
}

void Logger::CloseLogFile()
{
	if(m_fp)
	{
		fclose(m_fp);
		m_fp = NULL;
	}
}

void Logger::BackupLogFile()
{
	char szPathName[512];
	::gw_snprintf(szPathName, sizeof(szPathName), "%s.%04d-%02d-%02d",
		m_strPathName.c_str(),
		m_fileCreationDate.tm_year+1900, m_fileCreationDate.tm_mon+1, m_fileCreationDate.tm_mday);

	::gw_unlink(szPathName);
	rename(m_strPathName.c_str(), szPathName);
}

void Logger::CreateLockFile()
{
	FILE* fp = fopen(m_strLockPathName.c_str(), "w");

	if(fp)
	{
		fclose(fp);
	}
}

void Logger::DeleteLockFile()
{
	if( m_strLockPathName.empty() )
	{
		return;
	}

	::gw_unlink(m_strLockPathName.c_str());
}

bool Logger::IsItTimeToDivideLogFile()
{
	return (m_fileLogDate.tm_mday != 0 && m_fileCreationDate.tm_mday != m_fileLogDate.tm_mday);
}

GW_NAMESPACE_END

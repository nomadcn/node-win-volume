#pragma once
#ifndef GW_LOGGER_H
#define GW_LOGGER_H

#include "gw.h"
#include <string>
#include <memory>
#include <time.h>
#include "Flags.h"
#include "ThreadSafe.h"

enum LOG_LEVEL
{
	eLL_VERBOSE = 2,
	eLL_DEBUG,
	eLL_INFO,
	eLL_WARN,
	eLL_ERROR,
};

enum LOG_OUTPUT
{
	eLO_NONE			= 0x00000000,
    eLO_CONSOLE         = 0x00000001,
    eLO_FILE            = 0x00000002,
	eLO_DATE_DIVISION	= 0x00000004,
	eLO_APPEND			= 0x00000008, // append log to file.
	eLO_ALL				= 0xFFFFFFFF,
};

GW_NAMESPACE_BEGIN

class Logger
{
private:
	// logging works only if it is true.
	bool m_enable;
	FILE* m_fp;
	int m_nMinLogLevel;
	std::string m_strPathName;
	std::string m_strLockPathName;
	int m_nBufSize;
	char* m_szMessage;
	char* m_szLog;
	Flags m_flags;
	ThreadSafe m_threadSafe;

	struct tm m_fileLogDate;
	struct tm m_fileCreationDate;

private:
	void Lock();
	void Unlock();
	void FillLog(const int nLevel, const char* szTAG, const char* szMessage);
	void FillCurrentDateTimeString(char* szBuf, const int nBufSize);
	unsigned int GetCurrentThreadID() const;

	void PrintToConsole(const int nLevel, const char* szTAG);
	void PrintToFile();

	bool LockExists() const;
	void CreateLockFile();
	void DeleteLockFile();

	int CreateLogFile(const char* szMode);
	void CloseLogFile();
	void BackupLogFile();
	bool IsItTimeToDivideLogFile();

public:
	Logger(void);
	~Logger(void);

    bool IsEnable() const { return m_enable; }
    void SetEnable(bool value) { m_enable = value; }

	int GetMinLogLevel() const { return m_nMinLogLevel; };
	void SetMinLogLevel(const int nLevel) { m_nMinLogLevel = nLevel; };

	long Open(const char* szPathName, int nBufSize = 8192);
	long Close();
	long Print(int nLevel, const char* szTAG, const char* szFormat, ...);
	void SetFlags(unsigned int nFlags);
};

GW_NAMESPACE_END

EXTERN_C_BEGIN
	gw::Logger& GetLogger();
	void gwlog_init(
		const char* szPathName, const int nFlags,
        const int nMinLevel = eLL_DEBUG);
	void gwlog_exit();
	void gwlog_setEnable(bool value);
	int gwlog_getLogLevel();
	void gwlog_setLogLevel(const int nLevel);
EXTERN_C_END

#endif //__LOGGER_H__
